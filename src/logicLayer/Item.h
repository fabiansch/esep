
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
	static void ArrivalSlideAction(hardwareLayer::HardwareLayer* hal);



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
		virtual void timeframe_input_enter( 	Signal signal ){ forwardSignal( signal ); }




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
		ErrorHandler* errorHandler_;

	} *statePtr;

	struct HEAD : public State {

	};

	struct IDLE : public State{
		virtual void sensor_switch_is_closed( 	Signal signal ) override {}
		virtual void sensor_switch_is_open( 	Signal signal ) override {}
		virtual void lb_input_freed(			Signal signal ) override {}
		virtual void lb_height_interrupted( 	Signal signal ) override {}
		virtual void lb_height_freed( 			Signal signal ) override {}
		virtual void lb_switch_interrupted( 	Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_SWITCH_FREED)); }
		virtual void lb_switch_freed( 			Signal signal ) override {}
		virtual void lb_slide_interrupted( 		Signal signal ) override {}
		virtual void lb_slide_freed( 			Signal signal ) override {}
		virtual void lb_output_interrupted( 	Signal signal ) override { addPendingError(errorHandler_, Signal(Signalname::LB_OUTPUT_FREED)); }
		virtual void lb_output_freed( 			Signal signal ) override {}
		virtual void conveyer_belt_ready( 		Signal signal ) override {}
		virtual void timeframe_input_enter( 	Signal signal ) override {}


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
				new (this) DepartureOutputPreviousCB;
			}
		}
	};

	struct DepartureOutputPreviousCB : public State {
		DepartureOutputPreviousCB() {
			cout<<"DepartureOutputPreviousCB"<<endl;
			Item::startMotor(hal_);
			send_CB_busy(hal_);

			//copy item from hal
			copyItemFromHAL(hal_, item_);
		}

		virtual void timeframe_input_enter( Signal signal ) override {
			cout<<"timeframe_input_enter"<<endl;
			new (this) WaitForArrivalAtInput;
		}


	};

	struct WaitForArrivalAtInput : public State {
		WaitForArrivalAtInput() {
			cout<<"WaitForArrivalAtInput"<<endl;
			// start timers height
		}

		virtual void lb_input_interrupted( Signal signal ) override {
			new (this) ArrivalInput;
		}


	};

	struct ArrivalInput : public State {
		ArrivalInput() {
			cout<<"ArrivalInput"<<endl;
			Item::setID(&item_->id);
			items_on_cb = items_on_cb + 1;
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
		}
		virtual void lb_switch_interrupted( Signal signal ) override {
			cout<<"lb_switch_interrupted"<<endl;
			new (this) ArrivalSwitch;
		}
	};

	struct WaitForArrivalAtHeight : public State{

	};

	struct ArrivalHeight : public State{

	};

	struct DepatureHeight : public State{

	};

	struct WaitForArrivalAtSwitch : public State{

	};

	struct ArrivalSwitch : public State{
		ArrivalSwitch() {
			//get values from type identification
			item_->type = TypeIdentification::typeScans.front();
			TypeIdentification::typeScans.erase(TypeIdentification::typeScans.begin());
			amIwanted = Sorting::amIWanted(item_);
			if(amIwanted) {
				Item::openSwitchPoint(hal_);
			}
		}

		virtual void lb_switch_freed( Signal signal ) override {
			cout<<"lb_switch_freed"<<endl;
			Item::closeSwitchPoint(800, hal_);
			if(amIwanted) {
				new (this) DepatureSwitchToOutput;
			} else {
				new (this) DepatureSwitchToSlide;
			}
		}

	};

	struct DepatureSwitchToOutput : public State {
		DepatureSwitchToOutput() {
			cout<<"DepatureSwitchToOutput"<<endl;
		}

		virtual void lb_output_interrupted( Signal signal ) override {
			cout<<"lb_output_interrupted"<<endl;
			new (this) ArrivalOutput;
		}
	};

	struct DepatureSwitchToSlide : public State {
		DepatureSwitchToSlide() {
			cout<<"DepatureSwitchToSlide"<<endl;
		}
		virtual void lb_slide_interrupted( Signal signal ) override {
			cout<<"lb_slide_interrupted"<<endl;
			new (this) ArrivalSlide;
		}
	};

	struct WaitForArrivalAtSlide : public State{

	};

	struct ArrivalSlide : public State {
		ArrivalSlide() {
			cout<<"ArrivalSlide"<<endl;
			Item::dequeueAndDeleteItem(item_);
			Item::ArrivalSlideAction(hal_);
			Item::printItem(hal_, item_);
		}
	};

	struct DepatureSlide : public State{

	};

	struct SlideIsFull : public State{

	};

	struct WaitForArrivalAtOuput : public State{

	};

	struct ArrivalOutput : public State{
		ArrivalOutput(){
			cout<<"ArrivalOutput"<<endl;
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
					next_cb_busy.parameterList.showParameters();
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
		}
	};

	IDLE stateMember;

};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
