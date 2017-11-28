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
		hal.sendSerial(Signal(cb_this,cb_available,Signalname::BUTTON_E_STOP_PUSHED));
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		hal.sendSerial(Signal(cb_this,cb_available,Signalname::BUTTON_E_STOP_PULLED));
		break;
	case Signalname::BUTTON_RESET_PUSHED:
		break;
	case Signalname::BUTTON_START_PUSHED:
		break;
	case Signalname::BUTTON_STOP_PUSHED:
		break;
	default:
		break;
	}
	statePtr->isPending(signal);
}

} /* namespace logicLayer */
