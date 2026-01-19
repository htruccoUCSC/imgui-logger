#include "Logger.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>

// Define the static instance
Logger* Logger::instance = nullptr;

// Singleton getter
Logger& Logger::GetInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return *instance;
}

// Log methods
void Logger::LogInfo(const std::string& message) {
    AddEntry(LogLevel::Info, "[INFO]", message);
    std::cout << "[INFO]: " << message << std::endl;
}

void Logger::LogWarning(const std::string& message) {
    AddEntry(LogLevel::Warning, "[WARN]", message);
    std::cout << "[WARN]: " << message << std::endl;
}

void Logger::LogError(const std::string& message) {
    AddEntry(LogLevel::Error, "[ERROR]", message);
    std::cout << "[ERROR]: " << message << std::endl;
}

void Logger::LogGameEvent(const std::string& event) {
    AddEntry(LogLevel::GameEvent, "[GAME]", event);
    std::cout << "[INFO][GAME]: " << event << std::endl;
}

void Logger::LogGameWarning(const std::string& warning) {
    AddEntry(LogLevel::GameWarning, "[GAME]", warning);
    std::cout << "[WARN][GAME]: " << warning << std::endl;
}

void Logger::LogGameError(const std::string& error) {
    AddEntry(LogLevel::GameError, "[GAME]", error);
    std::cout << "[ERROR][GAME]: " << error << std::endl;
}

// Accessors
const std::vector<LogEntry>& Logger::GetEntries() const {
    return entries;
}

void Logger::ClearLog() {
    entries.clear();
}

size_t Logger::GetEntryCount() const {
    return entries.size();
}

// Helper to add an entry
void Logger::AddEntry(LogLevel level, const std::string& tag, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto localTime = std::localtime(&time);

    LogEntry entry;
    entry.level = level;
    entry.hour = localTime->tm_hour;
    entry.minute = localTime->tm_min;
    entry.second = localTime->tm_sec;
    entry.message = message;
    entry.tag = tag;

    entries.push_back(entry);
}

// Helper to format one entry line
static std::string format_entry(const LogEntry& entry)
{
	std::ostringstream os;
	os << '['
	   << std::setw(2) << std::setfill('0') << entry.hour << ':'
	   << std::setw(2) << std::setfill('0') << entry.minute << ':'
	   << std::setw(2) << std::setfill('0') << entry.second
	   << "] " << entry.tag << ' ' << entry.message;
	return os.str();
}

std::string Logger::ExportToString() const
{
	std::ostringstream os;
	for (const auto& e : entries)
	{
		os << format_entry(e) << '\n';
	}
	return os.str();
}

bool Logger::SaveToFile(const std::string& filepath) const
{
	std::ofstream out(filepath, std::ios::out | std::ios::trunc);
	if (!out.is_open())
		return false;
	for (const auto& e : entries)
	{
		out << format_entry(e) << '\n';
	}
	out.close();
	return true;
}
