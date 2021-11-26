#pragma once

// Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "Device.h"
#include "Logger.h"
#include "RTClib.h"
#include "TimerOverride.h"

// Types------------------------------------------------------------------------
class DeviceRtc : public DeviceInput
{
	public:
	explicit DeviceRtc(const char* name);
	void loop() override;
	void setup() override;
	void update_time(uint32_t ntp_time);
	void time_of_day(Config_run_table_time* time);

	protected:
          RTC_DS3231 rtc{};
          TimerOverride timer;
};
