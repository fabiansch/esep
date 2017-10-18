#include "Header.h"
#include <sys/neutrino.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include "HAL.h"
#include "GPIO.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Starting Sortingmachine ..." << endl;

	LOG_SCOPE;
	LOG_SET_LEVEL(DEBUG);
	LOG_DEBUG<<"hello world \n";

	hal::HAL hal;
	hal.motorStart();
	hal.motorRotateClockwise();
	hal.motorFast();
	WAIT(2000);
	hal.motorSlow();
	WAIT(2000);
	hal.motorStop();
	WAIT(2000);
	hal.switchPointOpen();
	WAIT(2000);
	hal.switchPointClose();
	WAIT(2000);

	hal.yellowLightOn();
	hal.redLightOn();
	hal.greenLightOn();
	WAIT(2000);
	hal.yellowLightOff();
	hal.redLightOff();
	hal.greenLightOff();

	hal.blinkGreen(Speed::fast);
	hal.blinkRed(Speed::fast);
	hal.blinkYellow(Speed::fast);
	WAIT(5000);
	hal.redLightOff();
	WAIT(5000);
	hal.yellowLightOff();
	WAIT(5000);
	hal.greenLightOff();
	WAIT(5000);
	hal.blinkGreen(Speed::slow);
	hal.blinkYellow(Speed::slow);
	hal.blinkRed(Speed::slow);
	WAIT(5000);
	hal.greenLightOff();
	WAIT(5000);
	hal.yellowLightOff();
	WAIT(5000);
	hal.redLightOff();

	cout << "Starting Sortingmachine ... done !" << endl;
  return EXIT_SUCCESS;
}

