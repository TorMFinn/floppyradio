#pragma once
#include "floppyradio/track_list.hpp"
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
    class module_loader {
    public:
        module_loader();
        virtual ~module_loader();

        std::variant<
            ModulePtr,
            SidTunePtr> load_from_track(const track_t &track);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
