/**
 * A GUI Window pretty much to draw information on
 */
#pragma once
#include <memory>
#include <string>

namespace floppyradio {
    class Display {
    public:
        /**
         * The name of the current track being played
         */
        virtual void SetTrackName(const std::string &name) = 0;

        /**
         * How long the song is estimated to last
         */
        virtual void SetDuration(int seconds) = 0;

        /**
         * How long the playback has lasted
         */
        virtual void SetCurrentTime(int seconds) = 0;

        /**
         * This is the main loop for the GUI
         * Blocking
         */
        virtual void Start() = 0;
    };

    using DisplayPtr = std::shared_ptr<Display>;

    class DisplayFactory {
    public:
        /**
         * Currently only supports the software rendered
         * Display
         */
        static DisplayPtr Create();
    };
}
