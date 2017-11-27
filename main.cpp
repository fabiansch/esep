#include "Header.h"
#include <iostream>

#include "Interface.h"
#include "Receiver.h"
#include "Parameter.h"

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "Test.h"
#include "Menu.h"


#include "HeightSensor.h"

#include "ItemBuffer.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Starting Sortingmachine ..." << endl;

	LOG_SCOPE;
	LOG_SET_LEVEL(DEBUG);

	bool restart = false;

	do {

		//============== ASSIGNMENT OF CONVEYOR BELT ROLE ==============================
		cb_first = CB_FIRST;
		cb_all = CB_ALL;

		string textInput;
		do {
			cout << "Are you conveyor belt 1? (yes/no):" << endl;
			cin >> textInput;
		} while (!(textInput.compare("yes") xor textInput.compare("no")));

		if (!textInput.compare("yes")) {
			cb_this = cb_first;
		}
		cin.get(); // get rid of extra return

		//===================== INITIALISATION ==========================================
		hardwareLayer::HardwareLayer hal;
		logicLayer::LogicLayer loLay(hal);
		restart = loLay.getMenu().isRestart();

		//===================== COMPUTATION OF INPUT ==========================================
	} while (restart);

	cout << "===================== Shutting down Sortingmachine =========================" << endl;

	return EXIT_SUCCESS;

}
