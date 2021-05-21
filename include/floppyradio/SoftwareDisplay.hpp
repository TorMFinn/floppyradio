#pragma once
#include "floppyradio/Display.hpp"

namespace floppyradio {
    class SoftDisplay : public Display {
    public:
        SoftDisplay();
        virtual ~SoftDisplay();

        void SetTrackName(const std::string &name) override;
        void SetDuration(int seconds) override;
        void SetCurrentTime(int seconds) override;
        void Start() override;

    private:
        struct Data;
        std::unique_ptr<Data> m_data;
    };
}
