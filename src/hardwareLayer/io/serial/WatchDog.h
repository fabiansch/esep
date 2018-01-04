/*
 * WatchDog.h
 *
 *  Created on: 30.10.2017
 *      Author: abx724
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_


#include "SignalGenerator.h"
#include <thread>


namespace hardwareLayer {
namespace io {
namespace serial {

class Serial;

enum class Connection { CONNECTED, LOST };

class WatchDog {
public:
	WatchDog(Serial* serial, SignalGenerator& sgen);
	virtual ~WatchDog();
	void operator()();
	void terminate();
	void feed();
private:
	Serial* serial_;
	SignalGenerator& sgen_;
	std::thread watchdog;
	bool dogWasFed;
	Connection status;
	bool running;
};

} /* namespace serial */
} /* namespace io */
} /* namespace hal */

#endif /* WATCHDOG_H_ */
