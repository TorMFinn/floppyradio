#include <iostream>
#include <thread>
#include <csignal>
#include "floppyradio/drive_poller.hpp"
#include "floppyradio/drive_mounter.hpp"
#include "floppyradio/module_loader.hpp"
#include "floppyradio/player.hpp"
#include "floppyradio/track_list.hpp"

bool quit = false;

void sighandler(int signum) {
    quit = true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "missing input module" << std::endl;
        return -1;
    }

    floppyradio::drive_mounter mounter;
    floppyradio::player player;

    mounter.set_drive_mounted_callback([&](const boost::filesystem::path &path) {
        std::cout << "Disk mounted at: " << path.string() << std::endl;
        int num_tracks = player.open_directory(path);
        if (num_tracks > 0) {
            // Start playback
            player.play_song(0);
        }
    });

    mounter.set_drive_unmounted_callback([&](){
        std::cout << "disk unmounted" << std::endl;
        player.close_current_directory();
    });

    std::signal(SIGTERM, sighandler);
    std::signal(SIGINT, sighandler);
    while(not quit) {
        std::string cmd;
        std::cout << "commands: n (next), p (prev), s (stop), r (repeat), e (exit)\n >> ";
        std::cin >> cmd;
        if (cmd == "n") {
            player.play_next_song();
        } else if (cmd == "p") {
            player.play_previous_song();
        } else if (cmd == "s") {
            player.stop();
        } else if (cmd == "r") {
            player.set_repeat(true);
        } else if (cmd == "e") {
            break;
        } else {
            std::cout << "unknown command: " << cmd << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
