#pragma once

#include <string>
#include <vector>
#include <memory>

namespace securebootx {

/**
 * @brief Interface for TPM operations.
 * Allows switching between a real hardware TPM and a software simulation.
 */
class ITpmProvider {
public:
    virtual ~ITpmProvider() = default;
    virtual bool isAvailable() = 0;
    virtual bool extendPcr(int pcrIndex, const std::string& digest) = 0;
    virtual std::string readPcr(int pcrIndex) = 0;
    virtual std::string getProviderName() const = 0;
};

} // namespace securebootx
