#include "crypto/verifier.hpp"
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace securebootx {

bool Verifier::verifyFile(const std::filesystem::path& filePath,
                         const std::filesystem::path& signaturePath,
                         const std::filesystem::path& publicKeyPath) {

    // 1. Load Public Key
    FILE* keyFile = fopen(publicKeyPath.string().c_str(), "r");
    if (!keyFile) throw std::runtime_error("Could not open public key file");
    EVP_PKEY* pkey = PEM_read_PUBKEY(keyFile, nullptr, nullptr, nullptr);
    fclose(keyFile);
    if (!pkey) throw std::runtime_error("Failed to load public key");

    // 2. Read Signature
    std::ifstream sigFile(signaturePath, std::ios::binary | std::ios::ate);
    if (!sigFile) throw std::runtime_error("Could not open signature file");
    std::streamsize sigSize = sigFile.tellg();
    sigFile.seekg(0, std::ios::beg);
    std::vector<uint8_t> signature(sigSize);
    if (!sigFile.read(reinterpret_cast<char*>(signature.data()), sigSize)) {
        throw std::runtime_error("Failed to read signature");
    }

    // 3. Setup Verification Context
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        EVP_PKEY_free(pkey);
        throw std::runtime_error("Failed to create MD context");
    }

    try {
        if (1 != EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey)) {
            throw std::runtime_error("EVP_DigestVerifyInit failed");
        }

        // Hash the file
        std::ifstream file(filePath, std::ios::binary);
        std::vector<uint8_t> buffer(8192);
        while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || file.gcount() > 0) {
            if (1 != EVP_DigestVerifyUpdate(mdctx, buffer.data(), file.gcount())) {
                throw std::runtime_error("EVP_DigestVerifyUpdate failed");
            }
        }

        int result = EVP_DigestVerifyFinal(mdctx, signature.data(), signature.size());

        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);

        return (result == 1);
    } catch (...) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        throw;
    }
}

} // namespace securebootx
