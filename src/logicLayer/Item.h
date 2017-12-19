
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

namespace hardwareLayer {
	class HardwareLayer;
}

static bool amIwanted = false;

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
	static void closeSwitchPoint(int milliseconds,hardwareLayer::HardwareLayer*);
	static void onOutputAction(hardwareLayer::HardwareLayer* , Item*, ErrorHandler*);
	static void lbOutputFreedAction(hardwareLayer::HardwareLayer*);
	static void addPendingError(ErrorHandler*, Signal);
	static void dequeueAndDeleteItem(Item* item);
	static void sendItem(hardwareLayer::HardwareLayer* hal, Item* item);
	static void printItem(hardwareLayer::HardwareLayer* hal, Item* item);
	static void copyItemFromHAL(hardwareLayer::HardwareLayer* hal, Item* item);
	static void setID(int* id);
	static void stopMotorIfNoItemsOnCB(hardwareLayer::HardwareLayer* hal);



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





		void createItem(){
			//cout << "create item" << endl;
			item_->previous_ = new Item(item_->hal_, item_->timerChannel_, item_, errorHandler_);
		}

		void forwardSignal( Signal signal ){
			//cout<<"forwardSignal"<<endl;
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
		virtual void timeframe_height_enter( 	Signal signal ) override {}
		virtual void timeframe_height_leave( 	Signal signal ) override {}
		virtual void timeframe_switch_enter( 	Signal signal ) override {}
		virtual void timeframe_switch_leave( 	Signal signal ) override {}
		virtual void timeframe_slide_enter( 	Signal signal ) override {}
		virtual void timeframe_slide_leave( 	Signal signal ) override {}
		virtual void timeframe_output_enter( 	Signal signal ) override {}
		virtual void timeframe_output_leave( 	Signal signal ) override {}



		virtual void lb_input_interrupted( Signal signal ) override {
			cout<<"lb_input_interrupted"<<endl;
			if( cb_this == cb_sorting_1 ) {
				new (this) ArrivalInput;
			} else {
				addPendingError(errorHandler_, Signal(Signalname::LB_INPUT_FREED));
			}
		}

		virtual void transfer_item( Signal signal ) override {
			cout<<"transfer_item"<<endl;
			if( cb_this != cb_first ) {
				new (this) WaitForArrivalAtInput;
			}
		}

		virtual void timeframe_input_leave( Signal signal ) override {
			cout<<"timeframe_input_leave"<<endl;
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
			}
		}
	};

	struct WaitForArrivalAtInput : public State {
		WaitForArrivalAtInput() {
			cout<<"WaitForArrivalAtInput"<<endl;
			Item::startMotor(hal_);
			send_CB_busy(hal_);

			//copy item from hal
			copyItemFromHAL(hal_, item_);
		}

		virtual void lb_input_interrupted( Signal signal ) override {
			new (this) ArrivalInput;
		}


	};

	struct ArrivalInput : public State {
		ArrivalInput() {
			cout<<"ArrivalInput"<<endl;
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
			cout<<"DepartureInput"<<endl;
			*timerChannel_ << Signal(Signalname::START_TIMERS_HEIGHT);
		}
		void timeframe_height_enter( Signal signal ) override {
			cout<<"timeframe_height_enter"<<endl;
			new (this) WaitForArrivalAtHeight;
		}
	};

		struct WaitForArrivalAtHeight : public State {
			WaitForArrivalAtHeight() {
				cout<<"WaitForArrivalAtHeight"<<endl;
			}

		virtual void timeframe_height_leave( Signal signal ) override {
			cout<<"timeframe_height_leave"<<endl;
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
			}
		}

		virtual void lb_height_interrupted( Signal signal ) override {
			cout<<"lb_height_interrupted"<<endl;
			new (this) ArrivalHeight;
		}
	};

	struct ArrivalHeight : public State {
		ArrivalHeight() {
			cout<<"ArrivalHeight"<<endl;
			*timerChannel_ << Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL);
			*timerChannel_ << Signal(Signalname::START_TIMERS_SWITCH);
		}
		void timeframe_switch_enter( Signal signal ) override {
			cout<<"timeframe_switch_enter"<<endl;
			new (this) WaitForArrivalAtSwitch;
		}
	};

	struct WaitForArrivalAtSwitch : public State {
		WaitForArrivalAtSwitch() {
			cout<<"WaitForArrivalAtSwitch"<<endl;
		}

		virtual void timeframe_switch_leave( Signal signal ) override {
			cout<<"timeframe_switch_leave"<<endl;
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
			}
		}

		virtual void lb_switch_interrupted( Signal signal ) override {
			cout<<"lb_switch_interrupted"<<endl;
			new (this) ArrivalSwitch;
		}
	};

	struct ArrivalSwitch : public State{
		ArrivalSwitch() {

			//get values from type identification
			item_->type = TypeIdentification::typeScans.front();
			TypeIdentification::typeScans.erase(TypeIdentification::typeScans.begin());

			*timerChannel_ << Signal(Signalname::TIMEFRAME_SWITCH_LEAVE_KILL);

			if(Sorting::amIWanted(item_)) {
				Item::openSwitchPoint(hal_);
				*timerChannel_ << Signal(Signalname::START_TIMERS_OUTPUT);
			} else {
				*timerChannel_ << Signal(Signalname::START_TIMERS_SLIDE);
			}
		}

		virtual void lb_switch_freed( Signal signal ) override {
			cout<<"lb_switch_freed"<<endl;
			Item::closeSwitchPoint(800, hal_);

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
			cout<<"WaitForArrivalAtSlide"<<endl;
		}

		virtual void timeframe_slide_leave( Signal signal ) override {
			cout<<"timeframe_slide_leave"<<endl;
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
			}
		}

		virtual void lb_slide_interrupted( Signal signal ) override {
			cout<<"lb_slide_interrupted"<<endl;
			new (this) ArrivalSlide;
		}

	};

	struct ArrivalSlide : public State {
		ArrivalSlide() {
			cout<<"ArrivalSlide"<<endl;

			*timerChannel_ << Signal(Signalname::TIMEFRAME_SLIDE_LEAVE_KILL);
			if(cb_this == cb_sorting_2) {
				send_CB_ready(hal_);
			}

			Item::printItem(hal_, item_);

			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);

		}
	};

	struct DepatureSlide : public State{

	};

	struct SlideIsFull : public State{

	};

	struct WaitForArrivalAtOuput : public State {
		WaitForArrivalAtOuput() {
			cout<<"WaitForArrivalAtOuput"<<endl;
		}

		virtual void timeframe_output_leave( Signal signal ) override {
			cout<<"timeframe_output_leave"<<endl;
			addPendingError(errorHandler_, Signal(Signalname::BUTTON_START_PUSHED));
			Item::dequeueAndDeleteItem(item_);
			Item::stopMotorIfNoItemsOnCB(hal_);
			if(cb_this == cb_sorting_2) {
				this_cb_busy = false;
			}
		}

		virtual void lb_output_interrupted( Signal signal ) override {
			cout<<"lb_output_interrupted"<<endl;
			new (this) ArrivalOutput;
		}

	};

	struct ArrivalOutput : public State{
		ArrivalOutput(){
			cout<<"ArrivalOutput"<<endl;
			*timerChannel_ << Signal(Signalname::TIMEFRAME_OUTPUT_LEAVE_KILL);
			Item::onOutputAction(hal_, item_, errorHandler_);

			Item::printItem(hal_, item_);
		}

		virtual void conveyer_belt_ready( Signal signal ) override {
			cout<<"conveyer_belt_ready"<<endl;
			new (this) ArrivalOutput;
		}

		virtual void lb_output_freed( Signal signal ) override {
			cout<<"lb_output_freed"<<endl;
			new (this) DepartureOutput;
		}
	};

	struct DepartureOutput : public State{
		DepartureOutput(){
			cout<<"DepartureOutput"<<endl;

			if(cb_this != cb_last) {
				if(next_cb_busy == false) {
//					next_cb_busy.parameterList.showParameters();
					cout<<"send item"<<endl;
					Item::sendItem(hal_, item_);
				} else {
					// TODO error Item lost
				}
			}
			if(cb_this == cb_sorting_2) {
				send_CB_ready(hal_);
			}
			Item::dequeueAndDeleteItem(item_);
			Item::lbOutputFreedAction(hal_);
			cout<<"DEPATURE OUTPUT DONE"<<endl;
		}
	};

	IDLE stateMember;

};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
