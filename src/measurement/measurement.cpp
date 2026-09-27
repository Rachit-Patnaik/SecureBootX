#include "measurement/measurement.hpp"
#include <sstream>
#include <iomanip>

namespace securebootx {

std::string Measurement::toJson() const {
    std::stringstream ss;
    ss << "    {\n";
    ss << "      \"component\": \"" << componentName << "\",\n";
    ss << "      \"hash\": \"" << hash << "\",\n";
    ss << "      \"pcr\": " << pcrIndex << ",\n";
    ss << "      \"timestamp\": \"" << timestamp << "\",\n";
    ss << "      \"status\": \"" << (success ? "SUCCESS" : "FAILED") << "\"\n";
    ss << "    }";
    return ss.str();
}

} // namespace securebootx
