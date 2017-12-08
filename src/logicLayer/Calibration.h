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
			virtual void lb_input_freed(){				}
			virtual void lb_height_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_height_freed(){				}
			virtual void lb_switch_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_switch_freed(){				}
			virtual void lb_slide_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_slide_freed(){				}
			virtual void lb_output_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_output_freed(){				}
			virtual void item_arrived(){				calibrationFailed(__FUNCTION__);}

			void calibrationFailed(string functionname) {
				cout<<"Calibration failed: "<<name()<<", caused of "<<functionname<<endl;
				new (this) FAIL_STATE;
			}

			std::string name() const { return typeid(*this).name(); }

			hardwareLayer::HardwareLayer* hal;
			Item* testItem;
			steady_clock::time_point* timeFrameStart;
			steady_clock::time_point* timeFrameStop;
			steady_clock::time_point* totalTimeStart;

		} *statePtr;



		// ============================= FAIL STATE =========================================
		struct FAIL_STATE : public State {
			virtual void calibration_start(){
				new (this) WaitingForItem;
			}
		};

		// ============================= IDLE =========================================
		struct IDLE: public State {
			virtual void calibration_start(){
				new (this) WaitingForItem;
			}
		};


		//============================ WAITING FOR ITEM =======================================
		struct WaitingForItem: public State {
			WaitingForItem() {
				cout<<"=============== ARRIVAL AT INPUT =================="<<endl;
				hal->blinkGreen(Speed::slow);
				hal->blinkYellow(Speed::slow);
				hal->blinkRed(Speed::slow);
				height_conveyor_belt = hal->getHeight();
				cout<<"height_conveyor_belt: "<<height_conveyor_belt<<endl;
			}
			virtual void lb_input_interrupted(){
				new (this) ArrivalAtInput;
			}
			virtual void item_arrived(){
				new (this) ItemArrived;
			}
		};

		//============================ ITEM ARRIVED =======================================
		struct ItemArrived: public State {
			ItemArrived() {
				cout<<"=============== ItemArrived =================="<<endl;
				*timeFrameStart = steady_clock::now();
				hal->motorFast();
				hal->motorRotateClockwise();
				hal->motorStart();
			}
			virtual void lb_input_interrupted(){
				new (this) ArrivalAtInput;
			}
		};


		//============================ ARRIVAL AT INPUT =======================================
		struct ArrivalAtInput: public State {
			ArrivalAtInput(){
				cout<<"=============== ArrivalAtInput =================="<<endl;
				if (cb_this != cb_first){
					std::chrono::duration<float> durationfs = steady_clock::now() - *timeFrameStart;
					std::chrono::milliseconds durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(durationfs);
					unsigned int time = durationMS.count();
					time_output_to_input = time;
					cout<<"time_output_to_input: "<<time_output_to_input<<endl;
				}
				hal->motorRotateClockwise();
				hal->motorFast();
				hal->motorStart();
			}
			virtual void lb_input_freed(){
				new (this) DepartureInput;
			}
		};

		//============================ DEPARTURE INPUT =======================================
		struct DepartureInput: public State {
			DepartureInput(){
				cout<<"=============== DepartureInput =================="<<endl;
				*timeFrameStart = steady_clock::now();
				*totalTimeStart = *timeFrameStart;
			}
			virtual void lb_height_interrupted(){
				new (this) ArrivalAtHeight;
			}
		};

		//============================ ARRIVAL AT HIGHT =======================================
		struct ArrivalAtHeight: public State {
			ArrivalAtHeight(){
				cout<<"=============== ArrivalAtHeight =================="<<endl;
				std::chrono::duration<float> durationfs = steady_clock::now() - *timeFrameStart;
				std::chrono::milliseconds durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(durationfs);
				unsigned int time = durationMS.count();
				time_input_to_height = time;
				cout<<"time_input_to_height: "<<time_input_to_height<<endl;

				*timeFrameStart =  steady_clock::now();

				height_item = hal->getHeight();
				cout<<"height_item: "<<height_item<<endl;
			}
			virtual void lb_switch_interrupted(){
				new (this) ArrivalSwitch;
			}
		};

		//============================ ARRIVAL SWITCH=======================================
		struct ArrivalSwitch: public State {
			ArrivalSwitch(){
				cout<<"=============== ArrivalSwitch =================="<<endl;
				std::chrono::duration<float> durationfs = steady_clock::now() - *timeFrameStart;
				std::chrono::milliseconds durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(durationfs);
				unsigned int time = durationMS.count();
				time_height_to_switch = time;
				cout<<"time_height_to_switch: "<<time_height_to_switch<<endl;

				*timeFrameStart =  steady_clock::now();

				hal->switchPointOpen();
			}
			virtual void lb_output_interrupted(){
				new (this) ArrivalOutput;
			}
		};

		//============================ ARRIVAL AT OUTPUT =======================================
		struct ArrivalOutput: public State {
			ArrivalOutput(){
				cout<<"=============== ArrivalOutput =================="<<endl;
				std::chrono::duration<float> durationfs = steady_clock::now() - *timeFrameStart;
				std::chrono::milliseconds durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(durationfs);
				unsigned int time = durationMS.count();
				time_switch_to_output = time;
				cout<<"time_switch_to_output: "<<time_switch_to_output<<endl;

				if (cb_this == cb_last){
					hal->motorStop();
				}
				if (cb_this != cb_last){
					hal->sendSerial(Signal(cb_this, cb_next, Signalname::SERIAL_TRANSFER_ITEM));
				}
			}
		};


		IDLE stateMember;
		steady_clock::time_point timeFrameStart = steady_clock::now();
		steady_clock::time_point timeFrameStop = steady_clock::now();
		steady_clock::time_point totalTimeStart= steady_clock::now();

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
