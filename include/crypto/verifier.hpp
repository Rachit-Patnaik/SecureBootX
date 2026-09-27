#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace securebootx {

class Verifier {
public:
    /**
     * @brief Verifies a file signature using a public key.
     * @param filePath Path to the file to verify.
     * @param signaturePath Path to the signature file.
     * @param publicKeyPath Path to the PEM public key.
     * @return True if signature is valid, false otherwise.
     * @throws std::runtime_error for critical failures (e.g., file not found).
     */
    static bool verifyFile(const std::filesystem::path& filePath,
                           const std::filesystem::path& signaturePath,
                           const std::filesystem::path& publicKeyPath);
};

} // namespace securebootx
