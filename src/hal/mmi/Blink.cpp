	/*
 * Blink.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "Blink.h"
#include "GPIO.h"
#include <iostream>

#include <thread>
#include <chrono>

namespace hal {
namespace mmi {


Blink::Blink() {
	running = true;
	this->bitmaskFast = 0;
	this->bitmaskSlow = 0;
}

Blink::~Blink() {
	// TODO Auto-generated destructor stub
}

void Blink::operator()() {
	while (running) {
			hal::io::GPIO::instance()->setBits(PORT::A, this->bitmaskFast | this->bitmaskSlow);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			hal::io::GPIO::instance()->clearBits(PORT::A, this->bitmaskFast);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			hal::io::GPIO::instance()->setBits(PORT::A, this->bitmaskFast);
			hal::io::GPIO::instance()->clearBits(PORT::A, this->bitmaskSlow);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			hal::io::GPIO::instance()->clearBits(PORT::A, this->bitmaskFast);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void Blink::add(int bitmask, Speed speed) {
	switch(speed) {
	case Speed::fast:
		bitmaskSlow &= ~bitmask;
		bitmaskFast |= bitmask;
		break;
	case Speed::slow:
		bitmaskFast &= ~bitmask;
		bitmaskSlow |= bitmask;
		break;
	default:
		// TODO log that given speed is not implemented
		break;
	}
}

void Blink::removeBitmask(int bitmask) {
	bitmaskFast &= ~bitmask;
	bitmaskSlow &= ~bitmask;
}

void Blink::terminate() {
	running = false;
}

} /* namespace mmi */
} /* namespace hal */
