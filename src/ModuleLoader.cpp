#include "floppyradio/ModuleLoader.hpp"

#include <libopenmpt/libopenmpt.hpp>
#include <sidplayfp/SidTune.h>

#include <spdlog/spdlog.h>
#include <iomanip>
#include <bzlib.h>
#include <boost/filesystem.hpp>

using namespace floppyradio;
using namespace boost;

class bz2file {
public:
    bz2file() {}
    virtual ~bz2file() {
        int bzerror;
        BZ2_bzReadClose(&bzerror, bz_file_);
        fclose(file_);
    }

    bool open(const filesystem::path &path) {
        file_ = fopen(path.c_str(), "r");
        if (!file_) {
            spdlog::error("failed to open bz2 file: {}", std::strerror(errno));
            return false;
        }

        int bzerror;
        bz_file_ = BZ2_bzReadOpen(&bzerror, file_, 0, 0, nullptr, 0);
        if (!bz_file_ || bzerror != BZ_OK) {
            spdlog::error("failed to open bz2 file, bzReadOpen");
            log_bzerror(bzerror);
            return false;
        }

        return true;
    }

    /**
     * decompress bz2 data into vector
     */
    bool decompress(std::vector<uint8_t> &output) {
        int amount_read = 0;
        int read_offset = 0;
        int bzerror = BZ_OK;
        char input_buf[1024];
        while(bzerror == BZ_OK) {
            amount_read = BZ2_bzRead(&bzerror, bz_file_, input_buf, 1024);
            spdlog::info("read {}", amount_read);
            if (bzerror == BZ_OK || BZ_STREAM_END) {
                output.insert(output.end(), input_buf, input_buf + amount_read);
            }
        }

        if (bzerror != BZ_STREAM_END) {
            spdlog::error("error reading data from bz2 file");
            return false;
        }

        log_bzerror(bzerror);

        return true;
    }

private:
    void log_bzerror(int error) {
        switch(error) {
        case BZ_CONFIG_ERROR:
            spdlog::error("BZ_CONFIG_ERROR");
            break;
        case BZ_PARAM_ERROR:
            spdlog::error("BZ_PARAM_ERROR");
            break;
        case BZ_IO_ERROR:
            spdlog::error("BZ_IO_ERROR");
            break;
        case BZ_MEM_ERROR:
            spdlog::error("BZ_MEM_ERROR");
            break;
        case BZ_OUTBUFF_FULL:
            spdlog::error("BZ_OUTBUFF_FULL");
            break;
        default:
            spdlog::warn("unhandled bzip2 error: {}", error);
        }
    }
    BZFILE* bz_file_;
    FILE* file_;
};

struct ModuleLoader::Impl {
    std::vector<uint8_t> load_into_memory (const filesystem::path &path) {
        if (!filesystem::exists(path)) {
            spdlog::error("module {} does not exist", path.string());
            return {};
        }

        if (path.extension() == ".bz2") {
            return load_bzip2(path);
        } else {
            return load_normal(path);
        }
    }

    std::vector<uint8_t> load_normal(const filesystem::path &path) {
        std::fstream ifile(path.string(), std::ios::binary | std::ios::in);
        if (!ifile.is_open()) {
            spdlog::error("failed to open file {}", path.string());
            return {};
        }
        std::vector<uint8_t> module_mem;
        module_mem.resize(filesystem::file_size(path));
        ifile.read(reinterpret_cast<char*>(module_mem.data()), filesystem::file_size(path));
        spdlog::info("memory_size: {}", module_mem.size());
        return module_mem;
    }

    std::vector<uint8_t> load_bzip2(const filesystem::path &path) {
        bz2file bz2;
        if (!bz2.open(path)) {
            return {};
        }
        std::vector<uint8_t> output;
        if (bz2.decompress(output)) {
            spdlog::info("OUTPUT SIZE: {}", output.size());
            return output;
        }
        return {};
    }
};

ModuleLoader::ModuleLoader()
    : impl_(new Impl()) {}

ModuleLoader::~ModuleLoader() = default;

std::variant<
    ModulePtr,
    SidTunePtr> ModuleLoader::LoadFromTrack(const track_t &track) {
    auto memory_vector = impl_->load_into_memory(track.path);
    if (memory_vector.size() == 0) {
        return {};
    }

    if (track.track_type == track_t::type::SID) {
        return std::make_shared<SidTune>(memory_vector.data(), memory_vector.size());
    } else {
        return std::make_shared<openmpt::module>(memory_vector);
    }
}
