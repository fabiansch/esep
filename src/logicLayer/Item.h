
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
//#include "ErrorHandler.h"

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
	static void openSwitchPoint(hardwareLayer::HardwareLayer* );
	static void closeSwitchPoint(int milliseconds,hardwareLayer::HardwareLayer*);
	static void onOutputAction(hardwareLayer::HardwareLayer* , Item*, ErrorHandler*);
	static void DepatureAtOutputAction(hardwareLayer::HardwareLayer*);


	void setNext(Item*);
	void setPrevious(Item*);

	int heightAbsolute;
	int heightCenter;

private:

	Item* next_;
	Item* previous_;

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
		virtual void item_arrived( 				Signal signal ){ forwardSignal( signal ); }



		void createItem(){
			cout << "create item" << endl;
			item_->previous_ = new Item(item_->hal_, item_->timerChannel_, item_, errorHandler_);
		}

		void forwardSignal( Signal signal ){
			cout<<"forwardSignal"<<endl;
			if(item_->previous_ == nullptr ){
				createItem();
			}
			item_->previous_->handle( signal );
		}

		Item* item_;
		hardwareLayer::HardwareLayer* hal_;
		ErrorHandler* errorHandler_;

	} *statePtr;

	struct Init : public State{

	};


	struct ArrivalInput : public State{
		virtual void sensor_switch_is_closed( 	Signal signal ) override {}
		virtual void sensor_switch_is_open( 	Signal signal ) override {}
		virtual void lb_height_interrupted( 	Signal signal ) override {}
		virtual void lb_height_freed( 			Signal signal ) override {}
		virtual void lb_switch_interrupted( 	Signal signal ) override {}
		virtual void lb_switch_freed( 			Signal signal ) override {}
		virtual void lb_slide_interrupted( 		Signal signal ) override {}
		virtual void lb_slide_freed( 			Signal signal ) override {}
		virtual void lb_output_interrupted( 	Signal signal ) override {}
		virtual void lb_output_freed( 			Signal signal ) override {}

		virtual void lb_input_interrupted( Signal signal ) override {
			cout<<"lb_input_interrupted"<<endl;
			items_on_cb = items_on_cb + 1;
			if( cb_this == cb_first ) {
				Item::startMotor(hal_);
			}

		}

		virtual void item_arrived( Signal signal ) override {
			cout<<"item_arrived"<<endl;
			Item::startMotor(hal_);
		}

		virtual void lb_input_freed( Signal signal ) override {
			new (this) DepartureInput;
		}

	};

	struct DepartureInput : public State {
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
		ArrivalSwitch(){
			Item::openSwitchPoint(hal_);
		}

		virtual void lb_switch_freed( Signal signal ) override {
			cout<<"lb_switch_freed"<<endl;
			Item::closeSwitchPoint(500, hal_);
		}

		virtual void lb_output_interrupted( Signal signal ) override {
			cout<<"lb_output_interrupted"<<endl;
			new (this) ArrivalOutput;
		}

	};

	struct DepatureSwitchToOutput : public State {

	};

	struct DepatureSwitchToSlide : public State{

	};

	struct WaitForArrivalAtSlide : public State{

	};

	struct ArrivalSlide : public State{

	};

	struct DepatureSlide : public State{

	};

	struct SlideIsFull : public State{

	};

	struct WaitForArrivalAtOuput : public State{

	};

	struct ArrivalOutput : public State{
		ArrivalOutput(){
			Item::onOutputAction(hal_, item_, errorHandler_);
		}

		virtual void lb_output_freed( Signal signal ) override {
			cout<<"lb_output_freed"<<endl;
			new (this) DepatureOutput;
		}

	};

	struct DepatureOutput : public State{
		DepatureOutput(){
			cout<<"setPrevious"<<endl;
			item_->next_->setPrevious(item_->previous_);
			cout<<"setNext"<<endl;
			if(item_->previous_) {
				item_->previous_->setNext(item_->next_);
			}
			if (items_on_cb > 0) {
				items_on_cb = items_on_cb - 1;
			} else {
				LOG_WARNING<<"items_on_cb was zero or negative."<<endl;
			}
			cout<<"items on CB "<<items_on_cb<<endl;

			Item::DepatureAtOutputAction(hal_);
			delete item_;
		}

	};

	struct WaitForArrivalInput : public State{

	};

	ArrivalInput stateMember;
};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
