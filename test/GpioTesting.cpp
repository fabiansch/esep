/*
 * GpioTesting.cpp
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */

#include "GpioTesting.h"
#include <iostream>
#include "Header.h"
#include <thread>
#include <chrono>

/**
 * @brief Constructor used to test Singleton
 * @param boolean value not used - only needed to differentiate between default constructor
 */
GpioTesting::GpioTesting(bool test){
	cout << "singleton created" << endl;
}

GpioTesting::GpioTesting(){
}

void GpioTesting::helloWorld(){
	cout << "hello World" << endl;
}

GpioTesting& GpioTesting::instance(bool test) {
	static GpioTesting instance(test);
	return instance;
}

void GpioTesting::write(PORT port, port_t val){
	cout << "# rmw-cycle starting #" << endl;
	WAIT(500);
	cout << "# rmw-cycle finished #" << endl;
}

