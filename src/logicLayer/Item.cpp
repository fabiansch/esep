/*
 * Item.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "HardwareLayer.h"
#include "ErrorHandler.h"
#include "Item.h"
#include <thread>


namespace logicLayer {

std::thread timer;

int idCounter_ = 0;

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel, Item* next, ErrorHandler* errorHandler)
:	heightAbsolute(0)
,	heightCenter(0)
,	next_(next)
, 	previous_(nullptr)
, 	hal_(  hal )
,	timerChannel_(timerChannel)
,	errorHandler_(errorHandler)
,	statePtr(&stateMember)
{
	LOG_SCOPE
	statePtr->item_ = this;
	statePtr->hal_ = hal;
	statePtr->errorHandler_ = errorHandler;

	id = 0;
}

Item::Item( hardwareLayer::HardwareLayer* hal, Channel<Signal>* timerChannel, bool head, ErrorHandler* errorHandler )
:	heightAbsolute(0)
,	heightCenter(0)
,	next_(nullptr)
,	previous_(nullptr)
,	hal_(  hal )
,	timerChannel_(timerChannel)
,	errorHandler_(errorHandler)
,	statePtr(&stateMember)
{
	LOG_SCOPE
	LOG_DEBUG<<__FUNCTION__<<": creating HEAD Item."<<endl;
	statePtr->item_ = this;
	statePtr->hal_ = hal;
	statePtr->errorHandler_ = errorHandler;

	if (head) {
		new (statePtr) HEAD;
	}

	id = ++idCounter_;
}


Item::~Item() {
	LOG_SCOPE
}


void Item::handle(Signal signal){
	switch (signal.name) {
	// sensors
		case Signalname::SENSOR_HEIGHT_MATCH:
			break;
		case Signalname::SENSOR_HEIGHT_NOT_MATCH:
			break;
		case Signalname::SENSOR_METAL_MATCH:
			break;
		case Signalname::SENSOR_METAL_NOT_MATCH:
			break;
		case Signalname::SENSOR_SWITCH_IS_OPEN:
			statePtr->sensor_switch_is_open( signal );
			break;
		case Signalname::SENSOR_SWITCH_IS_CLOSED:
			statePtr->sensor_switch_is_closed( signal );
			break;
		// light barriers
		case Signalname::LB_INPUT_INTERRUPTED:
			statePtr->lb_input_interrupted( signal );
			break;
		case Signalname::LB_INPUT_FREED:
			statePtr->lb_input_freed( signal );
			break;
		case Signalname::LB_HEIGHT_INTERRUPTED:
			statePtr->lb_height_interrupted( signal );
			break;
		case Signalname::LB_HEIGHT_FREED:
			statePtr->lb_height_freed( signal );
			break;
		case Signalname::LB_SWITCH_INTERRUPTED:
			item_on_switch = true;
			statePtr->lb_switch_interrupted( signal );
			break;
		case Signalname::LB_SWITCH_FREED:
			item_on_switch = false;
			statePtr->lb_switch_freed( signal );
			break;
		case Signalname::LB_SLIDE_INTERRUPTED:
			statePtr->lb_slide_interrupted( signal );
			break;
		case Signalname::LB_SLIDE_FREED:
			statePtr->lb_slide_freed( signal );
			break;
		case Signalname::LB_OUTPUT_INTERRUPTED:
			statePtr->lb_output_interrupted( signal );
			item_on_output = true;
			break;
		case Signalname::LB_OUTPUT_FREED:
			statePtr->lb_output_freed( signal );
			item_on_output = false;
			break;
		// mmi
		// buttons
		case Signalname::BUTTON_E_STOP_PUSHED:
			break;
		case Signalname::BUTTON_E_STOP_PULLED:
			break;
		case Signalname::BUTTON_RESET_PUSHED:
			break;
		case Signalname::BUTTON_RESET_PULLED:
			break;
		case Signalname::BUTTON_START_PUSHED:
			break;
		case Signalname::BUTTON_START_PULLED:
			break;
		case Signalname::BUTTON_STOP_PUSHED:
			break;
		case Signalname::BUTTON_STOP_PULLED:
			break;
		// traffic lights
		case Signalname::YELLOW_LIGHT_ON:
			hal_->yellowLightOn();
			break;
		case Signalname::RED_LIGHT_ON:
			hal_->redLightOn();
			break;
		case Signalname::GREEN_LIGHT_ON:
			hal_->greenLightOn();
			break;
		case Signalname::BLINK_GREEN_FAST:
			hal_->blinkGreen(Speed::fast);
			break;
		case Signalname::BLINK_RED_FAST:
			hal_->blinkRed(Speed::fast);
			break;
		case Signalname::BLINK_YELLOW_FAST:
			hal_->blinkYellow(Speed::fast);
			break;
		case Signalname::BLINK_GREEN_SLOW:
			hal_->blinkGreen(Speed::slow);
			break;
		case Signalname::BLINK_YELLOW_SLOW:
			hal_->blinkYellow(Speed::slow);
			break;
		case Signalname::BLINK_RED_SLOW:
			hal_->blinkRed(Speed::slow);
			break;
		case Signalname::RED_LIGHT_OFF:
			hal_->redLightOff();
			break;
		case Signalname::YELLOW_LIGHT_OFF:
			hal_->yellowLightOff();
			break;
		case Signalname::GREEN_LIGHT_OFF:
			hal_->greenLightOff();
			break;
		// leds
		case Signalname::Q1_LED_ON:
			hal_->Q1LEDOn();
			break;
		case Signalname::Q1_LED_OFF:
			hal_->Q1LEDOff();
			break;
		case Signalname::Q2_LED_ON:
			hal_->Q2LEDOn();
			break;
		case Signalname::Q2_LED_OFF:
			hal_->Q2LEDOff();
			break;
		case Signalname::RESET_LED_ON:
			hal_->ResetLEDOn();
			break;
		case Signalname::RESET_LED_OFF:
			hal_->ResetLEDOff();
			break;
		case Signalname::START_LED_ON:
			hal_->StartLEDOn();
			break;
		case Signalname::START_LED_OFF:
			hal_->StartLEDOff();
			break;
		// motor
		case Signalname::MOTOR_START:
			LOG_DEBUG << "call MOTOR start ";
			hal_->motorStart();
			LOG_DEBUG << "MOTOR started ";
			break;
		case Signalname::MOTOR_STOP:
			LOG_DEBUG << "call MOTOR stop ";
			hal_->motorStop();
			LOG_DEBUG << "call MOTOR stopped ";
			break;
		case Signalname::MOTOR_FAST:
			LOG_DEBUG << "call MOTOR fast ";
			hal_->motorFast();
			LOG_DEBUG << "end MOTOR  fast";
			break;
		case Signalname::MOTOR_SLOW:
			LOG_DEBUG << "call MOTOR slow ";
			hal_->motorSlow();
			LOG_DEBUG << "end MOTOR  slow";
			break;
		case Signalname::MOTOR_ROTATE_CLOCKWISE:
			LOG_DEBUG << "call MOTOR clockwise ";
			hal_->motorRotateClockwise();
			LOG_DEBUG << "end MOTOR  clockwise";
			break;
		case Signalname::MOTOR_ROTATE_COUNTER_CLOCKWISE:
			LOG_DEBUG << "call MOTOR counter clockwise ";
			hal_->motorRotateCounterclockwise();
			LOG_DEBUG << "end MOTOR  clockwise";
			break;
		// switch
		case Signalname::SWITCH_OPEN:
			hal_->switchPointOpen();
			break;
		case Signalname::SWITCH_CLOSE:
			hal_->switchPointClose();
			break;
		// item
		case Signalname::TRANSFER_ITEM:
			statePtr->transfer_item( signal );
			break;
		// controller
		case Signalname::STOP:
			break;
		// timer
		case Signalname::TIMEFRAME_INPUT_ENTER:
			statePtr->timeframe_input_enter( signal );
			break;
		// other conveyer belt
		case Signalname::CONVEYOR_BELT_READY:
			next_cb_busy = false;
			statePtr->conveyer_belt_ready( signal );
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR<<"Item does not support following Signal: "<<(int)signal.name<<endl;
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

void Item::copyData(Item item){
	this->id = item.id;
	this->type = item.type;
}

void Item::startMotor(hardwareLayer::HardwareLayer* hal) {
	if (hal != nullptr) {
		hal->motorFast();
		hal->motorRotateClockwise();
		hal->motorStart();
	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void Item::send_CB_busy(hardwareLayer::HardwareLayer* hal) {
	if (cb_this == cb_sorting_2) {
		hal->sendSerial(Signal(cb_this, cb_previous, Signalname::CONVEYOR_BELT_BUSY));
	}
}


void Item::openSwitchPoint(hardwareLayer::HardwareLayer* hal) {
	if (hal != nullptr) {
		hal->switchPointOpen();
	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void closeSwitchIfNoItemOn(int milliseconds, hardwareLayer::HardwareLayer* hal) {
	do {
		WAIT(milliseconds);
	} while (item_on_output == true);

	if (item_on_switch == false) {
		hal->switchPointClose();
	}
}

void Item::closeSwitchPoint(int milliseconds,hardwareLayer::HardwareLayer* hal) {
	timer = std::thread(closeSwitchIfNoItemOn, milliseconds, hal);
	timer.detach();
}


void Item::onOutputAction(hardwareLayer::HardwareLayer* hal, Item* item, ErrorHandler* errorHandler) {
	if (hal != nullptr) {

		if(cb_this == cb_last){
			hal->motorStop();
			errorHandler->addPending(Signal(Signalname::LB_OUTPUT_FREED));
		} else {
			if (next_cb_busy == true) {
				hal->motorStop();
			} else {
				hal->motorStart();
				hal->sendSerial(Signal(cb_this, cb_next, Signalname::START_TIMERS_INPUT));
			}
		}

	} else {
		LOG_ERROR<<__FUNCTION__<<": called nullptr"<<endl;
		exit(EXIT_FAILURE);
	}
}

void stopMotorIfNoItemAfter(int milliseconds, hardwareLayer::HardwareLayer* hal) {
	WAIT(milliseconds);
	if (items_on_cb == 0) {
		hal->motorStop();
	}
}

void Item::lbOutputFreedAction(hardwareLayer::HardwareLayer* hal) {
	if(items_on_cb > 0) {
		hal->motorStart();
	}
	if(cb_this != cb_last) {
		timer = std::thread(stopMotorIfNoItemAfter, 1000, hal);
		timer.detach();
	}
}

void Item::addPendingError(ErrorHandler* errorHandler, Signal signal) {
	errorHandler->addPending(signal);
}

void Item::dequeueAndDeleteItem(Item* item) {
	item->next_->setPrevious(item->previous_);
	if (item->previous_) {
		item->previous_->setNext(item->next_);
	}
	if (items_on_cb > 0) {
		items_on_cb = items_on_cb - 1;
	} else {
		LOG_WARNING << "items_on_cb was zero or negative." << endl;
	}
	cout << "items on CB " << items_on_cb << endl;
	delete item;
}

void Item::sendItem(hardwareLayer::HardwareLayer* hal, Item* item) {
	hal->sendItemViaSerial(item);
}

void Item::send_CB_ready(hardwareLayer::HardwareLayer* hal) {
	hal->sendSerial(Signal(cb_this, cb_previous, Signalname::CONVEYOR_BELT_READY));
}

void Item::printItem(hardwareLayer::HardwareLayer* hal, Item* item){
	cout << "### Item ###" << endl;
	cout << "ID: "<< item->id << endl;
	//cout << "Type: " << (int) item->type << endl;
}

void Item::copyItemFromHAL(hardwareLayer::HardwareLayer* hal, Item* item){
	item->copyData( hal->getPassedItem() );
}

void Item::setID(int* id) {
	*id = ++idCounter_;
}


void Item::resetId() {
	idCounter_ = 0;
}

void Item::ArrivalSlideAction(hardwareLayer::HardwareLayer* hal) {
	if(items_on_cb <= 0) {
		hal->motorStop();
	}
}





} /* namespace logicLayer */
