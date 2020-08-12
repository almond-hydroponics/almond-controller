//Includes---------------------------------------------------------------------
#include "IotHubAzure.h"


const char *onSuccess = "\"Successfully invoke device method\"";
const char *notFound = "\"No method found\"";


//Implementation---------------------------------------------------------------
IotHubAzure::IotHubAzure()
{
	this->timerOverride.reset();
}

void blinkLED()
{
	digitalWrite(PIN_LED, HIGH);
	delay(500);
	digitalWrite(PIN_LED, LOW);
}
