#pragma once
#include "floppyradio/Display.hpp"
#include <memory>

namespace floppyradio {
    class GLDisplay : public Display{
    public:
        GLDisplay();
        virtual ~GLDisplay();

        void SetTrackName(const std::string &name) override;
        void SetDuration(int seconds) override;
        void SetCurrentTime(int seconds) override;
    private:
        struct Data;
        std::unique_ptr<Data> m_data;
    };
}
