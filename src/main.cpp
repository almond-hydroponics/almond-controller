#define MAJOR_VER "00"
#define MINOR_VER "01"

// Includes---------------------------------------------------------------------
#include <EEPROM.h>
#include <FirebaseESP8266.h>

#include "AlmondConfiguration.h"
#include "AlmondOTA.h"
#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "ApplicationLogic.h"
#include "DevicePinInput.h"
#include "DevicePinOutput.h"
#include "DeviceRtc.h"
#include "EnvironmentSht.h"
#include "Globals.h"
#include "Logger.h"
#include "MqttServer.h"
#include "SecureCredentials.h"
#include "SendEmail.h"
#include "Serial.h"
#include "SetupWifi.h"
#include "WaterLevel.h"
#include "Webserver.h"

#ifndef STASSID
#define STASSID CONSTANTS.wlan.ssid
#define STAPSK CONSTANTS.wlan.password
#endif

const char* ID = "sec_mqtt_client_" MAJOR_VER "_" MINOR_VER;

// Declarations-----------------------------------------------------------------
DeviceRtc DEV_RTC("time");
WaterLevel DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO);
EnvironmentSht DEV_TEMP("temp");
EnvironmentSht DEV_HUMID("humid");
DevicePinOutput
		DEV_PUMP("pump", PIN_PUMP,
             false); // Pump inverted - writing 0 will start the pump
DevicePinInput DEV_SWITCH("switch", PIN_SWITCH, 8, true);
DevicePinInput DEV_WDETECT("water_detect", PIN_WDETECT, 4, true);
ApplicationLogic APPLICATION_LOGIC;

// Wifi declaration
SetupWifi setupWifi(STASSID, STAPSK, CA_CERT_PROG, CLIENT_CERT_PROG,
                    CLIENT_KEY_PROG
                    //		DeviceRtc(nullptr)
);

// Mqtt client declaration
MqttServer mqttClient(CONSTANTS.mqtt.mqtt_server, CONSTANTS.mqtt.mqtt_user,
                      CONSTANTS.mqtt.mqtt_password, setupWifi.getWiFiClient(),
                      reinterpret_cast<const char*>(ESP.getChipId()),
                      CONSTANTS.mqtt.mqtt_port);

// Firebase data object declaration
FirebaseData firebaseDto;

static const int UPDATE_DELAY = 20;
static const int FATAL_REBOOT_DELAY = 2250.0;

static TimerOverride Local_reset_fatal_timer;
static bool Local_reset_fatal_timer_started = false;

/** Make some artificial devices for more information */
class Device_uptime : public DeviceInput
{
	public:
	Device_uptime() : DeviceInput("uptime"){};
	void loop() override { this->value = millis() / 1000; };
};

class Device_status : public DeviceInput
{
	public:
	Device_status() : DeviceInput("status"){};
	void loop() override { this->value = (int)LOG.get_status(); };
};

class Topic
{
	private:
	std::unique_ptr<char> topic{};

	public:
	explicit Topic(const char size) { topic = std::make_unique<char>(size); }
	std::unique_ptr<char> getTopic() {
			return topic;
	};
};

class TopicObject
{
	public:
	TopicObject() : _ptr(new char) { }
	~TopicObject() { delete this->_ptr; }

	private:
	char* _ptr;
};

Device_uptime DEV_UPTIME;
Device_status DEV_STATUS;

Device* const DEVICES[] = {&DEV_TEMP, &DEV_HUMID, &DEV_WLEVEL, &DEV_WDETECT,
                           &DEV_PUMP, &DEV_UPTIME,
                           //	&DEV_RTC,
                           &DEV_STATUS, &DEV_SWITCH};
#define DEVICES_N (sizeof(DEVICES) / sizeof(Device*))

