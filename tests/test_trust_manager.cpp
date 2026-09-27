#include <gtest/gtest.h>
#include "trust/trust_manager.hpp"
#include "boot/boot_component.hpp"
#include "measurement/measurement_log.hpp"
#include <vector>
#include <memory>

using namespace securebootx;

TEST(TrustManagerTest, StrictPolicyFailure) {
    TrustManager tm(TrustPolicy::STRICT);

    std::vector<BootComponent> components = {
        {"Firmware", ComponentType::FIRMWARE, "f.bin", "hash1", "f.sig", 0, "hash1", true, true},
        {"Kernel", ComponentType::KERNEL, "k.bin", "hash2", "k.sig", 4, "hashX", false, true} // Failure
    };

    MeasurementLog log;
    // Add successful measurements to match components
    log.addEntry({"Firmware", "hash1", 0, "now", true});
    log.addEntry({"Kernel", "hashX", 4, "now", true});

    TrustReport report = tm.evaluateTrust(components, log, false);
    EXPECT_EQ(report.overallStatus, TrustLevel::UNTRUSTED);
}

TEST(TrustManagerTest, PermissivePolicyDegraded) {
    TrustManager tm(TrustPolicy::PERMISSIVE);

    std::vector<BootComponent> components = {
        {"Kernel", ComponentType::KERNEL, "k.bin", "hash2", "k.sig", 4, "hashX", false, true}
    };

    MeasurementLog log;
    log.addEntry({"Kernel", "hashX", 4, "now", true});

    TrustReport report = tm.evaluateTrust(components, log, false);
    EXPECT_EQ(report.overallStatus, TrustLevel::DEGRADED);
}
