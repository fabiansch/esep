/*
 * ErrorHandler.cpp
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#include "ErrorHandler.h"

namespace logicLayer {

ErrorHandler::ErrorHandler(hardwareLayer::HardwareLayer& hal)
: hal(hal)
{
	LOG_SCOPE
	statePtr = &memberState;
	statePtr->hal = &hal;
	statePtr->pendingSignals = &pendingSignals;
	hal.blinkGreen(Speed::slow);
}

ErrorHandler::~ErrorHandler() {
	LOG_SCOPE
}

void ErrorHandler::addPending(Signal signal) {
	pendingSignals.insert(signal);
	statePtr->errorOccurred();
}

void ErrorHandler::handle(Signal signal) {
	switch(signal.name) {
	case Signalname::CONNECTION_LOST:
		addPending(Signal(Signalname::CONNECTION_CONNECTED));

		cout<<"CONNECTION LOST"<<endl;
		LOG_WARNING<<"ERROR CONNECTION LOST"<<endl;
		break;
	case Signalname::CONNECTION_CONNECTED:
		statePtr->isPending(signal);
		broadcastEStopStatus();

		cout<<"CONNECTION CONNECTED"<<endl;
		LOG_DEBUG<<"CONNECTION CONNECTED"<<endl;
		break;
	case Signalname::BUTTON_E_STOP_PUSHED:
		addPending(Signal(signal.sender, signal.receiver, Signalname::BUTTON_E_STOP_PULLED));
		if(signal.sender == cb_this) {
			broadcastEStopStatus();
		}

		LOG_DEBUG<<"E STOP pushed on cb: "<<(int)signal.sender<<endl;
		if(signal.sender == cb_this) {
			cout<<"E STOP pushed on this cb."<<endl;
		} else {
			cout<<"E STOP pushed on other cb."<<endl;
		}
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		statePtr->isPending(signal);
		if(signal.sender == cb_this) {
			broadcastEStopStatus();
		}

		LOG_DEBUG<<"E STOP pulled on cb: "<<(int)signal.sender<<endl;
		if(signal.sender == cb_this) {
			cout<<"E STOP pulled on this cb."<<endl;
		} else {
			cout<<"E STOP pulled on other cb."<<endl;
		}
		break;
	case Signalname::BUTTON_STOP_PUSHED:
		hal.motorSlow();
		break;
	case Signalname::BUTTON_STOP_PULLED:
		hal.motorFast();
		break;

	default:
		statePtr->isPending(signal);
	}
}

void ErrorHandler::broadcastEStopStatus() {
	auto pending(Signal(Signalname::BUTTON_E_STOP_PULLED));

	if (pendingSignals.find(pending) != pendingSignals.end()) {
		hal.sendSerial(Signal(cb_this, cb_available, Signalname::BUTTON_E_STOP_PUSHED));
	} else {
		hal.sendSerial(Signal(cb_this, cb_available, Signalname::BUTTON_E_STOP_PULLED));
	}
}

} /* namespace logicLayer */
