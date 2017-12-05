
/*
 * Item.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "SignalReceiver.h"

namespace logicLayer {

class Item {
public:
	Item();
	Item( Item*, Item* );
	virtual ~Item();

	void handle( Signal );

	int heightAbsolute;
	int heightCenter;

private:

	Item* next_;
	Item* previous_;

	struct State {//top-level state
		virtual void lbInputFreed(){}
		virtual void tfHeightIn(){}
		virtual void heightMatch(){}
		virtual void heightNotMatch(){}
		virtual void lbHeightInt(){}
		virtual void tfHeightOut(){}
		virtual void lbHeightFreed(){}
		virtual void tfSwitchIn(){}
		virtual void metalMatch(){}
		virtual void metalNotMatch(){}
		virtual void lbSwitchInt(){}
		virtual void tfSwitchOut(){}
		virtual void lbSwitchFreed(){}
		virtual void tfOutputIn(){}
		virtual void lbOutputInt(){}
		virtual void lbOutputFreed(){}
		virtual void tfInputIn(){}
		virtual void lbInputInt(){}
		virtual void tfSlideIn(){}
		virtual void lbSlideInt(){}
		virtual void tfSlideOut(){}
		virtual void lbSlideFreed(){}
	} *statePtr;

	struct ArrivalInput : public State{

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

	struct ArrivalAtSwitch : public State{

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

	struct ArrivalAtOutput : public State{

	};

	struct DepatureAtOutput : public State{

	};

	struct WaitForArrivalInput : public State{

	};

	ArrivalInput stateMember;
};

} /* namespace logicLayer */
#endif /* ITEM_H_ */
