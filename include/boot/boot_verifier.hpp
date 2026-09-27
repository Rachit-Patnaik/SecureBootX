#pragma once

#include "boot/boot_component.hpp"
#include "boot/manifest.hpp"
#include "crypto/verifier.hpp"
#include <memory>

namespace securebootx {

class BootVerifier {
public:
    explicit BootVerifier(std::shared_ptr<Manifest> manifest);

    /**
     * @brief Verifies a specific component against the manifest.
     * @return True if component is trusted.
     */
    bool verifyComponent(BootComponent& component);

    /**
     * @brief Verifies the entire boot chain.
     * @return True if all components are trusted.
     */
    bool verifyChain();

private:
    std::shared_ptr<Manifest> m_manifest;
};

} // namespace securebootx
