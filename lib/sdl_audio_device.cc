#include "floppyradio/sdl_audio_device.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace floppyradio {
    void sdl_audio_device::audio_callback(void *userdata, uint8_t *stream, int len) {
        sdl_audio_device *dev = reinterpret_cast<sdl_audio_device*>(userdata);
        auto data = dev->m_on_audio_wanted();
        SDL_memcpy(stream, reinterpret_cast<uint8_t*>(data.data()), data.size() * 2);
    }

    sdl_audio_device::sdl_audio_device() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            throw std::runtime_error("failed to initialize sdl audio");
        }

        open_audio_device();
    }

    sdl_audio_device::~sdl_audio_device() {
        if (m_dev != 0) {
            SDL_CloseAudioDevice(m_dev);
        }
    }

    bool sdl_audio_device::open_audio_device() {
        m_spec = SDL_AudioSpec{};
        m_obtained = SDL_AudioSpec{};

        m_spec.freq = m_sample_rate;
        m_spec.format = AUDIO_S16LSB;
        m_spec.channels = 2; // Stereo Sound
        m_spec.samples = m_sample_size;
        m_spec.userdata = this;
        m_spec.callback = audio_callback;

        m_dev = SDL_OpenAudioDevice(nullptr, 0, &m_spec, &m_obtained, 0);

        if (m_spec.freq != m_obtained.freq) {
            std::cerr << "frequeny mismatch" << std::endl;
            return false;
        } else if (m_spec.format != m_obtained.format) {
            std::cerr << "format error" << std::endl;
            return false;
        } else if (m_spec.channels != m_obtained.channels) {
            std::cerr << "channels mismatch" << std::endl;
            return false;
        } else if (m_spec.samples != m_obtained.samples) {
            std::cerr << "sample size mismatch" << std::endl;
            return false;
        }
        return true;
    }

    void sdl_audio_device::start() {
        std::cout << "Starting audio device" << std::endl;
        SDL_PauseAudioDevice(m_dev, 0);
    }

    void sdl_audio_device::stop() {
        SDL_PauseAudioDevice(m_dev, 1);
    }
}
