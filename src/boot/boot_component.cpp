#include "boot/boot_component.hpp"
#include <unordered_map>
#include <algorithm>

namespace securebootx {

std::string componentTypeToString(ComponentType type) {
    switch (type) {
        case ComponentType::FIRMWARE:   return "firmware";
        case ComponentType::BOOTLOADER: return "bootloader";
        case ComponentType::KERNEL:     return "kernel";
        case ComponentType::INITRAMFS:   return "initramfs";
        case ComponentType::ROOTFS:     return "rootfs";
        default:                        return "unknown";
    }
}

ComponentType stringToComponentType(const std::string& typeStr) {
    std::string lowerStr = typeStr;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    if (lowerStr == "firmware")   return ComponentType::FIRMWARE;
    if (lowerStr == "bootloader") return ComponentType::BOOTLOADER;
    if (lowerStr == "kernel")     return ComponentType::KERNEL;
    if (lowerStr == "initramfs")   return ComponentType::INITRAMFS;
    if (lowerStr == "rootfs")     return ComponentType::ROOTFS;
    return ComponentType::UNKNOWN;
}

} // namespace securebootx
