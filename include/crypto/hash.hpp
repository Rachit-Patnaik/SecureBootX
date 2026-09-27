#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace securebootx {

class Hash {
public:
    /**
     * @brief Calculates the SHA-256 hash of a file.
     * @param filePath Path to the file to hash.
     * @return Lowercase hexadecimal SHA-256 string.
     * @throws std::runtime_error if file cannot be read or hashing fails.
     */
    static std::string calculateSHA256(const std::filesystem::path& filePath);

    /**
     * @brief Calculates the SHA-256 hash of a raw data buffer.
     * @param data Vector of bytes to hash.
     * @return Lowercase hexadecimal SHA-256 string.
     * @throws std::runtime_error if hashing fails.
     */
    static std::string calculateSHA256(const std::vector<uint8_t>& data);

private:
    static std::string bytesToHexString(const unsigned char* bytes, size_t length);
};

} // namespace securebootx
