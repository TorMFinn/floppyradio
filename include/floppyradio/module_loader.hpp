#pragma once
#include <cstdint>
#include <vector>
#include <memory>

// Forward declarations
namespace boost::filesystem {
    class path;
}

namespace openmpt {
    class module;
}

namespace floppyradio {
    class module_loader {
    public:
        module_loader();
        virtual ~module_loader();
        std::shared_ptr<openmpt::module> load_from_file(const boost::filesystem::path &path);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
