#define MAJOR_VER "00"
#define MINOR_VER "01"

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include <ESP8266WiFi.h>
#include "ApplicationConstants.h"
#include "ApplicationSetup.h"
#include "ApplicationLogic.h"
#include "DeviceRtc.h"
#include "WaterLevel.h"
#include "EnvironmentSht.h"
#include "SetupWifi.h"
#include "Webserver.h"
#include "SendEmail.h"
#include "NtpClient.h"
#include "Serial.h"
#include "DevicePinInput.h"
#include "DevicePinOutput.h"
#include "Logger.h"
#include "SecureCredentials.h"
#include "Globals.h"


#ifndef STASSID
#define STASSID CONSTANTS.wlan.ssid
#define STAPSK  CONSTANTS.wlan.password
#endif

const char *ssid = STASSID;
const char *password = STAPSK;


//Declarations-----------------------------------------------------------------
DeviceRtc			DEV_RTC("rtc");
WaterLevel			DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO);
EnvironmentSht		DEV_TEMP("temp");
EnvironmentSht		DEV_HUMID("humid");
DevicePinOutput		DEV_PUMP("pump", PIN_PUMP, false);
DevicePinInput		DEV_SWITCH("switch", PIN_SWITCH, 8, true);
ApplicationLogic	APPLICATION_LOGIC;

SetupWifi setupWifi(
	ssid,
	password,
	CA_CERT_PROG,
	CLIENT_CERT_PROG,
	CLIENT_KEY_PROG
);

static const int UPDATE_DELAY = 20;
static const int FATAL_REBOOT_DELAY = 2250.0;

static TimerOverride	Local_reset_fatal_timer;
static bool				Local_reset_fatal_timer_started = false;


/** Make some artificial devices for more information */
class Device_uptime : public DeviceInput
{
public:
	Device_uptime() : DeviceInput("uptime") {};
	void loop() override { this->value = millis()/1000; } ;
};


class Device_status : public DeviceInput
{
public:
	Device_status() : DeviceInput("status") {};
	void loop() override { this->value = (int)LOG.get_status(); } ;
};

Device_uptime DEV_UPTIME;
Device_status DEV_STATUS;

Device *const DEVICES[] = {&DEV_TEMP, &DEV_HUMID, &DEV_WLEVEL, &DEV_PUMP, &DEV_UPTIME, &DEV_RTC, &DEV_STATUS, &DEV_SWITCH};
#define DEVICES_N (sizeof(DEVICES)/sizeof(Device*))


//Implementation---------------------------------------------------------------
static void logger_fatal_hook(const char *log_line)
{
	// if we are not connected, we are not storing the messages for now.
	if (!setupWifi.isReadyForProcessing()) return;

	int buffer_len = Logger::max_line_len + 128;
	int subject_len = 256;
	char *buffer = (char *)malloc(buffer_len);
	char *subject = (char *)malloc(subject_len);

	memset(buffer, 0x00, buffer_len);
	memset(subject, 0x00, subject_len);

	// out of memory, lets skip the whole thing.
	if (buffer == nullptr || subject == nullptr)
		return;

	snprintf(
		buffer,
		buffer_len - 1,
		"Error on %s: %s.\n",
		CONSTANTS.hostname,
		log_line
	);
	snprintf(
		subject,
		subject_len - 1,
		"[ESP] %s : error detected",
		CONSTANTS.hostname
	);

	email_send(&CONSTANTS.email, CONSTANTS.email.receiver, subject, buffer);

	free(subject);
	free(buffer);
}

static int generate_device_json(char *buffer)
{
	strcpy(buffer, "{\"dev\":[");
	int buffer_offset = strlen(buffer);
	unsigned int loop;
	for (loop = 0; loop < DEVICES_N; loop++) {
		Device *dev = DEVICES[loop];
		int added = dev->jsonify(
			buffer + buffer_offset,
			WEBSERVER_MAX_RESPONSE_SIZE - buffer_offset
		);

		if (added == 0) break;

		buffer_offset += added;

		if (buffer_offset + 2 >= WEBSERVER_MAX_RESPONSE_SIZE) break;

		buffer[buffer_offset] = ',';
		buffer[buffer_offset + 1] = 0;
		buffer_offset += 1;
	}

	if (loop < DEVICES_N) {
		// exited with break
		return 0;
	} else {
		buffer[buffer_offset - 1] = ']';
		buffer[buffer_offset] = '}';
		buffer[buffer_offset + 1] = 0;
		return (buffer_offset + 1);
	}
}

static void handle_get_devices()
{
	char *buffer = webserver_get_buffer();

	if (buffer == nullptr) return;

	int blen = generate_device_json(buffer);

	if (blen == 0) {
		WEBSERVER.send(500, "application/json", R"({"error":"out of buffer"})");
	} else {
		WEBSERVER.send(200, "application/json", buffer);
	}
	free(buffer);
}

