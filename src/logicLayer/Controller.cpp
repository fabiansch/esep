/*
 * Controller.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Controller.h"

namespace logicLayer {

Controller::Controller() :
SignalReceiver::SignalReceiver()
{
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
	// TODO Auto-generated constructor stub
	std::cout << "create Controller" << endl;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::operator()(){

	Signal sig;
	while(running){
		std::cout << "blocking io at controller listener" << endl;
		sig << channel_;
		switch (sig.name) {
			case Signalname::SIGNAL_DUMMY:
				std::cout << "Controller receives signal." << endl;
				break;
			default:
				break;
		}
	}
}

} /* namespace logicLayer */
