/*
 * Controller.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Controller.h"

namespace logicLayer {

Controller::Controller(hardwareLayer::HardwareLayer& hal)
: hal(hal)
, sensorTest(hal)
{
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
	// TODO Auto-generated constructor stub
	std::cout << "create Controller" << endl;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::operator()(){

	Signal sig;
	while(running){
		std::cout << "blocking io at controller listener" << endl;
		sig << channel_;
		switch (sig.name) {
			// sensors
			case Signalname::SENSOR_TEST_START:
				sensorTest.sensor_test_start();
				break;
			case Signalname::SENSOR_TEST_SUCCESSFUL:
				sensorTest.sensor_test_successful(sig.sender);
				break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				sensorTest.sensor_height_match();
				break;
			case Signalname::SENSOR_HEIGHT_NOT_MATCH:
				sensorTest.sensor_height_not_match();
				break;
			case Signalname::SENSOR_METAL_MATCH:
				sensorTest.sensor_metal_match();
				break;
			case Signalname::SENSOR_METAL_NOT_MATCH:
				sensorTest.sensor_metal_not_match();
				break;
			case Signalname::SENSOR_SWITCH_IS_OPEN:
				sensorTest.sensor_switch_is_open();
				break;
			case Signalname::SENSOR_SWITCH_IS_CLOSED:
				sensorTest.sensor_switch_is_closed();
				break;
			// light barriers
			case Signalname::LB_INPUT_INTERRUPTED:
				sensorTest.lb_input_interrupted();
				break;
			case Signalname::LB_INPUT_FREED:
				sensorTest.lb_input_freed();
				break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				sensorTest.lb_height_interrupted();
				break;
			case Signalname::LB_HEIGHT_FREED:
				sensorTest.lb_height_freed();
				break;
			case Signalname::LB_SWITCH_INTERRUPTED:
				sensorTest.lb_switch_interrupted();
				break;
			case Signalname::LB_SWITCH_FREED:
				sensorTest.lb_switch_freed();
				break;
			case Signalname::LB_SLIDE_INTERRUPTED:
				sensorTest.lb_slide_interrupted();
				break;
			case Signalname::LB_SLIDE_FREED:
				sensorTest.lb_slide_freed();
				break;
			case Signalname::LB_OUTPUT_INTERRUPTED:
				sensorTest.lb_output_interrupted();
				break;
			case Signalname::LB_OUTPUT_FREED:
				sensorTest.lb_output_freed();
				break;
			// mmi
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
			default:

				break;
		}
	}
}

} /* namespace logicLayer */
