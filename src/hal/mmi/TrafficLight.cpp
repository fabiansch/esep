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
	blink = mmi::Blink();
	thread = std::thread(std::ref(blink));
}

TrafficLight::~TrafficLight() {
	blink.terminate();
	thread.join();
}

void TrafficLight::greenLightOn() {
	blink.remove(PIN_GREEN_LIGHT);
	io::GPIO::instance()->setBits(PORT::A, PIN_GREEN_LIGHT);
}

void TrafficLight::yellowLightOn() {
	blink.remove(PIN_YELLOW_LIGHT);
	io::GPIO::instance()->setBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOn() {
	blink.remove(PIN_RED_LIGHT);
	io::GPIO::instance()->setBits(PORT::A, PIN_RED_LIGHT);
}

void TrafficLight::greenLightOff() {
	blink.remove(PIN_GREEN_LIGHT);
	io::GPIO::instance()->clearBits(PORT::A, PIN_GREEN_LIGHT);
}

void TrafficLight::yellowLightOff() {
	blink.remove(PIN_YELLOW_LIGHT);
	io::GPIO::instance()->clearBits(PORT::A, PIN_YELLOW_LIGHT);
}

void TrafficLight::redLightOff() {
	blink.remove(PIN_RED_LIGHT);
	io::GPIO::instance()->clearBits(PORT::A, PIN_RED_LIGHT);
}


void TrafficLight::blinkGreen(bool fast) {
	blink.add(PIN_GREEN_LIGHT, fast);
}

void TrafficLight::blinkYellow(bool fast) {
	blink.add(PIN_YELLOW_LIGHT, fast);
}

void TrafficLight::blinkRed(bool fast) {
	blink.add(PIN_RED_LIGHT, fast);
}

} /* namespace hmi */
} /* namespace hal */