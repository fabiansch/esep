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
	statePtr->pendingSignals = &pendingSignals;

}

ErrorHandler::~ErrorHandler() {
	// TODO Auto-generated destructor stub
}

void ErrorHandler::setPending(Signal signal) {
	pendingSignals.insert(signal);
	statePtr->errorOccurred();
}

} /* namespace logicLayer */
