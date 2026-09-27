#include "common/cli.hpp"
#include "crypto/hash.hpp"
#include "crypto/signer.hpp"
#include "crypto/verifier.hpp"
#include "simulation/tamper_simulator.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

namespace securebootx {

CLI::CLI() {
    m_log = std::make_shared<MeasurementLog>();
    m_engine = std::make_unique<MeasurementEngine>(m_log);
    m_trustManager = std::make_unique<TrustManager>(TrustPolicy::STRICT);
}

void CLI::printBanner() {
    std::cout << "\n=========================================\n";
    std::cout << "           SECUREBOOTX\n";
    std::cout << "     BOOT TRUST FRAMEWORK\n";
    std::cout << "=========================================\n";
}

void CLI::printHelp() {
    std::cout << "SecureBootX - Educational Verified & Measured Boot Framework\n\n";
    std::cout << "Usage: securebootx <command> [args]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  status                Show system trust status\n";
    std::cout << "  boot-check            Perform full boot chain verification\n";
    std::cout << "  verify <file>        Verify a specific file signature\n";
    std::cout << "  measure <file>       Measure a specific file\n";
    std::cout << "  keygen                Generate test RSA keys\n";
    std::cout << "  sign <file>           Sign a file using private key\n";
    std::cout << "  simulate-tamper <comp> Modify a demo component to test detection\n";
    std::cout << "  restore-demo          Restore demo images to original state\n";
    std::cout << "  report                Show detailed trust report\n";
    std::cout << "  --help                Show this help\n";
}

std::shared_ptr<Manifest> CLI::loadManifest() {
    auto manifest = std::make_shared<Manifest>();
    if (!manifest->load("data/manifest.json")) {
        std::cerr << "[!] Warning: Could not load data/manifest.json. Using empty manifest." << std::endl;
    }
    return manifest;
}

int CLI::run(int argc, char** argv) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string cmd = argv[1];

    if (cmd == "--help") {
        printHelp();
    } else if (cmd == "status") {
        handleStatus();
    } else if (cmd == "boot-check") {
        handleBootCheck();
    } else if (cmd == "verify" && argc > 2) {
        handleVerify(argv[2]);
    } else if (cmd == "measure" && argc > 2) {
        handleMeasure(argv[2]);
    } else if (cmd == "keygen") {
        handleKeygen();
    } else if (cmd == "sign" && argc > 2) {
        handleSign(argv[2]);
    } else if (cmd == "simulate-tamper" && argc > 2) {
        handleSimulateTamper(argv[2]);
    } else if (cmd == "restore-demo") {
        handleRestoreDemo();
    } else if (cmd == "report") {
        handleReport();
    } else {
        std::cerr << "Unknown command: " << cmd << std::endl;
        printHelp();
        return 1;
    }

    return 0;
}

void CLI::handleStatus() {
    printBanner();
    std::cout << "Secure Boot      : SIMULATION\n";
    std::cout << "TPM              : NOT AVAILABLE (Software Mode)\n";
    std::cout << "Measurement Mode : SOFTWARE\n\n";
    std::cout << "Checking components...\n";

    auto manifest = loadManifest();
    for (const auto& comp : manifest->getComponents()) {
        std::cout << std::left << std::setw(20) << comp.name << " : PENDING\n";
    }

    std::cout << "\n-----------------------------------------\n";
    std::cout << "OVERALL TRUST     : UNKNOWN (Run boot-check first)\n";
    std::cout << "-----------------------------------------\n";
}

