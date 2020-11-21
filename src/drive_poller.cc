#include "drive_poller.hpp"

#include <thread>
#include <iostream>
#include <boost/process.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/fs.h>

struct drive_poller::Implementation {
    std::thread poll_thd;
    bool do_poll = false;
    bool drive_mounted = false;
    boost::filesystem::path device_path;
    int fd = -1;

    Implementation() {}

    ~Implementation() {
        stop_polling();
    }

    void start_polling() {
        if (poll_thd.joinable()) {
            // Allready started
            std::cout << "poller allready running" << std::endl;
            return;
        }
        do_poll = true;
        poll_thd = std::thread([&]() {
            poll_drive();
        });
    }

    void open_device() {
        while(fd < 0 && do_poll) {
            fd = open("/dev/sdb", O_RDONLY);
            if (fd < 0) {
                std::cerr << "failed to open scsi device, reason: "
                          << std::strerror(errno) << std::endl;
            } 
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void poll_drive() {
        open_device();
        while(do_poll) {
            long block_size;
            int r = ioctl(fd, BLKGETSIZE, &block_size);
            if (r < 0) {
                std::cerr << "failed to get block size" << std::endl;
            } else {
                std::cout << "Block size" << block_size << std::endl;
                if (block_size > 0 && not drive_mounted) {
                    drive_mounted = mount_drive();
                    if (drive_mounted) {
                        // Callback
                        std::cout << "mounted drive" << std::endl;
                    }
                } else if (block_size == 0 && drive_mounted) {
                    drive_mounted = !unmount_drive();
                    if (!drive_mounted) {
                        // Callback with empty path
                        std::cout << "unmounted drive" << std::endl;
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        close(fd);
    }

    bool mount_drive() {
        int result = boost::process::system(boost::process::search_path("mount"), "/dev/sdb", "/mnt/floppy");
        std::cout << "result of mount command: " << result << std::endl;
        return result == 0 || result == 32;
    }

    bool unmount_drive() {
         int result = boost::process::system(boost::process::search_path("umount"), "/mnt/floppy");
         std::cout << "result of umount command: " << result << std::endl;
        return result == 0 || result == 32;
    }

    void stop_polling() {
        if (poll_thd.joinable()) {
            do_poll = false;
            poll_thd.join();
        }
    }
};

drive_poller::drive_poller()
    : impl_(new Implementation()) {}

drive_poller::~drive_poller() = default;

void drive_poller::start_polling(boost::filesystem::path device_path,
                                 boost::filesystem::path mount_path) {
    impl_->device_path = device_path;
    impl_->start_polling();
}

void drive_poller::stop_polling() {
    impl_->stop_polling();
}

/**
 * When a disk is detected in the drive, it is automatically
 * mounted and this callback is executed to inform the application.
 */
void drive_poller::set_drive_mounted_callback(drive_mounted_callback callback) {
}
