#include "securebootx/logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::init(const std::string& log_path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!log_path.empty()) {
        log_file_.open(log_path, std::ios::out | std::ios::app);
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");

    std::string level_str;
    std::string color_code;
    std::string reset_code = "\033[0m";

    switch (level) {
        case LogLevel::INFO:
            level_str = "INFO";
            color_code = "\033[36m"; // Cyan
            break;
        case LogLevel::SUCCESS:
            level_str = "SUCCESS";
            color_code = "\033[32m"; // Green
            break;
        case LogLevel::WARNING:
            level_str = "WARNING";
            color_code = "\033[33m"; // Yellow
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            color_code = "\033[31m"; // Red
            break;
        case LogLevel::SECURITY:
            level_str = "SECURITY";
            color_code = "\033[35m"; // Magenta
            break;
    }

    std::string formatted = "[" + ss.str() + "] [" + level_str + "] " + message;

    std::cout << color_code << formatted << reset_code << std::endl;

    if (log_file_.is_open()) {
        log_file_ << formatted << std::endl;
    }
}
