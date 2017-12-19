/*
 * Motor.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "Motor.h"
#include "GPIO.h"
#include "Header.h"

constexpr port_t CLOCKWISE_ROTATION 		= 0b00000001;
constexpr port_t COUNTERCLOCKWISE_ROTATION 	= 0b00000010;
constexpr port_t MOTOR_SLOW					= 0b00000100;
constexpr port_t MOTOR_STOP 				= 0b00001000;

namespace hardwareLayer {
namespace actuators {


Motor& Motor::instance(io::SignalGenerator& signalGenerator) {
	static Motor instance(signalGenerator);
	return instance;
}

Motor::Motor(io::SignalGenerator& signalGenerator)
: signalGenerator(signalGenerator)
, locked(false)
, running(false)
{
	LOG_SCOPE;
}

Motor::~Motor() {
	LOG_SCOPE;
}

void Motor::start() {
	cout<<"start"<<endl;
	running = true;
	if(not locked) {
		io::GPIO::instance().clearBits(PORT::A, MOTOR_STOP);
		signalGenerator.pushBackOnSignalBuffer(Signal(Signalname::MOTOR_START));
	}
}

void Motor::stop() {
	cout<<"stop"<<endl;
	running = false;
	io::GPIO::instance().setBits(PORT::A, MOTOR_STOP);
	signalGenerator.pushBackOnSignalBuffer(Signal(Signalname::MOTOR_STOP));
}

void Motor::setSlow() {
	cout<<"setSlow"<<endl;
	io::GPIO::instance().setBits(PORT::A, MOTOR_SLOW);
	signalGenerator.pushBackOnSignalBuffer(Signal(Signalname::MOTOR_SLOW));
}

void Motor::clearSlow() {
	cout<<"clearSlow"<<endl;
	io::GPIO::instance().clearBits(PORT::A, MOTOR_SLOW);
	signalGenerator.pushBackOnSignalBuffer(Signal(Signalname::MOTOR_FAST));
}

void Motor::setClockwiseRotation() {
	io::GPIO::instance().clearBits(PORT::A, COUNTERCLOCKWISE_ROTATION);
	io::GPIO::instance().setBits(PORT::A, CLOCKWISE_ROTATION);
}

void Motor::setCounterclockwiseRotation() {
	io::GPIO::instance().clearBits(PORT::A, CLOCKWISE_ROTATION);
	io::GPIO::instance().setBits(PORT::A, COUNTERCLOCKWISE_ROTATION);
}

void Motor::lock(bool lock) {
	locked = lock;
	if(locked) {
		bool running_temp = running;
		stop();
		running = running_temp;
	} else {
		if(running) {
			start();
		}
	}
}

} /* namespace actuators */
} /* namespace hardwareLayer */

