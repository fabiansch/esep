
/*
 * Item.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "Header.h"
#include "Channel.h"
#include "SignalReceiver.h"
#include "Sorting.h"
#include "TypeIdentification.h"
#include <thread>

namespace hardwareLayer {
	class HardwareLayer;
}


namespace logicLayer {

class ErrorHandler;

class Item {
public:
	Item( hardwareLayer::HardwareLayer* , Channel<Signal>*, Item*, ErrorHandler* );
	Item( hardwareLayer::HardwareLayer* , Channel<Signal>*, bool head, ErrorHandler* );
	virtual ~Item();

	void handle( Signal );

	static void startMotor(hardwareLayer::HardwareLayer* );
	static void send_CB_busy(hardwareLayer::HardwareLayer* );
	static void send_CB_ready(hardwareLayer::HardwareLayer* );
	static void openSwitchPoint(hardwareLayer::HardwareLayer* );
	static void closeSwitchPoint(hardwareLayer::HardwareLayer*);
	static void onOutputAction(hardwareLayer::HardwareLayer* , Item*, ErrorHandler*);
	static void lbOutputFreedAction(hardwareLayer::HardwareLayer*);
	static void addPendingError(ErrorHandler*, Signal);
	static void dequeueAndDeleteItem(Item* item);
	static void sendItem(hardwareLayer::HardwareLayer* hal, Item* item);
	static void printItem(hardwareLayer::HardwareLayer* hal, Item* item);
	static void copyItemFromHAL(hardwareLayer::HardwareLayer* hal, Item* item);
	static void setID(int* id);
	static void stopMotorIfNoItemsOnCB(hardwareLayer::HardwareLayer* hal);
	static void stopMotorIfOneOrZeroItemsOnCB(hardwareLayer::HardwareLayer* hal);
	static void sendSlideFull(hardwareLayer::HardwareLayer* hal);
	static void broadcastSlideEmpty(hardwareLayer::HardwareLayer* hal, logicLayer::ErrorHandler* errorHandler);

	void turnYellowLightOn(bool on);




	void setNext(Item*);
	void setPrevious(Item*);
	void copyData( Item );

	static void resetId();
	int getId(){ return id; }

	const ItemType& getType() const { return type; }

	bool isPendingSortout() const {
		return pendingSortout;
	}

	void setPendingSortout(bool pendingSortout = false) {
		this->pendingSortout = pendingSortout;
	}

	//only need for sensor test
	int heightAbsolute;
	int heightCenter;

private:


	/**
	 *  Unique item identifier
	 */
	int id;

	/**
	 *  Type of passed item
	 */
	ItemType type;

	Item* next_;
	Item* previous_;

	/**
	 * Flag which marks if item has to be sorted out on next cb
	 */
	bool pendingSortout = false;

	hardwareLayer::HardwareLayer* hal_;
	Channel<Signal>* timerChannel_;
	ErrorHandler* errorHandler_;

	struct State {//top-level state

		virtual void lb_input_interrupted( 		Signal signal ){ forwardSignal( signal ); }
		virtual void lb_input_freed( 			Signal signal ){ forwardSignal( signal ); }
		virtual void sensor_switch_is_open( 	Signal signal ){ forwardSignal( signal ); }
		virtual void sensor_switch_is_closed( 	Signal signal ){ forwardSignal( signal ); }
		virtual void lb_height_interrupted( 	Signal signal ){ forwardSignal( signal ); }
		virtual void lb_height_freed( 			Signal signal ){ forwardSignal( signal ); }
		virtual void lb_switch_interrupted( 	Signal signal ){ forwardSignal( signal ); }
		virtual void lb_switch_freed( 			Signal signal ){ forwardSignal( signal ); }
		virtual void lb_slide_interrupted( 		Signal signal ){ forwardSignal( signal ); }
		virtual void lb_slide_freed( 			Signal signal ){ forwardSignal( signal ); }
		virtual void lb_output_interrupted( 	Signal signal ){ forwardSignal( signal ); }
		virtual void lb_output_freed( 			Signal signal ){ forwardSignal( signal ); }
		virtual void transfer_item( 			Signal signal ){ forwardSignal( signal ); }
		virtual void conveyer_belt_ready(		Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_input_leave( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_height_enter( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_height_leave( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_switch_enter( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_switch_leave( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_slide_enter( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_slide_leave( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_output_enter( 	Signal signal ){ forwardSignal( signal ); }
		virtual void timeframe_output_leave( 	Signal signal ){ forwardSignal( signal ); }
		virtual void slide_full(				Signal signal ){ forwardSignal( signal ); }
		virtual void close_switch(   			Signal signal ){ forwardSignal( signal ); }

		void createItem(){
			item_->previous_ = new Item(item_->hal_, item_->timerChannel_, item_, errorHandler_);
		}

		void forwardSignal( Signal signal ){
			if(item_->previous_ == nullptr ){
				createItem();
			}
			item_->previous_->handle( signal );
		}

		Item* item_;
		hardwareLayer::HardwareLayer* hal_;
		Channel<Signal>* timerChannel_;
		ErrorHandler* errorHandler_;

	} *statePtr;

	struct HEAD : public State {

	};

	struct IDLE : public State{
		virtual void sensor_switch_is_closed( 	Signal signal ) override {}
		virtual void sensor_switch_is_open( 	Signal signal ) override {}
		virtual void lb_input_freed(			Signal signal ) override {}
		virtual void lb_height_interrupted( 	Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_HEIGHT_FREED)); }
		virtual void lb_height_freed( 			Signal signal ) override {}
		virtual void lb_switch_interrupted( 	Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_SWITCH_FREED)); }
		virtual void lb_switch_freed( 			Signal signal ) override {}
		virtual void lb_slide_interrupted( 		Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_SLIDE_FREED)); }
		virtual void lb_slide_freed( 			Signal signal ) override {}
		virtual void lb_output_interrupted( 	Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_OUTPUT_FREED)); }
		virtual void lb_output_freed( 			Signal signal ) override {}
		virtual void conveyer_belt_ready( 		Signal signal ) override {}
		virtual void timeframe_input_leave( 	Signal signal ) override {}
		virtual void timeframe_height_enter( 	Signal signal ) override {}
		virtual void timeframe_height_leave( 	Signal signal ) override {}
		virtual void timeframe_switch_enter( 	Signal signal ) override {}
		virtual void timeframe_switch_leave( 	Signal signal ) override {}
		virtual void timeframe_slide_enter( 	Signal signal ) override {}
		virtual void timeframe_slide_leave( 	Signal signal ) override {}
		virtual void timeframe_output_enter( 	Signal signal ) override {}
		virtual void timeframe_output_leave( 	Signal signal ) override {}
		virtual void slide_full(				Signal signal ) override {}
		virtual void close_switch(   			Signal signal ) override {}

		virtual void lb_input_interrupted( Signal signal ) override {
			if( cb_this == cb_sorting_1 ) {
				new (this) ArrivalInput;
			} else {
				addPendingError(errorHandler_, Signal(Signalname::LB_INPUT_FREED));
			}
		}

		virtual void transfer_item( Signal signal ) override {
			if( cb_this != cb_first ) {
				new (this) WaitForArrivalAtInput;
			}
		}
	};

	struct WaitForArrivalAtInput : public State {
		WaitForArrivalAtInput() {
			Item::startMotor(hal_);
			send_CB_busy(hal_);

			//copy item from hal
			copyItemFromHAL(hal_, item_);
		}

		virtual void timeframe_input_leave( Signal signal ) override {
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
				//item is lost -> so inform cb1's sorting unit, which item is desired on cb2
				Sorting::instance().informCB1SortingUnit( hal_ );
			}

		}

		virtual void lb_input_interrupted( Signal signal ) override {
			new (this) ArrivalInput;
		}

	};

	struct ArrivalInput : public State {
		ArrivalInput() {
			if(cb_this == cb_1) {
				Item::setID(&item_->id);
			}
			items_on_cb = items_on_cb + 1;

			*timerChannel_ << Signal(Signalname::TIMEFRAME_INPUT_LEAVE_KILL);

			if (not item_on_output) {
				Item::startMotor(hal_);
			}
		}



		virtual void lb_input_freed( Signal signal ) override {
			new (this) DepartureInput;
		}
	};

	struct DepartureInput : public State {
		DepartureInput() {
			*timerChannel_ << Signal(Signalname::START_TIMERS_HEIGHT);
		}
		void timeframe_height_enter( Signal signal ) override {
			new (this) WaitForArrivalAtHeight;
		}
	};

	struct WaitForArrivalAtHeight : public State {
		WaitForArrivalAtHeight() {
		}

		virtual void timeframe_height_leave( Signal signal ) override {
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
				//item is lost -> so inform cb1's sorting unit, which item is desired on cb2
				Sorting::instance().informCB1SortingUnit( hal_ );
			}
		}

		virtual void lb_height_interrupted( Signal signal ) override {
			new (this) ArrivalHeight;
		}
	};

	struct ArrivalHeight : public State {
		ArrivalHeight() {
			*timerChannel_ << Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL);
			*timerChannel_ << Signal(Signalname::START_TIMERS_SWITCH);
		}
		void timeframe_switch_enter( Signal signal ) override {
			new (this) WaitForArrivalAtSwitch;
		}
	};

	struct WaitForArrivalAtSwitch : public State {
		WaitForArrivalAtSwitch() {
		}

		virtual void timeframe_switch_leave( Signal signal ) override {
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
				//item is lost -> so inform cb1's sorting unit, which item is desired on cb2
				Sorting::instance().informCB1SortingUnit( hal_ );
			}
		}

		virtual void lb_switch_interrupted( Signal signal ) override {
			new (this) ArrivalSwitch;
		}
	};

	struct ArrivalSwitch : public State {
		ArrivalSwitch() {

			//get values from type identification and keep value from cb 1
			if( cb_this == cb_sorting_1){
				item_->type = TypeIdentification::typeScans.front();
			}
			else{
				float height_at_cb_1 = item_->getType().height_cb_1;
				item_->type = TypeIdentification::typeScans.front();
				item_->type.height_cb_1 = height_at_cb_1;
			}


			TypeIdentification::typeScans.erase(TypeIdentification::typeScans.begin());

			*timerChannel_ << Signal(Signalname::TIMEFRAME_SWITCH_LEAVE_KILL);

			bool sortOut = Sorting::amIWanted(item_);
			if(sortOut) {
				Item::openSwitchPoint(hal_);
				*timerChannel_ << Signal(Signalname::START_TIMERS_OUTPUT);
				*timerChannel_ << Signal(Signalname::SWITCH_CLOSE);
			} else {
				*timerChannel_ << Signal(Signalname::START_TIMERS_SLIDE);
				if(this_slide_full) {
					addPendingError(errorHandler_, Signal(Signalname::SLIDE_EMPTY));
				}
				item_->turnYellowLightOn(true);
			}

			Item::printItem(hal_, item_);

			//check if pending sortout flag and current scan result dont fit togehter on cb2
			if(!item_->isPendingSortout() && sortOut && cb_this == cb_sorting_2){
				Sorting::instance().informCB1SortingUnit( hal_ ); //inform cb1 about cb2's sorting state
			}
		}

		virtual void close_switch(Signal signal) override {
			Item::closeSwitchPoint(hal_);
		}

		virtual void timeframe_output_enter( Signal signal ) override {
			new (this) WaitForArrivalAtOuput;
		}

		virtual void timeframe_slide_enter( Signal signal ) override {
			new (this) WaitForArrivalAtSlide;
		}

	};

	struct WaitForArrivalAtSlide : public State {
		WaitForArrivalAtSlide() {
		}

		virtual void timeframe_slide_leave( Signal signal ) override {
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
				//item is lost -> so inform cb1's sorting unit, which item is desired on cb2
				Sorting::instance().informCB1SortingUnit( hal_ );
			}
			item_->turnYellowLightOn(false);
		}

		virtual void lb_slide_interrupted( Signal signal ) override {
			new (this) ArrivalSlide;
		}

	};

	struct ArrivalSlide : public State {
		ArrivalSlide() {

			*timerChannel_ << Signal(Signalname::TIMEFRAME_SLIDE_LEAVE_KILL);
			if(cb_this == cb_sorting_2) {
				send_CB_ready(hal_);
			}
			Item::stopMotorIfOneOrZeroItemsOnCB(hal_);
			std::thread([=]() {
				WAIT(500);
				if(this->item_) {
					this->item_->handle(Signal(Signalname::SLIDE_FULL));
				}
			}).detach();
		}

		virtual void slide_full (Signal signal ) override {
			new (this) SlideFull;
		}

		virtual void lb_slide_freed( Signal signal ) override {
			new (this) DepatureSlide;
		}

	};

	struct SlideFull : public State {
		SlideFull() {
			this_slide_full = true;
			Item::sendSlideFull(hal_);
		}

		virtual void lb_slide_freed( Signal signal ) override {
			this_slide_full = false;
			Item::broadcastSlideEmpty(hal_, errorHandler_);
			new (this) DepatureSlide;
		}

	};

	struct DepatureSlide : public State {
		DepatureSlide() {
			Item::dequeueAndDeleteItem(item_);
			item_->turnYellowLightOn(false);
		}
	};

	struct WaitForArrivalAtOuput : public State {
		WaitForArrivalAtOuput() {
		}

		virtual void timeframe_output_leave( Signal signal ) override {
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);

			//set sorting to previous state
			Sorting::instance().setOrderState(Sorting::instance().getPreviousState());

			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
				//item is lost -> so inform cb1's sorting unit, which item is desired on cb2
				Sorting::instance().informCB1SortingUnit( hal_ );
			}



		}

		virtual void lb_output_interrupted( Signal signal ) override {
			new (this) ArrivalOutput;
		}

	};

	struct ArrivalOutput : public State{
		ArrivalOutput(){
			*timerChannel_ << Signal(Signalname::TIMEFRAME_OUTPUT_LEAVE_KILL);
			Item::onOutputAction(hal_, item_, errorHandler_);

			cout << "### Print at output ###" << endl;
			Item::printItem(hal_, item_);
		}

		virtual void conveyer_belt_ready( Signal signal ) override {
			new (this) ArrivalOutput;
		}

		virtual void lb_output_freed( Signal signal ) override {
			new (this) DepartureOutput;
		}
	};

	struct DepartureOutput : public State{
		DepartureOutput(){

			if(cb_this != cb_last) {
				if(next_cb_busy == false) {
					Item::sendItem(hal_, item_);
					Item::printItem(hal_, item_);
				} else {
					// TODO error Item lost
				}
			}
			if(cb_this == cb_sorting_2) {
				send_CB_ready(hal_);
			}
			Item::dequeueAndDeleteItem(item_);
			Item::lbOutputFreedAction(hal_);
		}
	};

	IDLE stateMember;

};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
