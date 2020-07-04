#pragma once


//Includes---------------------------------------------------------------------
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <climits>


//Types------------------------------------------------------------------------
using MilliSec = unsigned long;

class AsyncWait
{
public:
	AsyncWait()
	{
		reset();
	}

	void startWaiting(MilliSec currentMilliSec, MilliSec durationWait)
	{
		waitStartMilliSec = currentMilliSec;
		this->waitDuration = durationWait;
		isAsyncWaiting = true;
	}

	bool isWaiting(MilliSec currentMilliSec)
	{
		if (!isAsyncWaiting) return false;

		MilliSec delta = calculateMilliSecDelta(currentMilliSec);
		if (delta >= waitDuration) {
			reset();
			return false;
		}

		// if still waiting
		return true;
	}

	void cancel()
	{
		reset();
	}

private:
	bool isAsyncWaiting{};
	MilliSec waitStartMilliSec{};
	MilliSec waitDuration{};

	void reset()
	{
		isAsyncWaiting = false;
		waitStartMilliSec = 0;
		waitDuration = 0;
	}

	/**
	 * Calculate the difference between currentMilliSec and waitStartMilliSec
     * taking into account the event when 'millis()' wraps around.
	 * @param currentMilliSec
	 * @return
	 */
	MilliSec calculateMilliSecDelta(MilliSec currentMilliSec) const
	{
		if (currentMilliSec < waitStartMilliSec) {
			return ULLONG_MAX - waitStartMilliSec + currentMilliSec;
		}
		else {
			return currentMilliSec - waitStartMilliSec;
		}
	}
};
