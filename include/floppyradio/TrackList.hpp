#pragma once
#include <boost/filesystem.hpp>

namespace floppyradio {
    struct track_t {
        boost::filesystem::path path;
        enum class type {
            MOD,
            S3M,
            XM,
            IT,
            SID,
            UNKNOWN
        } track_type;
    };

    using TrackList = std::vector<track_t>;
    TrackList get_track_list(boost::filesystem::path directory);
}
