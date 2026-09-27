#include <gtest/gtest.h>
#include "crypto/hash.hpp"
#include <fstream>
#include <filesystem>

using namespace securebootx;

class HashTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream ofs("test_file.bin", std::ios::binary);
        ofs << "Hello SecureBootX!";
        ofs.close();
    }

    void TearDown() override {
        std::filesystem::remove("test_file.bin");
    }
};

TEST_F(HashTest, CalculateFileHash) {
    // Expected SHA-256 for "Hello SecureBootX!"
    std::string expected = "3748f303687513196897f1c73730425977a4306f59d3160a8609f962545746b2";
    EXPECT_EQ(Hash::calculateSHA256("test_file.bin"), expected);
}

TEST_F(HashTest, CalculateDataHash) {
    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o'};
    std::string expected = "185f8db32271fe25f561a6fc938b297a97df3556f28a5d262a117a78565bfdee";
    EXPECT_EQ(Hash::calculateSHA256(data), expected);
}

TEST_F(HashTest, FileNotFoundThrows) {
    EXPECT_THROW(Hash::calculateSHA256("non_existent_file"), std::runtime_error);
}
