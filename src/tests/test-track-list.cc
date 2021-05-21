#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include "floppyradio/TrackList.hpp"

const boost::filesystem::path test_dir = "/tmp/test-track-list/";

bool create_test_file(std::string name) {
    std::ofstream out(test_dir / boost::filesystem::path(name), out.trunc);
    return out.is_open();
}

bool create_test_files() {
    if (!boost::filesystem::exists(test_dir)) {
        if(!boost::filesystem::create_directories("/tmp/test-track-list/")) {
            std::cerr << "failed to create directories, permission error?" << std::endl;
            return false;
        }
    }

    return create_test_file("mod1.xm.bz2") && create_test_file("mod2.xm");
}

TEST(can_detect_tracks, can_detect_files) {
    ASSERT_TRUE(create_test_files());

    EXPECT_EQ(floppyradio::get_track_list(test_dir).size(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
