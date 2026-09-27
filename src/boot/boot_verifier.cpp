#include "boot/boot_verifier.hpp"
#include "crypto/hash.hpp"
#include <iostream>

namespace securebootx {

BootVerifier::BootVerifier(std::shared_ptr<Manifest> manifest) : m_manifest(manifest) {}

bool BootVerifier::verifyComponent(BootComponent& component) {
    std::cout << "[BootVerifier] Verifying " << component.name << "..." << std::endl;

    try {
        // 1. Calculate Actual Hash
        component.actualHash = Hash::calculateSHA256(component.path);

        // 2. Compare with Expected Hash
        if (component.actualHash != component.expectedHash) {
            std::cerr << "[!] Hash mismatch for " << component.name << "\n"
                      << "    Expected: " << component.expectedHash << "\n"
                      << "    Actual:   " << component.actualHash << std::endl;
            component.isVerified = false;
            return false;
        }

        // 3. Verify Digital Signature
        if (!component.signaturePath.empty()) {
            bool sigValid = Verifier::verifyFile(
                component.path,
                component.signaturePath,
                m_manifest->getPublicKeyPath()
            );
            if (!sigValid) {
                std::cerr << "[!] Invalid signature for " << component.name << std::endl;
                component.isVerified = false;
                return false;
            }
        }

        std::cout << "[+] " << component.name << " verified successfully." << std::endl;
        component.isVerified = true;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "[!] Error verifying " << component.name << ": " << e.what() << std::endl;
        component.isVerified = false;
        return false;
    }
}

bool BootVerifier::verifyChain() {
    bool allTrusted = true;
    for (auto& component : const_cast<std::vector<BootComponent>&>(m_manifest->getComponents())) {
        if (!verifyComponent(component)) {
            allTrusted = false;
        }
    }
    return allTrusted;
}

} // namespace securebootx
