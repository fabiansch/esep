/*
 * Message.h
 *
 *  Created on: 30.10.2017
 *      Author: Flo
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Header.h"
#include "Item.h"
#include "Signals.h"


namespace hardwareLayer {
namespace io {
namespace serial {


constexpr int CORRECT_CN = 654321;
constexpr int WRONG_CN = 123456;

struct Message {
	Message() :
	Message(Signal(), CORRECT_CN)
	{

	}

	Message(Item item) :
	Message(Signal(cb_this, cb_next, Signalname::SERIAL_TRANSFER_ITEM))
	{
		this->item = item;
	}

	Message(Signal signal) :
	Message(signal, CORRECT_CN)
	{

	}

	Message(bool correctCheckNumber) :
	Message(Signal(), correctCheckNumber ? CORRECT_CN : WRONG_CN)
	{

	}

	Message(Signal signal, int checkNumber) :
	checkNumber(checkNumber),
	signal(signal)
	{

	}


	int checkNumber;
	Signal signal;
	Item item;
};

} /* namespace serial */
} /* namespace io */
} /* namespace hal */





#endif /* MESSAGE_H_ */
