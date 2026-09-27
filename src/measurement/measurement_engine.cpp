#include "measurement/measurement_engine.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace securebootx {

MeasurementEngine::MeasurementEngine(std::shared_ptr<MeasurementLog> log)
    : m_log(log), m_tpm(std::make_unique<TpmManager>()) {
}

std::string MeasurementEngine::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

Measurement MeasurementEngine::measure(const BootComponent& component) {
    std::cout << "[MeasurementEngine] Measuring " << component.name << "..." << std::endl;

    Measurement m;
    m.componentName = component.name;
    m.pcrIndex = component.pcrIndex;
    m.timestamp = getCurrentTimestamp();

    try {
        m.hash = Hash::calculateSHA256(component.path);
        m.success = true;

        // Use the TPM Manager to extend the PCR
        if (m.pcrIndex >= 0) {
            m_tpm->extend(m.pcrIndex, m.hash);
        }

    } catch (const std::exception& e) {
        std::cerr << "[!] Measurement failed for " << component.name << ": " << e.what() << std::endl;
        m.success = false;
    }

    m_log->addEntry(m);
    return m;
}

std::string MeasurementEngine::getPcrValue(int pcrIndex) const {
    return m_tpm->read(pcrIndex);
}

} // namespace securebootx
