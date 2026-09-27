#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include "boot/boot_chain.hpp"
#include "boot/manifest.hpp"
#include "boot/boot_verifier.hpp"
#include "measurement/measurement_engine.hpp"
#include "measurement/measurement_log.hpp"
#include "trust/trust_manager.hpp"

namespace securebootx {

class CLI {
public:
    CLI();
    int run(int argc, char** argv);

private:
    void printHelp();

    // Command handlers
    void handleStatus();
    void handleBootCheck();
    void handleVerify(const std::string& filePath);
    void handleMeasure(const std::string& filePath);
    void handleKeygen();
    void handleSign(const std::string& filePath);
    void handleSimulateTamper(const std::string& componentName);
    void handleRestoreDemo();
    void handleReport();

    // Internal helpers
    void printBanner();
    std::shared_ptr<Manifest> loadManifest();

    // Core modules
    std::shared_ptr<Manifest> m_manifest;
    std::shared_ptr<MeasurementLog> m_log;
    std::unique_ptr<MeasurementEngine> m_engine;
    std::unique_ptr<TrustManager> m_trustManager;
};

} // namespace securebootx
