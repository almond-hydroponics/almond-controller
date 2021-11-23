// Includes---------------------------------------------------------------------
#include "SetupWifi.h"

// Declarations-----------------------------------------------------------------
ESP8266WiFiMulti wifiMulti;

#define NTP_SERVER_HOST_1 "time.google.com"
#define NTP_SERVER_HOST_2 "pool.ntp.org"
#define NTP_SERVER_HOST_3 "time.windows.com"
#define GMT_OFFSET_SEC 108000
#define DAYLIGHT_OFFSET 0

// Static IP configuration
#define IPSET_STATIC                                                           \
	{                                                                            \
		10, 10, 10, 13                                                             \
	}
#define IPSET_GATEWAY                                                          \
	{                                                                            \
		10, 10, 10, 254                                                            \
	}
#define IPSET_SUBNET                                                           \
	{                                                                            \
		255, 255, 255, 0                                                           \
	}
#define IPSET_DNS                                                              \
	{                                                                            \
		10, 10, 10, 254                                                            \
	}

byte ip_static[] = IPSET_STATIC;
byte ip_gateway[] = IPSET_GATEWAY;
byte ip_subnet[] = IPSET_SUBNET;
byte ip_dns[] = IPSET_DNS;

// Implementation---------------------------------------------------------------
// Set time via NTP, as required for x.509 validation
void SetupWifi::setClock()
{
	// void configTime(int timezone, int daylightOffset_sec, const char* server1,
	// const char* server2, const char* server3)
	// https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
	// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/time.cpp
	configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET, NTP_SERVER_HOST_1,
	           NTP_SERVER_HOST_2, NTP_SERVER_HOST_3);

	setClock_status = STARTED;
	LOG_INFO("Waiting for NTP time sync.");
	setClock_AsyncWait.startWaiting(millis(), 1000); // Log every 1 second
	// Asynchronously wait for network response via checkClockStatus().
}

// Check Clock Status and update 'setClock_status' accordingly.
void SetupWifi::checkClockStatus()
{
	time_t now = time(nullptr);
	if (now < 3 * 3600 * 20)
	{
		// The NTP request has not yet completed.
		if (!setClock_AsyncWait.isWaiting(millis()))
		{
			LOG_INFO("Setting time.");
			// DEBUG_LOG("*");
			// Log every 1 second.
			setClock_AsyncWait.startWaiting(millis(), 1000);
		}
		return;
	}

	// The NTP request has completed
	setClock_status = SUCCESS;
	//	setDeviceRtc.update_time(now);

#ifdef DEBUG
	struct tm timeinfo
	{
	};
	gmtime_r(&now, &timeinfo);
	LOG_INFO("Current time: %s", asctime(&timeinfo));
	LOG_INFO("Epoch time: %d", now);
#endif
}

String SetupWifi::getMacAddress()
{
	byte mac[6];
	String macStr;

	WiFi.macAddress(mac);
	macStr = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" +
	         String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" +
	         String(mac[4], HEX) + ":" + String(mac[5], HEX);

	return macStr;
}

void SetupWifi::setupWifi()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		WiFi.mode(WIFI_STA);
		wifiMulti.addAP(ssid, password);
		//		wifiMulti.addAP("Masha Francis", "fkonde14#");
		LOG_INFO("Wifi connecting to %s", ssid);

		int attempt = 0;
		while (wifiMulti.run() != WL_CONNECTED)
		{
			DEBUG_LOG(".");
			DEBUG_LOG(attempt);
			delay(500);
			attempt++;

			if (attempt == 150)
			{
				LOG_FATAL("Could not connect to WiFi: Rebooting");
				delay(100);
				ESP.restart();
				delay(500);
			}
		}

		LOG_INFO("Connected to %s - IP: %s", ssid,
		         WiFi.localIP().toString().c_str());

		WiFi.setAutoConnect(true);
		WiFi.persistent(true);

		randomSeed(micros());
		setClock();

		LOG_INFO("WiFi is ready.");
	}
}

void SetupWifi::loopWifi()
{
	// Prevent ALL other actions here until the clock as been set by NTP.
	if (setClock_status < FINISHED)
	{
		checkClockStatus();
		return;
	}
}
