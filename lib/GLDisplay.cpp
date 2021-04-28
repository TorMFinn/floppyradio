#include "floppyradio/GLDisplay.hpp"

namespace floppyradio {
    struct GLDisplay::Data {

    };

    GLDisplay::GLDisplay()
        : m_data(new Data()) {}

    GLDisplay::~GLDisplay() = default;
    
    void GLDisplay::SetTrackName(const std::string &name) {

    }

    void GLDisplay::SetDuration(int seconds) {

    }

    void GLDisplay::SetCurrentTime(int seconds) {
    }
}
