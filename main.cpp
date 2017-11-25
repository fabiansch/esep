#include "Header.h"
#include <iostream>

#include "Interface.h"
#include "Receiver.h"
#include "Parameter.h"

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "Test.h"


#include "HeightSensor.h"

#include "ItemBuffer.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Starting Sortingmachine ..." << endl;

	LOG_SCOPE;
	LOG_SET_LEVEL(DEBUG);

	cb_first =  CB_FIRST;
	cb_all = CB_ALL;


	string textInput;
	do {
	cout<<"Are you conveyor belt 1? (yes/no):"<<endl;
	cin >> textInput;
	} while(!(textInput.compare("yes") xor textInput.compare("no")));

	if(!textInput.compare("yes")) {
		cb_this =  cb_first;
	}
	cin.get(); // get rid of extra return


	hardwareLayer::HardwareLayer hal;
	logicLayer::test::Test test = logicLayer::test::Test(&hal);

	WAIT(5000);
	test.buttonsTest();

	logicLayer::LogicLayer loLay(hal);

	cin.get();
	if (cb_this == cb_1) {
		test.actuatorsTest();
		test.mmiTest();
		test.threadSafenessInGpioTest();
		test.singletonThreadSafeTest();
	}

	uint16_t heightValue;
	for(int i = 10; i>0; i--) {
		  heightValue = hal.getHeight();
		  cout << heightValue << endl;
	}

	cout<<"Shut down?"<<endl;
	cin.get();


	cout << "Starting Sortingmachine ... done !" << endl;

	return EXIT_SUCCESS;

}
