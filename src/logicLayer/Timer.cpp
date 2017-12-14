/*
 * Timer.cpp
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#include "Header.h"
#include "Timer.h"
#include "Signals.h"

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

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
, controller_channel(nullptr)
{
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Timer::~Timer() {
	LOG_SCOPE
}

void timer(TimerEvent& timerEvent)
{
	if(timerEvent.active) {
		if(timerEvent.receiverChannel) {
			*(timerEvent.receiverChannel) << timerEvent.signal;
		} else {
			//error nullptr
		}
	}
	timerEvent.finished = true;
	return;
}

void Timer::operator()() {
	Signal signal;
	TimerEvent timer_events[100];
	int i = 0;

	while(running) {

		signal << channel_;

		switch (signal.name) {
		case Signalname::START_TIMERS_HEIGHT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;

				timer_events[i] = TimerEvent(
										std::chrono::milliseconds(2154-500),
										Signal(Signalname::TIMEFRAME_HEIGHT_ENTER),
										controller_channel);
				startTimer(std::ref(timer_events[i]));
				i++;

				timer_events[i] = TimerEvent(
										std::chrono::milliseconds(2154+500),
										Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE),
										controller_channel);
				startTimer(std::ref(timer_events[i]));
//				timer_events[i].active = false;
				i++;

			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			exit(EXIT_FAILURE);
			break;
		}
	}

}

void Timer::startTimer(TimerEvent& timerEvent) {
	later(
		std::chrono::duration_cast<std::chrono::milliseconds>(timerEvent.duration).count()
		, true
		, &timer
		, timerEvent
	);
	// timerEvent.started = true;
}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


} /* namespace logicLayer */
