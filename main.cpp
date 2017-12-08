#include "Header.h"
#include <iostream>

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "Test.h"

using namespace std;

void evaluateIfCb_1();
void printStartMessage();

int main(int argc, char *argv[])
{
	LOG_SET_LEVEL(DEBUG);
	LOG_SCOPE;
	printStartMessage();

	cout<<"CONNECTION_LOST: "<<(int)Signalname::CONNECTION_LOST<<endl;
	cout<<"CONNECTION_CONNECTED: "<<(int)Signalname::CONNECTION_CONNECTED<<endl;
	cout<<"SIGNAL_BUFFER_EMPTY: "<<(int)Signalname::SIGNAL_BUFFER_EMPTY<<endl;
	cout<<"ITEM_ARRIVED: "<<(int)Signalname::TRANSFER_ITEM<<endl;
	cout<<"SIGNAL_DUMMY: "<<(int)Signalname::SIGNAL_DUMMY<<endl;
	cout<<"SENSOR_TEST: "<<(int)Signalname::SENSOR_TEST<<endl;
	cout<<"BUTTON_TEST: "<<(int)Signalname::BUTTON_TEST<<endl;
	cout<<"ACTUATOR_TEST: "<<(int)Signalname::ACTUATOR_TEST<<endl;
	cout<<"RUN: "<<(int)Signalname::RUN<<endl;
	cout<<"CALIBRATION: "<<(int)Signalname::CALIBRATION<<endl;
	cout<<"STOP: "<<(int)Signalname::STOP<<endl;


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
		cout << "\n\nAre you master conveyor belt? (yes/no):" << endl;
		cin >> textInput;
	} while (!(textInput.compare("yes") ^ textInput.compare("no")));
	if (!textInput.compare("yes")) {
		cb_this = cb_first;
	}
	cin.get(); // get rid of extra return
}

void printStartMessage() {
			cout<< "	  ____                                           _              _ " <<endl;
			cout<< "	 / ___|___  _ ____   _____ _   _  ___  _ __     / \\   _ __   __| |" <<endl;
			cout<< "	| |   / _ \\| '_ \\ \\ / / _ \\ | | |/ _ \\| '__|   / _ \\ | '_ \\ / _` |" <<endl;
			cout<< "	| |__| (_) | | | \\ V /  __/ |_| | (_) | |     / ___ \\| | | | (_| |" <<endl;
			cout<< "	 \\____\\___/|_| |_|\\_/ \\___|\\__, |\\___/|_|    /_/   \\_\\_| |_|\\__,_|" <<endl;
			cout<< "	                           |___/                                  " <<endl;
			cout<< "	 ____             _   _               ____            _                 " <<endl;
			cout<< "	/ ___|  ___  _ __| |_(_)_ __   __ _  / ___| _   _ ___| |_ ___ _ __ ___  " <<endl;
			cout<< "	\\___ \\ / _ \\| '__| __| | '_ \\ / _` | \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ " <<endl;
			cout<< "	 ___) | (_) | |  | |_| | | | | (_| |  ___) | |_| \\__ \\ ||  __/ | | | | |" <<endl;
			cout<< "	|____/ \\___/|_|   \\__|_|_| |_|\\__, | |____/ \\__, |___/\\__\\___|_| |_| |_|" <<endl;
			cout<< " 	                              |___/         |___/                       " <<endl;
}
