//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "Logger.h"
#include "Serial.h"

Logger LOG;

Logger::Logger()
{
	this->status = Status::BOOTING;
	this->led_pin = -1;
	this->line_loop = 0;
	this->serial_baudrate = 0;
	this->fatal_hook = nullptr;
	memset(this->buffer, 0x00, sizeof(this->buffer));
	memset(this->format, 0x00, sizeof(this->format));
}

void Logger::setup_serial(const char *hostname, int baudrate)
{
	serial_setup(hostname, baudrate);
	this->serial_baudrate = baudrate;
}

void Logger::setup_fatal_hook(LoggerFatalHook hook)
{
	this->fatal_hook = hook;
}

void Logger::setup_led(int _led_pin)
{
	this->led_pin = _led_pin;
	pinMode(_led_pin, OUTPUT);
	digitalWrite(_led_pin, 1);
	this->led_state = true;
	this->led_timer.reset();
}

Logger::Status Logger::get_status()
{
	this->status = status;
}

void get_uptime(InfoUptime *uptime)
{
	unsigned long c_s = (millis() + 500) / 1000;
	unsigned long c_h = (c_s / 3600);
	unsigned long c_m = (c_s - c_h * 3600) / 60;

	uptime->seconds = (c_s % 60);
	uptime->minutes = c_m;
	uptime->hours = c_h;
}

void Logger::log(Logger::Level level,
				 const __FlashStringHelper *format_flash,
				 ...)
{
	va_list argument_list;
	va_start (argument_list, format_flash);

	char *buff = &buffer[line_loop * max_line_len];

	memset(buff, 0x00, max_line_len);
	// make the level visible
	const char log_level_desc[] = {'D', 'I', 'W', 'E', 'F'};

	InfoUptime uptime{};
	get_uptime(&uptime);

	// the last zero will never get overwritten
	strncpy_P(format, (const char *)format_flash, max_line_len - 1);

	int prefix_len = snprintf(buff,
							  max_line_len,
							  "[%02u:%02u:%02u] %c:",
							  uptime.hours,
							  uptime.minutes,
							  uptime.seconds,
							  log_level_desc[to_integral(level)]);


	// fits or not, we have zeroed the whole thing so there will be ending zero.
	int print_len = vsnprintf(buff + prefix_len,
							  max_line_len - prefix_len - 1,
							  format,
							  argument_list);

	int buffer_len = prefix_len + print_len;

	if (buffer_len >= max_line_len)
		buffer_len = max_line_len - 1;

	va_end(argument_list);

	line_loop += 1;
	if (line_loop >= max_lines)
		line_loop = 0;

	if (serial_baudrate > 0) {
		serial_print_raw(buff, buffer_len, true);
	}

	if (level == Logger::Level::FATAL)
		Logger::set_status(Logger::Status::ERROR);

	if (level == Logger::Level::FATAL || level == Logger::Level::ERROR) {
		if (this->fatal_hook != nullptr)
			this->fatal_hook(buff);
	}
}

void Logger::loop()
{
	int interval = 4000;
	if (this->status == Logger::Status::RUNNING) {
		interval = 250;
	}
	else if (this->status == Logger::Status::ERROR) {
		interval = 1000;
	}

	if (this->led_timer.check(interval)) {
		this->led_state = !this->led_state;
		digitalWrite(led_pin, this->led_state);
		this->led_timer.reset();
	}
}

const char *Logger::get_log_line(int line_number)
{
	// we fill it like 0,1,2,3,N-1,0,1,2,3 -> return 3,2,1,N-1,
	// if we have 3 -> line loop = 3 -> return 2,1,0

	int loop = (2 * max_lines + (line_loop - 1) - line_number) % max_lines;

	char *buf = &buffer[loop * max_line_len];
	if (buf[0] == 0)
		return nullptr;
	return buf;

}

void Logger::set_status(Logger::Status status)
{
}
