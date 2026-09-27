#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <openssl/evp.h>

namespace securebootx {

class Signer {
public:
    /**
     * @brief Signs a file using a private key.
     * @param filePath Path to the file to sign.
     * @param privateKeyPath Path to the PEM private key.
     * @param signaturePath Path where the signature will be saved.
     * @return True if signing was successful.
     */
    static bool signFile(const std::filesystem::path& filePath,
                         const std::filesystem::path& privateKeyPath,
                         const std::filesystem::path& signaturePath);

private:
    static std::vector<uint8_t> readFile(const std::filesystem::path& path);
    static void writeFile(const std::filesystem::path& path, const std::vector<uint8_t>& data);
};

} // namespace securebootx
