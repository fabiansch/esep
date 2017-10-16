/*
 * TrafficLights.cpp
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#include "TrafficLight.h"

namespace hal {
namespace io {
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
	// TODO Auto-generated constructor stub

}

TrafficLight::~TrafficLight() {
	delete _instance;
}

void TrafficLight::setGreenLight() {
	io::GPIO::instance()->setBits(PORT_A_ADDR, PIN_GREEN_LIGHT);
}

void TrafficLight::setYellowLight() {
	io::GPIO::instance()->setBits(PORT_A_ADDR, PIN_YELLOW_LIGHT);
}

void TrafficLight::setRedLight() {
	io::GPIO::instance()->setBits(PORT_A_ADDR, PIN_RED_LIGHT);
}

void TrafficLight::clearGreenLight() {
	io::GPIO::instance()->clearBits(PORT_A_ADDR, PIN_GREEN_LIGHT);
}

void TrafficLight::clearYellowLight() {
	io::GPIO::instance()->clearBits(PORT_A_ADDR, PIN_YELLOW_LIGHT);
}

void TrafficLight::clearRedLight() {
	io::GPIO::instance()->clearBits(PORT_A_ADDR, PIN_RED_LIGHT);


} /* namespace io */
} /* namespace hal */
