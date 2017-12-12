/*
 * Timer.cpp
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#include "Header.h"
#include "Timer.h"
#include "Signals.h"

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Timer::~Timer() {
	LOG_SCOPE
}


void Timer::operator()() {
	LOG_SCOPE
	Signal signal;
	while(running) {
		signal << channel_;
		LOG_DEBUG<<"Timer got signal"<<endl;

		switch (signal.name) {
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			WAIT(550);
			*controller_ << Signal(Signalname::TIMEFRAME_INPUT_ENTER);
			cout<<"Timer sent TIMEFRAME_INPUT_ENTER"<<endl;
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR<<"Timer got unknown Signal"<<endl;
			exit(EXIT_FAILURE);
			break;
		}
	}

}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_ = controller;
}


} /* namespace logicLayer */
