// Includes---------------------------------------------------------------------
#include "TimerOverride.h"

// Implementation---------------------------------------------------------------
TimerOverride::TimerOverride() { this->reset_ms = ULONG_MAX / 2; }

void TimerOverride::reset() { this->reset_ms = millis(); };

void TimerOverride::reset_with_carry(unsigned long timeout_ms)
{
	// may overflow, but is still ok.
	this->reset_ms = reset_ms + timeout_ms;
}

unsigned long TimerOverride::elapsed() const
{
	unsigned long current_time = millis();
	if (current_time >= reset_ms)
		return (current_time - reset_ms);

	// overflow has occurred
	return (ULONG_MAX - reset_ms) + current_time;
}

bool TimerOverride::check(unsigned long timeout_ms) const
{
	unsigned long target_time = this->reset_ms + timeout_ms;
	unsigned long current_time = millis();

	// has the current time overflown
	if (current_time < this->reset_ms)
	{ // did the current time overflow
		if (this->reset_ms < target_time)
		{ // no, it did not -> we are way over.
			return true;
		}
		else
		{ // yes its overflown as well, normal functionality.
			return (current_time >= target_time);
		}
	}
	else
	{ // timer has not overflown, how about the target?
		if (this->reset_ms < target_time)
		{ // no overflow, here either. Normal business
			return (current_time >= target_time);
		}
		else
		{ // the target is overflown, so must we.
			return false;
		}
	}
}
