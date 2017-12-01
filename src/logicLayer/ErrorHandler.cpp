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
	statePtr = &memberState;
	statePtr->hal = &hal;
	statePtr->pendingSignals = &pendingSignals;
	statePtr->eStopCounter = &eStopCounter;
}

ErrorHandler::~ErrorHandler() {
	// TODO Auto-generated destructor stub
}

void ErrorHandler::addPending(Signal signal) {
	pendingSignals.insert(signal);
	statePtr->errorOccurred();
}

void ErrorHandler::handle(Signal signal) {
	switch(signal.name) {
	case Signalname::BUTTON_E_STOP_PUSHED:
		addPending(Signal(Signalname::BUTTON_E_STOP_PULLED));
		if(signal.sender == cb_this) {
			hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		}
		eStopCounter++;
		cout<<"ESTOPPPUSHED COUNTER "<<eStopCounter<<endl;
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		statePtr->isPending(signal);
		if(signal.sender == cb_this) {
			hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		}
		if(eStopCounter>0) {
			eStopCounter--;
		}
		cout<<"ESTOPPPUSHED COUNTER "<<eStopCounter<<endl;
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
