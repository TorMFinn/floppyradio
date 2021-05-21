#include "floppyradio/SoftwareDisplay.hpp"

namespace floppyradio {
    DisplayPtr DisplayFactory::Create() {
        return std::make_shared<SoftDisplay>();
    }
}
