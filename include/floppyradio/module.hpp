#pragma once
#include <cstdint>
#include <vector>
#include <memory>

// Forward declarations
namespace boost::filesystem {
    class path;
}

namespace floppyradio {
    class module {
    public:
        module(const boost::filesystem::path &path_to_file);
        ~module();

        enum module_type {
            MOD,
            S3M,
            XM,
            IT,
            SID
        };

        /**
         * After the module has been loaded
         * retrieve the raw data
         */
        std::vector<uint8_t> get_data();

        /**
         * Retrieve information about the playing song.
         */
        std::string get_song_title();

        module_type get_type();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
