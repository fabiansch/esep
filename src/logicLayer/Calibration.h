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

using namespace std::chrono;
namespace logicLayer {

class Calibration {
private:
	struct State {//top-level state
			virtual void sensor_test_start(){			calibrationFailed(__FUNCTION__);}
			virtual void stop() {
				LOG_TEST<<__FUNCTION__<<endl;
				LOG_TEST<<"CALIBRATION ABORTED BY MAIN MENU."<<endl;
				cout<<"CALIBRATION ABORTED BY MAIN MENU."<<endl;
				calibrationFailed(__FUNCTION__);
			}
			virtual void sensor_test_successful(uint8_t sender){	calibrationFailed(__FUNCTION__);}
			virtual void sensor_test_timeout(){			calibrationFailed(__FUNCTION__);}
			virtual void lb_input_interrupted(){		calibrationFailed(__FUNCTION__);}
			virtual void lb_input_freed(){				calibrationFailed(__FUNCTION__);}
			virtual void sensor_height_match(){			calibrationFailed(__FUNCTION__);}
			virtual void sensor_height_not_match(){		calibrationFailed(__FUNCTION__);}
			virtual void sensor_metal_match(){			calibrationFailed(__FUNCTION__);}
			virtual void sensor_metal_not_match(){		calibrationFailed(__FUNCTION__);}
			virtual void sensor_switch_is_open(){		calibrationFailed(__FUNCTION__);}
			virtual void sensor_switch_is_closed(){		calibrationFailed(__FUNCTION__);}
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

		} *statePtr;

		// ============================= FAIL STATE =========================================
		struct FAIL_STATE : public State {
			virtual void calibration(){
				new (this) START_STATE;
			}
		};

		//============================ START_STATE =======================================
		struct START_STATE: public State {
			virtual void lb_input_freed(){

			}
		};


		START_STATE stateMember;

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
