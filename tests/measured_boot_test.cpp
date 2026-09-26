#include "securebootx/measured_boot.hpp"
#include "securebootx/crypto_engine.hpp"
#include <iostream>
#include <cassert>

int main() {
    MeasuredBootEngine tpm;
    std::string test_data = "Stage1 SPL Hash Measurement";
    std::vector<uint8_t> data(test_data.begin(), test_data.end());

    auto hash = CryptoEngine::sha256(data);
    bool ext_ok = tpm.extendMeasurement(0, "Stage 1 SPL Measurement", hash);
    assert(ext_ok);

    std::string pcr_val = tpm.getPCRValue(0);
    assert(!pcr_val.empty());
    assert(pcr_val.length() == 64);

    std::cout << "[TEST MEASURED BOOT] TPM 2.0 PCR Extension test PASSED." << std::endl;
    return 0;
}
