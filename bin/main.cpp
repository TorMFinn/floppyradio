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

struct App {
    floppyradio::drive_mounter mounter;
    floppyradio::player player;
};

void handle_drive_mounted(App &app, const boost::filesystem::path &path) {
    std::cout << "Disk mounted at: " << path.string() << std::endl;
    int num_tracks = app.player.open_directory(path);
    if (num_tracks > 0) {
        // Start playback
        app.player.play_song(0);
    }
}

int main(int argc, char **argv) {
    App app;

    if (argc < 2) {
        app.mounter.set_drive_mounted_callback([&](const boost::filesystem::path &path) {
            handle_drive_mounted(app, path);
        });

        app.mounter.set_drive_unmounted_callback([&](){
            std::cout << "disk unmounted" << std::endl;
            app.player.close_current_directory();
        });
    } else {
        // Simulate a drive mount
        handle_drive_mounted(app, boost::filesystem::path(argv[1]));
    }

    std::signal(SIGTERM, sighandler);
    std::signal(SIGINT, sighandler);
    while(not quit) {
        std::string cmd;
        std::cout << "commands: n (next), p (prev), s (stop), r (repeat), e (exit)\n >> ";
        std::cin >> cmd;
        if (cmd == "n") {
            app.player.play_next_song();
        } else if (cmd == "p") {
            app.player.play_previous_song();
        } else if (cmd == "s") {
            app.player.stop();
        } else if (cmd == "r") {
            app.player.set_repeat(true);
        } else if (cmd == "e") {
            break;
        } else {
            std::cout << "unknown command: " << cmd << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
