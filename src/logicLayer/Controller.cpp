/*
 * Controller.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Controller.h"

namespace logicLayer {

Controller::Controller(hardwareLayer::HardwareLayer& hal, Channel<Signal>& timerChannel, Channel<Signal>& typeIdentCh )
: hal(hal)
, sensorTest(hal)
, errorHandler(hal)
, head_(&hal, &timerChannel, true, &errorHandler)
, typeIdCh_(typeIdentCh)
, statePtr(&stateMember)
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
	statePtr->sensorTest = &sensorTest;
	statePtr->errorHandler = &errorHandler;
	statePtr->head_ = &head_;
	statePtr->hal = &hal;
	statePtr->typeIdCh_ = &typeIdCh_;
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
			// errorHandler
			case Signalname::CONNECTION_LOST:
				// just forward to errorHandler
				break;
			case Signalname::CONNECTION_CONNECTED:
				// just forward to errorHandler
				break;
			case Signalname::BUTTON_E_STOP_PULLED:
				// just forward to errorHandler
				break;
			case Signalname::BUTTON_E_STOP_PUSHED:
				// just forward to errorHandler
				break;
			// menu
			case Signalname::SENSOR_TEST:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::SENSOR_TEST));
				}
				statePtr->sensor_test();
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
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::RUN));
				}
				statePtr->run();
				break;
			case Signalname::CALIBRATION:
				break;
			case Signalname::STOP:
				if(cb_this == cb_1) {
					hal.sendSerial(Signal(cb_this, cb_available, Signalname::STOP));
				}
				statePtr->forward(Signalname::STOP);
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
