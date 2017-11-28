/*
 * ErrorHandler.h
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include "Header.h"
#include "SignalReceiver.h"
#include "HardwareLayer.h"
#include <set>

namespace logicLayer {

class ErrorHandler {
private:
	struct State {
		virtual void isPending(Signal signal){}
		virtual void errorOccurred(){}

		hardwareLayer::HardwareLayer* hal;
		std::set<Signal>* pendingSignals;
	} *statePtr;

	struct NO_ERROR : public State {
		virtual void errorOccurred(){
			hal->motorStop();
			hal->greenLightOff();
			hal->blinkRed(Speed::fast);
			new (this) ERROR;
		}

	};

	struct ERROR : public State {
		virtual void isPending(Signal signal) {
			pendingSignals->erase(pendingSignals->find(signal));

			if(pendingSignals->empty()) {
				hal->motorStart();
				hal->redLightOff();
				hal->blinkGreen(Speed::slow);
				new (this) NO_ERROR;
			}
		}
	};

	NO_ERROR memberState;

public:
	ErrorHandler(hardwareLayer::HardwareLayer&);
	~ErrorHandler();
	void addPending(Signal);
	void handle(Signal);
private:
	hardwareLayer::HardwareLayer& hal;
	std::set<Signal> pendingSignals;
};

} /* namespace logicLayer */
#endif /* ERRORHANDLER_H_ */
