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
#include "Item.h"
#include "Menu.h"
#include "Test.h"

namespace logicLayer {

class Controller: public SignalReceiver {

private:
	hardwareLayer::HardwareLayer& hal;
	SensorTest sensorTest;
	ErrorHandler errorHandler;

	/**
	 *  @brief Head element in item queue
	 */

	Item head_;

	struct State {//top-level state
		virtual void run(){}
		virtual void stop() {
			hal->redLightOff();
			hal->yellowLightOff();
			hal->greenLightOn();
			hal->motorStop();
			hal->switchPointClose();
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

		SensorTest* sensorTest;
		ErrorHandler* errorHandler;
		hardwareLayer::HardwareLayer* hal;
		Item* head_;
	} *statePtr;

	struct Start : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
	};

	struct Idle : public State {
		Idle(){
			Menu::printInfo();
		}
		virtual void run(){
			new (this) Run;
		}
		virtual void sensor_test(){
			new (this) Sensor_Test;
		}
		virtual void button_test(){
			new (this) Button_Test;
		}
		virtual void actuator_test(){
			new (this) Actuator_Test;
		}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
	};

	struct Sensor_Test : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			sensorTest->handle(signal);
		}
	};

	struct Actuator_Test : public State{
		virtual void run(){}
		virtual void sensor_test(){
			new (this) Sensor_Test;
		}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			test::Test::actuatorsTestHelper(hal, signal);
		}
	};

	struct Button_Test : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			test::Test::testSignalBufferAdd(signal);
		}
	};

	struct Run : public State{
		Run(){
			cout << "enter run"<<endl;
		}
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			cout << "forward in run";
			head_->handle( signal );
		}
	};

	struct Safe : public State{
		virtual void run(){}
		virtual void stop(){/* do nothing */ }
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
	};

	struct Calibrate : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
	};

	Idle stateMember;

public:
	Controller(hardwareLayer::HardwareLayer&, Channel<Signal>& );
	virtual ~Controller();

	virtual void operator()();

};

} /* namespace logicLayer */
#endif /* CONTROLLER_H_ */
