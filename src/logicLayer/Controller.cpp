/*
 * Controller.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Controller.h"

namespace logicLayer {

Controller::Controller(hardwareLayer::HardwareLayer& hal)
: hal(hal)
, sensorTest(hal)
, errorHandler(hal)
, statePtr(&stateMember)
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
	statePtr->sensorTest = &sensorTest;
	statePtr->errorHandler = &errorHandler;
}

Controller::~Controller() {
	LOG_SCOPE
}

void Controller::operator()() {
	LOG_SCOPE

	Signal signal;
	while(running) {
		LOG_DEBUG<<"blocking read on controller channel" << endl;
		signal << channel_;
		LOG_DEBUG<<"Controller got Signal"<<endl;
		switch (signal.name) {
			// serial
			case Signalname::CONNECTION_LOST:
				errorHandler.addPending(Signal(Signalname::CONNECTION_CONNECTED));
				cout<<"CONNECTION LOST"<<endl;
				LOG_ERROR<<"CONNECTION LOST"<<endl;
				cb_this.parameterList.showParameters();
				break;
			case Signalname::CONNECTION_CONNECTED:
				errorHandler.handle(signal);
				cout<<"CONNECTION CONNECTED"<<endl;
				LOG_DEBUG<<"CONNECTION LOST"<<endl;
				cb_this.parameterList.showParameters();
				break;
			// menu
			case Signalname::TEST:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available - cb_this, Signalname::TEST));
				}
				statePtr->test();
				cout<<"### SENSOR TEST started ###"<<endl;
				break;
			case Signalname::RUN:
				cout<<"Signal normal arrived"<<endl;
				break;
			case Signalname::CALIBRATION:
				break;
			case Signalname::STOP:
				break;

			default:
				statePtr->forward(signal);
				break;
		}
	}
}

} /* namespace logicLayer */
