#pragma once

#include "boot/boot_component.hpp"
#include <vector>
#include <string>
#include <filesystem>

namespace securebootx {

class Manifest {
public:
    Manifest() = default;

    /**
     * @brief Loads the trusted manifest from a JSON file.
     * @param manifestPath Path to the manifest.json.
     * @return True if loaded successfully.
     */
    bool load(const std::filesystem::path& manifestPath);

    /**
     * @brief Saves the current manifest to a JSON file.
     * @param manifestPath Path to save.
     */
    void save(const std::filesystem::path& manifestPath) const;

    void addComponent(const BootComponent& component);
    const std::vector<BootComponent>& getComponents() const { return components; }

    void setPublicKeyPath(const std::string& path) { publicKeyPath = path; }
    std::string getPublicKeyPath() const { return publicKeyPath; }

private:
    std::vector<BootComponent> components;
    std::string version = "1.0";
    std::string algorithm = "SHA256";
    std::string publicKeyPath;
};

} // namespace securebootx
