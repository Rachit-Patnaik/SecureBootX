#pragma once

#include <string>
#include <iostream>

namespace securebootx {

class Logger {
public:
    enum class Level {
        INFO,
        WARNING,
        ERROR,
        DEBUG
    };

    static void log(Level level, const std::string& message);

    static void info(const std::string& message) { log(Level::INFO, message); }
    static void warn(const std::string& message) { log(Level::WARNING, message); }
    static void error(const std::string& message) { log(Level::ERROR, message); }
    static void debug(const std::string& message) { log(Level::DEBUG, message); }
};

} // namespace securebootx
