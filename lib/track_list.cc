#include "floppyradio/track_list.hpp"
#include "spdlog/spdlog.h"
#include <set>
#include <boost/algorithm/string/trim.hpp>

namespace floppyradio {

track_t::type extension_to_track_type(const std::string &extension) {
    if (extension == ".mod" || extension == ".MOD") {
        return track_t::type::MOD;
    } else if (extension == ".xm" || extension == ".XM") {
        return track_t::type::XM;
    } else if (extension == ".s3m" || extension == ".S3M") {
        return track_t::type::S3M;
    } else if (extension == ".it" || extension == ".IT") {
        return track_t::type::IT;
    } else if (extension == ".sid" || extension == ".SID") {
        return track_t::type::SID;
    }
    return track_t::type::UNKNOWN;
}

const std::set<std::string> known_extensions = {
    ".mod",
    ".xm",
    ".s3m",
    ".it",
    ".sid"
};

track_list get_track_list(boost::filesystem::path directory) {
    track_list tracks;

    if (!boost::filesystem::is_directory(directory)) {
        spdlog::error("path : {} is not a directory", directory.string());
        return {};
    }

    for (auto file : boost::filesystem::directory_iterator(directory)) {
        auto file_path = file.path();
        std::string extension = file.path().extension().string();

        if (extension == ".bz2") {
            // remove .bz2 part
            file_path.replace_extension("");
            extension = file_path.extension().string();
        }

        auto type = extension_to_track_type(extension);
        if (type != track_t::type::UNKNOWN) {
            tracks.push_back(track_t{file_path, type});
        }
    }

    return tracks;
}

}
