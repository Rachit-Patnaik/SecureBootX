#include "simulation/tamper_simulator.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace securebootx {

std::filesystem::path TamperSimulator::getBackupPath(const std::filesystem::path& filePath) {
    return filePath.string() + ".bak";
}

bool TamperSimulator::simulateTamper(const std::filesystem::path& filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "[!] TamperSimulator: File not found: " << filePath << std::endl;
        return false;
    }

    // SECURITY CHECK: Only allow tampering in demo/ or data/ directories
    std::string pathStr = filePath.string();
    if (pathStr.find("demo/") == std::string::npos && pathStr.find("data/") == std::string::npos) {
        std::cerr << "[!] SECURITY ERROR: Tampering is only allowed in demo/ or data/ directories!" << std::endl;
        return false;
    }

    try {
        // 1. Create Backup
        std::filesystem::path backup = getBackupPath(filePath);
        if (!std::filesystem::exists(backup)) {
            std::filesystem::copy_file(filePath, backup, std::filesystem::copy_options::overwrite_existing);
        }

        // 2. Modify the file
        // We open the file and flip a few bytes to ensure the hash changes
        std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return false;

        file.seekp(0, std::ios::beg);
        char tamperByte = 0x90; // NOP or random byte
        file.put(tamperByte);
        file.close();

        std::cout << "[+] Successfully tampered with " << filePath << " (byte 0 modified)" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[!] Tamper error: " << e.what() << std::endl;
        return false;
    }
}

bool TamperSimulator::restoreFile(const std::filesystem::path& filePath) {
    std::filesystem::path backup = getBackupPath(filePath);
    if (!std::filesystem::exists(backup)) {
        std::cerr << "[!] No backup found for " << filePath << std::endl;
        return false;
    }

    try {
        std::filesystem::copy_file(backup, filePath, std::filesystem::copy_options::overwrite_existing);
        std::cout << "[+] Restored " << filePath << " from backup." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[!] Restore error: " << e.what() << std::endl;
        return false;
    }
}

} // namespace securebootx
