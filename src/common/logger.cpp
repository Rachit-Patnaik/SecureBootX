#include "common/logger.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace securebootx {

void Logger::log(Level level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::string levelStr;
    switch (level) {
        case Level::INFO:    levelStr = "INFO"; break;
        case Level::WARNING: levelStr = "WARN"; break;
        case Level::ERROR:   levelStr = "ERR "; break;
        case Level::DEBUG:   levelStr = "DBUG"; break;
    }

    std::cout << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << levelStr << "] " << message << std::endl;
}

} // namespace securebootx
