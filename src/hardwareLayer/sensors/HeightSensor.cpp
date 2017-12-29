/*
 * HeightSensor.cpp
 *
 *  Created on: 30.10.2017
 *      Author: aca311
 */

#include "HeightSensor.h"
#include "GPIO.h"

#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <unistd.h>

// #include <hw/inout.h>

namespace hardwareLayer {
namespace sensors {

constexpr uint8_t readLowAddressOffset = 0x22;
constexpr uint8_t startConversionAddressOffset = 0x22;
constexpr uint8_t readHighAddressOffset = 0x23;
constexpr uint8_t startConversionOpCode = 0x10;
constexpr uint16_t only12BitBitmask = 0x0FFF;

HeightSensor::HeightSensor() {
	// TODO Auto-generated constructor stub

}

HeightSensor& HeightSensor::instance() {
	static HeightSensor instance;
	return instance;
}

HeightSensor::~HeightSensor() {
	// TODO Auto-generated destructor stub
}

uint16_t HeightSensor::getHeight(void) {


  uint16_t height;

  // write opcode to start conversion
  io::GPIO::instance().clearBits(startConversionAddressOffset,0xFF);
  io::GPIO::instance().setBits(startConversionAddressOffset,startConversionOpCode);

  //wait for conversion to complete
  usleep(10);

  //read low bytes
  height = io::GPIO::instance().read(readLowAddressOffset);

  //read high bytes
  height |= (uint16_t)io::GPIO::instance().read(readHighAddressOffset) << 8;

  // height sensor value is only 12 bit
  height &= only12BitBitmask;

  return height;
}


}/*sensors*/
}/*hardwareLayer*/
