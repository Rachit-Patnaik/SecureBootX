#pragma once

#include <string>
#include <filesystem>
#include <vector>

namespace securebootx {

enum class ComponentType {
    FIRMWARE,
    BOOTLOADER,
    KERNEL,
    INITRAMFS,
    ROOTFS,
    UNKNOWN
};

std::string componentTypeToString(ComponentType type);
ComponentType stringToComponentType(const std::string& typeStr);

struct BootComponent {
    std::string name;
    ComponentType type;
    std::filesystem::path path;
    std::string expectedHash;
    std::filesystem::path signaturePath;
    int pcrIndex = -1;

    // Runtime status
    std::string actualHash;
    bool isVerified = false;
    bool isMeasured = false;
};

} // namespace securebootx
