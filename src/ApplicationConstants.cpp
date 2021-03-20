//Includes---------------------------------------------------------------------
#include "ApplicationConstants.h"


//Implementation---------------------------------------------------------------
// Note: G++ seems to require that these are in the proper order (same as declared).
const Constants CONSTANTS = {
	.email = {
		.server_host = "smtp.gmail.com",
		.server_port = 465,
		.login = "",
		.password = "",
		.receiver = "",
	},
	.wlan = {
		.ssid = "",
		.password = "",
	},
	.runtable = {    // Run the pump for 1min every 15min from 7:00 to 20:00
		.active_start = {.hour = 7, .minute = 0, .second = 0},
		.active_stop = {.hour = 20, .minute = 15, .second = 0},
		.period_on = {.hour = 0, .minute = 1, .second = 0},
		.period_off = {.hour = 0, .minute = 14, .second = 0},
	},
	.pump = {
		.low_level_height_mm = 600,
		.threshold_water_up_s = 10,
	},
	.mqtt = {
		.mqtt_server = "mqtt.almondhydroponics.com",
		.mqtt_port = 8883,
		.mqtt_user = "almond",
		.mqtt_password = "froyogreen"
	},
	.firebase = {
		.firebase_host = "almond-data.firebaseio.com",
		.firebase_token = ""
	},
	.azure_connection = "",
	.hostname = "almond",
	.password = "",
	.timezone_h = 3,
	.baudrate = 115200
};
