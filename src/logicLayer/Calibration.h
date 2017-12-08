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
				hal->blinkGreen(Speed::slow);
				hal->blinkYellow(Speed::slow);
				hal->blinkRed(Speed::slow);
			//	height_item = hal->getHeight();
			}
			virtual void lb_input_interrupted() override {
				new (this) ArrivalAtInput;
			}
			virtual void item_arrived(){
				new (this) ItemArrived;
			}
		};

		//============================ ITEM ARRIVED =======================================
		struct ItemArrived: public State {
			ItemArrived() {
				*timeFrameStart = steady_clock::now();
				hal->motorFast();
				hal->motorRotateClockwise();
				hal->motorStart();
			}
			virtual void lb_input_interrupted(){
				//	set value in parameter pattern
				//time_output_to_input = duration_cast<milliseconds>(steady_clock::now()-timeFrameStart).count(); //do this in next state
				new (this) ArrivalAtInput;
			}
		};


		//============================ ARRIVAL AT INPUT =======================================
		struct ArrivalAtInput: public State {
			ArrivalAtInput(){
				if (cb_this != cb_first){
					//time_output_to_input = duration_cast<milliseconds>(steady_clock::now()-timeFrameStart).count();
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
				cout<<"======ArrivalAtHeight ====="<<endl;
				//TODO	FUNKTIONIERT AB HIER NICHT. NOCHMAL DIE ZEITERFASSUNG CHECKEN
				std::chrono::duration<float> durationfs = steady_clock::now() - *timeFrameStart;
				std::chrono::milliseconds durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(durationfs);
				unsigned int time = durationMS.count();
				std::cout << time << "s\n"<<endl;
				time_input_to_height = time;
				std::cout << time_input_to_height << "s\n"<<endl;




//				typedef std::chrono::high_resolution_clock Time;
//				typedef std::chrono::milliseconds ms;
//				typedef std::chrono::duration<float> fsec;
//
//				*timeFrameStop = steady_clock::now();
//
//
//				fsec fs = *timeFrameStop - *timeFrameStart;
//
//
//				ms d = std::chrono::duration_cast<ms>(fs);
//
//				std::cout << fs.count() << "s\n"<<endl;
//				std::cout << d.count() << "ms\n"<<endl;



//				timeFrameStop = steady_clock::now();
//				std::chrono::duration<float> duration = timeFrameStop - timeFrameStart;
//				std::chrono::milliseconds ms =  std::chrono::duration_cast<std::chrono::milliseconds>(duration);
//				unsigned int count = ms.count();
//				cout<<count<<"unsignedint"<<endl;
//				cout << ms.count() << "ms\n"<<endl;


//				auto time = duration_cast < std::chrono::milliseconds > (timeFrameStop - timeFrameStart);
//
//				auto timeAuto =  time.count();
//				int timeInt = (int) time.count();
//				unsigned int timeUnsignedInt = (unsigned int) time.count();
//
//				//cout<<"time"<<time<<endl;
//				cout<<"TIME auto "<<timeAuto<<endl;
//				cout<<"TIME unsigned int "<<timeInt<<endl;
//				cout<<"TIME int"<<timeUnsignedInt<<endl;

			//	time_input_to_height = duration_cast<milliseconds>(steady_clock::now()- timeFrameStart).count();
				//time_input_to_height.parameterList.showParameters();
				hal->motorStop();



			}
		};

		//============================ DEPARTURE HEIGHT =======================================
		struct DepartureHeight: public State {
			DepartureHeight(){
			}
			virtual void lb_switch_interrupted(){
				new (this) ArrivalSwitch;
			}
		};

		//============================ ARRIVAL SWITCH=======================================
		struct ArrivalSwitch: public State {
			ArrivalSwitch(){
			}
		};

		//============================ DEPARTURE SWITCH TO OUTPUT=======================================
		struct SwitchToOutput: public State {
			SwitchToOutput(){
			}
		};

		//============================ ARRIVAL AT OUTPUT =======================================
		struct ArrivalOutput: public State {
			ArrivalOutput(){
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
