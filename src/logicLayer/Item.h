
/*
 * Item.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "Channel.h"
#include "SignalReceiver.h"

namespace hardwareLayer {
	class HardwareLayer;
}

namespace logicLayer {

class Item {
public:
	Item( hardwareLayer::HardwareLayer* , Channel<Signal>* );
	virtual ~Item();

	void handle( Signal );

	int heightAbsolute;
	int heightCenter;

private:

	Item* next_;
	Item* previous_;

	hardwareLayer::HardwareLayer* hal_;
	Channel<Signal>* timerChannel_;

	struct State {//top-level state
			virtual void lbInputInt(){}
			virtual void itemArrived(){}
			virtual void lbSwitchInt(){}
			virtual void lbOutputInt(){}

			Item* item_;
			hardwareLayer::HardwareLayer* hal_;

		} *statePtr;

		struct Init : public State{

			virtual void lbInputInt(){
				if( cb_this == cb_first ){
					new (this) ArrivalInput;
				}
			}

			virtual void itemArrived(){
				if( cb_this == cb_last ){
					new (this) ArrivalInput;
				}
			}
		};


		struct ArrivalInput : public State{

			ArrivalInput(){
				//entry action
				hal_->motorFast();
				hal_->motorRotateClockwise();
				hal_->motorStart();
			}

			virtual void lbSwitchInt(){
				new (this) ArrivalSwitch;
			}

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
				hal_->switchPointOpen();
			}

			virtual void lbOutputInt(){
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

				hal_->switchPointClose();

				if(cb_this == cb_last){
					hal_->motorStop();
				}

				if(cb_this == cb_first){
					hal_->sendItemViaSerial(*item_);
				}
			}
		};

		struct DepatureAtOutput : public State{

		};

		struct WaitForArrivalInput : public State{

		};

		Init stateMember;
};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
