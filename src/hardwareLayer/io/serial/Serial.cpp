/*
 * Serial.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abj240
 */

#include "Serial.h"

namespace hardwareLayer {
namespace io {
namespace serial {

constexpr auto COM1 = "/dev/ser1";
constexpr auto COM2 = "/dev/ser2";

Serial::Serial(SignalGenerator& signalGenerator) :
_serialClockwise(COM1, COM2),
_watchDog(_serialClockwise, signalGenerator),
_receiver(_serialClockwise, _watchDog, signalGenerator)
{

}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

void Serial::send(Signal& signal) {
	// TODO error handling
	Message message(signal);
	_serialClockwise.send(message);
}

void Serial::send(Item* item) {
	// TODO error handling
	Message message(*item);
	_serialClockwise.send(message);
}

Receiver& Serial::getReceiver() {
	return _receiver;
}

} /* namespace serial */
} /* namespace io */
} /* namespace hardwareLayer */
