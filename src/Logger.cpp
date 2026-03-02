/**
 * @file Logger.cpp
 * @brief Implements the file-based logger used by the simulation.
 */
#include "Logger.hpp"
#include <stdexcept>

/**
 * @brief Opens the log file and prepares it for appending.
 * @throws std::runtime_error Thrown when the file cannot be opened.
 */
Logger::Logger(const std::string& filePath) {
    file_.open(filePath, std::ios::out | std::ios::app);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
}

/**
 * @brief Appends a message to the log file.
 */
void Logger::log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    file_ << msg << '\n';
    file_.flush();
}
