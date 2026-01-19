#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>


enum class LogLevel {
    Info,
    Warning,
    Error,
    GameEvent,
    GameWarning,
    GameError
};

struct LogEntry {
    LogLevel level;
    int hour;
    int minute;
    int second;
    std::string message;
    std::string tag;  // "[INFO]", "[WARN]", "[ERROR]", "[GAME]", etc.
};

class Logger {
public:
    // Get the singleton instance of the Logger
    static Logger& GetInstance();

    // Log an informational message
    void LogInfo(const std::string& message);

    // Log a warning message
    void LogWarning(const std::string& message);

    void LogError(const std::string& message);

    // Log a game event
    void LogGameEvent(const std::string& event);

    void LogGameWarning(const std::string& warning);

    void LogGameError(const std::string& error);

    // Get all log entries
    const std::vector<LogEntry>& GetEntries() const;

    // Clear all log entries
    void ClearLog();

    // Get the number of log entries
    size_t GetEntryCount() const;

    // Export all entries into a single formatted string suitable for saving or clipboard
    std::string ExportToString() const;

    // Save all entries into a text file. Returns true on success.
    bool SaveToFile(const std::string& filepath) const;

private:
    // Private constructor to prevent instantiation
    Logger() {}
    
    // Delete copy constructor and assignment operator
    static Logger* instance;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::vector<LogEntry> entries;

    // Helper function to add an entry
    void AddEntry(LogLevel level, const std::string& tag, const std::string& message);
};