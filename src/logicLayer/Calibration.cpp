/*
 * Calibration.cpp
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#include "Calibration.h"

namespace logicLayer {

	Calibration::Calibration(hardwareLayer::HardwareLayer& hal)
	: statePtr(&stateMember) // assigning start state
	, hal(hal)

	{
		LOG_SCOPE
		statePtr->hal = &hal;
		statePtr->timeFrameStart = &timeFrameStart;
		statePtr->timeFrameStop = &timeFrameStop;
		statePtr ->totalTimeStart = &totalTimeStart;
		statePtr->successful = &successful;
	}

	Calibration::~Calibration() {
		LOG_SCOPE
	}


	void Calibration::handle(Signal signal){
		switch (signal.name) {
			// sensors
			case Signalname::CALIBRATION_START:
				statePtr->calibration_start();
				break;
			case Signalname::CALIBRATION_SUCCESSFUL:
				statePtr->calibration_successful(signal.sender);
				break;
			case Signalname::CALIBRATION_UNSUCCESSFUL:
				statePtr->calibration_unsuccessful(signal.sender);
				break;
			case Signalname::CALIBRATION_TIMEOUT:
				statePtr->calibration_timeout(signal.sender);
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
			// sensors
			case Signalname::SENSOR_HEIGHT_MATCH:
				break;
			case Signalname::SENSOR_HEIGHT_NOT_MATCH:
				break;
			case Signalname::SENSOR_METAL_MATCH:
				break;
			case Signalname::SENSOR_METAL_NOT_MATCH:
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
			case Signalname::TRANSFER_ITEM:
				statePtr->item_arrived();
				break;
			case Signalname::STOP:
				statePtr->stop();
				break;
			case Signalname::SENSOR_SWITCH_IS_OPEN:
				break;
			case Signalname::SENSOR_SWITCH_IS_CLOSED:
				break;
			case Signalname::CONVEYOR_BELT_BUSY:
				break;
			case Signalname::CONVEYOR_BELT_READY:
				// hot fix
				break;
			default:
				LOG_ERROR<<"Calibration does not support following Signal: "<<(int)signal.name<<endl;
				exit(EXIT_FAILURE);
				break;
		}

	}

} /* namespace logicLayer */
