
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

namespace hardwareLayer {
	class HardwareLayer;
}

namespace logicLayer {

class Item {
public:
	Item( hardwareLayer::HardwareLayer* , Channel<Signal>*, Item* );
	Item( hardwareLayer::HardwareLayer* , Channel<Signal>*, bool head );
	virtual ~Item();

	void handle( Signal );

	static void startMotor(hardwareLayer::HardwareLayer* );
	static void openSwitchPoint(hardwareLayer::HardwareLayer* );
	static void onOutputAction(hardwareLayer::HardwareLayer* , Item*);
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

	struct State {//top-level state
			virtual void lbInputInt( Signal signal ){ forwardSignal( signal ); }
			virtual void itemArrived( Signal signal ){ forwardSignal( signal ); }
			virtual void lbSwitchInt( Signal signal ){ forwardSignal( signal ); }
			virtual void lbOutputInt( Signal signal ){ forwardSignal( signal ); }
			virtual void lbOutputFreed( Signal signal ){ forwardSignal( signal ); }


			void createItem(){
				cout << "create item" << endl;
				item_->previous_ = new Item(item_->hal_, item_->timerChannel_, item_);
			}

			void forwardSignal( Signal signal ){
				cout<<"forwardSignal"<<endl;
				if(item_->previous_ == nullptr ){
					cout<<"nullptr"<<endl;
					createItem();
				}
				cout<<"before handle"<<endl;
				item_->previous_->handle( signal );
				cout<<"after handle"<<endl;

			}

			Item* item_;
			hardwareLayer::HardwareLayer* hal_;

		} *statePtr;

		struct Init : public State{

		};


		struct ArrivalInput : public State{

			virtual void lbInputInt( Signal signal )override {
				cout<<"InputInt"<<endl;
				items_on_cb = items_on_cb + 1;
				if( cb_this == cb_first ) {
					Item::startMotor(hal_);
				}
				cout<<"end InputInt"<<endl;

			}

			virtual void itemArrived( Signal signal )override {
				if( cb_this == cb_last ){
					Item::startMotor(hal_);
				}
			}

			virtual void lbOutputInt( Signal signal ) override {

			}
			virtual void lbSwitchInt( Signal signal )override {
				new (this) ArrivalSwitch;
			}
			virtual void lbOutputFreed( Signal signal )override {}

	};

		struct TransportToHeight : public State{

		};

		struct WaitForArrivalAtHeight : public State{

		};

		struct ArrivalAtHeight : public State{

		};

		struct DepatureAtHeight : public State{

		};

		struct WaitForArrivalAtSwitch : public State{

		};

		struct ArrivalSwitch : public State{

			//entry actions
			ArrivalSwitch(){
				Item::openSwitchPoint(hal_);
			}

			virtual void lbOutputInt( Signal signal )override {
				new (this) ArrivalOutput;
			}

		};

		struct DepatureAtSwitchToOutput : public State{

		};

		struct DepatureAtSwitchToSlide : public State{

		};

		struct WaitForArrivalAtSlide : public State{

		};

		struct ArrivalAtSlide : public State{

		};

		struct DepatureAtSlide : public State{

		};

		struct SlideIsFull : public State{

		};

		struct WaitForArrivalAtOuput : public State{

		};

		struct ArrivalOutput : public State{
			ArrivalOutput(){
				Item::onOutputAction(hal_, item_);
			}

			virtual void lbOutputFreed( Signal signal )override {
				new (this) DepatureAtOutput;
			}
		};

		struct DepatureAtOutput : public State{
			DepatureAtOutput(){
				item_->next_->setPrevious(item_->previous_);
				item_->previous_->setNext(item_->next_);
				if (items_on_cb > 0) {
					items_on_cb = items_on_cb - 1;
				} else {
					LOG_WARNING<<"items_on_cb was zero or negative."<<endl;
				}
				Item::DepatureAtOutputAction(hal_);
			}

		};

		struct WaitForArrivalInput : public State{

		};

		ArrivalInput stateMember;
};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
