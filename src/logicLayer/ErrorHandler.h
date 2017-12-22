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
		int* eStopCounter;
	} *statePtr;


	struct IDLE : public State {
		IDLE() {
		}
		virtual void errorOccurred() {
			new (this) ERROR;
		}
	};

	struct NO_ERROR : public State {
		NO_ERROR() {
			cout<<"NO_ERROR"<<endl;
			hal->motorLock(false);
			hal->redLightOff();
			hal->blinkGreen(Speed::slow);
			if(this_cb_busy == false && cb_this == cb_sorting_2) {
				cout<<"send CONVEYOR_BELT_READY"<<endl;
				hal->sendSerial(Signal(cb_this,cb_previous,Signalname::CONVEYOR_BELT_READY));
			}
		}
		virtual void errorOccurred() {
			new (this) ERROR;
		}

	};

	struct ERROR : public State {
		ERROR() {
			cout<<"ERROR"<<endl;
			hal->motorLock(true);
			hal->greenLightOff();
			hal->blinkRed(Speed::fast);
		}
		virtual void isPending(Signal signal) {
			pendingSignals->erase(signal);

			for(auto &pendingSignal : *pendingSignals) {
				cout<<"pending signal:"<<endl;
				cout<<"sender: "<<(int)pendingSignal.sender<<endl;
				cout<<"receiver: "<<(int)pendingSignal.receiver<<endl;
				cout<<"name: "<<(int)pendingSignal.name<<endl<<endl;

			}
			if(pendingSignals->empty()) {
				new (this) NO_ERROR;
			}
		}
	};

	IDLE memberState;

public:
	ErrorHandler(hardwareLayer::HardwareLayer&);
	~ErrorHandler();
	void addPending(Signal);
	void handle(Signal);
private:
	hardwareLayer::HardwareLayer& hal;
	std::set<Signal> pendingSignals;

	void broadcastEStopStatus();
};

} /* namespace logicLayer */
#endif /* ERRORHANDLER_H_ */
