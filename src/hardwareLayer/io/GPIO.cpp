/*
 * GPIO.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include <sys/neutrino.h>
#include <hw/inout.h>
#include "Header.h"
#include <cerrno>

#include "GPIO.h"


namespace hardwareLayer {
namespace io {

constexpr int DIO_BASE = 0x300;

GPIO& GPIO::instance() {
	static GPIO instance;
	return instance;
}

GPIO::GPIO() {
	LOG_SCOPE;
}

GPIO::~GPIO() {
	LOG_SCOPE;
}

void GPIO::gainAccess(){
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
		LOG_ERROR<<errno<<" "<<strerror(errno);
		exit(EXIT_FAILURE);
	}
}

void GPIO::write(uint8_t address, port_t val){
	out8(DIO_BASE+(uint8_t)address, val);
}

uint8_t GPIO::read(uint8_t address){
	return in8(DIO_BASE + address);
}

port_t GPIO::read(PORT port){
    return in8(DIO_BASE+(port_t)port);
}

void GPIO::setBits(PORT port, port_t bitmask) {
	setBits((uint8_t)port,bitmask);
}

void GPIO::setBits(uint8_t address, uint8_t bitmask) {
	gpio_mutex.lock();
	uint8_t storedValue = this->read(address);
	uint8_t newValue = storedValue | bitmask;
	if(newValue != storedValue){
		this->write(address, newValue);
	}
	gpio_mutex.unlock();
}

void GPIO::clearBits(PORT port, port_t bitmask) {
	clearBits((uint8_t)port,bitmask);
}

void GPIO::clearBits(uint8_t address, port_t bitmask) {
	gpio_mutex.lock();
	port_t storedValue = this->read(address);
	port_t newValue = storedValue & ~bitmask;
	if(newValue != storedValue){
		this->write(address, newValue);
	}
	gpio_mutex.unlock();
}


} /* namespace gpio */
} /* namespace hardwareLayer */
