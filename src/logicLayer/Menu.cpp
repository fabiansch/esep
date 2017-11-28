/*
 * Menu.cpp
 *
 *  Created on: 24.11.2017
 *      Author: aca311
 */

#include "Menu.h"
#include "Logger.h"
#include "Parameter.h"

namespace logicLayer {

Menu::Menu(Channel<Signal>& controller)
: controller_(controller),
  restart(false)
{}

void Menu::computeInput(){

	printInfo();

	string textInput;

	while(true){
		cin >> textInput;
		cin.get();

		if(!textInput.compare("test")) {
			testInit();
			printInfo();
		} else	if(!textInput.compare("calibration")) {
			controller_ << Signalname::CALIBRATION;
		} else if(!textInput.compare("run")) {
			controller_ << Signalname::RUN;
		} else if (!(textInput.compare("stop"))){
			controller_ << Signalname::STOP;
		} else if (!(textInput.compare("shutdown"))){
			return;
		} else if (!(textInput.compare("restart"))){
			restart = true;
			return;
		} else if (!(textInput.compare("help"))){
			printOptions();
		} else {
			cout<< "Invalid command." <<endl;
			printInfo();
		}
	}
}

bool Menu::isRestart(){
	return restart;
}

void Menu::printInfo() {
	cout << "\nPlease enter command. Type \"help\" for options." << endl;
}

void Menu::printOptions() {
			cout<< "\nYou have following options:" <<endl;
			cout<< "\ntest\t\tStarts hardware tests. Follow the given instructions." <<endl;
			cout<< "\ncalibration\tStarts calibration. Follow the given instructions." <<endl;
			cout<< "\nrun\t\tStarts normal running mode. " <<endl;
			cout<< "\nstop\t\tStops current activity. You can then enter a new command." <<endl;
			cout<< "\nshutdown\tShut down system." <<endl;
			cout<< "\nrestart\t\tRestart System." <<endl;
			cout<< "\nhelp\t\tShows this help text.\n" <<endl;
}

void Menu::testInit() {
	string textInput;
	LOG_TEST<<"### NEW TEST START ###"<<endl;

	//=============== calculating number of connected conveyor belts ==============
	int val = (int) cb_available;
	val++;
	int cbCount = 0;
	while (val > 1) {
		val >>= 1;
		cbCount++;
	}
	LOG_TEST<<"Detected CBs:\t" << cbCount << endl;

	if (cbCount > 0) {
		cout << "\n"<<cbCount << " conveyor belts detected." << endl;
	} else {
		cout << "\n## NO CONVEYOR BELTS DETECTED! ABORTING TEST. ##" << endl;
		return;
	}

	//=============== entering name =======================================
	cout << "\n\nPlease enter required data for testing. \nEnter name:"<< endl;
	cin >> textInput;
	cin.get();
	LOG_TEST<<"Name:\t"<<textInput<<endl;

	//=============== entering code version =======================================
	cout << "\nEnter code version:" << endl;
	cin >> textInput;
	cin.get();
	LOG_TEST<<"Version:\t"<<textInput<<endl;


	//======================= entering each CB number ==========================================
	for (int i = 1; i <= cbCount; i++) {
		if (i == 1) {
			cout << "Enter unit number of the master festo machine:" << endl;
		} else {
			cout << "Enter unit number of the festo machine no. :"<< i << endl; }

		cin >> textInput;
		cin.get();
		LOG_TEST<<"Festo Machine Nr. "<<i<<": "<<textInput<<endl;
	}

	controller_ << Signalname::TEST;
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

} /* namespace logicLayer */
