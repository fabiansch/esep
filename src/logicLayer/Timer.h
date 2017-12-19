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

const int TIMER_EVENTS = 200;
const int TOLERANCE = 500;

namespace logicLayer {

class TimerEvent {
public:
	TimerEvent(std::chrono::steady_clock::duration duration, Signal signal, logicLayer::Channel<Signal>* receiverChannel)
	: begin(std::chrono::steady_clock::now())
	, duration(duration)
	, signal(signal)
	, receiverChannel(receiverChannel)
	, started(true)
	, active(true)
	{}

	TimerEvent()
	: begin(std::chrono::steady_clock::now())
	, duration(std::chrono::steady_clock::duration())
	, receiverChannel(nullptr)
	, started(true)
	, active(false)
	{}

	const std::chrono::steady_clock::time_point begin;
	const std::chrono::steady_clock::duration duration;

	Signal signal;
	logicLayer::Channel<Signal>* receiverChannel;

	bool started;
	bool active;
};

enum class Speed {FAST, SLOW};

class Timer : public SignalReceiver {

public:
	Timer();
	virtual ~Timer();
	void operator()();
	void setControllerChannel(Channel<Signal>*);


private:
	Speed speed;
	TimerEvent timer_events[TIMER_EVENTS];

	bool killTimerEvent(Signalname);
	void setTimerEvent(Signalname, unsigned int millies, bool active, Channel<Signal>* receiver);
	void pauseAll();
	void startAll();

	void makeInactive(TimerEvent&);
	void modifyTimerEvent(Signalname,std::chrono::steady_clock::duration);
	int getAvailablePos();
	void fire_timer(TimerEvent&);
	void initialize();

	Channel<Signal>* controller_channel;
};

} // end namespace

#endif /* TIMER_H_ */
