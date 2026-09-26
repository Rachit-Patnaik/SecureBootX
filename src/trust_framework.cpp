#include "securebootx/trust_framework.hpp"
#include "securebootx/logger.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool TrustFramework::loadPolicy(const std::string& config_file) {
    if (!fs::exists(config_file)) {
        Logger::getInstance().log(LogLevel::WARNING, "Policy config file not found: " + config_file + ", using default secure policy.");
        return true;
    }

    std::ifstream file(config_file);
    if (!file.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto extractLong = [&content](const std::string& key, long default_val) -> long {
        size_t pos = content.find("\"" + key + "\"");
        if (pos != std::string::npos) {
            size_t colon = content.find(':', pos);
            if (colon != std::string::npos) {
                size_t start = content.find_first_of("0123456789", colon);
                if (start != std::string::npos) {
                    return std::stol(content.substr(start));
                }
            }
        }
        return default_val;
    };

    policy_.minimum_allowed_version = (uint32_t)extractLong("minimum_allowed_version", 1);
    Logger::getInstance().log(LogLevel::INFO, "Loaded Security Policy: Minimum Allowed Version = " + std::to_string(policy_.minimum_allowed_version));

    return true;
}

bool TrustFramework::checkAntiRollback(uint32_t firmware_version) {
    if (firmware_version < policy_.minimum_allowed_version) {
        Logger::getInstance().log(LogLevel::SECURITY, "Anti-Rollback Violation! Image Version (" +
            std::to_string(firmware_version) + ") < Minimum Required Version (" +
            std::to_string(policy_.minimum_allowed_version) + ")");
        return false;
    }
    Logger::getInstance().log(LogLevel::SUCCESS, "Anti-Rollback Version Check Passed (Version " + std::to_string(firmware_version) + ").");
    return true;
}

bool TrustFramework::isKeyRevoked(const std::string& key_fingerprint) const {
    return policy_.revoked_key_fingerprints.find(key_fingerprint) != policy_.revoked_key_fingerprints.end();
}
