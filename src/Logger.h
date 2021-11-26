#pragma once

// Includes---------------------------------------------------------------------
#include "TimerOverride.h"
#include <ESP8266WiFi.h>

#define BAUD_RATE 9600

// Types------------------------------------------------------------------------
using LoggerFatalHook = void (*)(const char *);

class Logger
{
	public:
	enum class Status
	{
		BOOTING,
		RUNNING,
		ERROR
	};
	enum class Level
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	static const int max_lines = 50;
	static const int max_line_len = 80;

	Logger();

	void setup_serial(const char* hostname, int baudrate = BAUD_RATE);
	void setup_led(int _led_pin);
	void setup_fatal_hook(LoggerFatalHook hook);

	Logger::Status get_status();
	void set_status(Logger::Status status);

	void log(Logger::Level level, const __FlashStringHelper* format, ...);

	/// @returns pointer to the given line or NULL if not that many lines
	const char* get_log_line(int line_number);

	void loop();

	private:
	TimerOverride led_timer;
	LoggerFatalHook fatal_hook{nullptr};
	int line_loop{0};
	int serial_baudrate{0};
	int led_pin{-1};
	bool led_state{};
	Logger::Status status{Status::BOOTING};
	char buffer[max_lines * max_line_len]{};
	char format[max_line_len]{};
};

struct InfoUptime
{
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
};

void get_uptime(InfoUptime* uptime);

extern Logger LOG;

#define LOG_INFO(format, ...)::Logger::Level::INFO, F(format), ##__VA_ARGS__
#define LOG_WARN(format, ...)::Logger::Level::WARNING, F(format), ##__VA_ARGS__
#define LOG_ERROR(format, ...)::Logger::Level::ERROR, F(format), ##__VA_ARGS__
#define LOG_FATAL(format, ...)::Logger::Level::FATAL, F(format), ##__VA_ARGS__
