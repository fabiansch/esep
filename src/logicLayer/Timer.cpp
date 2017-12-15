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
			cout << std::chrono::duration_cast<std::chrono::milliseconds>(event.duration).count() << endl;
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
	i = 0;
	initialize();
	speed = Speed::FAST;
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Timer::~Timer() {
	LOG_SCOPE
}

void fire_timer(TimerEvent& timerEvent)
{
	if(timerEvent.active) {
		if(timerEvent.receiverChannel) {
			cout<<"FIRE"<<endl;
			*(timerEvent.receiverChannel) << timerEvent.signal;
		} else {
			//error nullptr
		}
	} else {
		cout<<"NOT FIRED"<<endl;
	}
	timerEvent.finished = true;
	return;
}

void Timer::operator()() {
	Signal signal;

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
			i++;
			break;
		case Signalname::START_TIMERS_HEIGHT:
			cout<<"Timer got START_TIMERS_HEIGHT"<<endl;
			setTimer(Signalname::TIMEFRAME_HEIGHT_ENTER,Signalname::TIMEFRAME_HEIGHT_LEAVE,time_input_to_height);
			break;
		case Signalname::START_TIMERS_SWITCH:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			setTimer(Signalname::TIMEFRAME_SWITCH_ENTER,Signalname::TIMEFRAME_SWITCH_LEAVE,time_height_to_switch);
			break;
		case Signalname::START_TIMERS_SLIDE:
			cout<<"Timer got START_TIMERS_SLIDE"<<endl;
			setTimer(Signalname::TIMEFRAME_SLIDE_ENTER,Signalname::TIMEFRAME_SLIDE_LEAVE,time_switch_to_slide);
			break;
		case Signalname::START_TIMERS_OUTPUT:
			cout<<"Timer got START_TIMERS_OUTPUT"<<endl;
			setTimer(Signalname::TIMEFRAME_OUTPUT_ENTER,Signalname::TIMEFRAME_OUTPUT_LEAVE,time_switch_to_output);
			break;
		case Signalname::MOTOR_STOP:
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			for(auto& event : timer_events) {
				event.active = false;
				if(event.finished == false) {
					event.duration = event.duration - (now - event.begin);
					cout<<"TIMER STOPPED"<<endl;
					event.started = false;
					event.finished = true;
				}
			}

			break;
		}
		case Signalname::MOTOR_START:
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			for(int j = 0; j<sizeof(timer_events)/sizeof(TimerEvent); j++) {
				if(timer_events[j].started == false) {
					timer_events[j].begin = now;
					timer_events[j].finished = false;
					timer_events[j].started = true;
					timer_events[j].active = true;
					later(&fire_timer, std::ref(timer_events[j]));
				}
			}
			break;
		}
		case Signalname::TIMEFRAME_INPUT_LEAVE_KILL:
			killTimer(Signalname::TIMEFRAME_INPUT_LEAVE);
			break;
		case Signalname::TIMEFRAME_HEIGHT_ENTER_KILL:
			killTimer(Signalname::TIMEFRAME_HEIGHT_ENTER);
			break;
		case Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL:
			cout<<"TIMEFRAME_HEIGHT_LEAVE_KILL SIGNAL"<<endl;
			killTimer(Signalname::TIMEFRAME_HEIGHT_LEAVE);
			break;
		case Signalname::TIMEFRAME_SWITCH_ENTER_KILL:
			killTimer(Signalname::TIMEFRAME_SWITCH_ENTER);
			break;
		case Signalname::TIMEFRAME_SWITCH_LEAVE_KILL:
			killTimer(Signalname::TIMEFRAME_SWITCH_LEAVE);
			break;
		case Signalname::TIMEFRAME_SLIDE_ENTER_KILL:
			killTimer(Signalname::TIMEFRAME_SLIDE_ENTER);
			break;
		case Signalname::TIMEFRAME_SLIDE_LEAVE_KILL:
			killTimer(Signalname::TIMEFRAME_SLIDE_LEAVE);
			break;
		case Signalname::TIMEFRAME_OUTPUT_ENTER_KILL:
			killTimer(Signalname::TIMEFRAME_OUTPUT_ENTER);
			break;
		case Signalname::TIMEFRAME_OUTPUT_LEAVE_KILL:
			killTimer(Signalname::TIMEFRAME_OUTPUT_LEAVE);
			break;
		case Signalname::MOTOR_FAST:
			speed = Speed::FAST;
			for (int j=0;j<sizeof(timer_events)/sizeof(TimerEvent);j++){
				if(timer_events[j].active == true){
					timer_events[j].duration = timer_events[j].duration * slow_factor;
				}
			}
			break;
		case Signalname::MOTOR_SLOW:
			speed = Speed::SLOW;
			for (int j=0;j<sizeof(timer_events)/sizeof(TimerEvent);j++){
				if(timer_events[j].active == true){
					timer_events[j].duration = timer_events[j].duration / slow_factor;
				}
			}
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR << __FUNCTION__ <<": Timer doen not handle signal: " << (int)signal.name <<endl;
			exit(EXIT_FAILURE);
			break;
		}
	}

}

bool Timer::killTimer(Signalname signalname){
	for(auto& event : timer_events){ // TODO walk from head to 0 and then from max to head
		if (event.signal.name == signalname){
			event.active = false;
			cout<<"TIMER KILLED"<<endl;
			return true;
		}
	}
	return false;
}

void Timer::checkIfAvailableSpace(){
	if (i == sizeof(timer_events)/sizeof(TimerEvent)-1){
		i = 0;
	}
	if (timer_events[i].active == true){
		LOG_ERROR<<__FUNCTION__<<"Trying to overwrite active timer which was expected to be inactive";
		//TODO FAIL
	}
}
void Timer::initialize(){
	for (int j =0;j< sizeof(timer_events)/sizeof(TimerEvent);j++ ){
		timer_events[j] = TimerEvent();
	}
}

void Timer::setTimer(Signalname entry, Signalname exit, unsigned int param){
	checkIfAvailableSpace();
	if (speed == Speed::FAST){
		timer_events[i] = TimerEvent(
								std::chrono::milliseconds(param-500),
								Signal(entry),
								controller_channel);
	}
	else{
		auto time = std::chrono::milliseconds(param -500);
		time = time / slow_factor;
		timer_events[i] = TimerEvent(
								std::chrono::milliseconds(time),
								Signal(entry),
								controller_channel);
	}

	later(&fire_timer, std::ref(timer_events[i]));
	i++;

	checkIfAvailableSpace();
	// TODO if else
	timer_events[i] = TimerEvent(
							std::chrono::milliseconds(param+500),
							Signal(exit),
							controller_channel);
	later(&fire_timer, std::ref(timer_events[i]));
	i++;
}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


} /* namespace logicLayer */
