#ifndef TPM_MODULE_HPP
#define TPM_MODULE_HPP

#include <vector>
#include <cstdint>
#include <string>

struct EventLog {
    uint32_t pcr_index;
    std::vector<uint8_t> digest;
    std::string desc;
};

class TpmModule {
    std::vector<std::vector<uint8_t>> pcrs;
    std::vector<EventLog> event_log;
public:
    TpmModule();
    void pcr_extend(uint32_t index, const std::vector<uint8_t>& measurement, const std::string& desc);
    std::vector<uint8_t> pcr_read(uint32_t index);
    void print_pcr(uint32_t index);
};

#endif
