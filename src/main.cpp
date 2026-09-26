#include "securebootx/verified_boot.hpp"
#include "securebootx/measured_boot.hpp"
#include "securebootx/trust_framework.hpp"
#include "securebootx/crypto_engine.hpp"
#include "securebootx/logger.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <filesystem>

void printUsage() {
    std::cout << "======================================================\n";
    std::cout << "  SecureBootX — Verified & Measured Boot Framework    \n";
    std::cout << "======================================================\n";
    std::cout << "Usage:\n";
    std::cout << "  securebootx run [policy.json]                 Execute complete boot chain\n";
    std::cout << "  securebootx sign <in.bin> <out.bin> [ver] [stage] Create signed firmware\n";
    std::cout << "  securebootx verify <firmware.bin> [policy.json] Verify firmware signature\n";
    std::cout << "  securebootx measure <firmware.bin>           Extend TPM PCR measurement\n";
    std::cout << "------------------------------------------------------\n";
}

int main(int argc, char* argv[]) {
    Logger::getInstance().init("securebootx.log");

    if (argc < 2) {
        printUsage();
        return 0;
    }

    std::string cmd = argv[1];

    if (cmd == "run") {
        std::string policy_file = (argc > 2) ? argv[2] : "configs/policy.json";
        TrustFramework trust_fw;
        trust_fw.loadPolicy(policy_file);

        std::string base_dir = "firmware_images/";
        if (!std::filesystem::exists(base_dir + "stage1_spl.bin") && std::filesystem::exists("../firmware_images/stage1_spl.bin")) {
            base_dir = "../firmware_images/";
        }

        std::vector<std::string> boot_chain = {
            base_dir + "stage1_spl.bin",
            base_dir + "stage2_uboot.bin",
            base_dir + "stage3_kernel.bin"
        };

        // 1. Verified Boot Stage
        auto boot_res = VerifiedBootEngine::verifyBootChain(boot_chain, trust_fw);
        if (!boot_res.success) {
            std::cerr << "BOOT HALTED: " << boot_res.failure_reason << std::endl;
            return 1;
        }

        // 2. Measured Boot Stage
        MeasuredBootEngine tpm;
        for (size_t i = 0; i < boot_chain.size(); ++i) {
            FirmwareImage img;
            if (FirmwareManager::loadFirmware(boot_chain[i], img)) {
                auto hash = CryptoEngine::sha256(img.payload);
                tpm.extendMeasurement(i, FirmwareManager::getStageName(static_cast<BootStage>(img.header.stage)), hash);
            }
        }

        tpm.printPCRSummary();
        return 0;
    }

    if (cmd == "sign") {
        if (argc < 4) {
            std::cout << "Error: Usage: securebootx sign <input.bin> <output.signed.bin> [version] [stage]" << std::endl;
            return 1;
        }
        std::string input_file = argv[2];
        std::string output_file = argv[3];
        uint32_t version = (argc > 4) ? std::stoul(argv[4]) : 1;
        uint32_t stage = (argc > 5) ? std::stoul(argv[5]) : 1;

        std::ifstream in(input_file, std::ios::binary);
        if (!in.is_open()) {
            std::cerr << "Error: Cannot open input file: " << input_file << std::endl;
            return 1;
        }

        FirmwareImage img;
        img.payload = std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        
        img.header.magic = FIRMWARE_MAGIC;
        img.header.version = version;
        img.header.stage = stage;
        img.header.payload_size = (uint32_t)img.payload.size();

        auto digest = CryptoEngine::sha256(img.payload);
        memcpy(img.header.payload_hash, digest.data(), 32);

        std::vector<uint8_t> sig;
        CryptoEngine::signData(img.payload, "", sig);
        memcpy(img.header.signature, sig.data(), 256);

        if (FirmwareManager::saveFirmware(output_file, img)) {
            std::cout << "Successfully signed firmware image: " << output_file << std::endl;
            return 0;
        }
        return 1;
    }

    if (cmd == "verify") {
        if (argc < 3) {
            std::cout << "Error: Usage: securebootx verify <firmware.bin> [policy.json]" << std::endl;
            return 1;
        }
        std::string fw_file = argv[2];
        std::string policy_file = (argc > 3) ? argv[3] : "configs/policy.json";

        TrustFramework trust_fw;
        trust_fw.loadPolicy(policy_file);

        FirmwareImage img;
        if (!FirmwareManager::loadFirmware(fw_file, img)) return 1;

        auto res = VerifiedBootEngine::verifyFirmwareImage(img, trust_fw);
        return res.success ? 0 : 1;
    }

    if (cmd == "measure") {
        if (argc < 3) {
            std::cout << "Error: Usage: securebootx measure <firmware.bin>" << std::endl;
            return 1;
        }
        std::string fw_file = argv[2];
        FirmwareImage img;
        if (!FirmwareManager::loadFirmware(fw_file, img)) return 1;

        MeasuredBootEngine tpm;
        auto hash = CryptoEngine::sha256(img.payload);
        tpm.extendMeasurement(0, FirmwareManager::getStageName(static_cast<BootStage>(img.header.stage)), hash);
        tpm.printPCRSummary();
        return 0;
    }

    printUsage();
    return 0;
}
