#ifndef BOOT_MANAGER_HPP
#define BOOT_MANAGER_HPP

#include "tpm_module.hpp"
#include <map>
#include <string>

struct Image {
    std::vector<uint8_t> payload;
    std::vector<uint8_t> sig;
};

class BootManager {
    TpmModule tpm;
    std::map<std::string, std::vector<std::vector<uint8_t>>> sec_vars;
    std::map<std::string, Image> firmware_vol;
public:
    void enroll_key(const std::string& var, const std::vector<uint8_t>& key);
    void flash_image(const std::string& name, const std::vector<uint8_t>& data, const std::vector<uint8_t>& sig);
    bool load_and_verify(const std::string& name, uint32_t pcr);
    void execute();
};

#endif
