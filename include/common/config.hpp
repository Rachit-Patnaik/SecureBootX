#pragma once

#include <string>
#include <filesystem>

namespace securebootx {

struct Config {
    std::string manifestPath = "data/manifest.json";
    std::string logPath = "data/measurement_log.json";
    std::string publicKeyPath = "keys/public_key.pem";
};

class ConfigManager {
public:
    static Config loadConfig(const std::filesystem::path& configPath);
};

} // namespace securebootx