void CLI::handleBootCheck() {
    printBanner();
    auto manifest = loadManifest();
    auto verifier = std::make_unique<BootVerifier>(manifest);

    std::cout << "Executing Boot Trust Chain Verification...\n\n";

    std::vector<BootComponent> components = manifest->getComponents();
    int count = 0;
    int total = components.size();

    for (auto& comp : components) {
        count++;
        std::cout << "[" << count << "/" << total << "] ";
        if (verifier->verifyComponent(comp)) {
            std::cout << "VERIFIED\n";
        } else {
            std::cout << "FAILED\n";
        }

        // Measure the component regardless of verification status (Measured Boot)
        m_engine->measure(comp);
    }

    std::cout << "\nMeasurement Log Updated.\n";
    m_log->saveToFile("data/measurement_log.json");

    // Final Trust Decision
    TrustReport report = m_trustManager->evaluateTrust(components, *m_log, false);

    std::cout << "\nTrust Decision: " << trustLevelToString(report.overallStatus) << "\n";
    std::cout << "-----------------------------------------\n";
}

void CLI::handleVerify(const std::string& filePath) {
    auto manifest = loadManifest();
    try {
        bool valid = Verifier::verifyFile(filePath, filePath + ".sig", manifest->getPublicKeyPath());
        std::cout << "[SecureBootX] File: " << filePath << "\n";
        std::cout << "[SecureBootX] Signature: " << (valid ? "VALID" : "INVALID") << "\n";
        std::cout << "[SecureBootX] Result: " << (valid ? "TRUSTED" : "UNTRUSTED") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void CLI::handleMeasure(const std::string& filePath) {
    BootComponent tempComp;
    tempComp.path = filePath;
    tempComp.name = std::filesystem::path(filePath).filename().string();

    Measurement m = m_engine->measure(tempComp);
    std::cout << "[SecureBootX] Measured " << tempComp.name << "\n";
    std::cout << "[SecureBootX] SHA-256: " << m.hash << "\n";
}

void CLI::handleKeygen() {
    std::cout << "[SecureBootX] Generating test RSA keys in keys/..." << std::endl;
    // In a real app, we'd call a tool or lib.
    // For now, the user will use the scripts/generate_test_keys.sh or the tools/keygen.cpp
    std::cout << "Please run: ./scripts/generate_test_keys.sh" << std::endl;
}

void CLI::handleSign(const std::string& filePath) {
    try {
        std::string sigPath = filePath + ".sig";
        if (Signer::signFile(filePath, "keys/private_key.pem", sigPath)) {
            std::cout << "[SecureBootX] Successfully signed " << filePath << " -> " << sigPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void CLI::handleSimulateTamper(const std::string& componentName) {
    std::cout << "[SecureBootX] Simulating tamper on " << componentName << "..." << std::endl;

    auto manifest = loadManifest();
    bool found = false;
    for (const auto& comp : manifest->getComponents()) {
        if (comp.name == componentName) {
            if (TamperSimulator::simulateTamper(comp.path)) {
                std::cout << "[SecureBootX] TAMPER SUCCESS: Component " << componentName << " is now corrupted." << std::endl;
                std::cout << "[SecureBootX] Run 'boot-check' to see if the system detects it." << std::endl;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "[!] Component " << componentName << " not found in manifest." << std::endl;
    }
}

void CLI::handleRestoreDemo() {
    std::cout << "[SecureBootX] Restoring demo images..." << std::endl;
    auto manifest = loadManifest();
    for (const auto& comp : manifest->getComponents()) {
        TamperSimulator::restoreFile(comp.path);
    }
    std::cout << "[SecureBootX] Demo environment restored." << std::endl;
}

void CLI::handleReport() {
    auto manifest = loadManifest();
    auto verifier = std::make_unique<BootVerifier>(manifest);
    std::vector<BootComponent> components = manifest->getComponents();

    for (auto& comp : components) {
        verifier->verifyComponent(comp);
    }

    TrustReport report = m_trustManager->evaluateTrust(components, *m_log, false);

    std::cout << "\nDetailed Trust Report:\n";
    std::cout << "=========================================\n";
    for (const auto& detail : report.details) {
        std::cout << "- " << detail << "\n";
    }
    std::cout << "=========================================\n";
    std::cout << "FINAL TRUST STATE: " << trustLevelToString(report.overallStatus) << "\n";
}

} // namespace securebootx
