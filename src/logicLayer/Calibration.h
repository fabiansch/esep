/*
 * Calibration.h
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "HardwareLayer.h"
#include "Header.h"
#include "Item.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "Channel.h"

using namespace std::chrono;
namespace logicLayer {

class Calibration {
private:
	struct State {//top-level state
			virtual void calibration_start(){			calibrationFailed(__FUNCTION__);}
			virtual void stop() {
				LOG_TEST<<__FUNCTION__<<endl;
				LOG_TEST<<"CALIBRATION ABORTED BY MAIN MENU."<<endl;
				cout<<"CALIBRATION ABORTED BY MAIN MENU."<<endl;
				calibrationFailed(__FUNCTION__);
			}
			virtual void calibration_successful(uint8_t sender){	calibrationFailed(__FUNCTION__);}
			virtual void lb_input_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_input_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void lb_height_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_height_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void lb_switch_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_switch_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void lb_slide_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_slide_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void lb_output_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_output_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void item_arrived(){				calibrationFailed(__FUNCTION__);}

			void calibrationFailed(string functionname) {
				cout<<"Calibration failed: "<<name()<<", caused of "<<functionname<<endl;
				new (this) FAIL_STATE;
			}

			std::string name() const { return typeid(*this).name(); }

			hardwareLayer::HardwareLayer* hal;
			Item* testItem;
			steady_clock::time_point firstTime;
			steady_clock::time_point secondTime;
			steady_clock::time_point startTime;



		} *statePtr;

		// ============================= FAIL STATE =========================================
		struct FAIL_STATE : public State {
			virtual void calibration(){
				new (this) WaitingForItem;
			}
		};

		struct IDLE: public State {
			virtual void calibration(){
				new (this) WaitingForItem;
			}
		};


		//============================ START_STATE =======================================
		struct WaitingForItem: public State {
			WaitingForItem() {
				hal->blinkGreen(Speed::slow);
				hal->blinkYellow(Speed::slow);
				hal->blinkRed(Speed::slow);
			}
			virtual void lb_input_INT(){
				new (this) ArrivalAtInput;
			}
		};

		struct ArrivalAtInput: public State {
			ArrivalAtInput(){
				hal->motorRotateClockwise();
				hal->motorFast();
			}
			virtual void lb_input_freed(){
				new (this) DepartureInput;
			}
		};

		struct DepartureInput: public State {
			DepartureInput(){
				firstTime = steady_clock::now();
				startTime = firstTime;
			}
			virtual void lb_height_int(){
				new (this) ArrivalAtHeight;
			}
		};

		struct ArrivalAtHeight: public State {
			ArrivalAtHeight(){
				durationInputToHeight = duration_cast <milliseconds> (firstTime - secondTime).count();
			}
		};


		IDLE stateMember;


		hardwareLayer::HardwareLayer& hal;
		milliseconds timeStart;

		milliseconds getSystemTime() {
			return duration_cast< milliseconds >(system_clock::now().time_since_epoch());
		}


public:
	Calibration(hardwareLayer::HardwareLayer&);
	virtual ~Calibration();
	void handle(Signal signal);
	std::string nameOf(State *state) const { return typeid(*state).name(); }
};

} /* namespace logicLayer */
#endif /* CALIBRATION_H_ */
