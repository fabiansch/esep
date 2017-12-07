/*
 * Item.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "HardwareLayer.h"
#include "Item.h"

namespace logicLayer {

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel) :
	hal_(  hal ),
	timerChannel_(timerChannel),
	statePtr(&stateMember)
{
	next_ = NULL;
	previous_ = NULL;

	statePtr->hal_ = hal_;
	//statePtr->item_ = this;
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
			statePtr->lbInputInt();
		break;
		case Signalname::ITEM_ARRIVED:
			statePtr->itemArrived();
		break;
		case Signalname::LB_SWITCH_INTERRUPTED:
			statePtr->lbSwitchInt();
		break;
		case Signalname::LB_OUTPUT_INTERRUPTED:
			statePtr->lbOutputInt();
		break;
		default:
			cout << "signal bubbles to item." << endl;
		break;
	}
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

} /* namespace logicLayer */
