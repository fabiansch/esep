/*
 * Menu.cpp
 *
 *  Created on: 24.11.2017
 *      Author: aca311
 */

#include "Menu.h"

namespace logicLayer {

Menu::Menu(Channel<Signal>& controller)
: controller_(controller),
  restart(false)
{}

void Menu::computeInput(){
	string textInput;
	while(true){
		cin >> textInput;
		cin.get();

		if(!textInput.compare("test")) {
			controller_ << Signalname::TEST;
		}

		if(!textInput.compare("normal")) {
			controller_ << Signalname::NORMAL;
		}

		if(!textInput.compare("calibration")) {
			controller_ << Signalname::CALIBRATION;
		}

		if (!(textInput.compare("stop"))){
			controller_ << Signalname::STOP;
		}

		if (!(textInput.compare("shutdown"))){
			return;
		}

		if (!(textInput.compare("restart"))){
			restart = true;
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
