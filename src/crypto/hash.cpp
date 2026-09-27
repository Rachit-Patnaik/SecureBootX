#include "crypto/hash.hpp"
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdexcept>

namespace securebootx {

std::string Hash::calculateSHA256(const std::filesystem::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file for hashing: " + filePath.string());
    }

    std::vector<uint8_t> buffer(8192);
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) throw std::runtime_error("Failed to create EVP_MD_CTX");

    try {
        if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr)) {
            throw std::runtime_error("EVP_DigestInit_ex failed");
        }

        while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || file.gcount() > 0) {
            if (1 != EVP_DigestUpdate(mdctx, buffer.data(), file.gcount())) {
                throw std::runtime_error("EVP_DigestUpdate failed");
            }
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;
        if (1 != EVP_DigestFinal_ex(mdctx, hash, &lengthOfHash)) {
            throw std::runtime_error("EVP_DigestFinal_ex failed");
        }

        EVP_MD_CTX_free(mdctx);
        return bytesToHexString(hash, lengthOfHash);
    } catch (...) {
        EVP_MD_CTX_free(mdctx);
        throw;
    }
}

std::string Hash::calculateSHA256(const std::vector<uint8_t>& data) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) throw std::runtime_error("Failed to create EVP_MD_CTX");

    try {
        if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr)) {
            throw std::runtime_error("EVP_DigestInit_ex failed");
        }

        if (1 != EVP_DigestUpdate(mdctx, data.data(), data.size())) {
            throw std::runtime_error("EVP_DigestUpdate failed");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;
        if (1 != EVP_DigestFinal_ex(mdctx, hash, &lengthOfHash)) {
            throw std::runtime_error("EVP_DigestFinal_ex failed");
        }

        EVP_MD_CTX_free(mdctx);
        return bytesToHexString(hash, lengthOfHash);
    } catch (...) {
        EVP_MD_CTX_free(mdctx);
        throw;
    }
}

std::string Hash::bytesToHexString(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        ss << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return ss.str();
}

} // namespace securebootx
