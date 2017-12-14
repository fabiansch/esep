/*
 * Timer.h
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>
#include <chrono>

#include "Channel.h"
#include "SignalReceiver.h"

namespace logicLayer {

class TimerEvent {
public:
	TimerEvent(int end, Signal signal, logicLayer::Channel<Signal>* receiverChannel)
	: begin(0)
	, end(end)
	, signal(signal)
	, receiverChannel(receiverChannel)
	, started(true)
	, active(true)
	, finished(false)
	{}

	TimerEvent()
	: begin(0)
	, end(0)
	, receiverChannel(nullptr)
	, started(true)
	, active(false)
	, finished(false)
	{
	}

	int begin;
	int end;

	Signal signal;
	logicLayer::Channel<Signal>* receiverChannel;

	bool started;
	bool active;
	bool finished;
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
