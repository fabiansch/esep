/*
 * Timer.cpp
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#include "Header.h"
#include "Timer.h"
#include "Signals.h"

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Timer::~Timer() {
	LOG_SCOPE
}


void Timer::operator()() {
	LOG_SCOPE
	Signal signal;
	while(running) {
		signal << channel_;
		LOG_DEBUG<<"Timer got signal"<<endl;

		switch (signal.name) {
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			WAIT(550);
			*controller_channel << Signal(Signalname::TIMEFRAME_INPUT_ENTER);
			cout<<"Timer sent TIMEFRAME_INPUT_ENTER"<<endl;
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR<<"Timer got unknown Signal"<<endl;
			exit(EXIT_FAILURE);
			break;
		}
	}

}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


////////

class later
{
public:
    template <class callable, class... arguments>
    later(int after, bool async, callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }

};

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

void timer(TimerEvent& timerEvent)
{
	if(timerEvent.active) {
		if(timerEvent.receiverChannel) {
			*(timerEvent.receiverChannel) << timerEvent.signal;
		} else {
			//error nullptr
		}
	}
	timerEvent.dead = true;
	return;
}

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
, controller_channel(nullptr)
{
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Timer::~Timer() {
}


void Timer::operator()() {
	Signal signal;
	TimerEvent timer_events[100];
	int i = 0;

	while(running) {

		signal << channel_;

		switch (signal.name) {
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;

			{
				timer_events[i] = TimerEvent(
										Signal(Signalname::TIMEFRAME_INPUT_ENTER),
										controller_channel);
				later(
					1000
					, true
					, &timer
					, std::ref(timer_events[i])
				);
				i++;

				timer_events[i] = TimerEvent(
										Signal(Signalname::TIMEFRAME_INPUT_LEAVE),
										controller_channel);
				later(
					2000
					, true
					, &timer
					, std::ref(timer_events[i])
				);
				timer_events[i].active = false;
				i++;



			}
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			exit(EXIT_FAILURE);
			break;
		}
	}

}
//////////////


} /* namespace logicLayer */
