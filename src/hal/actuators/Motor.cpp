/*
 * Motor.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "Motor.h"
#include "GPIO.h"
#include "Header.h"

constexpr int CLOCKWISE_ROTATION 	= 0b00000001;
constexpr int MOTOR_SLOW			= 0b00000100;
constexpr int MOTOR_STOP 			= 0b00001000;

namespace hal {
namespace actuators {

Motor *Motor::_instance = nullptr;

Motor *Motor::instance() {
	if (_instance == nullptr) {
		_instance = new Motor;
	}
	return _instance;
}

Motor::Motor() {
	LOG_SCOPE;
}

Motor::~Motor() {
	LOG_SCOPE;
}

void Motor::start() {
	io::GPIO::instance()->clearBits(PORT::A, MOTOR_STOP);
}

void Motor::stop() {
	io::GPIO::instance()->setBits(PORT::A, MOTOR_STOP);
}

void Motor::setSlow() {
	io::GPIO::instance()->setBits(PORT::A, MOTOR_SLOW);
}

void Motor::clearSlow() {
	io::GPIO::instance()->clearBits(PORT::A, MOTOR_SLOW);
}

void Motor::setClockwiseRotation() {
	io::GPIO::instance()->setBits(PORT::A, CLOCKWISE_ROTATION);
}

void Motor::setCounterclockwiseRotation() {
	io::GPIO::instance()->clearBits(PORT::A, CLOCKWISE_ROTATION);
}

} /* namespace actuators */
} /* namespace hal */