// Implementation---------------------------------------------------------------
static void logger_fatal_hook(const char* log_line)
{
	if (!Local_reset_fatal_timer_started)
	{
		Local_reset_fatal_timer.reset();
		Local_reset_fatal_timer_started = true;
	}

	// if we are not connected, we are not storing the messages for now.
	if (!setupWifi.isReadyForProcessing())
		return;

	int buffer_len = Logger::max_line_len + 128;
	int subject_len = 256;
	char* buffer = (char*)malloc(buffer_len);
	char* subject = (char*)malloc(subject_len);

	memset(buffer, 0x00, buffer_len);
	memset(subject, 0x00, subject_len);

	// out of memory, lets skip the whole thing.
	if (buffer == nullptr || subject == nullptr)
		return;

	snprintf(buffer, buffer_len - 1, "Error on %s: %s.\n", CONSTANTS.hostname,
	         log_line);

	snprintf(subject, subject_len - 1, "[ESP] %s : error detected",
	         CONSTANTS.hostname);

	email_send(&CONSTANTS.email, CONSTANTS.email.receiver, subject, buffer);

	free(subject);
	free(buffer);
}

static int generate_device_json(char* buffer)
{
	strcpy(buffer, "{\"dev\":[");
	int buffer_offset = strlen(buffer);
	unsigned int loop = 0;
	for (loop = 0; loop < DEVICES_N; loop++)
	{
		Device* dev = DEVICES[loop];
		int added = dev->jsonify(buffer + buffer_offset,
		                         WEBSERVER_MAX_RESPONSE_SIZE - buffer_offset);

		if (added == 0)
			break;

		buffer_offset += added;

		if (buffer_offset + 2 >= WEBSERVER_MAX_RESPONSE_SIZE)
			break;

		buffer[buffer_offset] = ',';
		buffer[buffer_offset + 1] = 0;
		buffer_offset += 1;
	}

	if (loop < DEVICES_N)
	{
		// exited with break
		return 0;
	}
	else
	{
		buffer[buffer_offset - 1] = ']';
		buffer[buffer_offset] = '}';
		buffer[buffer_offset + 1] = 0;
		return (buffer_offset + 1);
	}
}

static void handle_get_devices()
{
	char* buffer = webserver_get_buffer();
	if (buffer == nullptr)
		return;
	int blen = generate_device_json(buffer);

	if (blen == 0)
	{
		server.send(500, "application/json", R"({"error":"out of buffer"})");
	}
	else
	{
		server.send(200, "application/json", buffer);
	}
	free(buffer);
}

#ifndef NDEBUG
static bool handle_set_email()
{
	LOG_INFO("Status email requested.");
	const int subject_len = 256;
	char* subject = (char*)malloc(subject_len);
	char* buffer = (char*)malloc(WEBSERVER_MAX_RESPONSE_SIZE);

	memset(subject, 0x00, subject_len);
	memset(buffer, 0x00, WEBSERVER_MAX_RESPONSE_SIZE);

	snprintf(subject, subject_len, "[ESP] %s : Status report",
	         CONSTANTS.hostname);

	int blen = generate_device_json(buffer);
	LOG_INFO("Device JSON: %d", blen);
	serial_print_raw(buffer, blen, true);
	bool ret =
			email_send(&CONSTANTS.email, CONSTANTS.email.receiver, subject, buffer);

	free(subject);
	free(buffer);

	return ret;
}
#endif

static int raw_device_json(char* buffer)
{
	const int buffer_size = MESSAGE_MAX_LEN;
	int buffer_offset = snprintf(buffer, buffer_size, "{");
	unsigned int loop = 0;
	int ret = 0;

	for (loop = 0; loop < LOGGED_DEVICES; loop++)
	{
		Device* dev = DEVICES[loop];
		ret = dev->jsonify(buffer + buffer_offset, buffer_size - buffer_offset);

		if (ret == 0)
			break;

		buffer_offset += ret;

		if (buffer_offset + 2 >= MESSAGE_MAX_LEN)
			break;

		buffer[buffer_offset] = ',';
		buffer[buffer_offset + 1] = 0;
		buffer_offset += 1;
	}

	if (loop < LOGGED_DEVICES)
	{
		// exited with break
		return 0;
	}
	else
	{
		buffer[buffer_offset - 1] = '}';
		//		buffer[buffer_offset] = '}';
		buffer[buffer_offset + 1] = 0;

		return (buffer_offset + 1);
	}
}