static bool handle_set_email()
{
	LOG_INFO("Status email requested.");
	const int subject_len = 256;
	char *subject = (char *)malloc(subject_len);
	char *buffer = (char *)malloc(WEBSERVER_MAX_RESPONSE_SIZE);

	memset(subject, 0x00, subject_len);
	memset(buffer, 0x00, WEBSERVER_MAX_RESPONSE_SIZE);

	snprintf(
		subject,
		subject_len,
		"[ESP] %s : Status report",
		CONSTANTS.hostname
	);

	int blen = generate_device_json(buffer);
	serial_print_raw(buffer, blen, true);
	bool ret = email_send(&CONSTANTS.email, CONSTANTS.email.receiver, subject, buffer);

	free(subject);
	free(buffer);

	return ret;
}

static bool handle_set_ntp()
{
	uint32_t ntp_time = ntp_update();
	if (ntp_time == 0) return false;

	DeviceRtc::update_time(ntp_time);

	return true;
}

static void handle_http(bool ret)
{
	char *buffer = webserver_get_buffer();
	if (buffer == nullptr) return;

	int resp_code = ret ? 200 : 500;
	const char *code = ret ? "ok" : "err";

	snprintf(buffer, WEBSERVER_MAX_RESPONSE_SIZE, R"({"status":"%s"})", code);
	WEBSERVER.send(resp_code, "application/json", buffer);
	free(buffer);
}

void handle_get_time()
{
	Config_run_table_time time{};
	char *buffer = webserver_get_buffer();
	if (buffer == nullptr) return;

	DEV_RTC.time_of_day(&time);

	snprintf(
		buffer,
		WEBSERVER_MAX_RESPONSE_SIZE,
		R"({"hour":%d,"min":%d,"sec":%d})",
		time.hour,
		time.minute,
		time.second
	);
	WEBSERVER.send(200, "application/json", buffer);
	free(buffer);
}

static bool handle_reboot()
{
	LOG_INFO("Reboot requested: Reboot now!");
	ESP.restart();

	return true;
}

void add_password_protected(const char *url, void (*handler)())
{
	char *buffer = (char *)malloc(1024);
	strcpy(buffer, "/set/");
	strcat(buffer, CONSTANTS.password);
	strcat(buffer, "/");
	strcat(buffer, url);
	WEBSERVER.on(buffer, handler);
	free(buffer);
}

void setup()
{
	// SETUP ESP8266 DEVICE
#ifdef DEBUG
	LOG.setup_serial(CONSTANTS.hostname, CONSTANTS.baudrate);
#endif

	LOG.setup_led(PIN_LED);
	LOG.setup_fatal_hook(logger_fatal_hook);
	APPLICATION_SETUP.setup();
	setupWifi.setupWifi();

	for (auto loop : DEVICES) loop->setup();

	Logger::set_status(Logger::Status::RUNNING);

	WEBSERVER.on("/get/dev", handle_get_devices);
	WEBSERVER.on("/get/time", handle_get_time);
	add_password_protected("ntp", []{ handle_http(handle_set_ntp()); });
	add_password_protected("reboot", []{ handle_http(handle_reboot()); });
}

#ifndef NDEBUG
static void handle_serial()
{
	int line_len;
	char *line = serial_receive(&line_len);

	if (line == nullptr) return;

	LOG_WARN("Serial: %s ", line);

	if (strcmp(line, "email") == 0) {
		handle_set_email();
	} else {
		serial_print("Invalid command\n");
	}
}
#endif

void loop()
{
	LOG.loop();

	#ifndef NDEBUG
	handle_serial();
	#endif

	static TimerOverride update_timer;
	static bool update_when_elapsed = false;
	static unsigned long avail_memory_last = 0xFFFF;
	unsigned long avail_memory_now = APPLICATION_SETUP.get_free_heap();

	if (avail_memory_now < avail_memory_last) {
		LOG_INFO("Memory: %u", avail_memory_now);
		avail_memory_last = avail_memory_now;
	}

	setupWifi.loopWifi();
	// if wifi is not ready, don't do any other processing
	if (!setupWifi.isReadyForProcessing()) return;

	delay(10);

	for (auto loop : DEVICES) loop->loop();

	Config_run_table_time time_now{};
	DEV_RTC.time_of_day(&time_now);

	Serial.printf(
		"Temp: %d, Humid: %d \n",
		DEV_TEMP.get_value(),
		DEV_HUMID.get_value()
	);
	Serial.printf("Distance: %d \n", DEV_WLEVEL.get_value());
//	DEBUG_LOGLN(asctime(&time_now));
//	Serial.printf("Temp: %d \n", DEV_TEMP.get_value());
//	delay(5000);

	if (LOG.get_status() == Logger::Status::ERROR) {
		if (LOCAL_res)
	}
}
