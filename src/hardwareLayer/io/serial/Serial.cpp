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

Serial::Serial(SignalGenerator& signalGenerator)
: signalGenerator(signalGenerator)
, _serialClockwise(COM1, COM2)
, _receiver(_serialClockwise, _watchDog, signalGenerator)
, _watchDog(this, signalGenerator)
{
	LOG_SCOPE
}

Serial::~Serial() {
	LOG_SCOPE
}

void Serial::send(Signal& signal) {
	// TODO error handling
	Message message(signal);
	_serialClockwise.send(message);
}

void Serial::send(logicLayer::Item* item) {
	// TODO error handling
	Message message(*item);
	_serialClockwise.send(message);
}

void Serial::flush() {
  _serialClockwise.flush();
  _serialClockwise.flush();
  _serialClockwise.flush();
  _serialClockwise.send(Signal(cb_this, cb_all, Signalname::SERIAL_FLUSH));
}

Receiver& Serial::getReceiver() {
	return _receiver;
}

} /* namespace serial */
} /* namespace io */
} /* namespace hardwareLayer */
