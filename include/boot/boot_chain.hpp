#pragma once

#include "boot/boot_component.hpp"
#include <vector>
#include <memory>

namespace securebootx {

class BootChain {
public:
    void addComponent(const BootComponent& comp);
    const std::vector<BootComponent>& getComponents() const { return m_components; }
    std::vector<BootComponent>& getComponentsMutable() { return m_components; }

private:
    std::vector<BootComponent> m_components;
};

} // namespace securebootx
