#include "tpm/pcr_manager.hpp"
#include "crypto/hash.hpp"
#include <iostream>

namespace securebootx {

SoftwareTpmProvider::SoftwareTpmProvider() {
    // Initialize PCRs 0-23 to zero
    for (int i = 0; i < 24; ++i) {
        m_pcrs[i] = std::string(64, '0');
    }
}

bool SoftwareTpmProvider::extendPcr(int pcrIndex, const std::string& digest) {
    if (pcrIndex < 0 || pcrIndex >= 24) return false;

    // TPM Extend: PCR_new = SHA256(PCR_old || new_digest)
    std::string combined = m_pcrs[pcrIndex] + digest;

    // Convert hex string to bytes for hashing
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < combined.length(); i += 2) {
        std::string byteString = combined.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    m_pcrs[pcrIndex] = Hash::calculateSHA256(bytes);
    return true;
}

std::string SoftwareTpmProvider::readPcr(int pcrIndex) {
    if (m_pcrs.find(pcrIndex) == m_pcrs.end()) return "INVALID";
    return m_pcrs[pcrIndex];
}

} // namespace securebootx
