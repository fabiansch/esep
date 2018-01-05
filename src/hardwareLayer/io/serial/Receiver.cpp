/*
 * Receiver.cpp
 *
 *  Created on: 16.10.2017
 *      Author: Flo <florian.heuer@haw-hamburg.de>
 */

#include "Receiver.h"

namespace hardwareLayer {
namespace io {
namespace serial {

	Receiver::Receiver(Interface& serial, WatchDog& dog, SignalGenerator& sgen ):
	serial_(serial),
	dog_(dog),
	sgen_(sgen),
	running(true),
	_th_receiver(std::ref(*this))
	{
		LOG_SCOPE
	}

	Receiver::~Receiver() {
		LOG_SCOPE
		terminate();
		_th_receiver.join();
	}

	void Receiver::operator()(){
		LOG_SCOPE
		while(running) {


			Message msg = Message(false);
			LOG_DEBUG<<"MESSAGE RECEIVED"<<endl;

			//blocking io
			serial_.recv(&msg);


			//check sum is correct
			if(msg.checkNumber == CORRECT_CN ) {
				if((msg.signal.receiver & cb_this) > 0 || cb_this == 0) {

					switch (msg.signal.name) {
						case Signalname::SERIAL_WATCHDOG_TOKEN:
							if (cb_this == cb_1) {
								evaluateTokenAndSendFeed(msg);
							}
							registerOnToken(msg);
							forwardIfNotMaster(msg);
							break;
						case Signalname::SERIAL_WATCHDOG_FEED:
							evaluateFeed(msg);
							dog_.feed();
							forwardIfNotMaster(msg);
						break;
						case Signalname::SERIAL_FLUSH:
							serial_.flush();
							break;
						case Signalname::TRANSFER_ITEM:
							if(msg.signal.sender != cb_this) {

								itemBuffer_.pushItem(msg.item);
								sgen_.pushBackOnSignalBuffer(msg.signal);
							}
						break;
						default: // push signal to logic layer
							if(msg.signal.sender != cb_this) {
								sgen_.pushBackOnSignalBuffer(msg.signal);
								forward(msg);
							}
						break;
					}
				}

			}
			else if (msg.checkNumber == WRONG_CN) {  // timeout of blocking receive

			} else {
				serial_.flush();
				Message flushPreviousCB(Signal(cb_this, cb_previous, Signalname::SERIAL_FLUSH));
				serial_.send(flushPreviousCB);
			}
		}
	}

	void Receiver::terminate() {
		LOG_SCOPE
		running = false;
	}

	ItemBuffer& Receiver::getItemBuffer() {
		return itemBuffer_;
	}

	void Receiver::evaluateTokenAndSendFeed(const Message& msg) {
		if (cb_available == 0) {
			cb_available = msg.signal.sender;
			setNext_cb();
		}
		if (msg.signal.sender == cb_available) {
			if (cb_last == 0) {
				cb_last = (cb_available + 1) >> 1;
			}
			setPrevious_cb();
			setSorting_cbs();
			sendFeed();
		}
	}

	void Receiver::sendFeed() {
		Message feed(Signal(cb_this, cb_available, Signalname::SERIAL_WATCHDOG_FEED));
		serial_.send(feed);
	}

	void Receiver::registerOnToken(Message& msg) {
		if (cb_this != 0) {
			msg.signal.sender |= cb_this;
		} else if ((int) msg.signal.sender < 128) {
			cb_this = msg.signal.sender + 1;
			msg.signal.sender |= cb_this;
		} else {
			LOG_ERROR << __FUNCTION__ << "Machine number to big.";
			exit(EXIT_FAILURE);
		}
	}

	void Receiver::evaluateFeed(const Message& msg) {
		if (cb_available == 0) {
			cb_available = msg.signal.receiver;
			cb_last = (msg.signal.receiver + 1) >> 1;
			setNext_cb();
			setPrevious_cb();
			setSorting_cbs();
		}
	}

	void Receiver::forwardIfNotMaster(Message& msg) {
		if (cb_this != cb_1 && msg.signal.receiver > 0) {
			forward(msg);
		}
	}

	void Receiver::forward(Message& msg) {
		serial_.send(msg);
	}

	void Receiver::setPrevious_cb() {
		LOG_SCOPE
		if(cb_this == 0 or cb_available == 0 or cb_last == 0) {
			LOG_ERROR<<"cb_this or cb_available or cb_last not yet set."<<endl;
			exit(EXIT_FAILURE);
		} else {
			if((cb_this >> 1) == 0) {
				cb_previous = cb_last;
			} else {
				cb_previous = cb_this >> 1;
			}
		}
	}

	void Receiver::setNext_cb() {
		LOG_SCOPE
		if(cb_this == 0 or cb_available == 0) {
			LOG_ERROR<<"cb_this or cb_available not set yet."<<endl;
			exit(EXIT_FAILURE);
		} else {
			if(cb_this << 1 > cb_available) {
				cb_next = cb_first;
			} else {
				cb_next = cb_this << 1;
			}
		}
	}

	void Receiver::setSorting_cbs() {
		LOG_SCOPE
		switch (cb_available) {
		case 0b00000001:
			cb_sorting_1 = 0b00000001;
			cb_sorting_2 = 0b00000001;
			break;
		case 0b00000011:
			cb_sorting_1 = 0b00000001;
			cb_sorting_2 = 0b00000010;
			break;
		default:
			cb_sorting_1 = 0b00000010;
			cb_sorting_2 = 0b00000100;
			break;
		}
	}


} /* namespace serial */
} /* namespace io */
} /* namespace hal */


