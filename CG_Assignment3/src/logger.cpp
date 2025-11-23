// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  _     ___   ____  ____ _____ ____  
// | |   / _ \ / ___|/ ___| ____|  _ \ 
// | |  | | | | |  _| |  _|  _| | |_) |
// | |__| |_| | |_| | |_| | |___|  _ < 
// |_____\___/ \____|\____|_____|_| \_\
// 
// ====================================================================
// File: logger.cpp
// Description: Logger implementation for debug and info messages.
// 
// ====================================================================

#include "logger.h"
#include <iostream>
#include <ctime>

//
Logger& Logger::instance() {
	static Logger instance;
	return instance;
}	

// Destructor: Close log file if open
Logger::~Logger() {
	if (m_logFile.is_open()) {
		m_logFile.close();
	}
}

void Logger::setLevel(LogLevel level) {
	m_level = level;
}

void Logger::enableFileOutput(const std::string& filename) {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_logFile.is_open()) {
		m_logFile.close();
	}
	m_logFile.open(filename, std::ios::out | std::ios::app);
	if (!m_logFile) {
		std::cerr << "[Logger] Failed to open log file: " << filename << std::endl;
	}
}

const char* Logger::levelToString(LogLevel level) {
	switch (level) {
	case LogLevel::Debug: return "DEBUG";
	case LogLevel::Info:  return "INFO";
	case LogLevel::Warn:  return "WARN";
	case LogLevel::Error: return "ERROR";
	default:              return "UNKNOWN";
	}
}

void Logger::log(LogLevel level,
	const std::string& message,
	const char* file,
	int line,
	const char* func)
{
	// Check log level
	if (level < m_level) return;

	// Lock for thread safety`
	std::lock_guard<std::mutex> lock(m_mutex);

	// Get current time
	std::time_t now = std::time(nullptr);
	char timeStr[26];
	ctime_s(timeStr, sizeof(timeStr), &now);
	timeStr[24] = '\0'; // Remove newline

	// Format log message
	std::ostringstream os;
	os << "[" << timeStr << "] "
		<< "[" << levelToString(level) << "] "
		<< file << ":" << line << " (" << func << ") - "
		<< message << std::endl;

	std::cout << os.str();

	if (m_logFile.is_open()) {
		m_logFile << os.str();
		m_logFile.flush();
	}
}