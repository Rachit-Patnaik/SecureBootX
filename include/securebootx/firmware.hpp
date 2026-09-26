#ifndef SECUREBOOTX_FIRMWARE_HPP
#define SECUREBOOTX_FIRMWARE_HPP

#include <cstdint>
#include <string>
#include <vector>

constexpr uint32_t FIRMWARE_MAGIC = 0x53425831; // "SBX1"

enum class BootStage : uint32_t {
    STAGE_1_SPL = 1,
    STAGE_2_UBOOT = 2,
    STAGE_3_KERNEL = 3
};

#pragma pack(push, 1)
struct FirmwareHeader {
    uint32_t magic = FIRMWARE_MAGIC;
    uint32_t version = 1;              // Anti-Rollback Security Version
    uint32_t stage = 1;                // BootStage
    uint32_t payload_size = 0;         // Size of raw binary payload
    uint8_t payload_hash[32];          // SHA-256 Digest of binary payload
    uint8_t signature[256];            // Cryptographic RSA-2048 signature
    uint8_t reserved[64];              // Reserved padding
};
#pragma pack(pop)

struct FirmwareImage {
    FirmwareHeader header;
    std::vector<uint8_t> payload;
};

class FirmwareManager {
public:
    static bool loadFirmware(const std::string& filepath, FirmwareImage& image);
    static bool saveFirmware(const std::string& filepath, const FirmwareImage& image);
    static std::string getStageName(BootStage stage);
};

#endif // SECUREBOOTX_FIRMWARE_HPP
