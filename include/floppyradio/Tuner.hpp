#pragma once
#include <vector>
#include <string>

namespace floppyradio {

    struct station {
        std::string name;
        double frequency;
    };

    using station_list = std::vector<station>;

    class Tuner {
    public:
        Tuner();
        virtual ~Tuner();

        virtual void tune_to(const station &station) = 0;
        virtual station_list GetStationList() = 0;

    };
}
