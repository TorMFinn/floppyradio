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

    floppyradio::module_loader loader;
    floppyradio::drive_mounter mounter;
    track_list tracks;
    int track_index = 0;
    player playback;

    auto play_song = [&]() {
        std::cout << "loading module: " << tracks[track_index] << std::endl;
        auto mod = loader.load_from_file(tracks[track_index]);
        if (mod) {
            playback.start_playback(mod);
        } else {
            std::cerr << "failed to load module" << std::endl;
        }
    };

    auto play_next_song = [&]() {
        std::cout << "track_index: " << track_index - 1 << " is less than " << tracks.size() << std::endl;
        if (track_index-1 < static_cast<int>(tracks.size())) {
            track_index++;
            play_song();
        } else {
            std::cout << "Last song" << std::endl;
            // Last song;
        }
    };

    auto play_prev_song = [&]() {
        if (track_index > 0 && !tracks.empty()) {
            track_index--;
            play_song();
        } else {
            // First song
        }
    };

    mounter.set_drive_mounted_callback([&](const boost::filesystem::path &path) {
        std::cout << "Disk mounted at: " << path.string() << std::endl;
        tracks = get_track_list(path);
        track_index = 0;

        if (!tracks.empty()) {
            std::cout << "Tracks size: " << tracks.size() << std::endl;
            play_song();
        }
    });

    mounter.set_drive_unmounted_callback([&](){
        std::cout << "disk unmounted" << std::endl;
        // Empty Track list
        tracks = track_list();
    });

    playback.on_song_complete.connect([&]() {
        std::cout << "Song complete" << std::endl;
        play_next_song();
    });

    playback.on_info.connect([](player::playback_info info) {
        std::cout << "song name: " << info.track_name << " "
                  << "duration : " << info.duration_seconds << " "
                  << "position : " << info.current_position << "\r";
    });

    std::signal(SIGTERM, sighandler);
    std::signal(SIGINT, sighandler);
    while(not quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    playback.stop_playback();
    return 0;
}
