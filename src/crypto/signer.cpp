#include "crypto/signer.hpp"
#include "crypto/hash.hpp"
#include <openssl/pem.h>
#include <openssl/err.h>
#include <fstream>
#include <stdexcept>

namespace securebootx {

std::vector<uint8_t> Signer::readFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Could not open file: " + path.string());
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + path.string());
    }
    return buffer;
}

void Signer::writeFile(const std::filesystem::path& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Could not open file for writing: " + path.string());
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

bool Signer::signFile(const std::filesystem::path& filePath,
                     const std::filesystem::path& privateKeyPath,
                     const std::filesystem::path& signaturePath) {

    // 1. Calculate hash of the file
    std::string fileHash = Hash::calculateSHA256(filePath);
    std::vector<uint8_t> hashBytes(fileHash.begin(), fileHash.end()); // This is the hex string, but for signing we should use raw bytes

    // Correction: We need the raw bytes of the hash for the actual signing operation
    // Let's implement a helper in Hash to get raw bytes or just recalculate here.
    // For simplicity in this implementation, we'll use a simplified approach:
    // we hash the file and sign that digest.

    // Load Private Key
    FILE* keyFile = fopen(privateKeyPath.string().c_str(), "r");
    if (!keyFile) throw std::runtime_error("Could not open private key file");
    EVP_PKEY* pkey = PEM_read_PrivateKey(keyFile, nullptr, nullptr, nullptr);
    fclose(keyFile);
    if (!pkey) throw std::runtime_error("Failed to load private key");

    // Create Signature Context
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to create MD context");
    }

    try {
        if (1 != EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey)) {
            throw std::runtime_error("EVP_DigestSignInit failed");
        }

        // Re-hash the file inside the sign context
        std::ifstream file(filePath, std::ios::binary);
        std::vector<uint8_t> buffer(8192);
        while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || file.gcount() > 0) {
            if (1 != EVP_DigestSignUpdate(mdctx, buffer.data(), file.gcount())) {
                throw std::runtime_error("EVP_DigestSignUpdate failed");
            }
        }

        size_t sigLen = 0;
        if (1 != EVP_DigestSignFinal(mdctx, nullptr, &sigLen)) {
            throw std::runtime_error("EVP_DigestSignFinal (length) failed");
        }

        std::vector<uint8_t> signature(sigLen);
        if (1 != EVP_DigestSignFinal(mdctx, signature.data(), &sigLen)) {
            throw std::runtime_error("EVP_DigestSignFinal (data) failed");
        }

        writeFile(signaturePath, signature);

        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        return true;
    } catch (...) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw;
    }
}

} // namespace securebootx
