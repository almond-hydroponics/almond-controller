//Includes---------------------------------------------------------------------
#include "AlmondOTA.h"
#include <Logger.h>


int led_pin = PIN_LED;
int dimmer_pin[] = {14, 5, 15};

//Implementation---------------------------------------------------------------
void AlmondOTA::setup()
{
		ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    LOG_INFO("Start updating %s", type.c_str());
  });

  ArduinoOTA.onEnd([]() {
    LOG_INFO("OTA End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    LOG_INFO("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      LOG_ERROR("OTA Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      LOG_ERROR("OTA Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      LOG_ERROR("OTA Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      LOG_ERROR("OTA Receive Failed");
    } else if (error == OTA_END_ERROR) {
      LOG_ERROR("OTA End Failed");
    }
  });

  ArduinoOTA.begin();
//// Switch off led
//		digitalWrite(led_pin, HIGH);
//
//// Configure dimmers, and OTA server events
//		analogWriteRange(1000);
//		analogWrite(led_pin, 900);
//
//		for (int i = 0; i < N_DIMMERS; i++) {
//				pinMode(i, OUTPUT);
//				analogWrite(i, 50);
//		}
//
//		ArduinoOTA.setHostname(CONSTANTS.hostname);
//		ArduinoOTA.onStart([]() {
//				// Switch off all the PWMs during upgrade
//				for (int i = 0; i < N_DIMMERS; i++) {
//						analogWrite(dimmer_pin[i], 0);
//				}
//				analogWrite(led_pin, 0);
//		});
//
//		ArduinoOTA.onEnd([]() {
//				for (int i = 0; i < 30; i++) {
//						analogWrite(led_pin, (i * 100) % 1001);
//						delay(50);
//				}
//		});
//
//		ArduinoOTA.onError([](ota_error_t error) {
//				(void)error;
//				ESP.restart();
//		});
//
//		// Set up the OTA server
//		ArduinoOTA.begin();
//		LOG_INFO("OTA Ready");
}

void AlmondOTA::loop()
{
		ArduinoOTA.handle();
}
