#include "securebootx/measured_boot.hpp"
#include "securebootx/crypto_engine.hpp"
#include "securebootx/logger.hpp"
#include <iostream>
#include <iomanip>

MeasuredBootEngine::MeasuredBootEngine() {
    resetPCRs();
}

void MeasuredBootEngine::resetPCRs() {
    pcrs_.assign(24, std::vector<uint8_t>(32, 0)); // 24 PCR registers initialized to 0
    event_log_.clear();
}

bool MeasuredBootEngine::extendMeasurement(uint32_t pcr_index, const std::string& event_name, const std::vector<uint8_t>& measurement_hash) {
    if (pcr_index >= pcrs_.size()) {
        Logger::getInstance().log(LogLevel::ERROR, "Invalid TPM PCR Index: " + std::to_string(pcr_index));
        return false;
    }

    // PCR_new = SHA256(PCR_old || measurement_hash)
    std::vector<uint8_t> concat_buffer = pcrs_[pcr_index];
    concat_buffer.insert(concat_buffer.end(), measurement_hash.begin(), measurement_hash.end());

    std::vector<uint8_t> pcr_new = CryptoEngine::sha256(concat_buffer);
    pcrs_[pcr_index] = pcr_new;

    TCGEventLogEntry entry;
    entry.pcr_index = pcr_index;
    entry.event_name = event_name;
    entry.digest_hex = CryptoEngine::sha256HexString(measurement_hash);
    entry.pcr_new_val_hex = CryptoEngine::sha256HexString(pcr_new);

    event_log_.push_back(entry);

    Logger::getInstance().log(LogLevel::INFO, "TPM PCR[" + std::to_string(pcr_index) + "] extended for event: " + event_name);
    return true;
}

std::string MeasuredBootEngine::getPCRValue(uint32_t pcr_index) const {
    if (pcr_index >= pcrs_.size()) return "";
    return CryptoEngine::sha256HexString(pcrs_[pcr_index]);
}

void MeasuredBootEngine::printPCRSummary() const {
    std::cout << "======================================================" << std::endl;
    std::cout << "        TPM 2.0 MEASURED BOOT PCR SUMMARY             " << std::endl;
    std::cout << "======================================================" << std::endl;
    for (const auto& entry : event_log_) {
        std::cout << "  PCR[" << entry.pcr_index << "] Event: " << std::left << std::setw(28) << entry.event_name << std::endl;
        std::cout << "    Digest  : " << entry.digest_hex.substr(0, 32) << "..." << std::endl;
        std::cout << "    PCR Val : " << entry.pcr_new_val_hex.substr(0, 32) << "..." << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
    }
}
