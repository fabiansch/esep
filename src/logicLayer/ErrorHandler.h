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
		NO_ERROR() {
			hal->redLightOff();
			hal->blinkGreen(Speed::slow);
		}
		virtual void errorOccurred(){
			new (this) ERROR;
		}

	};

	struct ERROR : public State {
		ERROR() {
			hal->greenLightOff();
			hal->blinkRed(Speed::fast);
		}
		virtual void isPending(Signal signal) {
			pendingSignals->erase(pendingSignals->find(signal));

			if(pendingSignals->empty()) {
				new (this) NO_ERROR;
			}
		}
	};

	NO_ERROR memberState;

public:
	ErrorHandler(hardwareLayer::HardwareLayer&);
	virtual ~ErrorHandler();
	void setPending(Signal);
private:
	hardwareLayer::HardwareLayer& hal;
	std::set<Signal> pendingSignals;
};

} /* namespace logicLayer */
#endif /* ERRORHANDLER_H_ */
