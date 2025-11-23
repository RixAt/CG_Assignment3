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
// File: logger.h
// Description: Logging utility header file
// 
// ====================================================================

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <mutex>

// Enum Class: LogLevel, defines logging severity levels
enum class LogLevel {
	Debug,
	Info,
	Warn,
	Error
};

// Class: Logger
class Logger {
public:
	static Logger& instance();

	void setLevel(LogLevel level);
	void enableFileOutput(const std::string& filename);
	void log(LogLevel level,
		const std::string& message,
		const char* file,
		int line,
		const char* func);
private:
	Logger() = default;
	~Logger();

	LogLevel m_level = LogLevel::Debug;
	std::ofstream m_logFile;
	std::mutex m_mutex;

	const char* levelToString(LogLevel level);
};

// Macro definitions for easy logging
#define LOG_DEBUG(msg) Logger::instance().log(LogLevel::Debug, (msg), __FILE__, __LINE__, __func__)
#define LOG_INFO(msg)  Logger::instance().log(LogLevel::Info,  (msg), __FILE__, __LINE__, __func__)
#define LOG_WARN(msg)  Logger::instance().log(LogLevel::Warn,  (msg), __FILE__, __LINE__, __func__)
#define LOG_ERROR(msg) Logger::instance().log(LogLevel::Error, (msg), __FILE__, __LINE__, __func__)

// Macros for logging expressions
#define LOGI(expr) do { std::ostringstream _os; _os << expr; LOG_INFO(_os.str()); } while(0)
#define LOGE(expr) do { std::ostringstream _os; _os << expr; LOG_ERROR(_os.str()); } while(0)