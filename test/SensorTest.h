/*
 * SensorTest.h
 *
 *  Created on: 16.11.2017
 *      Author: abj240
 */

#ifndef SENSORTEST_H_
#define SENSORTEST_H_

#include "HardwareLayer.h"
#include "Header.h"
#include "Item.h"
#include <iostream>
#include <string>
#include <thread>

namespace logicLayer {

class SensorTest {
private:
	struct State {//top-level state
		virtual void sensor_test_enter_on_enter(){			testFailed(__FUNCTION__);}
		virtual void sensor_test_start(){			testFailed(__FUNCTION__);}
		virtual void stop() {
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<"SENSOR TEST ABORTED BY MAIN MENU."<<endl;
			cout<<"SENSOR TEST ABORTED BY MAIN MENU."<<endl;
			testFailed(__FUNCTION__);
		}
		virtual void sensor_test_successful(uint8_t sender){	testFailed(__FUNCTION__);}
		virtual void sensor_test_timeout(){			testFailed(__FUNCTION__);}
		virtual void lb_input_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_input_freed(){				testFailed(__FUNCTION__);}
		virtual void sensor_height_match(){			testFailed(__FUNCTION__);}
		virtual void sensor_height_not_match(){		testFailed(__FUNCTION__);}
		virtual void sensor_metal_match(){			testFailed(__FUNCTION__);}
		virtual void sensor_metal_not_match(){		testFailed(__FUNCTION__);}
		virtual void sensor_switch_is_open(){		testFailed(__FUNCTION__);}
		virtual void sensor_switch_is_closed(){		testFailed(__FUNCTION__);}
		virtual void lb_height_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_height_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_switch_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_switch_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_slide_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_slide_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_output_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_output_freed(){				testFailed(__FUNCTION__);}
		virtual void item_arrived(){				testFailed(__FUNCTION__);}

		void testFailed(string functionname) {
			cout<<"Test failed: "<<name()<<", caused of "<<functionname<<endl;
			LOG_TEST<<"Test failed: "<<name()<<", caused of "<<functionname<<endl;
			hal->motorStop();
			hal->greenLightOff();
			hal->blinkRed(Speed::fast);
			hal->switchPointClose();
			hal->sendSerial(Signal(cb_this, cb_1, Signalname::SENSOR_TEST_UNSUCCESSFUL));
			LOG_TEST<<name()<<" => ";
			new (this) FAIL_STATE;
			LOG_TEST<<name()<<endl;
		}

		std::string name() const { return typeid(*this).name(); }

		hardwareLayer::HardwareLayer* hal;
		Item* testItem;
		std::thread* timeout_timer_th;
	} *statePtr;   // a pointer to current state. Used for polymorphism.


	// ============================= FAIL STATE =========================================
	struct FAIL_STATE : public State {
		virtual void sensor_test_start() {
			LOG_TEST<<name()<<" => ";
			new (this) LB_INPUT_Test;
			LOG_TEST<<name()<<endl;
		}
	};


	//============================ LB_INPUT_Test =======================================
	struct LB_INPUT_Test : public State {
		virtual void stop() {}
		virtual void lb_input_interrupted() {
			LOG_TEST<<__FUNCTION__<<endl;
			hal->blinkGreen(Speed::slow);
			hal->redLightOff();
		}

		virtual void sensor_test_enter_on_enter() {
			hal->motorRotateClockwise();
			hal->motorFast();
			hal->motorStart();
		}

		virtual void lb_input_freed() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_start(){
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<"### SENSOR TEST started ###"<<endl;
		}

		virtual void item_arrived(){
			hal->motorRotateClockwise();
			hal->motorFast();
			hal->motorStart();
		}

	};


	//============================ SENSOR_HEIGHT_MATCH_Test =======================================
	struct SENSOR_HEIGHT_MATCH_Test : public State {
		virtual void sensor_height_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			testItem->heightAbsolute = hal->getHeight();
			LOG_TEST<<"ABSOLUTE HOEHE: "<<testItem->heightAbsolute<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_HEIGHT_NOT_MATCH_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ SENSOR_HEIGHT_NOT_MATCH_Test =======================================
	struct SENSOR_HEIGHT_NOT_MATCH_Test : public State {

