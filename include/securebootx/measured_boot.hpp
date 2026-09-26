#ifndef SECUREBOOTX_MEASURED_BOOT_HPP
#define SECUREBOOTX_MEASURED_BOOT_HPP

#include "firmware.hpp"
#include <string>
#include <vector>

struct TCGEventLogEntry {
    uint32_t pcr_index = 0;
    std::string event_name;
    std::string digest_hex;
    std::string pcr_new_val_hex;
};

class MeasuredBootEngine {
public:
    MeasuredBootEngine();

    // Reset TPM Platform Configuration Registers (PCRs)
    void resetPCRs();

    // Extend a measurement into a PCR register: PCR[i] = SHA256(PCR[i] || hash)
    bool extendMeasurement(uint32_t pcr_index, const std::string& event_name, const std::vector<uint8_t>& measurement_hash);

    // Get event log and current PCR register state
    const std::vector<TCGEventLogEntry>& getEventLog() const { return event_log_; }
    std::string getPCRValue(uint32_t pcr_index) const;
    void printPCRSummary() const;

private:
    std::vector<std::vector<uint8_t>> pcrs_; // Simulated TPM PCR 0..23
    std::vector<TCGEventLogEntry> event_log_;
};

#endif // SECUREBOOTX_MEASURED_BOOT_HPP
