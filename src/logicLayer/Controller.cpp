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
, statePtr(&stateMember)
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
	statePtr->sensorTest = &sensorTest;
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
			// Menu
			case Signalname::TEST:
				cout<<"Signal test arrived"<<endl;
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available - cb_this, Signalname::TEST));
				}
				statePtr->test();
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
