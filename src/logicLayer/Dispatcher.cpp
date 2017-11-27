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
}

void Dispatcher::notify() {
	LOG_SCOPE
	Signal signal;
	while((signal = hal.getSignal()).name != Signalname::SIGNAL_BUFFER_EMPTY) {
		LOG_DEBUG<<"Dispatcher is notified"<<endl;

		switch (signal.name) {
		// serial
		case Signalname::CONNECTION_LOST:
			cout<<"CONNECTION LOST"<<endl;
			hal.blinkRed(Speed::slow);
			hal.greenLightOff();
			cb_this.parameterList.showParameters();
			break;
		case Signalname::CONNECTION_CONNECTED:
			cout<<"CONNECTION CONNECTED"<<endl;
			hal.blinkGreen(Speed::slow);
			hal.redLightOff();
			cb_this.parameterList.showParameters();
			break;
		default:
			LOG_ERROR<<"Dispatcher got unknown Signal"<<endl;
			controller_ << signal;
			break;
		}
	}
}


} /* namespace logicLayer */
