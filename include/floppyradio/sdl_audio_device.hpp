#pragma once
#include "floppyradio/audio_device.hpp"
#include <SDL2/SDL.h>

namespace floppyradio {
    class sdl_audio_device : public audio_device {
    public:
        sdl_audio_device();
        virtual ~sdl_audio_device();

        void start() override;
        void stop() override;

    private:
        bool open_audio_device();
        static void audio_callback(void *userdata, uint8_t *stream, int len);
        SDL_AudioSpec m_spec;
        SDL_AudioSpec m_obtained;
        SDL_AudioDeviceID m_dev;
    };
}
