#pragma once

#include "tpm/tpm_manager.hpp"
#include "tpm/pcr_manager.hpp"
#include <memory>

namespace securebootx {

class TpmManager {
public:
    TpmManager();

    bool isTpmAvailable() const;
    bool extend(int pcr, const std::string& digest);
    std::string read(int pcr);
    std::string getMode() const;

private:
    std::unique_ptr<ITpmProvider> m_provider;
    void initializeProvider();
};

} // namespace securebootx
