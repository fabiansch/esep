/*
 * Controller.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "SignalReceiver.h"
#include "SensorTest.h"
#include "ErrorHandler.h"
#include "Menu.h"
#include "Test.h"

namespace logicLayer {

class Controller: public SignalReceiver {

private:
	hardwareLayer::HardwareLayer& hal;
	SensorTest sensorTest;
	ErrorHandler errorHandler;



	struct State {//top-level state
		virtual void run(){}
		virtual void stop(){ new (this) Idle;}
		virtual void sensor_test(){}
		virtual void button_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal){}

		SensorTest* sensorTest;
		ErrorHandler* errorHandler;
		Test* test;
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
		virtual void run(){}
		virtual void sensor_test(){
			new (this) Test;
		}
		virtual void button_test(){
			new (this) Test;
		}
		virtual void mmi_test(){
			new (this) Test;
		}
		virtual void actuators_test(){
			new (this) Test;
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
			errorHandler->handle(signal);
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
			errorHandler->handle(signal);
		}
	};

	struct	Test : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
			test->handle(signal);
			errorHandler->handle(signal);
		}
	};

	struct Run : public State{
		virtual void run(){}
		virtual void sensor_test(){}
		virtual void alert(){}
		virtual void restart(){}
		virtual void ready(){}
		virtual void calibrate(){}
		virtual void forward(Signal signal) {
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
	Controller(hardwareLayer::HardwareLayer&);
	virtual ~Controller();

	virtual void operator()();

};

} /* namespace logicLayer */
#endif /* CONTROLLER_H_ */
