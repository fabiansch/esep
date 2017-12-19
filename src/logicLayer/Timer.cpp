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
, speed(Speed::FAST)
, controller_channel(nullptr)
{
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
	//timerEvent.finished = true;
	return;
}

void Timer::operator()() {
	Signal signal;

	while(running) {

		signal << channel_;

		switch (signal.name) {
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;

			setTimerEvent(Signalname::TIMEFRAME_INPUT_LEAVE, time_output_to_input-TOLERANCE, true, controller_channel);
			setTimerEvent(Signalname::TIMEFRAME_INPUT_ENTER, time_output_to_input+TOLERANCE, true, controller_channel);

			break;
		case Signalname::START_TIMERS_HEIGHT:
			cout<<"Timer got START_TIMERS_HEIGHT"<<endl;

			setTimerEvent(Signalname::TIMEFRAME_HEIGHT_ENTER, time_output_to_input-TOLERANCE, true, controller_channel);
			setTimerEvent(Signalname::TIMEFRAME_HEIGHT_LEAVE, time_output_to_input+TOLERANCE, true, controller_channel);

			break;
		case Signalname::START_TIMERS_SWITCH:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			setTimers(Signalname::TIMEFRAME_SWITCH_ENTER,Signalname::TIMEFRAME_SWITCH_LEAVE,time_height_to_switch);
			break;
		case Signalname::START_TIMERS_SLIDE:
			cout<<"Timer got START_TIMERS_SLIDE"<<endl;
			setTimers(Signalname::TIMEFRAME_SLIDE_ENTER,Signalname::TIMEFRAME_SLIDE_LEAVE,time_switch_to_slide);
			break;
		case Signalname::START_TIMERS_OUTPUT:
			cout<<"Timer got START_TIMERS_OUTPUT"<<endl;
			setTimers(Signalname::TIMEFRAME_OUTPUT_ENTER,Signalname::TIMEFRAME_OUTPUT_LEAVE,time_switch_to_output);
			break;

		case Signalname::MOTOR_STOP:

			pauseAll();

			break;
		case Signalname::MOTOR_START:


			break;

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
		{
			if (speed == Speed::SLOW){
				std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
				int j = i;
				do {
					if(timer_events[j].active == true){
						//TODO hier wird die zurückgelegte zeit abgezogen. darf aber NICHT passieren, wenn motor stopp ist. D.h. state oder flag benötigt.
						timer_events[j].duration = timer_events[j].duration - (now - timer_events[j].begin);
						timer_events[j].duration = timer_events[j].duration * slow_factor;
					}
					if(j ==0){
						j = sizeof(timer_events)/sizeof(TimerEvent);
					}
					j--;
				} while ( j != i );
			}
			break;
		}
		case Signalname::MOTOR_SLOW:
		{
			if (speed == Speed::SLOW){
				std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
				int j = i;
				do {
					if(timer_events[j].active == true){
						timer_events[j].duration = timer_events[j].duration - (now - timer_events[j].begin);
						timer_events[j].duration = timer_events[j].duration / slow_factor;
					}
					if(j ==0){
						j = sizeof(timer_events)/sizeof(TimerEvent)-1;
					}
					j--;
				} while ( j != i );
			}
			break;
		}
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR << __FUNCTION__ <<": Timer doen not handle signal: " << (int)signal.name <<endl;
			exit(EXIT_FAILURE);
			break;
		}
	}
}

void Timer::pauseAll() {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	for ( int i=0; i<sizeof(timer_events) / sizeof(TimerEvent); i++) {
		if(not timer_events[i].active) {
			makeInactive(std::ref(timer_events[i]));
		}
	}



//			OLD CODE:
//		//timer_events[j].duration = timer_events[j].duration - (now - timer_events[j].begin);
//		cout<<"TIMER STOPPED"<<endl;
//
//		if(timer_events[j].active) {
//			modifyTimer(timer_events[j].signal, timer_events[j].duration - (now - timer_events[j].begin));
//			timer_events[j].started = false;
//		}
//
//		timer_events[j].finished = true;
//		timer_events[j].active = false;
//
//		if (j==0){
//			j = sizeof(timer_events)/sizeof(TimerEvent);
//		}
}

void Timer::startAll() {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// alle 'not started' timer starten
	int j = i;
	do {
		if(timer_events[j].started == false) {
			timer_events[j].begin = now;
			timer_events[j].finished = false;
			timer_events[j].started = true;
			timer_events[j].active = true;
			later(&fire_timer, std::ref(timer_events[j]));
		}
		if (j==0){
			j = sizeof(timer_events)/sizeof(TimerEvent);
		}
		j--;
	} while ( j != i );
}

bool Timer::killTimerEvent(Signalname signalname){
	for(auto& event : timer_events){ // TODO walk from head to 0 and then from max to head, kill first others not
		if (event.signal.name == signalname){
			event.active = false;
			cout<<"TIMER KILLED"<<endl;
		}
	}
	return false;
}

int Timer::getAvailablePos(){
	for ( int i=0; i<sizeof(timer_events)/sizeof(TimerEvent); i++) {
		if(not timer_events[i].active) {
			return i;
		}
	}
	LOG_ERROR << __FUNCTION__ << " Timer: no free places in timer array"<<endl;
	cout<<" ERROR: Timer: no free places in timer array"<<endl;
	return -1; // cause of warning
}

void Timer::initialize(){
	for (int j =0;j< sizeof(timer_events)/sizeof(TimerEvent);j++ ){
		timer_events[j] = TimerEvent();
	}
}

void Timer::modifyTimerEvent(Signalname signal, std::chrono::steady_clock::duration duration){
	timer_events[i] = TimerEvent(duration, Signal(signal),controller_channel);
	later(&fire_timer, std::ref(timer_events[i]));
	i++;
}

void Timer::setTimerEvent(Signalname entry, unsigned int millies, bool active, Channel<Signal>* receiver){
	int i = getAvailablePos();
	timer_events[i] = TimerEvent(
							std::chrono::milliseconds(millies),
							Signal(entry),
							receiver);

	later(&fire_timer, std::ref(timer_events[i]));
}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}

} /* namespace logicLayer */
