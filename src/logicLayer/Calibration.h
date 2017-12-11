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
	struct State { //top-level state
		virtual void calibration_start() { calibrationFailed(__FUNCTION__);}
		virtual void stop() {
			LOG_TEST << __FUNCTION__ << endl;
			LOG_TEST << "CALIBRATION ABORTED BY MAIN MENU." << endl;
			cout << "CALIBRATION ABORTED BY MAIN MENU." << endl;
			calibrationFailed(__FUNCTION__);
		}
		virtual void calibration_successful(uint8_t sender) {		calibrationFailed(__FUNCTION__);}
		virtual void lb_input_interrupted() {	calibrationFailed(__FUNCTION__);}
		virtual void lb_input_freed() {}
		virtual void lb_height_interrupted() {	calibrationFailed(__FUNCTION__);}
		virtual void lb_height_freed() {}
		virtual void lb_switch_interrupted() {	calibrationFailed(__FUNCTION__);}
		virtual void lb_switch_freed() {}
		virtual void lb_slide_interrupted() {	calibrationFailed(__FUNCTION__);}
		virtual void lb_slide_freed() {}
		virtual void lb_output_interrupted() {	calibrationFailed(__FUNCTION__);}
		virtual void lb_output_freed() {}
		virtual void item_arrived() {			calibrationFailed(__FUNCTION__);}

		void calibrationFailed(string functionname) {
			cout << "Calibration failed: " << name() << ", caused of "<< functionname << endl;
			new (this) FAIL_STATE;
		}

		std::string name() const { return typeid(*this).name();}

				hardwareLayer::HardwareLayer* hal;
				Item* testItem;
				steady_clock::time_point* timeFrameStart;
				steady_clock::time_point* timeFrameStop;
				steady_clock::time_point* totalTimeStart;

			}*statePtr;

			// ============================= FAIL STATE =========================================
			struct FAIL_STATE: public State {
				FAIL_STATE() {
					hal->motorStop();
					hal->blinkRed(Speed::slow);
					hal->greenLightOff();
					hal->yellowLightOff();
				}
				virtual void calibration_start() {
					new (this) WaitingForItem;
				}
			};

			// ============================= IDLE =========================================
			struct IDLE: public State {
				virtual void calibration_start() {
					new (this) WaitingForItem;
				}
			};

			//============================ WAITING FOR ITEM =======================================
			struct WaitingForItem: public State {
				WaitingForItem() {
					cout<< "=============== WAITING FOR ITEM =================="<< endl;
					hal->blinkGreen(Speed::slow);
					hal->blinkYellow(Speed::slow);
					hal->blinkRed(Speed::slow);
					height_conveyor_belt = hal->getHeight();
					cout << "height_conveyor_belt: " << height_conveyor_belt << endl;
				}
				virtual void lb_input_interrupted() {
					new (this) ArrivalAtInput;
				}
				virtual void item_arrived() {
					new (this) ItemArrived;
				}
			};

			//============================ ITEM ARRIVED =======================================
			struct ItemArrived: public State {
				ItemArrived() {
					cout << "=============== ItemArrived =================="<< endl;
					*timeFrameStart = steady_clock::now();
					hal->motorFast();
					hal->motorRotateClockwise();
					hal->motorStart();
				}
				virtual void lb_input_interrupted() {
					new (this) ArrivalAtInput;
				}
			};

			//============================ ARRIVAL AT INPUT =======================================
			struct ArrivalAtInput: public State {
				ArrivalAtInput() {
					cout << "=============== ArrivalAtInput =================="
							<< endl;
					if (cb_this != cb_first) {
						duration<float> durationfs = steady_clock::now() - *timeFrameStart;
						milliseconds durationMS = duration_cast < milliseconds> (durationfs);
						time_output_to_input = durationMS.count();
						cout << "time_output_to_input: " << time_output_to_input<< endl;
					}
					hal->motorRotateClockwise();
					hal->motorFast();
					hal->motorStart();
				}
				virtual void lb_input_freed() {
					new (this) DepartureInput;
				}
			};

			//============================ DEPARTURE INPUT =======================================
			struct DepartureInput: public State {
				DepartureInput() {
					cout << "=============== DepartureInput =================="<< endl;
					*timeFrameStart = steady_clock::now();
					cout<<timeFrameStart<<" timeFrameStart"<<endl;
					*totalTimeStart = *timeFrameStart;
					cout<<totalTimeStart<<" totalTimeStart"<<endl;
				}
				virtual void lb_height_interrupted() {
					new (this) ArrivalAtHeight;
				}
			};

			//============================ ARRIVAL AT HIGHT =======================================
			struct ArrivalAtHeight: public State {
				ArrivalAtHeight() {
					cout << "=============== ArrivalAtHeight =================="<< endl;
					duration<float> durationfs = steady_clock::now() - *timeFrameStart;
					milliseconds durationMS = duration_cast < milliseconds> (durationfs);
					unsigned int time = durationMS.count();
					time_input_to_height = time;
					cout << "time_input_to_height: " << time_input_to_height<< endl;

					*timeFrameStart = steady_clock::now();

					height_item = hal->getHeight();
					cout << "height_item: " << height_item << endl;
				}
				virtual void lb_switch_interrupted() {
					new (this) ArrivalSwitch;
				}
			};

			//============================ ARRIVAL SWITCH=======================================
			struct ArrivalSwitch: public State {
				ArrivalSwitch() {
					cout << "=============== ArrivalSwitch =================="<< endl;
					duration<float> durationfs = steady_clock::now() - *timeFrameStart;
					milliseconds durationMS = duration_cast< milliseconds > (durationfs);
					unsigned int time = durationMS.count();
					time_height_to_switch = time;
					cout << "time_height_to_switch: " << time_height_to_switch<< endl;

					*timeFrameStart = steady_clock::now();

					hal->switchPointOpen();
				}
				virtual void lb_output_interrupted() {
					new (this) ArrivalOutput;
				}
			};

			//============================ ARRIVAL AT OUTPUT =======================================
			struct ArrivalOutput: public State {
				ArrivalOutput() {
					cout << "=============== ArrivalOutput =================="<< endl;
					hal->switchPointClose();
					steady_clock::time_point now = steady_clock::now();
					duration<float> durationSmallFrame = now - *timeFrameStart;
					duration<float> durationTotal = now - *totalTimeStart;
					time_switch_to_output = (unsigned int)duration_cast <milliseconds> (durationSmallFrame).count();
					time_cb_unit_total = (unsigned int) duration_cast<milliseconds>(durationTotal).count();
					cout << "time_switch_to_output: " << time_switch_to_output<< endl;
					cout << "time_cb_unit_total: " << time_cb_unit_total<< endl;

					if (cb_this == cb_last) {
						hal->motorStop();
						cout<< "Item arrived last CB's output."<< endl;
						cout<< "Please put one Item on input of each conveyor belt. "<< endl;

					}
					if (cb_this != cb_last) {
						hal->sendSerial(Signal(cb_this, cb_next, Signalname::SERIAL_TRANSFER_ITEM));
						std::thread thread = std::thread(motor_stop_timer,hal,1000);

						cout<< "Calibration active on next conveyor belt. Please wait for instructions"<< endl;
					}
				}
				virtual void lb_output_freed() {
					new (this) WaitForItemSlow;
				}
			};

			//============================ WAIT FOR ITEM SLOW =======================================
			struct WaitForItemSlow: public State {
				WaitForItemSlow() {
				}
				virtual void lb_input_interrupted() {
					new (this) ArrivalAtInputSlow;
				}
			};

			//============================ ARRIVAL AT INPUT SLOW =======================================
			struct ArrivalAtInputSlow: public State {
				ArrivalAtInputSlow() {
					hal->motorSlow();
					hal->motorStart();
				}
				virtual void lb_input_freed() {
					new (this) DepartureInputSlow;
				}
			};

			//============================ DEPARTURE INPUT SLOW =======================================
			struct DepartureInputSlow: public State {
				DepartureInputSlow() {
					*timeFrameStart = steady_clock::now();
				}
				virtual void lb_switch_interrupted() {
					new (this) ArrivalSwitchSlow;
				}
				virtual void lb_height_interrupted() {}
				virtual void lb_height_freed() {}
			};

			//============================ ARRIVAL SWITCH SLOW =======================================
			struct ArrivalSwitchSlow: public State {
				ArrivalSwitchSlow() {
					hal->switchPointOpen();
				}
				virtual void lb_output_interrupted() {
					new (this) ArrivalOutputSlow;
				}
				virtual void lb_switch_interrupted() {}
				virtual void lb_switch_freed() {}
			};

			//============================ ARRIVAL OUTPUT SLOW =======================================
			struct ArrivalOutputSlow: public State {
				ArrivalOutputSlow() {
					*timeFrameStop = steady_clock::now();
					unsigned int slowTotalTime = duration_cast<milliseconds> (*timeFrameStop - *timeFrameStart).count();
					cout<<"slowTotalTime: "<<slowTotalTime<<endl;
					slow_factor.parameterList.showParameters();
					cout<< (float)(((float) time_cb_unit_total)/  ((float)slowTotalTime)) <<endl;
					cout<< (float) time_cb_unit_total <<endl;
					cout<< (float)slowTotalTime <<endl;
					cout<< ((float) time_cb_unit_total)/  ((float)slowTotalTime) <<endl;
					slow_factor = (float)(((float) time_cb_unit_total)/  ((float)slowTotalTime));
					cout<<"slow_factor: "<<(float)slow_factor<<endl;
					// TODO calculate slow factor

					hal->motorRotateCounterclockwise();
					hal->motorFast();
				}
				virtual void lb_height_interrupted() {
					new (this) ArrivalHeightFinish;
				}
				virtual void lb_switch_interrupted() {}
				virtual void lb_switch_freed() {}
			};

			//============================ ARRIVAL HEIGHT FINISH =======================================
			struct ArrivalHeightFinish: public State {
				ArrivalHeightFinish() {
					hal->switchPointClose();
					hal->motorRotateClockwise();
				}
				virtual void lb_switch_interrupted() {
					new (this) ArrivalSwitchFinish;
				}
			};

			//============================ ARRIVAL SWITCH FINISH =======================================
			struct ArrivalSwitchFinish: public State {
				ArrivalSwitchFinish() {
					//TODO start slide timer
					//TODO start timeout timer
				}
				virtual void lb_slide_freed() {
					new (this) InSlide;
				}
				virtual void lb_slide_interrupted() {
				}
			};

			//============================ IN SLIDE =======================================
			struct InSlide: public State {
				InSlide() {
					hal->motorStop();
					//TODO calculate slide arrival time
					// TODO send 'success' to cb_first
					//TODO store data in calibration file
					cout << "Calibration completed." << endl;
					new (this) IDLE;
				}
			};

			IDLE stateMember;
			steady_clock::time_point timeFrameStart = steady_clock::now();
			steady_clock::time_point timeFrameStop = steady_clock::now();
			steady_clock::time_point totalTimeStart = steady_clock::now();

			hardwareLayer::HardwareLayer& hal;
			milliseconds timeStart;

			static void timeout_timer(hardwareLayer::HardwareLayer* hal, int milliseconds) {
				if(milliseconds > 0) {
					WAIT(milliseconds);
					hal->getSignalGenerator().pushBackOnSignalBuffer(Signal(Signalname::SENSOR_TEST_TIMEOUT));
				}
			}

			static void motor_stop_timer(hardwareLayer::HardwareLayer* hal, int milliseconds) {
				if(milliseconds > 0) {
					WAIT(milliseconds);
					hal->motorStop();
				}
			}

		public:
			Calibration(hardwareLayer::HardwareLayer&);
			virtual ~Calibration();
			void handle(Signal signal);
			std::string nameOf(State *state) const {
				return typeid(*state).name();
			}
		};

	} /* namespace logicLayer */
#endif /* CALIBRATION_H_ */
