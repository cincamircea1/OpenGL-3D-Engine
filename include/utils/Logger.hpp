#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>

/**
 * Logger class for application-wide logging.
 * Provides formatted logging with timestamps and log levels.
 */
class Logger {
public:
    /**
     * Log levels for different types of messages
     */
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    /**
     * Get the singleton instance of the logger
     * @return Reference to the logger instance
     */
    static Logger& getInstance();

    /**
     * Log a message with the specified log level
     * @param level The severity level of the log
     * @param message The message to log
     */
    void log(LogLevel level, const std::string& message);

    /**
     * Set the minimum log level to display
     * @param level The minimum log level to show
     */
    void setLogLevel(LogLevel level);

    // Convenience methods for different log levels
    void trace(const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);

private:
    // Private constructor (singleton pattern)
    Logger();
    
    // Deleted copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Current minimum log level
    LogLevel m_logLevel;
    
    // Mutex for thread safety
    std::mutex m_mutex;

    /**
     * Convert LogLevel to string representation
     * @param level The log level to convert
     * @return String representation of the log level
     */
    std::string getLogLevelString(LogLevel level);
    
    /**
     * Get current time as formatted string
     * @return Current timestamp as string
     */
    std::string getCurrentTimestamp();
}; 