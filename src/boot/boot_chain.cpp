#include "boot/boot_chain.hpp"

namespace securebootx {
    void BootChain::addComponent(const BootComponent& comp) {
        m_components.push_back(comp);
    }
} // namespace securebootx
