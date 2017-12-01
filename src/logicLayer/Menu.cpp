/*
 * Menu.cpp
 *
 *  Created on: 24.11.2017
 *      Author: aca311
 */

#include "Menu.h"
#include "Logger.h"
#include "Parameter.h"
#include "Test.h"

namespace logicLayer {

Menu::Menu(hardwareLayer::HardwareLayer& hal, Channel<Signal>& controller)
: hal(hal)
, controller_(controller)
, restart(false)
{}

void Menu::computeInput(){

	string textInput;

	while(true) {
		cin >> textInput;
		cin.get();

		if(!textInput.compare("test")) {
			cout<<"please specify test:"<<endl<<endl;
			cout<<"actuators"<<endl;
			cout<<"mmi"<<endl;
			cout<<"threadSafeness"<<endl;
			cout<<"channel"<<endl;
			cout<<"sensors"<<endl;
			cout<<"buttons"<<endl;
			cout<<"all"<<endl<<endl;
			cin >> textInput;
			cin.get();

			if(!textInput.compare("actuators")) {
			test::Test::actuatorsTest(hal);
			} else if(!textInput.compare("mmi")) {
				test::Test::mmiTest(hal);
			} else if(!textInput.compare("threadSafeness")) {
				test::Test::singletonThreadSafeTest();
				test::Test::threadSafenessInGpioTest();
			} else if(!textInput.compare("channel")) {
				test::Test::channelTest();
			} else if(!textInput.compare("sensors")) {
				initSensorTest();
				controller_ << Signalname::SENSOR_TEST;
			} else if(!textInput.compare("buttons")) {
				test::Test::buttonsTest();
			} else if(!textInput.compare("all")) {
				test::Test::actuatorsTest(hal);
				test::Test::mmiTest(hal);
				test::Test::singletonThreadSafeTest();
				test::Test::threadSafenessInGpioTest();
				test::Test::channelTest();
				initSensorTest();
				controller_ << Signalname::SENSOR_TEST;
			}
		} else if(!textInput.compare("calibration")) {
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
	cout << "\n##################### Main Menu ##############################\nPlease enter command. Type \"help\" for options." << endl;
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

void Menu::initSensorTest() {
	string textInput;
	LOG_TEST<<"################ Initialize Automated Sensor Test ####################"<<endl;
	cout<<"################ Initialize Automated Sensor Test ####################"<<endl;


	//=============== calculating number of connected conveyor belts ==============
	int val = (int) cb_available;
	val++;
	int cbCount = 0;
	while (val > 1) {
		val >>= 1;
		cbCount++;
	}
	LOG_TEST<<"Detected CBs:\t" << cbCount << endl;

	if (cbCount == 0) {
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
	cout << "\n"<<cbCount << " conveyor belts detected." << endl;

	for (int i = 1; i <= cbCount; i++) {
		if (i == 1) {
			cout << "Enter unit number of the master festo machine:" << endl;
		} else {
			cout << "Enter unit number of the festo machine no. :"<< i << endl; }

		cin >> textInput;
		cin.get();
		LOG_TEST<<"Festo Machine Nr. "<<i<<": "<<textInput<<endl;
	}
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

} /* namespace logicLayer */
