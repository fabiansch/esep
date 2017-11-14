/*
 * Timer.h
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>

#include "Channel.h"
#include "SignalReceiver.h"

namespace logicLayer {

class Timer : public SignalReceiver {
public:
	Timer();
	virtual ~Timer();
	void operator()();
private:
};

} // end namespace

#endif /* TIMER_H_ */
