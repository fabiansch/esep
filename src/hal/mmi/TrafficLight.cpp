/*
 * TrafficLights.cpp
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#include "TrafficLight.h"
#include <iostream>

namespace hal {
namespace mmi {

constexpr int PIN_RED_LIGHT = 0x80;
constexpr int PIN_YELLOW_LIGHT = 0x40;
constexpr int PIN_GREEN_LIGHT = 0x20;
constexpr int PORT_A_ADDR = 0x300;


TrafficLight *TrafficLight::_instance = nullptr;

TrafficLight *TrafficLight::instance() {
	if (_instance == nullptr) {
		_instance = new TrafficLight;
	}
	return _instance;
}

TrafficLight::TrafficLight() {
	this->blink_slow = mmi::Blink(false);
	this->blink_fast = mmi::Blink(true);
	this->blink_fast.add(PIN_RED_LIGHT);
	this->blink_fast.add(PIN_GREEN_LIGHT);
	this->blink_slow.add(PIN_YELLOW_LIGHT);

	t_slow = std::thread(blink_slow);
	t_fast = std::thread(blink_fast);
	std::cout << "RELEASED" << std::endl;

}

TrafficLight::~TrafficLight() {

}

void TrafficLight::greenLightOn() {
	io::GPIO::instance()->setBits(PORT::A, PIN_GREEN_LIGHT);
}

void TrafficLight::yellowLightOn() {
	io::GPIO::instance()->setBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOn() {
	io::GPIO::instance()->setBits(PORT::A, PIN_RED_LIGHT);
}

void TrafficLight::greenLightOff() {
	io::GPIO::instance()->clearBits(PORT::A, PIN_GREEN_LIGHT);
}

void TrafficLight::yellowLightOff() {
	io::GPIO::instance()->clearBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOff() {
	io::GPIO::instance()->clearBits(PORT::A, PIN_RED_LIGHT);
}

void TrafficLight::blinkGreen(bool fast) {
	if (fast) {
		blink_fast.add(PIN_GREEN_LIGHT);
	} else {
		blink_slow.add(PIN_GREEN_LIGHT);
	}
}

} /* namespace hmi */
} /* namespace hal */
