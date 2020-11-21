#pragma once
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

class player {
public:
    player();
    ~player();

    bool start_playback(boost::filesystem::path path);
    bool stop_playback();

    std::vector<boost::filesystem::path> list_songs(boost::filesystem::path dir);

    /**
     * Retrieve information about the playing song.
     */
    std::string get_song_title();

    /**
     * Toggle if the player should repeat the song
     */
    void set_repeat(bool repeat);

private:
    struct Implementation;
    std::unique_ptr<Implementation> m_impl;
};
