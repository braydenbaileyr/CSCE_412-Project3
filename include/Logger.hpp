/**
 * @file Logger.hpp
 * @brief Declares a simple file logger for simulation events.
 */
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mutex>
#include <fstream>

/**
 * @class Logger
 * @brief Writes synchronized log messages to the configured output file.
 */
class Logger {
    public:
        /**
         * @brief Opens the log file for appending.
         * @param filePath Path to the log file.
         */
        explicit Logger(const std::string& filePath);

        /**
         * @brief Writes a single message to the log.
         * @param msg Message text to append.
         */
        void log(const std::string& msg);

    private:
        /** @brief Output file used for persistent logs. */
        std::ofstream file_;
        /** @brief Mutex that serializes concurrent log writes. */
        std::mutex mutex_;
};

#endif
