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
, eStopPushedCounter(0)
{
	statePtr = &memberState;
	statePtr->hal = &hal;
	statePtr->pendingSignals = &pendingSignals;
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
		hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		eStopPushedCounter++;
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		statePtr->isPending(signal);
		hal.sendSerial(Signal(cb_this, cb_available, signal.name));
		if(eStopPushedCounter>0) {
			eStopPushedCounter--;
		}
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
	cout<<"ESTOPPPUSHED COUNTER "<<eStopPushedCounter<<endl;
}

} /* namespace logicLayer */
