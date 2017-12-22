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

constexpr int SIZE = 100;


class TimerEvent {
public:
	TimerEvent(std::chrono::steady_clock::duration duration, Signal signal, logicLayer::Channel<Signal>* receiverChannel)
	: begin(std::chrono::steady_clock::now())
	, duration(duration)
	, signal(signal)
	, receiverChannel(receiverChannel)
	, started(true)
	, active(true)
	, finished(false)
	, speed(Speed::fast)
	{}

	TimerEvent()
	: begin(std::chrono::steady_clock::now())
	, duration(std::chrono::steady_clock::duration())
	, receiverChannel(nullptr)
	, started(true)
	, active(false)
	, finished(true)
	, speed(Speed::fast)
	{
	}

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::duration duration;

	Signal signal;
	logicLayer::Channel<Signal>* receiverChannel;

	bool started;
	bool active;
	bool finished;

	Speed speed;
};

class Timer : public SignalReceiver {

public:
	Timer();
	virtual ~Timer();
	void operator()();
	void setControllerChannel(Channel<Signal>*);


private:
	Speed speed;
	bool killTimer(Signalname);
	void setTimerEvent(Signalname,unsigned int,bool);
	void setTimerEvent(Signalname,std::chrono::steady_clock::duration,bool);
	void initialize();
	TimerEvent timer_events[SIZE];
	void checkIfAvailableSpace();
	int i;
	void pauseAll();
	void startAll();


	Channel<Signal>* controller_channel;
};

} // end namespace

#endif /* TIMER_H_ */
