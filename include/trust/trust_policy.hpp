#pragma once

#include <string>

namespace securebootx {

enum class TrustLevel {
    TRUSTED,
    UNTRUSTED,
    DEGRADED,
    UNKNOWN
};

std::string trustLevelToString(TrustLevel level);

enum class TrustPolicy {
    STRICT,      // Any failure = UNTRUSTED
    PERMISSIVE   // Report failures but don't block simulation
};

} // namespace securebootx
