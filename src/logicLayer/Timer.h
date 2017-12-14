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
	void setControllerChannel(Channel<Signal>*);
private:
	Channel<Signal>* controller_channel;
};

} // end namespace

#endif /* TIMER_H_ */
