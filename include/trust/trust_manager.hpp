#pragma once

#include "boot/boot_component.hpp"
#include "measurement/measurement_log.hpp"
#include "trust/trust_policy.hpp"
#include <vector>
#include <memory>
#include <string>

namespace securebootx {

struct TrustReport {
    TrustLevel overallStatus;
    std::vector<std::string> details;
    bool tpmActive;
    std::string measurementMode;
};

class TrustManager {
public:
    explicit TrustManager(TrustPolicy policy = TrustPolicy::STRICT);

    /**
     * @brief Evaluates the trust state based on the boot chain and measurements.
     * @param components The list of boot components and their verification status.
     * @param log The boot measurement log.
     * @param tpmAvailable Whether a TPM was actually used.
     * @return A detailed TrustReport.
     */
    TrustReport evaluateTrust(const std::vector<BootComponent>& components,
                             const MeasurementLog& log,
                             bool tpmAvailable);

    void setPolicy(TrustPolicy policy) { m_policy = policy; }

private:
    TrustPolicy m_policy;
};

} // namespace securebootx
