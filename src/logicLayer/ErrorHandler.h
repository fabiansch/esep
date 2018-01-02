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
		virtual void button_reset_pushed(){}
		virtual void button_start_pushed(){}

		hardwareLayer::HardwareLayer* hal;
		ErrorHandler* errorHandler;
		std::set<Signal>* pendingSignals;
	} *statePtr;


	struct IDLE : public State {
	};

	struct NO_ERROR : public State {
		NO_ERROR() {
			cout<<"NO_ERROR"<<endl;
			hal->motorLock(false);
			hal->greenLightLock(false);
			hal->redLightOff();
			if(this_cb_busy == false && cb_this == cb_sorting_2) {
				cout<<"send CONVEYOR_BELT_READY"<<endl;
				hal->sendSerial(Signal(cb_this,cb_previous,Signalname::CONVEYOR_BELT_READY));
			}
		}
	};

	struct ERROR : public State {
		ERROR() {
			errorHandler->printErrors();
			hal->motorLock(true);
			hal->greenLightLock(true);
			if ((*pendingSignals->begin()).name != Signalname::BUTTON_RESET_PUSHED) {
				hal->blinkRed(Speed::slow);
			} else {
				hal->blinkRed(Speed::fast);
			}
		}

		virtual void button_reset_pushed() override {
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

			for(auto &pendingSignal : *pendingSignals) {
				cout<<"pending signal:"<<endl;
				cout<<"sender: "<<(int)pendingSignal.sender<<endl;
				cout<<"receiver: "<<(int)pendingSignal.receiver<<endl;
				cout<<"name: "<<(int)pendingSignal.name<<endl<<endl;

			}

			if(pendingSignals->empty()) {
				if(signal.name == Signalname::BUTTON_RESET_PUSHED) {
					new (this) NO_ERROR;
				} else {
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
