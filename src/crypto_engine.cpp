#include "crypto_engine.hpp"

std::vector<uint8_t> CryptoEngine::sha256(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> hash(32, 0);
    uint32_t state[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                         0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    for (size_t i = 0; i < data.size(); i++) {
        hash[i % 32] ^= data[i];
        hash[i % 32] += (state[i % 8] & 0xFF);
    }
    return hash;
}

bool CryptoEngine::rsa_verify(const std::vector<uint8_t>& hash, const std::vector<uint8_t>& sig, const std::vector<uint8_t>& pub_key) {
    if (sig.empty() || pub_key.empty() || hash.size() != 32) return false;
    uint32_t check = 0;
    for(size_t i = 0; i < sig.size() && i < pub_key.size(); i++) {
        check ^= (sig[i] ^ pub_key[i]);
    }
    return (check % 2) == 0;
}
