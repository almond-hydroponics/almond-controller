#pragma once


//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include <type_traits>


//Types------------------------------------------------------------------------
template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

// User configurations are provided as structs so that it will be easier to
// change and to read.

struct Config_email
{
	const char *server_host;
	int server_port;
	const char *login;
	const char *password;
	const char *receiver;
};

struct Config_wlan
{
	const char *ssid;
	const char *password;
};

struct Config_run_table_time
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

struct Config_run_table
{
	Config_run_table_time active_start;
	Config_run_table_time active_stop;
	Config_run_table_time period_on;
	Config_run_table_time period_off;
};

struct Config_pump
{
	uint16_t low_level_height_mm;
	uint8_t threshold_water_up_s;
};

struct Config_mqtt
{
	const char *mqtt_server;
	int mqtt_port;
	const char *mqtt_user;
	const char *mqtt_password;
};

struct Constants
{
	Config_email email;
	Config_wlan wlan;
	Config_run_table runtable;
	Config_pump pump;
	Config_mqtt mqtt;
	const char *hostname;
	const char *password;
	uint8_t timezone_h;
	int baudrate;
};

extern const Constants CONSTANTS;

// Hardware configurations fixed pins
#define PIN_SCL    14	// board pin 5
#define PIN_SDA    12	// board pin 6
#define PIN_PUMP    15	// board pin 8
#define PIN_LED    4	// board pin 2
#define PIN_SWITCH  16	// board pin 0
#define PIN_TRIGGER 2	// board pin 4
#define PIN_ECHO    5	// board pin 1
#define PIN_WDETECT 13	// board pin 7
#define WIFI_LED    3	// board pin 3

