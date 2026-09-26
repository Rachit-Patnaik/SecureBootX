#include "securebootx/firmware.hpp"
#include "securebootx/logger.hpp"
#include <fstream>
#include <cstring>

bool FirmwareManager::loadFirmware(const std::string& filepath, FirmwareImage& image) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to open firmware file: " + filepath);
        return false;
    }

    file.read(reinterpret_cast<char*>(&image.header), sizeof(FirmwareHeader));
    if (image.header.magic != FIRMWARE_MAGIC) {
        Logger::getInstance().log(LogLevel::ERROR, "Invalid firmware magic header: 0x" + std::to_string(image.header.magic));
        return false;
    }

    image.payload.resize(image.header.payload_size);
    if (image.header.payload_size > 0) {
        file.read(reinterpret_cast<char*>(image.payload.data()), image.header.payload_size);
    }

    return true;
}

bool FirmwareManager::saveFirmware(const std::string& filepath, const FirmwareImage& image) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to create firmware file: " + filepath);
        return false;
    }

    file.write(reinterpret_cast<const char*>(&image.header), sizeof(FirmwareHeader));
    if (!image.payload.empty()) {
        file.write(reinterpret_cast<const char*>(image.payload.data()), image.payload.size());
    }

    return true;
}

std::string FirmwareManager::getStageName(BootStage stage) {
    switch (stage) {
        case BootStage::STAGE_1_SPL: return "Stage 1 (SPL Bootloader)";
        case BootStage::STAGE_2_UBOOT: return "Stage 2 (Main U-Boot)";
        case BootStage::STAGE_3_KERNEL: return "Stage 3 (Linux Kernel)";
        default: return "Unknown Stage";
    }
}
