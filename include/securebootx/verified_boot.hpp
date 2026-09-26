#ifndef SECUREBOOTX_VERIFIED_BOOT_HPP
#define SECUREBOOTX_VERIFIED_BOOT_HPP

#include "firmware.hpp"
#include "trust_framework.hpp"
#include <string>
#include <vector>

struct BootVerificationResult {
    bool success = false;
    BootStage failed_stage = BootStage::STAGE_1_SPL;
    std::string failure_reason;
    std::vector<std::string> verified_stages;
};

class VerifiedBootEngine {
public:
    VerifiedBootEngine() = default;

    static BootVerificationResult verifyFirmwareImage(const FirmwareImage& image, const TrustFramework& trust_fw);
    static BootVerificationResult verifyBootChain(const std::vector<std::string>& firmware_files, const TrustFramework& trust_fw);
};

#endif // SECUREBOOTX_VERIFIED_BOOT_HPP
