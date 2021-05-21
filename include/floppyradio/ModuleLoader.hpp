#pragma once
#include "floppyradio/TrackList.hpp"
#include <cstdint>
#include <vector>
#include <memory>
#include <variant>

// Forward declarations
namespace boost::filesystem {
    class path;
}

namespace openmpt {
    class module;
}

class SidTune;

using ModulePtr = std::shared_ptr<openmpt::module>;
using SidTunePtr = std::shared_ptr<SidTune>;

namespace floppyradio {
    class ModuleLoader {
    public:
        ModuleLoader();
        virtual ~ModuleLoader();

        std::variant<
            ModulePtr,
            SidTunePtr> LoadFromTrack(const track_t &track);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
