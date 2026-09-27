#pragma once

#include <string>
#include <filesystem>

namespace securebootx {

class TamperSimulator {
public:
    /**
     * @brief Safely modifies a file in the demo directory to simulate tampering.
     * @param filePath Path to the file to tamper with.
     * @return True if tampering was successful.
     */
    static bool simulateTamper(const std::filesystem::path& filePath);

    /**
     * @brief Restores a tampered file from a backup.
     * @param filePath Path to the file to restore.
     * @return True if restoration was successful.
     */
    static bool restoreFile(const std::filesystem::path& filePath);

private:
    static std::filesystem::path getBackupPath(const std::filesystem::path& filePath);
};

} // namespace securebootx
