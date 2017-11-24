/*
 * Dummy.cpp
 *
 *  Created on: 17.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "Dispatcher.h"


namespace logicLayer {

Dispatcher::Dispatcher(
		hardwareLayer::HardwareLayer& hal,
		Channel& controller,
		Channel& typeIdent
) :
hal(hal),
controller_(controller),
typeIdent_(typeIdent)
{
	hal.register_observer(this);
}

Dispatcher::~Dispatcher() {
	cout << "call Dispatcher's deconstructor " << endl;
	terminate();
}

void Dispatcher::notify(){
	Signal signal;
	while((signal = hal.getSignal()).name != Signalname::SIGNAL_BUFFER_EMPTY) {

		switch (signal.name) {
		// serial
		case Signalname::CONNECTION_LOST:
			cout<<"CONNECTION LOST"<<endl;
			hal.blinkRed(Speed::slow);
			hal.greenLightOff();
			cb_this.parameterList.showParameters();
			break;
		case Signalname::CONNECTION_CONNECTED:
			cout<<"CONNECTION CONNECTED"<<endl;
			hal.blinkGreen(Speed::slow);
			hal.redLightOff();
			cb_this.parameterList.showParameters();
			break;
		default:
			controller_ << signal;
			break;
		}
	}
}


} /* namespace logicLayer */
