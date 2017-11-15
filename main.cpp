#include "Header.h"
#include <iostream>

#include "Interface.h"
#include "Receiver.h"
#include "Parameter.h"

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "Test.h"


#include "HeightSensor.h"

using namespace std;

int main(int argc, char *argv[])
{


	cout << "Starting Sortingmachine ..." << endl;

	LOG_SCOPE;
	LOG_SET_LEVEL(DEBUG);

	string textInput;
	do {
	cout<<"Are you conveyor belt 1? (yes/no):"<<endl;
	cin >> textInput;
	} while(!(textInput.compare("yes") xor textInput.compare("no")));

	if(!textInput.compare("yes")) {
		cb_this =  Parameter<uint8_t>(cb_1, "Conveyer belt 1");
	}
	cin.get(); // get rid of extra return

	cb_all = Parameter<uint8_t>(0xff, "All possible conveyer belts.");


	hardwareLayer::HardwareLayer hal;
	logicLayer::LogicLayer loLay = logicLayer::LogicLayer(hal);

	WAIT(5000);
	cout<<"READY FOR TESTING"<<endl;

	logicLayer::test::Test test = logicLayer::test::Test(&hal);

	if (cb_this == cb_1) {
		test.actuatorsTest();
		test.mmiTest();
		test.sensorsTest();
		test.threadSafenessInGpioTest();
		test.singletonThreadSafeTest();
	}

	cout<<"Shut down?"<<endl;
	cin.get(); // get rid of extra return


	cout << "Starting Sortingmachine ... done !" << endl;

	return EXIT_SUCCESS;

}

