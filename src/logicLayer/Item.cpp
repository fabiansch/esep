/*
 * Item.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "HardwareLayer.h"
#include "Item.h"

namespace logicLayer {

Item::Item( hardwareLayer::HardwareLayer& hal, Channel<Signal>& timerChannel) :
	hal_(  hal ),
	timerChannel_(timerChannel),
	statePtr(&stateMember)
{
	next_ = NULL;
	previous_ = NULL;
}

Item::~Item() {
	// TODO Auto-generated destructor stub
}


void Item::handle(Signal signal){
	cout << "signal bubbles to item." << endl;
	switch (signal.name) {
		case Signalname::SIGNAL_DUMMY:
			//silent is golden
			break;
		default:
			cout << "signal bubbles to item." << endl;
		break;
	}
}

} /* namespace logicLayer */
