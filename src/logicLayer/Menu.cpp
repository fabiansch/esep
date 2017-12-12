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
{
	LOG_SCOPE
}

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
				if(cb_this != cb_1) {cout<<"please start test on master."<<endl; continue;}
				controller_ << Signalname::ACTUATOR_TEST;
				test::Test::actuatorsTest(hal);
				controller_ << Signalname::STOP;
			} else if(!textInput.compare("mmi")) {
				if(cb_this != cb_1) {cout<<"please start test on master."<<endl; continue;}
				controller_ << Signalname::ACTUATOR_TEST;
				test::Test::mmiTest(hal);
				controller_ << Signalname::STOP;
			} else if(!textInput.compare("threadSafeness")) {
				test::Test::singletonThreadSafeTest();
				test::Test::threadSafenessInGpioTest();
				controller_ << Signalname::STOP;
			} else if(!textInput.compare("channel")) {
				test::Test::channelTest();
				controller_ << Signalname::STOP;
			} else if(!textInput.compare("sensors")) {
				if(cb_this != cb_1) {cout<<"please start test on master."<<endl; continue;}
				initSensorTest();
			} else if(!textInput.compare("buttons")) {
				controller_ << Signalname::BUTTON_TEST;
				test::Test::buttonsTest();
				controller_ << Signalname::STOP;
			} else if(!textInput.compare("all")) {
				if(cb_this != cb_1) {cout<<"please start test on master."<<endl; continue;}

				controller_ << Signalname::ACTUATOR_TEST;
				test::Test::actuatorsTest(hal);
				test::Test::mmiTest(hal);
				test::Test::singletonThreadSafeTest();
				test::Test::threadSafenessInGpioTest();
				test::Test::channelTest();
				initSensorTest();
				controller_ << Signalname::SENSOR_TEST;
			} else {
				printInfo();
			}
		} else if(!textInput.compare("calibration")) {
			cout << "Please make sure that slide has no items on it and hit enter." << endl;
			cin.get();
			controller_ << Signalname::CALIBRATION;
		} else if(!textInput.compare("run")) {
			cout<<"Please assure that no Items are on the conveyer belts and hit enter."<<endl<<endl;
			cin.get();
			controller_ << Signalname::RUN;
		} else if (!(textInput.compare("stop"))){
			controller_ << Signalname::STOP;
		} else if (!(textInput.compare("shutdown"))){
			return;
		} else if (!(textInput.compare("restart"))){
			controller_ << Signalname::STOP;
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

	controller_ << Signalname::SENSOR_TEST; //init

	//wait for input
	cout << "################ Automated Sensor Test Start ###############" << endl;
	cout << "Please put item (metal above) on master's input."<< endl;
	cout << "Continue with Enter..."<< endl;
	cin.get();

	controller_ << Signalname::SENSOR_TEST_TIMER_START;
	controller_ << Signalname::SENSOR_TEST_ENTER;



}

Menu::~Menu() {
	LOG_SCOPE
}

} /* namespace logicLayer */
