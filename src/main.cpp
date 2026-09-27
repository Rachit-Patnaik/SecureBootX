#include "boot_manager.hpp"

std::vector<uint8_t> mock_sig(const std::vector<uint8_t>& d) {
    std::vector<uint8_t> s(256, 0x01);
    s[0] = d[0];
    return s;
}

int main() {
    BootManager bm;
    
    bm.enroll_key("db", std::vector<uint8_t>(256, 0x01));
    
    bm.flash_image("SEC", {0x11, 0x22}, mock_sig({0x11, 0x22}));
    bm.flash_image("PEI", {0x33, 0x44}, mock_sig({0x33, 0x44}));
    bm.flash_image("DXE", {0x55, 0x66}, mock_sig({0x55, 0x66}));
    bm.flash_image("OS", {0x77, 0x88}, mock_sig({0x77, 0x88}));
    
    bm.execute();
    
    return 0;
}
