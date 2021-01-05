#include "floppyradio/track_list.hpp"
#include "spdlog/spdlog.h"
#include <set>
#include <boost/algorithm/string/trim.hpp>

const std::set<std::string> known_extensions = {
    ".mod",
    ".xm",
    ".s3m",
    ".it"
};

std::vector<boost::filesystem::path> get_track_list(boost::filesystem::path directory) {
    std::vector<boost::filesystem::path> tracks;

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
        if (known_extensions.contains(extension)) {
            tracks.push_back(file.path());
        }
    }

    return tracks;
}
