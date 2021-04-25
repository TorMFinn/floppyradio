#include "floppyradio/audio_device.hpp"
#include "floppyradio/player.hpp"
#include "floppyradio/track_list.hpp"
#include "floppyradio/module_loader.hpp"

#include "libopenmpt/libopenmpt.hpp"
#include "spdlog/spdlog.h"
#include "SDL2/SDL.h"
#include <chrono>
#include <boost/signals2.hpp>

constexpr int sample_rate = 48000;
constexpr int sample_size = 4096;

using namespace floppyradio;

class sid_player {
public:
    sid_player() {
    }

private:
    audio
    
};

class mod_player {
public:
    mod_player() {
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

    ~mod_player() {
        if (audio_device != 0) {
            SDL_CloseAudioDevice(audio_device);
        }
    }

    void start_playback(std::shared_ptr<openmpt::module> mod) {
        module = mod;
        spdlog::info("Starting playback of module {}", mod->get_metadata("title"));
        SDL_PauseAudioDevice(audio_device, 0);
    }

    void pause_playback() {
        SDL_PauseAudioDevice(audio_device, 1);
    }

    void stop_playback() {
        pause_playback();
    }

    /*
    void update_playback_info(std::shared_ptr<openmpt::module> module) {
        playback_info info;
        info.duration_seconds = module->get_duration_seconds();
        info.current_position = module->get_position_seconds();
        info.track_name = module->get_metadata("title");
        parent->on_info(info);
    }
    */

    static void audio_callback(void *userdata, uint8_t *stream, int len) {
        spdlog::debug("Audio callback requested length: {}", len);
        mod_player* mp = reinterpret_cast<mod_player*>(userdata);
        std::vector<int16_t> interleaved_buffer(sample_size * 2);
        int amount_read = mp->module->read_interleaved_stereo(sample_rate, sample_size,
                                                                interleaved_buffer.data());
        if (amount_read > 0) {
            spdlog::debug("did read {} bytes from module", amount_read);
            SDL_memcpy(stream, reinterpret_cast<void*>(interleaved_buffer.data()),
                       amount_read * 2 * 2);
            //mp->update_playback_info(mp->module);
        } else if (amount_read == 0) {
            mp->stop_playback();
            mp->on_song_complete();
        }
    }

    std::shared_ptr<openmpt::module> module;
    SDL_AudioDeviceID audio_device = 0;

    // Do not play tricks with this one!
    //player* parent;
    //std::chrono::time_point<std::chrono::steady_clock> playback_start;

    boost::signals2::signal<void ()> on_song_complete;
};

struct player::Data {
    mod_player mp;
    module_loader loader;

    display* disp;
    track_list tracks;
    int current_track = 0;

    bool repeat = false;
};

player::player()
    : m_data(new Data()) {
    m_data->mp.on_song_complete.connect([&]() {
        if (m_data->repeat) {
            spdlog::info("repeat");
            play_song(m_data->current_track);
        } else {
            spdlog::info("song complete, playing next");
            play_next_song();
        }
    });
}

player::~player() {
    stop();
}

void player::set_display(display* display) {
    m_data->disp = display;
}

void player::play_song(int track) {
    if (track > static_cast<int>(m_data->tracks.size())) {
        spdlog::error("Track out of range");
        return;
    }

    auto module = m_data->loader.load_from_file(m_data->tracks[track]);
    if (module) {
        m_data->mp.start_playback(module);
        m_data->current_track = track;
    }
}

void player::play_next_song() {
    if (m_data->current_track + 1 < static_cast<int>(m_data->tracks.size())) {
        play_song(++m_data->current_track);
    }
}

void player::play_previous_song() {
    if (m_data->current_track > 0 && !m_data->tracks.empty()) {
        play_song(--m_data->current_track);
    }
}

void player::set_repeat(bool enable) {
    m_data->repeat = enable;
}

int player::open_directory(boost::filesystem::path dir) {
    auto tracks = get_track_list(dir);
    if (!tracks.empty()) {
        m_data->tracks = tracks;
    }
    return tracks.size();
}

void player::close_current_directory() {
    m_data->mp.stop_playback();
    m_data->tracks = track_list();
    m_data->current_track = 0;
}

void player::stop() {
    m_data->mp.stop_playback();
}

void player::pause() {
    m_data->mp.pause_playback();
}
