/*
 * WatchDog.cpp
 *
 *  Created on: 30.10.2017
 *      Author: abx724
 */

#include "Header.h"
#include "WatchDog.h"

namespace hardwareLayer {
namespace io {
namespace serial {

constexpr int period = 250;
int connectionLostCounter = 0;

WatchDog::WatchDog(Interface& serial, SignalGenerator& sgen) :
serial_(serial),
sgen_(sgen),
watchdog(std::ref(*this)),
dogWasFed(false),
status(Connection::CONNECTED),
running(true)
{
	LOG_SCOPE
}

WatchDog::~WatchDog() {
	LOG_SCOPE

	terminate();
	watchdog.join();
}


void WatchDog::operator()(){
	LOG_SCOPE

	Message token( Signal(cb_1, cb_all, Signalname::SERIAL_WATCHDOG_TOKEN) );

	while(running) {

		dogWasFed = false;

		if(cb_this == cb_1) serial_.send(token);
		WAIT(period);
		if(cb_this == cb_1) serial_.send(token);
		WAIT(period);
		if(cb_this == cb_1) serial_.send(token);
		WAIT(period);

		dogWasFed ? connectionLostCounter = 0 : connectionLostCounter += 1;

		if(status == Connection::LOST && dogWasFed){
			status = Connection::CONNECTED;
			sgen_.pushBackOnSignalBuffer(Signal(Signalname::CONNECTION_CONNECTED));
		}
		else if(status == Connection::CONNECTED && !dogWasFed){
			status = Connection::LOST;
			sgen_.pushBackOnSignalBuffer(Signal(Signalname::CONNECTION_LOST));
		}

		if(connectionLostCounter >= 3) {
			serial_.flush(); WAIT(10);
			serial_.flush(); WAIT(10);
			serial_.flush();

			Message flushAll( Signal(cb_1, cb_all, Signalname::SERIAL_FLUSH) );
			serial_.send(flushAll);
		}

	}
}

void WatchDog::terminate() {
	LOG_SCOPE
	running = false;
}

void WatchDog::feed() {
	LOG_SCOPE
	dogWasFed = true;
}

} /* namespace serial */
} /* namespace io */
} /* namespace hal */
