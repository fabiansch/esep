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
	string textInput;
	cout << "Please enter command. Type \"help\" for options.\n" << endl;

	while(true){
		cin >> textInput;
		cin.get();

		if(!textInput.compare("test")) {
			Logger& testLogger = Logger::getLogger();
			cout << "\nStarts test. \nSome data is needed for logging. \n Enter name:\n" << endl;
			cin >> textInput;
			cin.get();
			testLogger.logTest("Name:\t"+textInput);

			cout << "\nEnter code version:\n" << endl;
			cin >> textInput;
			cin.get();
			testLogger.logTest("Version:\t"+textInput);

			int val = (int) cb_available;
			val++;
			int cbCount = 0;
			while (val > 1) {
				val >>= 1;
				cbCount++;
			}
			testLogger.logTest("Anzahl:\t"+cbCount);
			cout<<"ES WURDEN "<<cbCount<<" CONVEYOR BELTS ERKANNT ================"<<endl;


			cout << "\nEnter unit number of the tested festo machine:\n" << endl;
			cin >> textInput;
			cin.get();
			testLogger.logTest("Name:\t"+textInput);

			controller_ << Signalname::TEST;
		}

		if(!textInput.compare("calibration")) {
			controller_ << Signalname::CALIBRATION;
		}

		if(!textInput.compare("run")) {
			controller_ << Signalname::RUN;
		}

		if (!(textInput.compare("stop"))){
			controller_ << Signalname::STOP;
		}

		if (!(textInput.compare("shutdown"))){
			return;
		}

		if (!(textInput.compare("restart"))){
			restart = true;
			return;
		}
		if (!(textInput.compare("help"))){
			cout<< "You have following options:" <<endl;
			cout<< "\ntest\t\tStarts hardware tests. Follow the given instructions." <<endl;
			cout<< "\ncalibration\tStarts calibration. Follow the given instructions." <<endl;
			cout<< "\nrun\t\tStarts normal running mode. " <<endl;
			cout<< "\nstop\t\tStops current activity. You can then enter a new command." <<endl;
			cout<< "\nshutdown\tShut down system." <<endl;
			cout<< "\nrestart\t\tRestart System." <<endl;
			cout<< "\nhelp\t\tShows this help text.\n" <<endl;
		}
	}
}

bool Menu::isRestart(){
	return restart;
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

} /* namespace logicLayer */
