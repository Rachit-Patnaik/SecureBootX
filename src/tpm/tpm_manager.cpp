#include "tpm/tpm_manager_impl.hpp"
#include <iostream>
#include <fstream>

namespace securebootx {

TpmManager::TpmManager() {
    initializeProvider();
}

void TpmManager::initializeProvider() {
    // Attempt to detect real TPM
    // In a real Linux system, we would check for /dev/tpm0 or use tpm2_tss
    bool hardwareTpmFound = false;
    std::ifstream tpmDev("/dev/tpm0");
    if (tpmDev.good()) {
        hardwareTpmFound = true;
    }

    if (hardwareTpmFound) {
        // In a full implementation, we would instantiate a HardwareTpmProvider here
        // For now, we default to Software to ensure WSL2 stability, but indicate we found the device
        std::cout << "[TPM] Hardware TPM detected at /dev/tpm0. Integration pending." << std::endl;
    }

    // Default to software provider for educational consistency and WSL2 support
    m_provider = std::make_unique<SoftwareTpmProvider>();
}

bool TpmManager::isTpmAvailable() const {
    return m_provider && m_provider->isAvailable();
}

bool TpmManager::extend(int pcr, const std::string& digest) {
    if (!m_provider) return false;
    return m_provider->extendPcr(pcr, digest);
}

std::string TpmManager::read(int pcr) {
    if (!m_provider) return "ERROR";
    return m_provider->readPcr(pcr);
}

std::string TpmManager::getMode() const {
    return m_provider ? m_provider->getProviderName() : "NONE";
}

} // namespace securebootx
