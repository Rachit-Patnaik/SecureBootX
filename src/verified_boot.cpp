#include "securebootx/verified_boot.hpp"
#include "securebootx/crypto_engine.hpp"
#include "securebootx/logger.hpp"
#include <iostream>

BootVerificationResult VerifiedBootEngine::verifyFirmwareImage(const FirmwareImage& image, const TrustFramework& trust_fw) {
    BootVerificationResult res;
    res.failed_stage = static_cast<BootStage>(image.header.stage);

    // 1. Anti-Rollback Check
    if (!const_cast<TrustFramework&>(trust_fw).checkAntiRollback(image.header.version)) {
        res.success = false;
        res.failure_reason = "Anti-Rollback Version Check Failed";
        return res;
    }

    // 2. Hash Verification
    auto computed_hash = CryptoEngine::sha256(image.payload);
    std::vector<uint8_t> header_hash(image.header.payload_hash, image.header.payload_hash + 32);
    if (computed_hash != header_hash) {
        res.success = false;
        res.failure_reason = "Payload SHA-256 Digest Mismatch (Payload Tampered)";
        Logger::getInstance().log(LogLevel::ERROR, "Payload hash mismatch on " + FirmwareManager::getStageName(res.failed_stage));
        return res;
    }

    // 3. Signature Verification
    std::vector<uint8_t> signature(image.header.signature, image.header.signature + 256);
    if (!CryptoEngine::verifySignature(image.payload, signature, trust_fw.getPolicy().root_public_key)) {
        res.success = false;
        res.failure_reason = "Cryptographic RSA-2048 Signature Verification Failed";
        Logger::getInstance().log(LogLevel::ERROR, "Signature verification failed on " + FirmwareManager::getStageName(res.failed_stage));
        return res;
    }

    res.success = true;
    res.verified_stages.push_back(FirmwareManager::getStageName(res.failed_stage));
    Logger::getInstance().log(LogLevel::SUCCESS, "Successfully Verified Stage: " + FirmwareManager::getStageName(res.failed_stage));
    return res;
}

BootVerificationResult VerifiedBootEngine::verifyBootChain(const std::vector<std::string>& firmware_files, const TrustFramework& trust_fw) {
    BootVerificationResult chain_res;
    chain_res.success = true;

    Logger::getInstance().log(LogLevel::INFO, "=== Initiating SecureBootX Verified Boot Sequence ===");

    for (const auto& file : firmware_files) {
        FirmwareImage img;
        if (!FirmwareManager::loadFirmware(file, img)) {
            chain_res.success = false;
            chain_res.failure_reason = "Failed to load firmware binary: " + file;
            return chain_res;
        }

        auto stage_res = verifyFirmwareImage(img, trust_fw);
        if (!stage_res.success) {
            chain_res.success = false;
            chain_res.failed_stage = stage_res.failed_stage;
            chain_res.failure_reason = stage_res.failure_reason;
            Logger::getInstance().log(LogLevel::ERROR, "Boot Chain halted at stage: " + FirmwareManager::getStageName(stage_res.failed_stage));
            return chain_res;
        }

        chain_res.verified_stages.push_back(FirmwareManager::getStageName(static_cast<BootStage>(img.header.stage)));
    }

    Logger::getInstance().log(LogLevel::SUCCESS, "=== Chain of Trust Verification PASSED. Proceeding to Kernel Execution ===");
    return chain_res;
}
