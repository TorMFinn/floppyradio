#pragma once
#include <functional>
#include <memory>
#include <string>

// Forward declarations
namespace boost::filesystem {
    class path;
}

namespace floppyradio {
    class drive_mounter {
    public:
        drive_mounter();
        virtual ~drive_mounter();

        /**
         * Check for presence of media in device specified by serial
         * And automount it when connected.
         */
        void automount_device(const std::string &media_type = "floppy");

        using drive_mounted_callback = std::function<void (boost::filesystem::path)>;
        using drive_unmounted_callback = std::function<void ()>;

        void set_drive_mounted_callback(drive_mounted_callback callback);
        void set_drive_unmounted_callback(drive_unmounted_callback callback);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
