#include "trust/trust_manager.hpp"
#include <sstream>
#include <iostream>

namespace securebootx {

TrustManager::TrustManager(TrustPolicy policy) : m_policy(policy) {}

TrustReport TrustManager::evaluateTrust(const std::vector<BootComponent>& components,
                                      const MeasurementLog& log,
                                      bool tpmAvailable) {
    TrustReport report;
    report.tpmActive = tpmAvailable;
    report.measurementMode = tpmAvailable ? "HARDWARE (TPM 2.0)" : "SOFTWARE SIMULATION";
    report.overallStatus = TrustLevel::TRUSTED;

    // 1. Check Component Verification (Verified Boot)
    bool verificationFailure = false;
    for (const auto& comp : components) {
        if (!comp.isVerified) {
            std::stringstream ss;
            ss << "Component [" << comp.name << "] FAILED verification check.";
            report.details.push_back(ss.str());
            verificationFailure = true;
        } else {
            std::stringstream ss;
            ss << "Component [" << comp.name << "] is TRUSTED.";
            report.details.push_back(ss.str());
        }
    }

    // 2. Check Measurement Integrity (Measured Boot)
    bool measurementFailure = false;
    if (log.getEntries().empty()) {
        report.details.push_back("Measurement log is empty! No components were measured.");
        measurementFailure = true;
    } else {
        for (const auto& entry : log.getEntries()) {
            if (!entry.success) {
                std::stringstream ss;
                ss << "Measurement of [" << entry.componentName << "] FAILED.";
                report.details.push_back(ss.str());
                measurementFailure = true;
            }
        }
    }

    // 3. Final Trust Decision based on Policy
    if (m_policy == TrustPolicy::STRICT) {
        if (verificationFailure || measurementFailure) {
            report.overallStatus = TrustLevel::UNTRUSTED;
        }
    } else { // PERMISSIVE
        if (verificationFailure || measurementFailure) {
            report.overallStatus = TrustLevel::DEGRADED;
        }
    }

    return report;
}

} // namespace securebootx
