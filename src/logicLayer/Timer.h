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

class TimerEvent {
public:
	TimerEvent(Signal signal, logicLayer::Channel<Signal>* receiverChannel)
	: signal(signal)
	, receiverChannel(receiverChannel)
	, active(true)
	, dead(false)
	{}

	TimerEvent()
	: receiverChannel(nullptr)
	, active(false)
	, dead(false)
	{}

	Signal signal;
	logicLayer::Channel<Signal>* receiverChannel;
	bool active;
	bool dead;
};

class Timer : public SignalReceiver {
public:
	Timer();
	virtual ~Timer();
	void operator()();
	void setControllerChannel(Channel<Signal>*);
private:
	void startTimer(TimerEvent&);

	Channel<Signal>* controller_channel;
};

} // end namespace

#endif /* TIMER_H_ */
