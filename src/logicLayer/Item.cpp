/*
 * Item.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Item.h"

namespace logicLayer {

Item::Item() :
	statePtr( &stateMember )
{
	next_ = NULL;
	previous_ = NULL;
}

Item::Item( Item* prev, Item* next) :
	next_(prev),
	previous_(next),
	statePtr(&stateMember)
{

}

Item::~Item() {
	// TODO Auto-generated destructor stub
}


void Item::handle(Signal signal){
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
