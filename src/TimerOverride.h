#pragma once

// Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"

// Types------------------------------------------------------------------------
class TimerOverride
{
	public:
	TimerOverride();
	// Use all time
	void reset();
	// User @param timeout_ms amount of time.
	void reset_with_carry(unsigned long timeout_ms);
	// Check if @param timeout_ms milliseconds has passed since the last reset.
	bool check(unsigned long timeout_ms) const;
	// @returns time since last reset
	unsigned long elapsed() const;

	protected:
	unsigned long reset_ms;
};
