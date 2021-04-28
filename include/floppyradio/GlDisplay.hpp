/**
 * Driver for the LCD display that shows
 * Information about current track.
 */
#pragma once
#include <memory>
#include <string>

namespace floppyradio {
    class display {
    public:
        /**
         * The name of the current track being played
         */
        virtual void set_track_name(const std::string &name) = 0;

        /**
         * How long the song is estimated to last
         */
        virtual void set_duration(int seconds) = 0;

        /**
         * How long the playback has lasted
         */
        virtual void set_current_time(int seconds) = 0;
    private:
    };
}
