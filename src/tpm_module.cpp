#include "tpm_module.hpp"
#include "crypto_engine.hpp"
#include <iostream>
#include <iomanip>

TpmModule::TpmModule() {
    pcrs.resize(24, std::vector<uint8_t>(32, 0));
}

void TpmModule::pcr_extend(uint32_t index, const std::vector<uint8_t>& measurement, const std::string& desc) {
    if (index >= 24) return;
    std::vector<uint8_t> buffer = pcrs[index];
    buffer.insert(buffer.end(), measurement.begin(), measurement.end());
    pcrs[index] = CryptoEngine::sha256(buffer);
    event_log.push_back({index, measurement, desc});
}

std::vector<uint8_t> TpmModule::pcr_read(uint32_t index) {
    if (index >= 24) return std::vector<uint8_t>();
    return pcrs[index];
}

void TpmModule::print_pcr(uint32_t index) {
    if (index >= 24) return;
    std::cout << "PCR[" << index << "]: ";
    for (auto b : pcrs[index]) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)b;
    }
    std::cout << std::dec << "\n";
}
