#pragma once
#include "display.hpp"

namespace floppyradio {
    class ncurses_display : public display {
    public:
        ncurses_display();
        virtual ~ncurses_display();

        void set_track_name(const std::string &name) override;
        void set_duration(int seconds) override;
        void set_current_time(int seconds) override;
        void set_repeat(bool enabled);

    private:
        struct Data;
        std::unique_ptr<Data> m_data;
    };
}
