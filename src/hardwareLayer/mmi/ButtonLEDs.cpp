/*
 * ButtonLEDs.cpp
 *
 *  Created on: 10.11.2017
 *      Author: aca311
 */

#include "ButtonLEDs.h"
#include "Header.h"

constexpr port_t PIN_Start_LED	= 0x01;
constexpr port_t PIN_Reset_LED 	= 0x02;
constexpr port_t PIN_Q1_LED		= 0x04;
constexpr port_t PIN_Q2_LED		= 0x08;


namespace hardwareLayer {
namespace mmi {

ButtonLEDs& ButtonLEDs::instance() {
	static ButtonLEDs instance;
	return instance;
}

void ButtonLEDs::LEDStartOn(){
	io::GPIO::instance().setBits(PORT::C, PIN_Start_LED);
}

void ButtonLEDs::LEDStartOff(){
	io::GPIO::instance().clearBits(PORT::C, PIN_Start_LED);
}

void ButtonLEDs::LEDResetOn(){
	io::GPIO::instance().setBits(PORT::C, PIN_Reset_LED);
}

void ButtonLEDs::LEDResetOff(){
	io::GPIO::instance().clearBits(PORT::C, PIN_Reset_LED);
}

void ButtonLEDs::LEDQ1On(){
	io::GPIO::instance().setBits(PORT::C, PIN_Q1_LED);
}

void ButtonLEDs::LEDQ1Off(){
	io::GPIO::instance().clearBits(PORT::C, PIN_Q1_LED);
}

void ButtonLEDs::LEDQ2On(){
	io::GPIO::instance().setBits(PORT::C, PIN_Q2_LED);
}

void ButtonLEDs::LEDQ2Off(){
	io::GPIO::instance().clearBits(PORT::C, PIN_Q2_LED);
}


ButtonLEDs::ButtonLEDs() {
	LOG_SCOPE;
}

ButtonLEDs::~ButtonLEDs() {
	LOG_SCOPE;
}

} /* namespace mmi */
} /* namespace hal */