		virtual void sensor_height_not_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) LB_HEIGHT_INTERRUPT_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_HEIGHT_INTERRUPT_Test =======================================
	struct LB_HEIGHT_INTERRUPT_Test : public State {
		virtual void sensor_height_match() {
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_HEIGHT_NOT_MATCH_Test;
			LOG_TEST<<name()<<endl;
		}
		virtual void lb_height_interrupted() {
			LOG_TEST<<__FUNCTION__<<endl;

			testItem->heightCenter = hal->getHeight();
			LOG_TEST<<"HOEHE DER MITTE: "<<testItem->heightCenter<<endl;

			if(cb_this == cb_last && hal->getItemBufferSize() > 0) {
				Item passedItem = hal->getPassedItem();
				if( abs(passedItem.heightCenter - testItem->heightCenter) < 200) {
					LOG_TEST<<"DIFFERENCE IS ACCEPTABLE."<<endl;
				} else {
					LOG_TEST<<"DIFFERENCE TOO HIGH"<<endl;
					testFailed(__FUNCTION__);
				}
			}
			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_2_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ SENSOR_HEIGHT_MATCH_2_Test =======================================
	struct SENSOR_HEIGHT_MATCH_2_Test : public State {
		virtual void sensor_height_match() {
			LOG_TEST<<__FUNCTION__<<endl;
		}
		virtual void sensor_height_not_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) LB_HEIGHT_FREE_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_HEIGHT_FREE_Test =======================================
	struct LB_HEIGHT_FREE_Test : public State {
		virtual void sensor_height_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_2_Test;
			LOG_TEST<<name()<<endl;
		}
		virtual void lb_height_freed() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_METAL_MATCH_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ SENSOR_METAL_MATCH_Test =======================================
	struct SENSOR_METAL_MATCH_Test : public State {
		virtual void sensor_metal_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) LB_SWITCH_INTERRUPT_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_SWITCH_INTERRUPT_Test =======================================
	struct LB_SWITCH_INTERRUPT_Test : public State {
		virtual void lb_switch_interrupted() {
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_SWITCH_IS_OPEN_test;
			LOG_TEST<<name()<<endl;
			hal->switchPointOpen();
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ SENSOR_SWITCH_IS_OPEN_test =======================================
	struct SENSOR_SWITCH_IS_OPEN_test : public State {
		virtual void sensor_switch_is_open() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) SENSOR_METAL_NOT_MATCH_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ SENSOR_METAL_NOT_MATCH_Test =======================================
	struct SENSOR_METAL_NOT_MATCH_Test : public State {

		virtual void sensor_metal_not_match() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) LB_SWITCH_FREE_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_SWITCH_FREE_Test =======================================
	struct LB_SWITCH_FREE_Test : public State {
		virtual void lb_switch_freed() {
			LOG_TEST<<__FUNCTION__<<endl;

			timeout_timer_th->detach();
			*timeout_timer_th = std::thread(timeout_timer, hal, 2150);

			LOG_TEST<<name()<<" => ";
			new (this) LB_OUTPUT_INTERRUPTED_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_OUTPUT_Test =======================================
	struct LB_OUTPUT_INTERRUPTED_Test : public State {
		virtual void lb_output_interrupted() {
			LOG_TEST<<__FUNCTION__<<endl;
			hal->switchPointClose();

			if(cb_this != cb_last) {
				LOG_TEST<<"WE ARE NOT LAST"<<endl;
				hal->sendSerial(Signal(cb_this, cb_next, Signalname::SENSOR_TEST_START));
				hal->sendItemViaSerial(testItem);
			} else {
				LOG_TEST<<"WE ARE LAST"<<endl;
				hal->motorStop();
				cout<<"Please put item on master's input again."<<endl;
			}
			LOG_TEST<<name()<<" => ";
			new (this) LB_OUTPUT_FREED_Test;
			LOG_TEST<<name()<<endl;
		}
		virtual void sensor_switch_is_closed(){}

		virtual void sensor_test_timeout(){

		}
	};

	//============================ LB_OUTPUT_FREED_Test =======================================
	struct LB_OUTPUT_FREED_Test : public State {
		virtual void sensor_test_timeout() {}
		virtual void lb_output_freed() {
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<name()<<" successfully"<<endl;
			if(cb_this != cb_last) {
				cout<<"Test continues on next conveyer belt. When item reaches last conveyer belt's output, please put it on input of master again."<<endl;
				std::thread thread = std::thread(motor_stop_timer,hal,1000);
				thread.detach();
				LOG_TEST<<name()<<" => ";
				new (this) LB_OUTPUT_TIMEOUT_State;
				LOG_TEST<<name()<<endl;
			} else {
				LOG_TEST<<name()<<" => ";
				new (this) LB_SLIDE_Test;
				LOG_TEST<<name()<<endl;
			}
		}
		virtual void sensor_switch_is_closed() {}


	};

	//============================ LB_OUTPUT_TIMEOUT_State =======================================
	struct LB_OUTPUT_TIMEOUT_State: public State {
		virtual void sensor_test_timeout() {
			LOG_TEST<<__FUNCTION__<<endl;

			LOG_TEST<<name()<<" => ";
			new (this) LB_SLIDE_Test;
			LOG_TEST<<name()<<endl;
		}


	};

	//============================ LB_SLIDE_Test =======================================
	struct LB_SLIDE_Test : public State {
		virtual void lb_input_interrupted() {
			hal->motorRotateClockwise();
			hal->motorFast();
			hal->motorStart();
		}
		virtual void lb_input_freed() {}
		virtual void lb_height_interrupted() {}
		virtual void lb_height_freed() {}
		virtual void lb_switch_interrupted() {}
		virtual void lb_switch_freed() {
			timeout_timer_th->detach();
			*timeout_timer_th = std::thread(timeout_timer, hal, 2000);
		}
		virtual void sensor_height_match(){	}
		virtual void sensor_height_not_match(){}
		virtual void sensor_metal_match(){}
		virtual void sensor_metal_not_match(){}
		virtual void lb_slide_interrupted() {
			LOG_TEST<<__FUNCTION__<<endl;
			hal->motorStop();
		}
		virtual void lb_slide_freed() {
			LOG_TEST<<__FUNCTION__<<endl;
			LOG_TEST<<name()<<" => ";
			new (this) LB_SLIDE_TIMEOUT_State;
			LOG_TEST<<name()<<endl;

		}

		virtual void sensor_test_timeout(){

		}
	};


	//============================ LB_SLIDE_TIMEOUT_State =======================================
	struct LB_SLIDE_TIMEOUT_State: public State {
		virtual void sensor_test_timeout() {
			LOG_TEST << name() << " => ";
			new (this) OTHER_CBs_Test;
			LOG_TEST << name() << endl;
			if(cb_this == cb_1) {
				hal->getSignalGenerator().pushBackOnSignalBuffer(Signal(cb_this, cb_1, Signalname::SENSOR_TEST_SUCCESSFUL));
			} else {
				hal->sendSerial(Signal(cb_this, cb_1, Signalname::SENSOR_TEST_SUCCESSFUL));
			}
		}


	};

	//============================ OTHER_CBs_Test =======================================
	struct OTHER_CBs_Test : public State {
		virtual void sensor_test_successful(uint8_t sender) {
			LOG_TEST<<"Test was successful on conveyer belt: "<<(int)sender<<endl;
			cout<<"Test was successful on conveyer belt: "<<(int)sender<<endl;
			if(sender == cb_last) {
				LOG_TEST<<"################ Automated Sensor Test finished ###################"<<endl;
				cout<<    "################ Automated Sensor Test finished ###################"<<endl;
				cout<<"Result stored in testLog.txt"<<endl<<endl<<endl;
				LOG_TEST<<name()<<" => ";
				new (this) LB_INPUT_Test;
				LOG_TEST<<name()<<endl;
				hal->getSignalGenerator().pushBackOnSignalBuffer(Signal(Signalname::STOP));
			}
		}
		virtual void sensor_test_unsuccessful(uint8_t sender) {
			LOG_TEST<<"Test UNsuccessful on conveyer belt: "<<(int)sender<<endl;
			cout<<"Test UNsuccessful on conveyer belt: "<<(int)sender<<endl;
			cout<<"Please restart test."<<endl;
			LOG_TEST<<name()<<" => ";
			new (this) LB_INPUT_Test;
			LOG_TEST<<name()<<endl;
		}

		virtual void sensor_test_timeout(){

		}
	};

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


	LB_INPUT_Test stateMember;//The memory for the state is part of context object

	hardwareLayer::HardwareLayer& hal;
	Item testItem;
	std::thread timeout_timer_th;
	std::thread timeout_input_th;

public:
	SensorTest(hardwareLayer::HardwareLayer& hal)
	: statePtr(&stateMember) // assigning start state
	, hal(hal)
	, testItem( &hal, nullptr, nullptr )
	, timeout_timer_th(std::thread(timeout_timer, &hal, 0))
	{
		statePtr->hal = &hal;
		statePtr->testItem = &testItem; // connecting state->testItem with the SensorTest::testItem
		statePtr->timeout_timer_th = &timeout_timer_th;
	}

	virtual ~SensorTest(){
		timeout_timer_th.join();
	};



	std::string nameOf(State *state) const { return typeid(*state).name(); }

	void handle(Signal signal){
		switch (signal.name) {
			// sensors
			case Signalname::SENSOR_TEST_START:
				statePtr->sensor_test_start();
				break;
			case Signalname::SENSOR_TEST_SUCCESSFUL:
				statePtr->sensor_test_successful(signal.sender);
				break;
			case Signalname::SENSOR_TEST_TIMEOUT:
				statePtr->sensor_test_timeout();
				break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				statePtr->sensor_height_match();
				break;
			case Signalname::SENSOR_HEIGHT_NOT_MATCH:
				statePtr->sensor_height_not_match();
				break;
			case Signalname::SENSOR_METAL_MATCH:
				statePtr->sensor_metal_match();
				break;
			case Signalname::SENSOR_METAL_NOT_MATCH:
				statePtr->sensor_metal_not_match();
				break;
			case Signalname::SENSOR_SWITCH_IS_OPEN:
				statePtr->sensor_switch_is_open();
				break;
			case Signalname::SENSOR_SWITCH_IS_CLOSED:
				statePtr->sensor_switch_is_closed();
				break;
			// light barriers
			case Signalname::LB_INPUT_INTERRUPTED:
				statePtr->lb_input_interrupted();
				break;
			case Signalname::LB_INPUT_FREED:
				statePtr->lb_input_freed();
				break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				statePtr->lb_height_interrupted();
				break;
			case Signalname::LB_HEIGHT_FREED:
				statePtr->lb_height_freed();
				break;
			case Signalname::LB_SWITCH_INTERRUPTED:
				statePtr->lb_switch_interrupted();
				break;
			case Signalname::LB_SWITCH_FREED:
				statePtr->lb_switch_freed();
				break;
			case Signalname::LB_SLIDE_INTERRUPTED:
				statePtr->lb_slide_interrupted();
				break;
			case Signalname::LB_SLIDE_FREED:
				statePtr->lb_slide_freed();
				break;
			case Signalname::LB_OUTPUT_INTERRUPTED:
				statePtr->lb_output_interrupted();
				break;
			case Signalname::LB_OUTPUT_FREED:
				statePtr->lb_output_freed();
				break;
			// mmi
			// buttons
			case Signalname::BUTTON_E_STOP_PUSHED:
				break;
			case Signalname::BUTTON_E_STOP_PULLED:
				break;
			case Signalname::BUTTON_RESET_PUSHED:
				break;
			case Signalname::BUTTON_RESET_PULLED:
				break;
			case Signalname::BUTTON_START_PUSHED:
				break;
			case Signalname::BUTTON_START_PULLED:
				break;
			case Signalname::BUTTON_STOP_PUSHED:
				break;
			case Signalname::BUTTON_STOP_PULLED:
				break;
			// traffic lights
			case Signalname::YELLOW_LIGHT_ON:
				hal.yellowLightOn();
				break;
			case Signalname::RED_LIGHT_ON:
				hal.redLightOn();
				break;
			case Signalname::GREEN_LIGHT_ON:
				hal.greenLightOn();
				break;
			case Signalname::BLINK_GREEN_FAST:
				hal.blinkGreen(Speed::fast);
				break;
			case Signalname::BLINK_RED_FAST:
				hal.blinkRed(Speed::fast);
				break;
			case Signalname::BLINK_YELLOW_FAST:
				hal.blinkYellow(Speed::fast);
				break;
			case Signalname::BLINK_GREEN_SLOW:
				hal.blinkGreen(Speed::slow);
				break;
			case Signalname::BLINK_YELLOW_SLOW:
				hal.blinkYellow(Speed::slow);
				break;
			case Signalname::BLINK_RED_SLOW:
				hal.blinkRed(Speed::slow);
				break;
			case Signalname::RED_LIGHT_OFF:
				hal.redLightOff();
				break;
			case Signalname::YELLOW_LIGHT_OFF:
				hal.yellowLightOff();
				break;
			case Signalname::GREEN_LIGHT_OFF:
				hal.greenLightOff();
				break;
			// leds
			case Signalname::Q1_LED_ON:
				hal.Q1LEDOn();
				break;
			case Signalname::Q1_LED_OFF:
				hal.Q1LEDOff();
				break;
			case Signalname::Q2_LED_ON:
				hal.Q2LEDOn();
				break;
			case Signalname::Q2_LED_OFF:
				hal.Q2LEDOff();
				break;
			case Signalname::RESET_LED_ON:
				hal.ResetLEDOn();
				break;
			case Signalname::RESET_LED_OFF:
				hal.ResetLEDOff();
				break;
			case Signalname::START_LED_ON:
				hal.StartLEDOn();
				break;
			case Signalname::START_LED_OFF:
				hal.StartLEDOff();
				break;
			// motor
			case Signalname::MOTOR_START:
				LOG_DEBUG << "call MOTOR start ";
				hal.motorStart();
				LOG_DEBUG << "MOTOR started ";
				break;
			case Signalname::MOTOR_STOP:
				LOG_DEBUG << "call MOTOR stop ";
				hal.motorStop();
				LOG_DEBUG << "call MOTOR stopped ";
				break;
			case Signalname::MOTOR_FAST:
				LOG_DEBUG << "call MOTOR fast ";
				hal.motorFast();
				LOG_DEBUG << "end MOTOR  fast";
				break;
			case Signalname::MOTOR_SLOW:
				LOG_DEBUG << "call MOTOR slow ";
				hal.motorSlow();
				LOG_DEBUG << "end MOTOR  slow";
				break;
			case Signalname::MOTOR_ROTATE_CLOCKWISE:
				LOG_DEBUG << "call MOTOR clockwise ";
				hal.motorRotateClockwise();
				LOG_DEBUG << "end MOTOR  clockwise";
				break;
			case Signalname::MOTOR_ROTATE_COUNTER_CLOCKWISE:
				LOG_DEBUG << "call MOTOR counter clockwise ";
				hal.motorRotateCounterclockwise();
				LOG_DEBUG << "end MOTOR  clockwise";
				break;
			// switch
			case Signalname::SWITCH_OPEN:
				hal.switchPointOpen();
				break;
			case Signalname::SWITCH_CLOSE:
				hal.switchPointClose();
				break;
			// item
			case Signalname::ITEM_ARRIVED:
				statePtr->item_arrived();
				break;
			case Signalname::SENSOR_TEST_ENTER:
				statePtr->sensor_test_enter_on_enter();
			break;
			case Signalname::SENSOR_TEST_TIMER_START:
				timeout_input_th = std::thread( timeout_timer, &hal, 1000 );
				timeout_input_th.detach();
			break;
			case Signalname::STOP:
				statePtr->stop();
			break;

			default:
				LOG_ERROR<<"SensorTest does not support following Signal: "<<(int)signal.name<<endl;
				exit(EXIT_FAILURE);
				break;
		}
	}
};

} /* namespace logicLayer */
#endif /* SENSORTEST_H_ */
