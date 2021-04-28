#include "floppyradio/SoftDisplay.hpp"

namespace floppyradio {
    DisplayPtr DisplayFactory::Create() {
        return std::make_shared<SoftDisplay>();
    }
}
