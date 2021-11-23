// Includes---------------------------------------------------------------------
#include "ApplicationConstants.h"
#include "AlmondSecrets.h"

// Implementation---------------------------------------------------------------
//  Note: G++ seems to require that these are in the proper order (same as
//  declared).
const Constants CONSTANTS = {
		.email =
				{
						.server_host = EMAIL_HOST,
						.server_port = EMAIL_PORT,
						.login = EMAIL_LOGIN,
						.password = EMAIL_PASSWORD,
						.receiver = "francismasha96@gmail.com",
				},
		.wlan =
				{
						.ssid = SECRET_SSID,
						.password = SECRET_PASS,
				},
		.runtable =
				{
						// Run the pump for 1min every 15min from 7:00 to 20:00
						.active_start = {.hour = 7, .minute = 0, .second = 0},
						.active_stop = {.hour = 20, .minute = 15, .second = 0},
						.period_on = {.hour = 0, .minute = 1, .second = 0},
						.period_off = {.hour = 0, .minute = 14, .second = 0},
				},
		.pump =
				{
						.low_level_height_mm = 600,
						.threshold_water_up_s = 10,
				},
		.mqtt = {.mqtt_server = MQTT_SERVER_URL,
             .mqtt_port = MQTT_PORT,
             .mqtt_user = MQTT_USER,
             .mqtt_password = MQTT_PASSWORD},
		.firebase = {.firebase_host = FIREBASE_HOST,
                 .firebase_token = FIREBASE_TOKEN,
                 .firebase_server_key = FIREBASE_FCM_SERVER_KEY},
		.password = "",
		.timezone_h = 3,
		.baudrate = 115200};
