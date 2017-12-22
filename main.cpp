#include "Header.h"
#include <iostream>

#include "HardwareLayer.h"
#include "LogicLayer.h"

#include <chrono>

using namespace std;

void evaluateIfCb_1();
void printStartMessage();

int main(int argc, char *argv[])
{
	LOG_SET_LEVEL(DEBUG);
	LOG_SCOPE;

	std::chrono::steady_clock::time_point tp1,tp2,tp3;
	std::chrono::steady_clock::duration d1,d2(std::chrono::milliseconds(1000));
	float f1 = 0.5;

	tp1 = std::chrono::steady_clock::now();
	WAIT(200);
	tp2 = std::chrono::steady_clock::now();

	//tp3 = std::chrono::steady_clock::time_point(tp2-tp1);
//
//	d1 = ((d2-(tp2-tp1))*1000) / (int)(0*1000);
//
//	cout<<"time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(d1).count()<<endl;


	bool restart = false;
	do {
		printStartMessage();
		cb_this.parameterList.initParameters();
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
