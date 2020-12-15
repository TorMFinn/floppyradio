#pragma once
#include <boost/filesystem.hpp>

using track_list = std::vector<boost::filesystem::path>;
track_list get_track_list(boost::filesystem::path directory);
