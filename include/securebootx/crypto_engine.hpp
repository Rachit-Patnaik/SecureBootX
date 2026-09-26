#ifndef SECUREBOOTX_CRYPTO_ENGINE_HPP
#define SECUREBOOTX_CRYPTO_ENGINE_HPP

#include <string>
#include <vector>
#include <cstdint>

class CryptoEngine {
public:
    CryptoEngine() = default;

    // Computes SHA-256 hash of binary buffer
    static std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);
    static std::string sha256HexString(const std::vector<uint8_t>& data);

    // Cryptographic signature generation and verification
    static bool signData(const std::vector<uint8_t>& data, const std::string& private_key_pem, std::vector<uint8_t>& signature_out);
    static bool verifySignature(const std::vector<uint8_t>& data, const std::vector<uint8_t>& signature, const std::string& public_key_pem);
};

#endif // SECUREBOOTX_CRYPTO_ENGINE_HPP
