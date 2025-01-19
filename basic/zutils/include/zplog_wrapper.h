#pragma once
#include <cstdarg>
#include <cstdio>
#include<plog/Log.h>
inline void log_info_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_INFO << buffer;
}

inline void log_verbose_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_VERBOSE << buffer;
}

inline void log_debug_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_DEBUG << buffer;
}

inline void log_error_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_ERROR << buffer;
}

inline void log_warning_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_WARNING << buffer;
}

inline void log_fatal_c_style(const char* fmt, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	std::vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	LOG_FATAL << buffer;
}


#define LOG_INFO_C_STYLE(fmt, ...) log_info_c_style(fmt, __VA_ARGS__)
#define LOG_VERBOSE_C_STYLE(fmt, ...) log_verbose_c_style(fmt, __VA_ARGS__)
#define LOG_DEBUG_C_STYLE(fmt, ...) log_debug_c_style(fmt, __VA_ARGS__)
#define LOG_ERROR_C_STYLE(fmt, ...) log_error_c_style(fmt, __VA_ARGS__)
#define LOG_WARNING_C_STYLE(fmt, ...) log_warning_c_style(fmt, __VA_ARGS__)
#define LOG_FATAL_C_STYLE(fmt, ...) log_fatal_c_style(fmt, __VA_ARGS__)


