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
, eStopCounter(0)
{
	LOG_SCOPE
	statePtr = &memberState;
	statePtr->hal = &hal;
	statePtr->pendingSignals = &pendingSignals;
	statePtr->eStopCounter = &eStopCounter;
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
		cb_this.parameterList.showParameters();
		LOG_WARNING<<"ERROR CONNECTION LOST"<<endl;
		break;
	case Signalname::CONNECTION_CONNECTED:
		statePtr->isPending(signal);
		cout<<"CONNECTION CONNECTED"<<endl;
		LOG_DEBUG<<"CONNECTION CONNECTED"<<endl;
		cb_this.parameterList.showParameters();
		break;
	case Signalname::BUTTON_E_STOP_PUSHED:
		addPending(Signal(Signalname::BUTTON_E_STOP_PULLED));
		if(signal.sender == cb_this) {
			hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		}
		eStopCounter++;
		LOG_DEBUG<<"ESTOP COUNTER "<<eStopCounter<<endl;
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		if(signal.sender == cb_this) {
			hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		}
		if(eStopCounter>0) {
			eStopCounter--;
		}
		LOG_DEBUG<<"ESTOP COUNTER "<<eStopCounter<<endl;
		statePtr->isPending(signal);
		break;
	case Signalname::BUTTON_RESET_PUSHED:
		break;
	case Signalname::BUTTON_START_PUSHED:
		break;
	case Signalname::BUTTON_STOP_PUSHED:
		break;
	default:
		statePtr->isPending(signal);
	}
}

} /* namespace logicLayer */
