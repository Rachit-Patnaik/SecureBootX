#include "trust/trust_policy.hpp"

namespace securebootx {

std::string trustLevelToString(TrustLevel level) {
    switch (level) {
        case TrustLevel::TRUSTED:    return "TRUSTED";
        case TrustLevel::UNTRUSTED:  return "UNTRUSTED";
        case TrustLevel::DEGRADED:   return "DEGRADED";
        case TrustLevel::UNKNOWN:    return "UNKNOWN";
        default:                    return "UNKNOWN";
    }
}

} // namespace securebootx
