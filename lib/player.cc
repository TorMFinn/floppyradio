#include "floppyradio/player.hpp"
#include "libopenmpt/libopenmpt.hpp"
#include "spdlog/spdlog.h"
#include "SDL2/SDL.h"

constexpr int sample_rate = 48000;
constexpr int sample_size = 4096;

struct player::Implementation {
    Implementation() {
        init_audio();
    }

    ~Implementation() {
        close_audio();
    }

    void init_audio() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            throw std::runtime_error("failed to initialize sdl audio");
        }

        SDL_AudioSpec spec = {};
        SDL_AudioSpec obtained = {};
        spec.freq = sample_rate;
        spec.format = AUDIO_S16;
        spec.channels = 2;
        spec.samples = sample_size;
        spec.userdata = this;
        spec.callback = audio_callback;

        audio_device = SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 0);
        if (audio_device == 0) {
            spdlog::error("failed to open audio device: {}", SDL_GetError());
            return;
        }

        if (spec.freq != obtained.freq) {
            std::cerr << "frequeny mismatch" << std::endl;
        } else if (spec.format != obtained.format) {
            std::cerr << "format error" << std::endl;
        } else if (spec.channels != obtained.channels) {
            std::cerr << "channels mismatch" << std::endl;
        } else if (spec.samples != obtained.samples) {
            std::cerr << "sample size mismatch" << std::endl;
        }
    }

    void close_audio() {
        if (audio_device != 0) {
            SDL_CloseAudioDevice(audio_device);
        }
    }

    static void audio_callback(void *userdata, uint8_t *stream, int len) {
        spdlog::info("Audio callback requested length: {}", len);
        Implementation* impl = reinterpret_cast<Implementation*>(userdata);
        std::vector<int16_t> interleaved_buffer(sample_size * 2);
        int amount_read = impl->module->read_interleaved_stereo(sample_rate, sample_size,
                                                                interleaved_buffer.data());
        if (amount_read > 0) {
            spdlog::info("did read {} bytes from module", amount_read);
            SDL_memcpy(stream, reinterpret_cast<void*>(interleaved_buffer.data()),
                       amount_read * 2 * 2);
        }
    }

    std::shared_ptr<openmpt::module> module;
    SDL_AudioDeviceID audio_device = 0;
};

player::player()
    : m_impl(new Implementation()) {}

player::~player() {
    stop_playback();
}

void player::start_playback(std::shared_ptr<openmpt::module> mod) {
    m_impl->module = mod;
    spdlog::info("Starting playback of module {}",
                 mod->get_metadata("title"));
    SDL_PauseAudioDevice(m_impl->audio_device, 0);
}

void player::pause_playback() {
    SDL_PauseAudioDevice(m_impl->audio_device, 1);
}

void player::stop_playback() {
    pause_playback();
}
