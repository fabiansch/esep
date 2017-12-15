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
    later(callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        std::tuple<arguments...> tuples{args...};
        logicLayer::TimerEvent& event = std::get<0>(tuples);

        std::thread([event, task]() {
			WAIT(std::chrono::duration_cast<std::chrono::milliseconds>(event.duration).count());
			task();
		}).detach();
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

void fire_timer(TimerEvent& timerEvent)
{
	if(timerEvent.active) {
		if(timerEvent.receiverChannel) {
			*(timerEvent.receiverChannel) << timerEvent.signal;
		} else {
			//error nullptr
		}
	}
	cout<<"FIRE"<<endl;
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
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_output_to_input+500),
									Signal(Signalname::TIMEFRAME_INPUT_LEAVE),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
//				timer_events[i].active = false;
			i++;
			break;
		case Signalname::START_TIMERS_HEIGHT:
			cout<<"Timer got START_TIMERS_HEIGHT"<<endl;

			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_input_to_height-500),
									Signal(Signalname::TIMEFRAME_HEIGHT_ENTER),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
			i++;

			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_input_to_height+500),
									Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
//				timer_events[i].active = false;
			i++;
			break;
		case Signalname::START_TIMERS_SWITCH:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;

			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_height_to_switch-500),
									Signal(Signalname::TIMEFRAME_SWITCH_ENTER),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
			i++;

			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_height_to_switch+500),
									Signal(Signalname::TIMEFRAME_SWITCH_LEAVE),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
//				timer_events[i].active = false;
			i++;
			break;
		case Signalname::START_TIMERS_SLIDE:
			cout<<"Timer got START_TIMERS_SLIDE"<<endl;
			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_switch_to_slide-500),
									Signal(Signalname::TIMEFRAME_SLIDE_ENTER),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
			i++;
			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_switch_to_slide-500),
									Signal(Signalname::TIMEFRAME_SLIDE_LEAVE),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
			i++;
			break;
		case Signalname::START_TIMERS_OUTPUT:
			cout<<"Timer got START_TIMERS_OUTPUT"<<endl;
			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_switch_to_output-500),
									Signal(Signalname::TIMEFRAME_OUTPUT_ENTER),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
			i++;

			timer_events[i] = TimerEvent(
									std::chrono::milliseconds(time_switch_to_output+500),
									Signal(Signalname::TIMEFRAME_OUTPUT_LEAVE),
									controller_channel);
			later(&fire_timer, std::ref(timer_events[i]));
//				timer_events[i].active = false;
			i++;
			break;
		case Signalname::MOTOR_STOP:
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			for(auto& event : timer_events) {
				event.active = false;
				if(event.finished == false) {
					event.duration = event.duration - (now - event.begin);
					event.started = false;
				}
			}
			break;
		}
		case Signalname::MOTOR_START:
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			for(auto& event : timer_events) {
				if(event.started == false) {
					event.begin = now;
					event.started = true;
					event.active = true;
					later(&fire_timer, event);
				}
			}
			break;
		}
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			exit(EXIT_FAILURE);
			break;
		}
	}

}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


} /* namespace logicLayer */
