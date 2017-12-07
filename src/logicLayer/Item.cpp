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
:	hal_(  hal )
,	timerChannel_(timerChannel)
,	statePtr(&stateMember)
,	next_(next)
{
	next_ = nullptr;
	previous_ = nullptr;

	statePtr->hal_ = hal_;
	statePtr->item_ = this;
}

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel, bool head )
:	hal_(  hal )
,	timerChannel_(timerChannel)
,	statePtr(&stateMember)
{
	next_ = nullptr;
	previous_ = nullptr;
	statePtr->hal_ = hal_;

	cout<<"hello"<<endl;

	if (head) {
		new (statePtr) Init;
		cout<<typeid((*statePtr)).name()<<endl;
	}
	statePtr->item_ = this;
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
		default:
			cout <<__FUNCTION__<< "default" << endl;
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
