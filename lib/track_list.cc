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
        std::string extension = file.path().extension().string();
        if (extension == ".bz2") {
            // remove .bz2 part
            extension.erase(extension.find_last_of(".bz2"));
        }
        if (known_extensions.contains(extension)) {
            tracks.push_back(file.path());
        }
    }

    return tracks;
}
