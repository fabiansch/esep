#include <sys/neutrino.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include "HAL.h"

#define WAIT(x) (std::this_thread::sleep_for(std::chrono::milliseconds(x)))


using namespace std;

int main(int argc, char *argv[])
{
//#ifdef SIMULATION
//  IOaccess_open();
//#endif

	ThreadCtl(_NTO_TCTL_IO_PRIV, 0);
	cout << "Starting Sortingmachine ..." << endl;



	hal::HAL hal;
//	hal.motorStart();
//	hal.motorRotateClockwise();
//	hal.motorFast();
//	WAIT(2000);
//	hal.motorSlow();
//	WAIT(2000);
//	hal.motorStop();
//	WAIT(2000);
//	hal.switchPointOpen();
//	WAIT(2000);
//	hal.switchPointClose();
//	WAIT(2000);

//	hal.yellowLightOn();
//	hal.redLightOn();
//	hal.greenLightOn();
//	WAIT(2000);
//	hal.yellowLightOff();
//	hal.redLightOff();
//	hal.greenLightOff();

	hal.blinkGreen(true);
	hal.blinkRed(true);
	hal.blinkYellow(true);
	WAIT(5000);
	hal.redLightOff();
	WAIT(5000);
	hal.yellowLightOff();
	WAIT(5000);
	hal.greenLightOff();
	WAIT(5000);
	hal.blinkGreen(false);
	hal.blinkYellow(false);
	hal.blinkRed(false);
	WAIT(5000);
	hal.greenLightOff();
	WAIT(5000);
	hal.yellowLightOff();
	WAIT(5000);
	hal.redLightOff();

	std::cout << "now green of" << std::endl;

	WAIT(20000);
	cout << "Starting Sortingmachine ... done !" << endl;
//#ifdef SIMULATION
//  IOaccess_close();
//#endif
  return EXIT_SUCCESS;
}

