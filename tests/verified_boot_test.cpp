#include "securebootx/verified_boot.hpp"
#include "securebootx/crypto_engine.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    TrustFramework trust_fw;
    std::string fw_file = "firmware_images/stage1_spl.bin";
    if (!fs::exists(fw_file)) {
        if (fs::exists("../firmware_images/stage1_spl.bin")) fw_file = "../firmware_images/stage1_spl.bin";
    }

    FirmwareImage img;
    bool load_ok = FirmwareManager::loadFirmware(fw_file, img);
    assert(load_ok);

    auto res = VerifiedBootEngine::verifyFirmwareImage(img, trust_fw);
    assert(res.success);

    std::cout << "[TEST VERIFIED BOOT] Single stage verified boot PASSED." << std::endl;
    return 0;
}
