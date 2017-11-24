/*
 * Test.cpp
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */


#include <iostream>
#include <thread>
#include "Test.h"
#include "HardwareLayer.h"
#include "Header.h"
#include "GpioTesting.h"
#include "SignalGenerator.h"

using namespace std;

namespace logicLayer{
namespace test{


Test::Test(hardwareLayer::HardwareLayer* hal) {
	_hal = hal;
}

Test::~Test() {
}


void Test::actuatorsTest(){

	cout << "start " << __FUNCTION__ << endl;

	cout << "test motor (counterclockwise, fast) " << endl;
	_hal->motorStart();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_START));
	_hal->motorRotateCounterclockwise();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_ROTATE_COUNTER_CLOCKWISE));
	_hal->motorFast();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_FAST));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (counterclockwise, slow) " << endl;
	_hal->motorStart();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_START));
	_hal->motorRotateCounterclockwise();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_ROTATE_COUNTER_CLOCKWISE));
	_hal->motorSlow();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_SLOW));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (clockwise, fast) " << endl;
	_hal->motorStart();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_START));
	_hal->motorRotateClockwise();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_ROTATE_CLOCKWISE));
	_hal->motorFast();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_FAST));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (clockwise, slow) " << endl;
	_hal->motorSlow();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_SLOW));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor stop" << endl;
	_hal->motorStop();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::MOTOR_STOP));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test open switch point - please continue soon" << endl;
	_hal->switchPointOpen();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::SWITCH_OPEN));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test close switch point " << endl;
	_hal->switchPointClose();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::SWITCH_CLOSE));

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ <<  "  successful." << endl;

}


void Test::mmiTest(){

	cout << "start " << __FUNCTION__ << endl;
	cout<< "testButtonLEDs on: Reset,Start,Q1,Q2" <<endl;
	_hal->Q1LEDOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::Q1_LED_ON));
	_hal->Q2LEDOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::Q2_LED_ON));
	_hal->ResetLEDOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RESET_LED_ON));
	_hal->StartLEDOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::START_LED_ON));
	if( !nextTest(__FUNCTION__) ) return;

	cout<< "testButtonLEDs off: Reset,Start,Q1,Q2" <<endl;
	_hal->Q1LEDOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::Q1_LED_OFF));
	_hal->Q2LEDOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::Q2_LED_OFF));
	_hal->ResetLEDOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RESET_LED_OFF));
	_hal->StartLEDOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::START_LED_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test lamps on: red, yellow, green" << endl;
	_hal->yellowLightOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::YELLOW_LIGHT_ON));
	_hal->redLightOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RED_LIGHT_ON));
	_hal->greenLightOn();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::GREEN_LIGHT_ON));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: red, yellow, green " << endl;
	_hal->blinkGreen(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_GREEN_FAST));
	_hal->blinkRed(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_RED_FAST));
	_hal->blinkYellow(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_YELLOW_FAST));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: yellow, green" << endl;
	_hal->redLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RED_LIGHT_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: green" << endl;
	_hal->yellowLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::YELLOW_LIGHT_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_GREEN_SLOW));
	_hal->blinkYellow(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_YELLOW_SLOW));
	_hal->blinkRed(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_RED_SLOW));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: yellow, green" << endl;
	_hal->redLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RED_LIGHT_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: green" << endl;
	_hal->yellowLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::YELLOW_LIGHT_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_GREEN_SLOW));
	_hal->blinkYellow(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_YELLOW_SLOW));
	_hal->blinkRed(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_RED_SLOW));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_GREEN_FAST));
	_hal->blinkYellow(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_YELLOW_FAST));
	_hal->blinkRed(Speed::fast);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_RED_FAST));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_GREEN_SLOW));
	_hal->blinkYellow(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_YELLOW_SLOW));
	_hal->blinkRed(Speed::slow);
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::BLINK_RED_SLOW));
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking: nothing" << endl;
	_hal->greenLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::GREEN_LIGHT_OFF));
	_hal->yellowLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::YELLOW_LIGHT_OFF));
	_hal->redLightOff();
	_hal->sendSerial(Signal(cb_this, cb_all, Signalname::RED_LIGHT_OFF));
	if( !nextTest(__FUNCTION__) ) return;

	cout  << __FUNCTION__ << " successful. " << endl;
}


