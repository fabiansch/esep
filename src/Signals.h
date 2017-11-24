/*
 * Signals.h
 *
 *  Created on: 18.10.2017
 *      Author: abj240
 */

#ifndef SIGNALS_H_
#define SIGNALS_H_

#include "Logger.h"
#include "Parameter.h"
#include <cstdint>

// conveyer belts
constexpr uint8_t cb_1 = 0b00000001;
constexpr uint8_t cb_2 = 0b00000010;
constexpr uint8_t cb_3 = 0b00000100;
constexpr uint8_t cb_4 = 0b00001000;
constexpr uint8_t cb_5 = 0b00010000;
constexpr uint8_t cb_6 = 0b00100000;
constexpr uint8_t cb_7 = 0b01000000;
constexpr uint8_t cb_8 = 0b10000000;


enum class Speed {fast, slow};

enum class Signalname : uint16_t {
	// buttons
	BUTTON_START_PUSHED,
	BUTTON_START_PULLED,
	BUTTON_STOP_PUSHED,
	BUTTON_STOP_PULLED,
	BUTTON_RESET_PUSHED,
	BUTTON_RESET_PULLED,
	BUTTON_E_STOP_PUSHED,
	BUTTON_E_STOP_PULLED,
	// sensors
	SENSOR_TEST_START,
	SENSOR_TEST_SUCCESSFUL,
	SENSOR_TEST_UNSUCCESSFUL,
	// -- light barriers
	LB_INPUT_INTERRUPTED,
	LB_INPUT_FREED,
	LB_HEIGHT_INTERRUPTED,
	LB_HEIGHT_FREED,
	LB_SWITCH_INTERRUPTED,
	LB_SWITCH_FREED,
	LB_SLIDE_INTERRUPTED,
	LB_SLIDE_FREED,
	LB_OUTPUT_INTERRUPTED,
	LB_OUTPUT_FREED,
	// -- height
	SENSOR_HEIGHT_MATCH,
	SENSOR_HEIGHT_NOT_MATCH,
	// -- metal
	SENSOR_METAL_MATCH,
	SENSOR_METAL_NOT_MATCH,
	// -- switch
	SENSOR_SWITCH_IS_OPEN,
	SENSOR_SWITCH_IS_CLOSED,
	// motor
	MOTOR_FAST,
	MOTOR_SLOW,
	MOTOR_STOP,
	MOTOR_START,
	MOTOR_ROTATE_CLOCKWISE,
	MOTOR_ROTATE_COUNTER_CLOCKWISE,
	//switch
	SWITCH_OPEN,
	SWITCH_CLOSE,
	// mmi
	// traffic light
	GREEN_LIGHT_ON,
	GREEN_LIGHT_OFF,
	YELLOW_LIGHT_ON,
	YELLOW_LIGHT_OFF,
	RED_LIGHT_ON,
	RED_LIGHT_OFF,
	BLINK_GREEN_FAST,
	BLINK_GREEN_SLOW,
	BLINK_YELLOW_FAST,
	BLINK_YELLOW_SLOW,
	BLINK_RED_FAST,
	BLINK_RED_SLOW,
	// leds
	Q1_LED_ON,
	Q1_LED_OFF,
	Q2_LED_ON,
	Q2_LED_OFF,
	RESET_LED_ON,
	RESET_LED_OFF,
	START_LED_ON,
	START_LED_OFF,
	// serial
	CONVEYOR_BELT_BUSY,
	CONVEYOR_BELT_READY,
	SLIDE_FULL,
	SLIDE_EMPTY,
	// timer
	// -- in
	START_TIMERS_INPUT,
	START_TIMERS_HEIGHT,
	START_TIMERS_SWITCH,
	START_TIMERS_SLIDE,
	START_TIMERS_OUTPUT,
	// -- out
	TIMEFRAME_INPUT_ENTER,
	TIMEFRAME_INPUT_LEAVE,
	TIMEFRAME_HEIGHT_ENTER,
	TIMEFRAME_HEIGHT_LEAVE,
	TIMEFRAME_SWITCH_ENTER,
	TIMEFRAME_SWITCH_LEAVE,
	TIMEFRAME_SLIDE_ENTER,
	TIMEFRAME_SLIDE_LEAVE,
	TIMEFRAME_OUTPUT_ENTER,
	TIMEFRAME_OUTPUT_LEAVE,
	// calibration
	CALIBRATION_START,
	CALIBRATION_STOP,
	CALIBRATION_SUCCESSFUL,
	CALIBRATION_UNSUCCESSFUL,
	MEASURE_REACHING_LB_HEIGHT_START,
	MEASURE_REACHING_LB_HEIGHT_STOP,
	MEASURE_REACHING_LB_SWITCH_START,
	MEASURE_REACHING_LB_SWITCH_STOP,
	MEASURE_REACHING_LB_SLIDE_START,
	MEASURE_REACHING_LB_SLIDE_STOP,
	MEASURE_REACHING_LB_OUTPUT_START,
	MEASURE_REACHING_LB_OUTPUT_STOP,
	MEASURE_VELOCITY_FAST_START,
	MEASURE_VELOCITY_FAST_STOP,
	MEASURE_VELOCITY_SLOW_START,
	MEASURE_VELOCITY_SLOW_STOP,

	//Serial Interface
	SERIAL_TRANSFER_ITEM,
	SERIAL_WATCHDOG_TOKEN,
	SERIAL_WATCHDOG_FEED,

	//CONNECTION
	CONNECTION_LOST,
	CONNECTION_CONNECTED,

	// signal generator
	SIGNAL_BUFFER_EMPTY,
	ITEM_ARRIVED,

	// dummy signal
	SIGNAL_DUMMY

};

/**
 * @brief next to signal's name a signal has one specific sender and can have several receivers
 *
 * Signal consists of Signalname name (uint16_t), sender (uint8_t) and receiver (uint8_t). Compiling
 * it on GNU 4.7.3, its size is 4 bytes. This means that on this system Signal can be considered as
 * an atomic type. For sure, this is not platform independent.
 *
 * @param sender cb_1 as an example for cb_1 as signal's sender. Possible values cb_1-cb_8.
 * @param receiver  cb_1 | cb_2 as an example for cb_1 and cb_2 as receivers of the signal.
 * 					receiver 0 means that the signal is a shadow signal
 * @param name specific signal name
 */
struct Signal {
	Signal(Signalname name) :
	name(name),
	sender(cb_this),
	receiver(cb_this)
	{
		LOG_SCOPE
	}
	Signal(uint8_t sender, uint8_t receiver, Signalname name) :
	name(name),
	sender(sender),
	receiver(receiver)
	{
		LOG_SCOPE
	}
	Signal() :
		name(Signalname::SIGNAL_DUMMY),
		sender(0),
		receiver(0)
	{
		LOG_SCOPE
	}
	~Signal()
	{
		LOG_SCOPE
	}
	Signalname name;
	uint8_t sender;
	uint8_t receiver;
};


#endif /* SIGNALS_H_ */
