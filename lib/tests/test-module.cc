#include <gtest/gtest.h>
#include <memory>
#include "paths.hpp"
#include "floppyradio/module_loader.hpp"

TEST(load_module, normal_file) {
    floppyradio::module_loader loader;
    auto module = loader.load_from_file(floppyradio::test::module_path);
    EXPECT_TRUE(module != nullptr);
}

TEST(load_module, bzip2_compressed_file) {
    floppyradio::module_loader loader;
    auto module = loader.load_from_file(floppyradio::test::module_path_bzip2);
    EXPECT_TRUE(module != nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
