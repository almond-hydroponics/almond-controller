//Includes---------------------------------------------------------------------
#include "ApplicationConstants.h"
#include "AlmondSecrets.h"


//Implementation---------------------------------------------------------------
// Note: G++ seems to require that these are in the proper order (same as declared).
const Constants CONSTANTS = {
	.email = {
		.server_host = "smtp.gmail.com",
		.server_port = 465,
		.login = "almond.froyo@gmail.com",
		.password = "Froyogreen",
		.receiver = "francismasha96@gmail.com",
	},
	.wlan = {
		.ssid = SECRET_SSID,
		.password = SECRET_PASS,
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
		.mqtt_server = "mqtt.froyohydroponics.com",
		.mqtt_port = 8883,
		.mqtt_user = "almond",
		.mqtt_password = "froyogreen"
	},
	.firebase = {
		.firebase_host = "almond-data.firebaseio.com",
		.firebase_token = "hryGBdsP9la99hmfc57UbLztBCQDRe8FmgmvbJ3e"
	},
	.azure_connection = "HostName=almond-hub.azure-devices.net;DeviceId=almond;SharedAccessKey=4U1zFRpk9YD44aXLety+NzbvAQqosz7i6vZPSCVEpwI=",
	.hostname = "almond",
	.password = "",
	.timezone_h = 3,
	.baudrate = 115200
};
