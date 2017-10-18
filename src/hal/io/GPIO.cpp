/*
 * GPIO.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include <sys/neutrino.h>
#include <hw/inout.h>
#include "header.h"

#include "GPIO.h"

namespace hal {
namespace io {

GPIO *GPIO::_instance = nullptr;

GPIO* GPIO::instance() {
	if (_instance == nullptr) {
		_instance = new GPIO;
	}
	return _instance;
}

GPIO::GPIO() {
	LOG_SCOPE;

}

GPIO::~GPIO() {
	LOG_SCOPE;
}

void GPIO::gainAccess(){

}

void GPIO::write(PORT port, int val){
    out8(DIO_BASE+(int)port, val);
}

int GPIO::read(PORT port){
    return in8(DIO_BASE+(int)port);
}

void GPIO::setBits(PORT port, int bitmask) {
	int storedPortValue = this->read(port);
	this->write(port, storedPortValue | bitmask);
}

void GPIO::clearBits(PORT port, int bitmask) {
	int storedPortValue = this->read(port);
	this->write(port, storedPortValue & ~bitmask);
}


} /* namespace gpio */
} /* namespace hal */
