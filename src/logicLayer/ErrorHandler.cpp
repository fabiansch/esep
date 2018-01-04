/*
 * ErrorHandler.cpp
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#include "ErrorHandler.h"
#include <map>


namespace logicLayer {

std::map<Signal,string> messages;

void initMessages();

ErrorHandler::ErrorHandler(hardwareLayer::HardwareLayer& hal)
: hal(hal)
{
	LOG_SCOPE
	statePtr = &memberState;
	statePtr->hal = &hal;
	statePtr->errorHandler = this;
	statePtr->pendingSignals = &pendingSignals;

	initMessages();
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
		initMessages();
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
		break;
	case Signalname::BUTTON_E_STOP_PULLED:
		statePtr->isPending(signal);
		if(signal.sender == cb_this) {
			broadcastEStopStatus();
		}

		LOG_DEBUG<<"E STOP pulled on cb: "<<(int)signal.sender<<endl;
		break;
	case Signalname::BUTTON_RESET_PUSHED:
		statePtr->button_reset_pushed();
		break;
	case Signalname::BUTTON_START_PUSHED:
		statePtr->button_start_pushed();
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

void ErrorHandler::printErrors() {
	cout<<"\n### pending errors ###"<<endl;
	for( auto& pending : pendingSignals ) {
		cout<<messages[pending]<<endl;
	}
	cout<<"######################"<<endl<<endl;
}


void initMessages() {
	messages[Signal(Signalname::LB_INPUT_FREED)]  = "Item on input. Please remove it.";
	messages[Signal(Signalname::LB_HEIGHT_FREED)] = "Item on height. Please remove it.";
	messages[Signal(Signalname::LB_SWITCH_FREED)] = "Item on switch. Please remove it.";
	messages[Signal(Signalname::LB_SLIDE_FREED)]  = "Item on slide. Please remove it.";
	messages[Signal(Signalname::LB_OUTPUT_FREED)] = "Item on output. Please remove it.";
	messages[Signal(Signalname::BUTTON_START_PUSHED)] = "Item lost. Press Start button to go on.";
	messages[Signal(Signalname::CONNECTION_CONNECTED)] = "Connection to other conveyer belt lost.";
	messages[Signal(cb_sorting_2, cb_all, Signalname::BUTTON_E_STOP_PULLED)] = "E STOP on cb_sorting_2 pushed.";
	messages[Signal(cb_sorting_1, cb_all, Signalname::BUTTON_E_STOP_PULLED)] = "E STOP on cb_sorting_1 pushed.";
	messages[Signal(cb_sorting_2, cb_all, Signalname::SLIDE_EMPTY)] = "Slide 2 full. Please empty it.";
	messages[Signal(cb_sorting_1, cb_all, Signalname::SLIDE_EMPTY)] = "Slide 1 full. Please empty it.";
}


} /* namespace logicLayer */
