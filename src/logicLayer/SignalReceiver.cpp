/*
 * SignalReceiver.cpp
 *
 *  Created on: 14.11.2017
 *      Author: abx724
 */
#include "Header.h"
#include "SignalReceiver.h"


namespace logicLayer {

	SignalReceiver::SignalReceiver()
	: channel_(100)
	, running(true)
	{
		LOG_SCOPE
	}

	SignalReceiver::~SignalReceiver() {
		LOG_SCOPE
		terminate();
	}

	Channel<Signal>& SignalReceiver::getChannel() {
		LOG_SCOPE
		return channel_;
	}

	void SignalReceiver::terminate() {
		LOG_SCOPE
		running = false;
		channel_.destroy();
		receiver_.join();
	}


} /* namespace logicLayer */
