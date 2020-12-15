#include "floppyradio/drive_mounter.hpp"
#include "spdlog/spdlog.h"
#include <udisks/udisks.h>
#include <thread>
#include <iostream>
#include <boost/filesystem.hpp>

using namespace floppyradio;

struct drive_mounter::Impl {
    Impl() {
        init_udisks();
        find_and_mount_floppy();
    }

    ~Impl() {
        if (main_loop_thd.joinable()) {
            g_main_loop_quit(loop);
            main_loop_thd.join();
        }

        g_main_loop_unref(loop);
    }

    void init_udisks() {
        GError *error;
        udisks_client = udisks_client_new_sync(nullptr, &error);
        if (!udisks_client) {
            spdlog::error("failed to init udisks client {}", error->message);
            return;
        }

        GDBusObjectManager *manager = udisks_client_get_object_manager(udisks_client);
        g_signal_connect(manager,
                         "object-added",
                         G_CALLBACK(on_object_added),
                         this);
        g_signal_connect(manager,
                         "interface-proxy-properties-changed",
                         G_CALLBACK(on_interface_proxy_properties_changed),
                         this);

        main_loop_thd = std::thread([&](){
            main_loop();
        });
    }

    static bool drive_is_floppy(UDisksDrive *drive) {
        std::string drive_media(udisks_drive_get_media(drive));
        return drive_media == "floppy";
    }

    static void mount_drive(UDisksObject *object) {
        UDisksFilesystem *fs = udisks_object_peek_filesystem(object);
        if (!fs) {
            spdlog::error("failed to get filesystem from object");
            return;
        }
        const gchar *const *mount_points;
        mount_points = udisks_filesystem_get_mount_points(fs);
        if (mount_points != nullptr && g_strv_length ((gchar **) mount_points) == 0) {
            // Not mounted
            GError *error = nullptr;
            gchar *mount_path;
            if (udisks_filesystem_call_mount_sync(fs,
                                                  g_variant_new("a{sv}", nullptr),
                                                  &mount_path,
                                                  nullptr,
                                                  &error)) {
                std::cout << "mounted drive at: " << mount_path << std::endl;
            } else {
                spdlog::error("failed to mount drive: {}", error->message);
                return;
            }
        } else {
            // Allready mounted, lets report it
            // TODO
        }
    }

    static void on_object_added(GDBusObjectManager *manager,
                                GDBusObject *obj,
                                gpointer user_data) {
        Impl* impl = reinterpret_cast<Impl*>(user_data);
        UDisksObject *object = UDISKS_OBJECT(obj);
        if (object_is_floppy(impl->udisks_client, object)) {
            mount_drive(object);
        }
    }

    void find_and_mount_floppy() {
        GError *error = nullptr;
        GList *list;
        GList *objects;
        objects = g_dbus_object_manager_get_objects(udisks_client_get_object_manager(udisks_client));
        for (list = objects; list != nullptr; list = list->next) {
            UDisksObject *object = UDISKS_OBJECT(list->data);
            if (object_is_floppy(udisks_client, object)) {
                mount_drive(object);
            }
        }
    }

    static bool object_is_floppy(UDisksClient* client, UDisksObject *object) {
        UDisksBlock *block = udisks_object_peek_block(object);
        if (block) {
            UDisksDrive *drive = udisks_client_get_drive_for_block(client, block);
            if (drive && drive_is_floppy(drive)) {
                return true;
            }
        }
        return false;
    }

    static std::string mount_point_to_string(gchar *mount_points) {
        std::string raw(mount_points);
        int first_delim = raw.find_first_of("'");
        int last_delim = raw.find_first_of("'", first_delim + 1);
        return raw.substr(first_delim + 1, last_delim - 3);
    }

    static void on_interface_proxy_properties_changed (GDBusObjectManagerClient *manager,
                                                       GDBusObjectProxy         *object_proxy,
                                                       GDBusProxy               *interface_proxy,
                                                       GVariant                 *changed_properties,
                                                       const gchar* const       *invalidated_properties,
                                                       gpointer                  user_data) {
        Impl* impl = reinterpret_cast<Impl*>(user_data);
        UDisksObject* object= udisks_client_get_object(impl->udisks_client,
                                                       g_dbus_proxy_get_object_path(interface_proxy));
        if (!object_is_floppy(impl->udisks_client, object)) {
            return;
        }

        GVariantIter *iter;
        const gchar *property_name;
        GVariant *value;
        g_variant_get(changed_properties, "a{sv}", &iter);
        while (g_variant_iter_next(iter, "{&sv}", &property_name, &value)) {
            if (g_strcmp0(property_name, "MountPoints") == 0) {
                spdlog::info("Got MountPoints property change");
                // Our mount points changed, check if empty and report back.
                gsize length;
                std::string mount_points_str(mount_point_to_string(g_variant_print(value, 0)));
                if (mount_points_str == "[]" && impl->unmounted_callback) {
                    // Notfiy Unmounted
                    impl->unmounted_callback();
                    spdlog::info("drive unmounted");
                } else {
                    if (impl->mounted_callback) {
                    impl->mounted_callback(boost::filesystem::path(mount_points_str));
                    spdlog::info("drive mounted {}", mount_points_str);
                    }
                }
            }

            if (g_strcmp0(property_name, "IdUsage") == 0) {
                spdlog::debug("Got IdUsage property change");
                gsize length;
                const char* id_usage = g_variant_get_string(value, &length);
                if (std::string(id_usage, length) == "filesystem") {
                    // A floppy disk was inserted
                    mount_drive(object);
                } else if (std::string(id_usage, length).empty()) {
                    // Floppy disk removed, unmount happens automatically
                    // By udisks
                }
            }
        }
    }

    void main_loop () {
        loop = g_main_loop_new(nullptr, 0);
        g_main_loop_run(loop);
    }

    bool run;
    std::thread main_loop_thd;
    GMainLoop *loop;
    UDisksClient *udisks_client;
    std::string floppy_block_object_path;

    drive_unmounted_callback unmounted_callback;
    drive_mounted_callback mounted_callback;
};

drive_mounter::drive_mounter()
    : impl_(new Impl()) {
}

drive_mounter::~drive_mounter() = default;

void drive_mounter::automount_device(const std::string &drive_serial) {
}

void drive_mounter::set_drive_mounted_callback(drive_mounted_callback callback) {
    impl_->mounted_callback = callback;
}

void drive_mounter::set_drive_unmounted_callback(drive_unmounted_callback callback) {
    impl_->unmounted_callback = callback;
}
