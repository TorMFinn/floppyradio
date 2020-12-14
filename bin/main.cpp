#include <iostream>
#include <thread>
#include <csignal>
#include "floppyradio/drive_poller.hpp"
#include "floppyradio/module_loader.hpp"
#include "floppyradio/player.hpp"

bool quit = false;

void sighandler(int signum) {
    quit = true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "missing input module" << std::endl;
        return -1;
    }

    floppyradio::module_loader loader;
    player playback;

    std::cout << "starting drive poller" << std::endl;
    drive_poller poller;
    poller.start_polling("/dev/sdb", "/mnt/floppy");
    poller.set_drive_mounted_callback([&](const boost::filesystem::path &path) {
        std::cout << "DRIVE MOUNTED" << std::endl;
        for (auto file : boost::filesystem::directory_iterator(path)) {
            if (file.path().extension() == ".mod") {
                std::cout << "found module" << std::endl;
                auto module = loader.load_from_file(file.path());
                if (module) {
                    playback.start_playback(module);
                }
            }
        }
    });

    poller.set_drive_unmounted_callback([&](const boost::filesystem::path &path) {
        std::cout << "Drive unmounted" << std::endl;
    });

    /*
    auto module = loader.load_from_file(argv[1]);
    if (module) {
        std::cout << "Starting playback" << std::endl;
        playback.start_playback(module);
    }
    */

    std::signal(SIGTERM, sighandler);
    std::signal(SIGINT, sighandler);
    while(not quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    playback.stop_playback();
    return 0;
}
