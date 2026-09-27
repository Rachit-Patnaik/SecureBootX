#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace securebootx {

struct Measurement {
    std::string componentName;
    std::string hash;
    int pcrIndex;
    std::string timestamp;
    bool success;

    // For serialization
    std::string toJson() const;
};

} // namespace securebootx