void Test::buttonsTest(){
	cout << "start " << __FUNCTION__ <<endl;

	buttonTestHelper(	hardwareLayer::io::SignalGenerator::BUTTON_START,
						Signalname::BUTTON_START_PUSHED,
						Signalname::BUTTON_START_PULLED);

	buttonTestHelper(	hardwareLayer::io::SignalGenerator::BUTTON_STOP,
						Signalname::BUTTON_STOP_PUSHED,
						Signalname::BUTTON_STOP_PULLED);

	buttonTestHelper(	hardwareLayer::io::SignalGenerator::BUTTON_RESET,
						Signalname::BUTTON_RESET_PUSHED,
						Signalname::BUTTON_RESET_PULLED);

	buttonTestHelper(	hardwareLayer::io::SignalGenerator::BUTTON_E_STOP,
						Signalname::BUTTON_E_STOP_PUSHED,
						Signalname::BUTTON_E_STOP_PULLED);

	if( !nextTest(__FUNCTION__) ) return;

	cout  << __FUNCTION__ << " successful. " <<endl<<endl;
}

void Test::buttonTestHelper(hardwareLayer::io::SensorEvent signalBitmask, Signalname eventTriggerStart, Signalname eventTriggerEnd) {

	_hal->clearSignalBuffer();

	cout <<endl<< "test " << signalBitmask.name << "\n - please trigger button one or several times. Hit return key afterwards."<<endl;

	while (cin.get() != '\n');

	int pushedCounter = 0;
	int pulledCounter = 0;
	int failureCounter = 0;
	Signal signal;

	while ((signal = _hal->getSignal()).name != Signalname::SIGNAL_BUFFER_EMPTY) {

		if (signal.name == eventTriggerStart) {
			pushedCounter++;
		} else if (signal.name == eventTriggerEnd){
			pulledCounter++;
		} else{
			failureCounter++;
		}
	}

	bool success = failureCounter == 0 && pushedCounter > 0;

	if ( success ){
		cout << "pushed successfully "   << pushedCounter << " time(s))" << endl;
		cout << "pulled successfully "   << pulledCounter  << " time(s))" << endl;
	} else {
		cout << "Wrong Signals Triggered" << failureCounter << " time(s))\n" <<
				"pushed successfully "   << pushedCounter << " time(s))\n" <<
				"pulled successfully "   << pulledCounter  << " time(s))\n" << endl;
	}
}

void createInstance(){
	GpioTesting& instance = GpioTesting::instance(true);
	instance.helloWorld();
}


void Test::singletonThreadSafeTest(){
	cout << "start " << __FUNCTION__ << endl;

	cout << "Get one singleton created and Two Hello Worlds?"<<endl;
	thread t1(&createInstance);
	thread t2(&createInstance);
	t1.join();
	t2.join();

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ << " successful." << endl<<endl;

}

void Test::writeSomethingElse(hardwareLayer::io::GPIO *gpio, int difference) {
	port_t port = gpio->read(PORT::A); // read port to write definetly something different so write method gets called
	gpio->setBits(PORT::A, port + difference);
}

void Test::threadSafenessInGpioTest(){
	cout << "start " << __FUNCTION__ << endl;

	cout << "Looks the same?"<<endl;
	cout << "######################" 	<< endl;
	cout << "# rmw-cycle starting #" 	<< endl;
	cout << "# rmw-cycle finished #" 	<< endl;
	cout << "# rmw-cycle starting #" 	<< endl;
	cout << "# rmw-cycle finished #" 	<< endl;
	cout << "# ================== #" 	<< endl;

	hardwareLayer::io::GPIO *gpio = new GpioTesting();
	gpio->gainAccess();
	thread t1(&writeSomethingElse, gpio, 1);
	thread t2(&writeSomethingElse, gpio, 2);
	t1.join();
	t2.join();
	cout << "######################" << endl;

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ << " successful." << endl<<endl;
}


bool Test::nextTest(string functionName){
	cout <<endl<< "Was the test successful and do you want to go on?\n"<<
			"  yes: hit return\n"<<
			"  no : hit any key followed by return" << endl;
	if (cin.get() == '\n'){
		return true;
	} else {
		cout << "##### "<<functionName << " NOT successfull." << " #####"<< endl;
		cout << "Hit return to go on" << endl;
		cin.get();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
}


} /* test */
} /* logicLayer */
