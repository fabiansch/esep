/*
 * Dummy.cpp
 *
 *  Created on: 17.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "Dispatcher.h"


namespace logicLayer {

Dispatcher::Dispatcher(
		hardwareLayer::HardwareLayer& hal,
		Channel<Signal>& controller,
		Channel<Signal>& timer
		)
: hal(hal)
, controller_(controller)
, timer_(timer)
{
	LOG_SCOPE
	hal.getSignalGenerator().register_observer(this);
}

Dispatcher::~Dispatcher() {
	LOG_SCOPE
	hal.getSignalGenerator().unregister_observer(this);
}

void Dispatcher::notify() {
	LOG_SCOPE
	Signal signal;
	while((signal = hal.getSignal()).name != Signalname::SIGNAL_BUFFER_EMPTY) {
		LOG_DEBUG<<"Dispatcher is notified"<<endl;

		switch (signal.name) {
		case Signalname::CONVEYOR_BELT_BUSY:
			next_cb_busy = true;
			break;
		case Signalname::CONVEYOR_BELT_READY:
			next_cb_busy = false;
			controller_ << signal;
			break;
		case Signalname::START_TIMERS_INPUT:
			timer_ << signal;
//			hal.motorStart(); // do not call hal here....
			break;
		case Signalname::MOTOR_STOP:
			timer_ << signal;
			break;
		case Signalname::MOTOR_START:
			timer_ << signal;
			break;
		case Signalname::MOTOR_SLOW:
//			timer_ << signal;
			break;
		case Signalname::MOTOR_FAST:
			timer_ << signal;
			break;
		default:
			LOG_ERROR<<"Dispatcher got unknown Signal"<<endl;
			controller_ << signal;
			break;
		}
	}
}


} /* namespace logicLayer */
