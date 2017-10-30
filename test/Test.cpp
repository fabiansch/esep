/*
 * Test.cpp
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */


#include <iostream>
#include <thread>
#include "Test.h"
#include "HAL.h"
#include "Header.h"
#include "GpioTesting.h"
#include "SingletonTest.h"
using namespace std;

namespace logicLayer{
namespace test{


Test::Test(hal::HAL* hal) {
	_hal = hal;
}

Test::~Test() {
}


void Test::actuatorsTest(){

	cout << "start " << __FUNCTION__ << endl;

	cout << "test motor (counterclockwise, fast) " << endl;
	_hal->motorStart();
	_hal->motorRotateCounterclockwise();
	_hal->motorFast();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (counterclockwise, slow) " << endl;
	_hal->motorStart();
	_hal->motorRotateCounterclockwise();
	_hal->motorSlow();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (clockwise, fast) " << endl;
	_hal->motorStart();
	_hal->motorRotateClockwise();
	_hal->motorFast();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor (clockwise, slow) " << endl;
	_hal->motorSlow();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test motor stop" << endl;
	_hal->motorStop();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test open switch point - please continue soon" << endl;
	_hal->switchPointOpen();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test close switch point " << endl;
	_hal->switchPointClose();

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ <<  "  successful." << endl;

}


void Test::mmiTest(){

	cout << "start " << __FUNCTION__ << endl;
	cout << "test lamps on: red, yellow, green" << endl;
	_hal->yellowLightOn();
	_hal->redLightOn();
	_hal->greenLightOn();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test lamps off: red, yellow, green" << endl;
	_hal->yellowLightOff();
	_hal->redLightOff();
	_hal->greenLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: red, yello, green " << endl;
	_hal->blinkGreen(Speed::fast);
	_hal->blinkRed(Speed::fast);
	_hal->blinkYellow(Speed::fast);
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: yellow, green" << endl;
	_hal->redLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: green" << endl;
	_hal->yellowLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->blinkYellow(Speed::slow);
	_hal->blinkRed(Speed::slow);
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: yellow, green" << endl;
	_hal->redLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: green" << endl;
	_hal->yellowLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: nothing" << endl;
	_hal->greenLightOff();
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->blinkYellow(Speed::slow);
	_hal->blinkRed(Speed::slow);
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking fast: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::fast);
	_hal->blinkYellow(Speed::fast);
	_hal->blinkRed(Speed::fast);
	if( !nextTest(__FUNCTION__) ) return;

	cout << "test blinking slow: red, yellow, green" << endl;
	_hal->blinkGreen(Speed::slow);
	_hal->blinkYellow(Speed::slow);
	_hal->blinkRed(Speed::slow);
	if( !nextTest(__FUNCTION__) ) return;

	cout  << __FUNCTION__ << " successful. " << endl;
}


void Test::sensorsTest(){

}

void Test::writeSomethingElse(hal::io::GPIO *gpio, int difference) {
	port_t port = gpio->read(PORT::A); // read port to write definetly something different so write method gets called
	gpio->setBits(PORT::A, port + difference);
}

void createInstance(){
	GpioTesting& instance = GpioTesting::instance(true);
	instance.helloWorld();
}


void Test::singletonThreadSafeTest(){
	cout << "start " << __FUNCTION__ << endl;

	cout << "Get just one singleton created?"<<endl;
	thread t1(&createInstance);
	thread t2(&createInstance);
	t1.join();
	t2.join();

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ << " successful." << endl;

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

	GpioTesting *gpio = new GpioTesting();
	gpio->gainAccess();
	thread t1(&writeSomethingElse, gpio, 1);
	thread t2(&writeSomethingElse, gpio, 2);
	t1.join();
	t2.join();
	cout << "######################" << endl;

	if( !nextTest(__FUNCTION__) ) return;

	cout << __FUNCTION__ << " successful." << endl;
}


bool Test::nextTest(string functionName){
	cout << "Was the test successful and do you want to go on?\n"<<
			"  yes: hit return\n"<<
			"  no : hit any key followed by return" << endl;
	if (cin.get() == '\n'){
		return true;
	} else {
		cout << "##### "<<functionName << " NOT succesfull." << " #####"<< endl;
		cout << "Hit return to go on" << endl;
		cin.get();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
}


} /* test */
} /* logicLayer */
