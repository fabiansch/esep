#include "Header.h"
#include <iostream>

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "Test.h"

using namespace std;

void evaluateIfCb_1();

int main(int argc, char *argv[])
{
	LOG_SET_LEVEL(DEBUG);
	LOG_SCOPE;
	cout << "========================= Start Sortingmachine ============================" << endl;


	bool restart = false;
	do {
		evaluateIfCb_1();

		hardwareLayer::HardwareLayer hal;
		logicLayer::LogicLayer loLay(hal);

		restart = loLay.getMenu().isRestart();
	} while (restart);

	cout << "===================== Shutting down Sortingmachine =========================" << endl;

	return EXIT_SUCCESS;

}


void evaluateIfCb_1() {
	//============== ASSIGNMENT OF CONVEYOR BELT ROLE ==============================
	cb_first = CB_FIRST;
	cb_all = CB_ALL;
	string textInput;
	do {
		cout << "Are you conveyor belt 1? (yes/no):" << endl;
		cin >> textInput;
	} while (!(textInput.compare("yes") ^ textInput.compare("no")));
	if (!textInput.compare("yes")) {
		cb_this = cb_first;
	}
	cin.get(); // get rid of extra return
}
