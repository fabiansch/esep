/*
 * TrafficLights.cpp
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#include "TrafficLight.h"
#include <iostream>
#include "Header.h"

namespace hardwareLayer {
namespace mmi {

constexpr port_t PIN_GREEN_LIGHT 	= 0x20;
constexpr port_t PIN_YELLOW_LIGHT 	= 0x40;
constexpr port_t PIN_RED_LIGHT 		= 0x80;


TrafficLight& TrafficLight::instance() {
	static TrafficLight instance;
	return instance;
}

TrafficLight::TrafficLight()
: greenStatus(LightStatus::off)
, greenLocked(false)
, blink()
{
	LOG_SCOPE;
	io::GPIO::instance().gainAccess();
	thread = std::thread(std::ref(blink));
}

TrafficLight::~TrafficLight() {
	LOG_SCOPE;
	blink.terminate();
	thread.join();
}

void TrafficLight::greenLightOn() {
	greenStatus = LightStatus::on;
	if (not greenLocked) {
		blink.removeBitmask(PIN_GREEN_LIGHT);
		io::GPIO::instance().setBits(PORT::A, PIN_GREEN_LIGHT);
	}
}

void TrafficLight::yellowLightOn() {
	greenStatus = LightStatus::off;
	blink.removeBitmask(PIN_YELLOW_LIGHT);
	io::GPIO::instance().setBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOn() {
	blink.removeBitmask(PIN_RED_LIGHT);
	io::GPIO::instance().setBits(PORT::A, PIN_RED_LIGHT);
}

void TrafficLight::greenLightOff() {
	greenStatus = LightStatus::off;
	if (not greenLocked) {
		blink.removeBitmask(PIN_GREEN_LIGHT);
		io::GPIO::instance().clearBits(PORT::A, PIN_GREEN_LIGHT);
	}
}

void TrafficLight::yellowLightOff() {
	blink.removeBitmask(PIN_YELLOW_LIGHT);
	io::GPIO::instance().clearBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOff() {
	blink.removeBitmask(PIN_RED_LIGHT);
	io::GPIO::instance().clearBits(PORT::A, PIN_RED_LIGHT);
}


void TrafficLight::blinkGreen(Speed speed) {
	switch (speed) {
	case Speed::slow: greenStatus = LightStatus::blinkSlow;		break;
	case Speed::fast: greenStatus = LightStatus::blinkFast; 	break;
	default: LOG_ERROR<<__FUNCTION__<<endl; exit(EXIT_FAILURE); break;
	}
	if (not greenLocked) blink.add(PIN_GREEN_LIGHT, speed);
}

void TrafficLight::blinkYellow(Speed speed) {
	blink.add(PIN_YELLOW_LIGHT, speed);
}

void TrafficLight::blinkRed(Speed speed) {
	blink.add(PIN_RED_LIGHT, speed);
}

void TrafficLight::lockGreen(bool lock) {
	if(lock) {
		if(greenLocked == false) {
			greenLightOff();
		}
		greenLocked = true;
	} else {
		switch(greenStatus) {
		case LightStatus::on:		 	greenLightOn(); 		 	break;
		case LightStatus::off:		 	greenLightOff(); 		 	break;
		case LightStatus::blinkSlow: 	blinkGreen(Speed::slow); 	break;
		case LightStatus::blinkFast: 	blinkGreen(Speed::fast); 	break;
		default: LOG_ERROR<<__FUNCTION__<<endl; exit(EXIT_FAILURE); break;
		}
		greenLocked = false;
	}
}


} /* namespace hmi */
} /* namespace hardwareLayer */
