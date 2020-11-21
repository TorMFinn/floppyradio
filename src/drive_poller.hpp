#pragma once
#include <boost/filesystem.hpp>
#include <memory>

class drive_poller {
    using drive_mounted_callback = std::function<void (boost::filesystem::path)>;
public:
    drive_poller();
    ~drive_poller();

    /**
     * Begins polling the drive to figure out if there is
     * A disk connected, will mount to the specified location
     */
    void start_polling(boost::filesystem::path device_path,
                       boost::filesystem::path mount_path);

    void stop_polling();

    /**
     * When a disk is detected in the drive, it is automatically
     * mounted and this callback is executed to inform the application.
     * When the drive is unmounted, a empty path is returned.
     */
    void set_drive_mounted_callback(drive_mounted_callback callback);

private:
    struct Implementation;
    std::unique_ptr<Implementation> impl_;
};
