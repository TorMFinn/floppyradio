#include "floppyradio/sdl_audio_device.hpp"
#include "floppyradio/player.hpp"
#include "floppyradio/track_list.hpp"
#include "floppyradio/module_loader.hpp"

#include "libopenmpt/libopenmpt.hpp"

#include "sidplayfp/sidplayfp.h"
#include "sidplayfp/SidTune.h"
#include "sidplayfp/SidInfo.h"
#include "sidplayfp/builders/residfp.h"

#include "spdlog/spdlog.h"
#include "SDL2/SDL.h"
#include <chrono>
#include <boost/signals2.hpp>

constexpr int sample_rate = 48000;
constexpr int sample_size = 4096;

using namespace floppyradio;

class SidPlayer {
public:
    SidPlayer() {
        // Create the audio device
        m_audio_dev = std::make_shared<sdl_audio_device>();
        
        // Use reSID
        m_resid = std::make_shared<ReSIDfpBuilder>("floppyradio");
        m_resid->create(m_engine.info().maxsids());

        // Configure SID engine
        SidConfig cfg;
        cfg.frequency = m_audio_dev->get_sample_rate();
        cfg.samplingMethod = SidConfig::INTERPOLATE;
        cfg.fastSampling = true;
        cfg.playback = SidConfig::STEREO;
        cfg.sidEmulation = m_resid.get();

        if (!m_engine.config(cfg)) {
            throw std::runtime_error("Failed to configure SID emulation");
        }

        m_audio_dev->on_audio_wanted([&]() -> audio_device::audio_data {
            return data_wanted_handler();
        });
    }

    void start_playback(std::shared_ptr<SidTune> tune) {
        // Play the first song
        if (!tune) {
            return;
        }

        if (!tune->getStatus()) {
            std::cerr << "Tune status is bad?: " << tune->statusString() << std::endl;
            return;
        }

        tune->selectSong(0);
        if (!m_engine.load(tune.get())) {
            std::cerr << "failed to load tune: " << m_engine.error() << std::endl;
            return;
        }

        m_audio_dev->start();
    }

    audio_device::audio_data data_wanted_handler() {
        std::vector<int16_t> buffer(m_audio_dev->get_sample_size() * 2);
        if(m_engine.play(buffer.data(), buffer.size()) > 0) {
            return buffer;
        }
        return {};
    }

private:
    std::vector<uint8_t> load_rom(const char *path, size_t romSize);
    sidplayfp m_engine;
    std::shared_ptr<ReSIDfpBuilder> m_resid;

    // C64 ROM data
    std::vector<uint8_t> m_kernal, m_basic, m_chargen;

    audio_device_ptr m_audio_dev;
};

class mod_player {
public:
    mod_player() {
        m_audio_dev = std::make_shared<sdl_audio_device>();
        m_audio_dev->on_audio_wanted([&]() -> audio_device::audio_data {
            return data_wanted_handler();
        });
    }

    ~mod_player() = default;

    void start_playback(std::shared_ptr<openmpt::module> mod) {
        module = mod;
        spdlog::info("Starting playback of module {}", mod->get_metadata("title"));
        m_audio_dev->start();
    }

    void pause_playback() {
        m_audio_dev->stop();
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

    audio_device::audio_data data_wanted_handler() {
        std::vector<int16_t> interleaved_buffer(sample_size * 2);
        int amount_read = module->read_interleaved_stereo(sample_rate, sample_size,
                                                                interleaved_buffer.data());
        if (amount_read > 0) {
            return std::move(interleaved_buffer);
        } else {
            // Song is most likely complete
            stop_playback();
            on_song_complete();
        }
        return {};
    }


    std::shared_ptr<openmpt::module> module;
    audio_device_ptr m_audio_dev;

    boost::signals2::signal<void ()> on_song_complete;
};

struct player::Data {
    mod_player mp;
    SidPlayer sid_player;
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

    auto variant = m_data->loader.load_from_track(m_data->tracks[track]);
    if (std::holds_alternative<SidTunePtr>(variant)) {
        auto sid_tune = std::get<SidTunePtr>(variant);
        m_data->sid_player.start_playback(sid_tune);
    } else {
        auto module = std::get<ModulePtr>(variant);
        m_data->mp.start_playback(module);
        m_data->current_track = track;
    }
    m_data->current_track = track;
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
