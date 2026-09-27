#include <gtest/gtest.h>
#include "crypto/signer.hpp"
#include "crypto/verifier.hpp"
#include "crypto/hash.hpp"
#include <fstream>
#include <filesystem>

using namespace securebootx;

class CryptoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a dummy file to sign
        std::ofstream ofs("crypto_test.bin", std::ios::binary);
        ofs << "Secret Boot Data";
        ofs.close();

        // Note: In a real test, we would use a pre-generated key pair in a test_keys/ folder
        // Since we are in a controlled env, we'll assume keys exist or mock them.
    }

    void TearDown() override {
        std::filesystem::remove("crypto_test.bin");
        std::filesystem::remove("crypto_test.bin.sig");
    }
};

TEST_F(CryptoTest, SignAndVerifySuccess) {
    // This test requires keys/private_key.pem and keys/public_key.pem to exist
    if (!std::filesystem::exists("keys/private_key.pem")) {
        GTEST_SKIP() << "Skipping: private_key.pem not found. Run keygen first.";
    }

    EXPECT_TRUE(Signer::signFile("crypto_test.bin", "keys/private_key.pem", "crypto_test.bin.sig"));
    EXPECT_TRUE(Verifier::verifyFile("crypto_test.bin", "crypto_test.bin.sig", "keys/public_key.pem"));
}

TEST_F(CryptoTest, VerifyTamperedFileFails) {
    if (!std::filesystem::exists("keys/private_key.pem")) {
        GTEST_SKIP() << "Skipping: private_key.pem not found.";
    }

    Signer::signFile("crypto_test.bin", "keys/private_key.pem", "crypto_test.bin.sig");

    // Tamper with file
    std::fstream file("crypto_test.bin", std::ios::in | std::ios::out | std::ios::binary);
    file.seekp(0);
    file.put('X');
    file.close();

    EXPECT_FALSE(Verifier::verifyFile("crypto_test.bin", "crypto_test.bin.sig", "keys/public_key.pem"));
}
