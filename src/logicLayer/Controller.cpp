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
	statePtr->hal = &hal;
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
				cout<<"CONNECTION LOST"<<endl;
				LOG_ERROR<<"CONNECTION LOST"<<endl;
				cb_this.parameterList.showParameters();
				break;
			case Signalname::CONNECTION_CONNECTED:
				cout<<"CONNECTION CONNECTED"<<endl;
				LOG_DEBUG<<"CONNECTION LOST"<<endl;
				cb_this.parameterList.showParameters();
				break;
			// menu
			case Signalname::SENSOR_TEST:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::SENSOR_TEST));
				}
				statePtr->sensor_test();
				if (cb_this == cb_1) {
					cout << "################ Automated Sensor Test Start ###############" << endl;
					cout << "Please put item (metal above) on master's input."<< endl;
				}
				break;
			case Signalname::BUTTON_TEST:
				statePtr->button_test();
				cout << "################ Button Test Start ###############" << endl;
				break;
			case Signalname::ACTUATOR_TEST:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::ACTUATOR_TEST));
				}
				statePtr->actuator_test();
				if (cb_this == cb_1) {
					cout << "################ Actuator Test Start ###############" << endl;
				}
				break;
			case Signalname::RUN:
				cout<<"Signal RUN arrived"<<endl;
				break;
			case Signalname::CALIBRATION:
				break;
			case Signalname::STOP:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::STOP));
				}
				statePtr->stop();
				break;
			default:
				statePtr->forward(signal);
				break;
		}

		errorHandler.handle(signal);
	}
}

} /* namespace logicLayer */
