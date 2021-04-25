#pragma once
#include <boost/filesystem.hpp>
#include <string>
#include "display.hpp"
#include "module.hpp"

namespace floppyradio {
    class player {
    public:
        player();
        ~player();

        virtual void set_display(display* display);

        virtual void play_song(int track);
        virtual void play_next_song();
        virtual void play_previous_song();

        void stop();
        void pause();

        /**
         * Toggle if the player should repeat the song
         */
        void set_repeat(bool repeat);

        /**
         * Open a directory containing songs
         * @return the number of valid tracks.
         */
        int open_directory(boost::filesystem::path dir);

        /**
         * Invalidate the current directory
         * Important if the disc is unmounted
         */
        void close_current_directory();

    private:
        struct Data;
        std::unique_ptr<Data> m_data;
    };
}
