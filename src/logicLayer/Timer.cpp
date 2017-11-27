/*
 * Timer.cpp
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#include "Timer.h"
#include "Signals.h"

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
{
	LOG_SCOPE
}

void Timer::operator()() {
	LOG_SCOPE

}

Timer::~Timer() {
	LOG_SCOPE
}

} /* namespace logicLayer */
