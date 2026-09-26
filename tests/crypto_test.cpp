#include "securebootx/crypto_engine.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::string test_data_str = "SecureBootX Cryptographic Verification Payload";
    std::vector<uint8_t> data(test_data_str.begin(), test_data_str.end());

    auto hash_hex = CryptoEngine::sha256HexString(data);
    assert(!hash_hex.empty());
    assert(hash_hex.length() == 64);

    std::vector<uint8_t> signature;
    bool sign_ok = CryptoEngine::signData(data, "", signature);
    assert(sign_ok);
    assert(signature.size() == 256);

    bool verify_ok = CryptoEngine::verifySignature(data, signature, "");
    assert(verify_ok);

    std::cout << "[TEST CRYPTO] SHA-256 Digest: " << hash_hex << std::endl;
    std::cout << "[TEST CRYPTO] Cryptographic Engine test PASSED." << std::endl;
    return 0;
}
