#include "common/config.hpp"
#include <iostream>
#include <fstream>

namespace securebootx {

Config ConfigManager::loadConfig(const std::filesystem::path& configPath) {
    Config config;
    // Simple implementation: returns defaults.
    // In a full version, we would parse config/securebootx.json
    return config;
}

} // namespace securebootx
