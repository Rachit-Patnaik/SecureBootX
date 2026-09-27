#pragma once

#include "boot/boot_component.hpp"
#include "measurement/measurement_log.hpp"
#include "crypto/hash.hpp"
#include "tpm/tpm_manager_impl.hpp"
#include <memory>

namespace securebootx {

class MeasurementEngine {
public:
    explicit MeasurementEngine(std::shared_ptr<MeasurementLog> log);

    /**
     * @brief Measures a boot component and records it in the log.
     * @param component The component to measure.
     * @return The measurement result.
     */
    Measurement measure(const BootComponent& component);

    std::string getPcrValue(int pcrIndex) const;

private:
    std::shared_ptr<MeasurementLog> m_log;
    std::unique_ptr<TpmManager> m_tpm;

    std::string getCurrentTimestamp();
};

} // namespace securebootx
