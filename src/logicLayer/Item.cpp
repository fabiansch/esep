/*
 * Item.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "HardwareLayer.h"
#include "Item.h"

namespace logicLayer {

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel, Item* next)
:	heightAbsolute(0)
,	heightCenter(0)
,	next_(next)
, 	previous_(nullptr)
, 	hal_(  hal )
,	timerChannel_(timerChannel)
,	statePtr(&stateMember)
{
	LOG_SCOPE
	statePtr->item_ = this;
	statePtr->hal_ = hal;
}

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel, bool head )
:	heightAbsolute(0)
,	heightCenter(0)
,	next_(nullptr)
,	previous_(nullptr)
,	hal_(  hal )
,	timerChannel_(timerChannel)
,	statePtr(&stateMember)
{
	LOG_SCOPE
	LOG_DEBUG<<__FUNCTION__<<": creating HEAD Item."<<endl;
	statePtr->item_ = this;
	statePtr->hal_ = hal;
	if (head) {
		new (statePtr) Init;
	}
}


Item::~Item() {
	LOG_SCOPE
}


void Item::handle(Signal signal){
	cout << "signal bubbles to item." << endl;
	switch (signal.name) {
		case Signalname::SIGNAL_DUMMY:
			//silent is golden
		break;
		case Signalname::LB_INPUT_INTERRUPTED:
			statePtr->lbInputInt( signal );
		break;
		case Signalname::ITEM_ARRIVED:
			statePtr->itemArrived( signal );
		break;
		case Signalname::LB_SWITCH_INTERRUPTED:
			statePtr->lbSwitchInt( signal );
		break;
		case Signalname::LB_OUTPUT_INTERRUPTED:
			statePtr->lbOutputInt( signal );
		break;
		case Signalname::LB_OUTPUT_FREED:
			statePtr->lbOutputFreed( signal );
		break;
		default:
			LOG_ERROR <<__FUNCTION__<< ": unknown signal in Item occurred." << endl;
			exit(EXIT_FAILURE);
		break;
	}
}

void Item::setNext(Item* item) {
	next_ = item;
}

void Item::setPrevious(Item* item) {
	previous_ = item;
}

void Item::startMotor(hardwareLayer::HardwareLayer* hal_) {
	if (hal_ != nullptr) {
		hal_->motorFast();
		hal_->motorRotateClockwise();
		hal_->motorStart();
	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void Item::openSwitchPoint(hardwareLayer::HardwareLayer* hal_) {
	if (hal_ != nullptr) {
		hal_->switchPointOpen();
	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void Item::onOutputAction(hardwareLayer::HardwareLayer* hal_, Item* item) {
	if (hal_ != nullptr) {

		hal_->switchPointClose();

		if(cb_this == cb_last){
			hal_->motorStop();
		}

		if(cb_this == cb_first){
			hal_->sendItemViaSerial(item);
		}

	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void Item::DepatureAtOutputAction(hardwareLayer::HardwareLayer* hal_) {
	if(items_on_cb > 0) {
		hal_->motorStart();
	}
}

} /* namespace logicLayer */