static bool handle_push_devices(bool force)
{
	static TimerOverride timer;
	static const int push_interval_s = PUSH_INTERVAL;
	static const int buffer_size = 4096;

	if (!force && !timer.check(push_interval_s))
		return false;
	timer.reset();

	char* buffer = (char*)malloc(buffer_size);
	if (buffer == nullptr)
		return false;
	raw_device_json(buffer);

//	char* topic_data = (char*)malloc(40);
	Topic topic(40);
//	sprintf(topicObject, "%08X/data", ESP.getChipId());

	// send data through publish topic
	bool ret = mqttClient.publish(topic, buffer);

//	free(topic_data);
	free(buffer);

	return ret;
}

static bool handle_set_push()
{
	LOG_INFO("Data push requested.");
	return handle_push_devices(true);
}

static void handle_http(bool ret)
{
	char* buffer = webserver_get_buffer();
	if (buffer == nullptr)
		return;

	int resp_code = ret ? 200 : 500;
	const char* code = ret ? "ok" : "err";

	snprintf(buffer, WEBSERVER_MAX_RESPONSE_SIZE, R"({"status":"%s"})", code);
	server.send(resp_code, "application/json", buffer);
	free(buffer);
}

void handle_get_time()
{
	Config_run_table_time time{};
	char* buffer = webserver_get_buffer();
	if (buffer == nullptr)
		return;

	DEV_RTC.time_of_day(&time);

	snprintf(buffer, WEBSERVER_MAX_RESPONSE_SIZE,
	         R"({"hour":%d,"min":%d,"sec":%d})", time.hour, time.minute,
	         time.second);

	server.send(200, "application/json", buffer);
	free(buffer);
}

static bool handle_reboot()
{
	LOG_INFO("Reboot requested: Reboot now!");
	ESP.restart();

	return true;
}

void add_password_protected(const char* url, void (*handler)())
{
	char* buffer = (char*)malloc(1024);
	strcpy(buffer, "/set/");
	strcat(buffer, CONSTANTS.password);
	strcat(buffer, "/");
	strcat(buffer, url);
	server.on(buffer, handler);
	free(buffer);
}

void onConnectionEstablished()
{
	char* topic_pump = (char*)malloc(40);
	sprintf(topic_pump, "%08X/pump", ESP.getChipId());
	// Subscribe to "almond/Pump" and display received message on serial
	mqttClient.subscribe(topic_pump,
	                     [](const String& payload) {
												 LOG_INFO("Subscribed to %08X/pump", ESP.getChipId());
											 });

	free(topic_pump);

	char* topic_test = (char*)malloc(40);
	sprintf(topic_test, "%08X/test", ESP.getChipId());
	// Publish a message to "almond/Pump
	mqttClient.publish(topic_test, "This is a test.");

	// Execute delayed instructions
	//	mqttClient.executeDelayed(5 * 1000, [&topic_test]() {
	//		mqttClient.publish(topic_test,
	//									 "This is a message sent after 5 seconds
	// later."); 		LOG_INFO("Published to %08X/test", ESP.getChipId());
	//	});

	free(topic_test);
}

void wifiLedOnConnecting()
{
	digitalWrite(WIFI_LED, HIGH);
	delay(500);
	digitalWrite(WIFI_LED, LOW);
	delay(500);
}

