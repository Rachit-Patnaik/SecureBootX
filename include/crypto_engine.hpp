#ifndef CRYPTO_ENGINE_HPP
#define CRYPTO_ENGINE_HPP

#include <vector>
#include <cstdint>

class CryptoEngine {
public:
    static std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);
    static bool rsa_verify(const std::vector<uint8_t>& hash, const std::vector<uint8_t>& sig, const std::vector<uint8_t>& pub_key);
};

#endif
