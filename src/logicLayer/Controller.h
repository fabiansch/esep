/*
 * Controller.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Channel.h"
#include "SignalReceiver.h"
#include "SensorTest.h"
#include "ErrorHandler.h"
#include "Calibration.h"
#include "Item.h"
#include "Menu.h"
#include "Test.h"
#include "Channel.h"

namespace logicLayer {

class Controller : public SignalReceiver {

private:
	hardwareLayer::HardwareLayer& hal;
	SensorTest sensorTest;
	ErrorHandler errorHandler;
	Calibration calibration;

	/**
	 *  @brief Head element in item queue
	 */

	Item head_;

	/**
	 *  @brief Type Identification
	 */
	Channel<Signal>& typeIdCh_;

	struct State {//top-level state
		virtual void run(){}
		virtual void stop() {
			hal->redLightOff();
			hal->yellowLightOff();
			hal->greenLightOn();
			hal->motorStop();
			hal->switchPointClose();
			Menu::printInfo();
			new (this) Idle;
		}
		virtual void sensor_test(){}
		virtual void button_test(){}
		virtual void actuator_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal){}

		Calibration* calibration;
		SensorTest* sensorTest;
		ErrorHandler* errorHandler;
		hardwareLayer::HardwareLayer* hal;
		Item* head_;
		Channel<Signal>* typeIdCh_;
	} *statePtr;

	struct Idle : public State {
		Idle(){
		}
		virtual void run() override {
			new (this) Run;
		}
		virtual void sensor_test() override {
			new (this) Sensor_Test;
		}
		virtual void button_test() override {
			new (this) Button_Test;
		}
		virtual void actuator_test() override {
			new (this) Actuator_Test;
		}
		virtual void calibrate() override {
			new (this) Calibrate;
		}
	};

	struct Sensor_Test : public State {
		virtual void forward(Signal signal) override {
			sensorTest->handle(signal);
		}
	};

	struct Actuator_Test : public State {
		virtual void sensor_test() override {
			new (this) Sensor_Test;
		}
		virtual void forward(Signal signal) override {
			test::Test::actuatorsTestHelper(hal, signal);
		}
	};

	struct Button_Test : public State {
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			test::Test::testSignalBufferAdd(signal);
		}
	};

	struct Run : public State{
		Run(){
			hal->blinkGreen(Speed::slow);
			items_on_cb = 0;
			item_on_switch = false;
			item_on_output = false;
			this_cb_busy = false;
			next_cb_busy = false;
			Item::resetId();

			TypeIdentification::setUnitToMm();
			TypeIdentification::setHoleLevel();
			hal->clearItemBuffer();
		}
		virtual void forward(Signal signal) override {
			head_->handle( signal );
			*typeIdCh_ << signal;
		}
	};

	struct Calibrate : public State{
		Calibrate() {
			calibration->handle(Signal(Signalname::CALIBRATION_START));
		}
		virtual void forward(Signal signal) override {
			calibration->handle(signal);
		}
	};

	Idle stateMember;

public:
	Controller(hardwareLayer::HardwareLayer&, Channel<Signal>&, Channel<Signal>& );
	virtual ~Controller();

	virtual void operator()();

};

} /* namespace logicLayer */
#endif /* CONTROLLER_H_ */
