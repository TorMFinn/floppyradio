#include "player.hpp"
#include "libopenmpt/libopenmpt.hpp"
#include "SDL2/SDL.h"

/*
struct player::Implementation {
    void init_sdl_audio() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            throw std::runtime_error("failed to initialize sdl audio");
        }

        SDL_AudioSpec spec = {};
        spec.freq = 48000;
        spec.format = AUDIO_S16;
        spec.channels = 2;
        spec.samples = 4096;
        spec.callback = this;
    }

    SDL_AudioDeviceID audio_dev;
};

player::player()
    : m_impl(new Implementation()) {}

player::~player() = default;

void player::start_playback(boost::filesystem::path path) {
}

void player::stop_playback() {
}

std::string player::get_song_title() {
}

void player::set_repeat(bool repeat) {
}

*/