void fcmSendMessage()
{
	int count = 0;
	firebaseDto.fcm.setNotifyMessage("Notification",
	                                 "Hello World! " + String(count));
	firebaseDto.fcm.setDataMessage("{\"myData\":" + String(count) + "}");

	// Firebase.broadcastMessage(fbdo)
	// Firebase.sendTopic(fbdo)
	LOG_INFO("Send message... %s\n", Firebase.sendMessage(firebaseDto, 0)
	                                     ? "ok"
	                                     : firebaseDto.errorReason().c_str());

	if (firebaseDto.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
		LOG_INFO("%s", firebaseDto.fcm.getSendResult());

	count++;
}

void setup()
{
	// SETUP ESP8266 DEVICE
#ifdef DEBUG
	LOG.setup_serial(CONSTANTS.hostname, CONSTANTS.baudrate);
#endif

	//	LOG.setup_fatal_hook(logger_fatal_hook);
	AlmondConfiguration::setup();
	setupWifi.setupWifi();
	AlmondOTA::setup();
	webserver_setup();

	// setup firebase
	//	LOG_INFO("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
	//	Firebase.reconnectWiFi(true);
	//	firebaseDto.fcm.begin(firebaseFcmServerKey);
	//	firebaseDto.fcm.addDeviceToken(FIREBASE_FCM_DEVICE_TOKEN);
	//	firebaseDto.fcm.setPriority("high");
	//	firebaseDto.fcm.setTimeToLive(1000);

	//	fcmSendMessage();

	// setup mqtt
	mqttClient.enableLastWillMessage("almond/lastWill", "Going offline...");

	for (auto loop : DEVICES)
		loop->setup();

	LOG.set_status(Logger::Status::RUNNING);

	//	server.on("/get/dev", handle_get_devices);
	//	server.on("/get/time", handle_get_time);
	add_password_protected("reboot", [] { handle_http(handle_reboot()); });
}

#ifndef NDEBUG
static void handle_serial()
{
	int line_len = 0;
	char* line = serial_receive(&line_len);

	if (line == nullptr)
		return;

	LOG_WARN("Serial: %s ", line);

	if (strcmp(line, "email") == 0)
	{
		handle_set_email();
	}
	else if (strcmp(line, "push") == 0)
	{
		handle_set_push();
	}
	else
	{
		LOG_WARN("Invalid command");
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
	unsigned long avail_memory_now = AlmondConfiguration::get_free_heap();

	if (avail_memory_now < avail_memory_last)
	{
		LOG_INFO("Memory available: %u", avail_memory_now);
		avail_memory_last = avail_memory_now;
	}

	setupWifi.loopWifi();
	// if Wi-Fi is not ready, don't do any other processing
	if (!setupWifi.isReadyForProcessing())
	{
		wifiLedOnConnecting();
		return;
	};

	digitalWrite(WIFI_LED, HIGH);

	// OTA loop
	AlmondOTA::loop();

	// webserver loop
	webserver_loop();
	// mqtt loop
	mqttClient.loop();

	delay(10);

	for (auto loop : DEVICES)
		loop->loop();

	Config_run_table_time time_now{};
	//	DEV_RTC.time_of_day(&time_now);

	if (LOG.get_status() == Logger::Status::ERROR)
	{
		if (Local_reset_fatal_timer_started &&
		    Local_reset_fatal_timer.check(FATAL_REBOOT_DELAY * 1000))
		{
			LOG_INFO("Reboot time is up: Rebooting now!");
			ESP.restart();
		}
	}

	if (update_when_elapsed)
	{
		if (update_timer.check(UPDATE_DELAY * 1000))
		{
			update_when_elapsed = false;
			int delays[2];
			bool valid = APPLICATION_LOGIC.get_measurements(delays);
			if (valid)
			{
				LOG_INFO("Values read successfully. Replace with push function "
				         "to azure or "
				         "GCP ####");
			}
			else
			{
				LOG_WARN("Delay measurement failed");
			}
		}
	}

	bool logic_changed = APPLICATION_LOGIC.run_logic(
			&time_now, &DEV_PUMP, DEV_WLEVEL.get_value(), DEV_WDETECT.get_value(),
			DEV_SWITCH.get_value());

	if (logic_changed)
	{
		LogicStatus status = APPLICATION_LOGIC.get_status();

		// if new status is idle, then we were pumping -> push the delays
		// (assuming valid)
		if (status == LogicStatus::IDLE)
		{
			update_when_elapsed = true;
			update_timer.reset();
		}
		else if (status == LogicStatus::PUMP_STARTED ||
		         status == LogicStatus::DRAINING)
		{
			handle_push_devices(true);
		}
	}
	else
	{
		handle_push_devices(false);
	}
}
