#pragma once
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>
#include <string>
#include "module.hpp"

struct track_info {
    std::string title;
    std::string artist;
};

// Forward declaration
namespace openmpt {
    class module;
}

class player {
public:
    player();
    ~player();

    void start_playback(std::shared_ptr<openmpt::module> mod);
    void pause_playback();
    void stop_playback();

    /**
     * Toggle if the player should repeat the song
     */
    void set_repeat(bool repeat);

private:
    struct Implementation;
    std::unique_ptr<Implementation> m_impl;
};
