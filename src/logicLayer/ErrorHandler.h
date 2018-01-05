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
		void errorOccurred() { new (this) ERROR; }
		virtual void isPending(Signal signal){}
		virtual void button_reset_pushed(){ errorHandler->printErrors(); }
		virtual void button_start_pushed(){}

		hardwareLayer::HardwareLayer* hal;
		ErrorHandler* errorHandler;
		std::set<Signal>* pendingSignals;
	} *statePtr;


	struct IDLE : public State {
	};

	struct NO_ERROR : public State {
		NO_ERROR() {
			hal->motorLock(false);
			hal->greenLightLock(false);
			hal->redLightOff();
			if(this_cb_busy == false && cb_this == cb_sorting_2) {
				hal->sendSerial(Signal(cb_this,cb_previous,Signalname::CONVEYOR_BELT_READY));
			}
		}
	};

	struct ERROR : public State {
		ERROR() {
			hal->motorLock(true);
			hal->greenLightLock(true);
			if ((*pendingSignals->begin()).name != Signalname::BUTTON_START_PUSHED) {
				hal->blinkRed(Speed::slow);
			} else {
				hal->blinkRed(Speed::fast);
			}
			errorHandler->printErrors();
		}

		virtual void button_reset_pushed() override {
			errorHandler->printErrors();
			hal->redLightOn();
		}

		virtual void button_start_pushed() override {
			pendingSignals->erase(Signal(Signalname::BUTTON_START_PUSHED));
			if(pendingSignals->empty()) {
				new (this) NO_ERROR;
			}
		}

		virtual void isPending(Signal signal) override {
			pendingSignals->erase(signal);

			if(pendingSignals->empty()) {
				if(signal.name == Signalname::BUTTON_E_STOP_PULLED){
					new (this) NO_ERROR;
				}
				else if(signal.name == Signalname::LB_OUTPUT_FREED && cb_this == cb_last){
					new (this) NO_ERROR;
				}
				else {
					new (this) WaitForStart;
				}

			}
		}
	};

	struct WaitForStart : public State {
		WaitForStart() {
		}

		virtual void button_start_pushed() override {
			new (this) NO_ERROR;
		}
	};

	IDLE memberState;

public:
	ErrorHandler(hardwareLayer::HardwareLayer&);
	~ErrorHandler();
	void addPending(Signal);
	void handle(Signal);
private:
	void broadcastEStopStatus();
	void printErrors();

	hardwareLayer::HardwareLayer& hal;
	std::set<Signal> pendingSignals;
};

} /* namespace logicLayer */
#endif /* ERRORHANDLER_H_ */
