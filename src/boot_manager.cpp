#include "boot_manager.hpp"
#include "crypto_engine.hpp"
#include <iostream>
#include <cstdlib>

void BootManager::enroll_key(const std::string& var, const std::vector<uint8_t>& key) {
    sec_vars[var].push_back(key);
}

void BootManager::flash_image(const std::string& name, const std::vector<uint8_t>& data, const std::vector<uint8_t>& sig) {
    firmware_vol[name] = {data, sig};
}

bool BootManager::load_and_verify(const std::string& name, uint32_t pcr) {
    if (firmware_vol.find(name) == firmware_vol.end()) return false;
    auto img = firmware_vol[name];
    auto digest = CryptoEngine::sha256(img.payload);
    
    bool revoked = false;
    for (const auto& r : sec_vars["dbx"]) {
        if (r == digest) revoked = true;
    }
    if (revoked) return false;
    
    bool allowed = false;
    for (const auto& pub : sec_vars["db"]) {
        if (CryptoEngine::rsa_verify(digest, img.sig, pub)) {
            allowed = true;
            break;
        }
    }
    
    if (!allowed) return false;
    tpm.pcr_extend(pcr, digest, name);
    return true;
}

void BootManager::execute() {
    const std::string stages[] = {"SEC", "PEI", "DXE", "OS"};
    uint32_t pcrs[] = {0, 0, 2, 4};
    
    for (int i = 0; i < 4; i++) {
        if (!load_and_verify(stages[i], pcrs[i])) {
            std::cout << "Boot Failed at " << stages[i] << "\n";
            exit(1);
        }
    }
    std::cout << "Boot Success\n";
    tpm.print_pcr(0);
    tpm.print_pcr(2);
    tpm.print_pcr(4);
}
