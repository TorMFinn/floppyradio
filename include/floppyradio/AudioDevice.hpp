#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>

namespace floppyradio {
    class audio_config {
        int sample_rate;
        int sample_size;
        int channels;
    };

    class AudioDevice {
    public:
        using AudioData = std::vector<int16_t>;
        using AudioDataWantedCB = std::function<AudioData(void)>;

        //virtual void configure(audio_config &cfg) = 0;

        virtual void start() = 0;
        virtual void stop() = 0;

        /**
         * Register a callback that is called when the device 
         * Wants more audio data
         */
        virtual void on_audio_wanted(AudioDataWantedCB cb) {
            m_on_audio_wanted = cb;
        }

        virtual int get_sample_rate() {
            return m_sample_rate;
        }

        virtual int get_sample_size() {
            return m_sample_size;
        }

    protected:
        AudioDataWantedCB m_on_audio_wanted;
        int m_sample_rate = 44100;
        int m_sample_size = 4096;
    };

    using AudioDevicePtr = std::shared_ptr<AudioDevice>;
}
