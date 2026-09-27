#pragma once

#include "tpm/tpm_manager.hpp"
#include <unordered_map>

namespace securebootx {

/**
 * @brief Software-based TPM simulation for environments without hardware TPM (e.g. WSL2).
 */
class SoftwareTpmProvider : public ITpmProvider {
public:
    SoftwareTpmProvider();
    bool isAvailable() override { return true; }
    bool extendPcr(int pcrIndex, const std::string& digest) override;
    std::string readPcr(int pcrIndex) override;
    std::string getProviderName() const override { return "Software Simulation"; }

private:
    std::unordered_map<int, std::string> m_pcrs;
};

} // namespace securebootx
