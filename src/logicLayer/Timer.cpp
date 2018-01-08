/*
 * Timer.cpp
 *
 *  Created on: 13.11.2017
 *      Author: aca311
 */

#include "Header.h"
#include "Timer.h"
#include "Signals.h"
#include "ThreadPool.h"

ThreadPool thread_pool(logicLayer::SIZE);

class later
{
public:
    template <class callable, class... arguments>
    later(callable&& f, arguments&&... args)
    {
      std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

      std::tuple<arguments...> tuples{args...};
      logicLayer::TimerEvent& event = std::get<0>(tuples);

   		thread_pool.enqueue([event, task]() {
				cout << std::chrono::duration_cast<std::chrono::milliseconds>(event.duration).count() << endl;
				WAIT(std::chrono::duration_cast<std::chrono::milliseconds>(event.duration).count());
				task();
			});
		}

};

namespace logicLayer {

Timer::Timer()
: SignalReceiver::SignalReceiver()
, controller_channel(nullptr)
, i(0)
, speed(Speed::fast)
, stopped(true)
{
	i = 0;
	initialize();
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
	} else {
	}
	timerEvent.finished = true;
	return;
}

void Timer::operator()() {
	Signal signal;
	int head_saved;
	std::chrono::steady_clock::time_point now;
	while(running) {

		signal << channel_;

		switch (signal.name) {
		case Signalname::SWITCH_CLOSE:
			setNewTimerEvent(Signalname::SWITCH_CLOSE, 1000);
			break;
		case Signalname::START_TIMERS_INPUT:
			setNewTimerEvent(Signalname::TIMEFRAME_INPUT_LEAVE, time_output_to_input + 2500);
			break;
		case Signalname::START_TIMERS_HEIGHT:
			setNewTimerEvent(Signalname::TIMEFRAME_HEIGHT_ENTER, time_input_to_height - 500);
			setNewTimerEvent(Signalname::TIMEFRAME_HEIGHT_LEAVE, time_input_to_height + 500);
			break;
		case Signalname::START_TIMERS_SWITCH:
			setNewTimerEvent(Signalname::TIMEFRAME_SWITCH_ENTER,time_height_to_switch - 500);
			setNewTimerEvent(Signalname::TIMEFRAME_SWITCH_LEAVE,time_height_to_switch + 500);
			break;
		case Signalname::START_TIMERS_SLIDE:
			setNewTimerEvent(Signalname::TIMEFRAME_SLIDE_ENTER,500);
			setNewTimerEvent(Signalname::TIMEFRAME_SLIDE_LEAVE,time_switch_to_slide + 2000);
			break;
		case Signalname::START_TIMERS_OUTPUT:
			setNewTimerEvent(Signalname::TIMEFRAME_OUTPUT_ENTER,time_switch_to_output - 500);
			setNewTimerEvent(Signalname::TIMEFRAME_OUTPUT_LEAVE,time_switch_to_output + 500);
			break;
		case Signalname::MOTOR_STOP:
			pauseAll();
			stopped = true;
			break;
		case Signalname::MOTOR_START:
			stopped = false;
			startAll();
			break;
		case Signalname::TIMEFRAME_INPUT_LEAVE_KILL:
			killTimer(Signalname::TIMEFRAME_INPUT_LEAVE);
			break;
		case Signalname::TIMEFRAME_HEIGHT_ENTER_KILL:
			killTimer(Signalname::TIMEFRAME_HEIGHT_ENTER);
			break;
		case Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL:
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
			now = std::chrono::steady_clock::now();
			speed = Speed::fast;
			head_saved = i;
			for(int j = 0; j < SIZE; j++) {
				int index = (head_saved + j) % SIZE;
				if(!timer_events[index].finished && timer_events[index].active && timer_events[index].speed == Speed::slow) {
					setModifiedTimerEvent(timer_events[index], stopped ? false : true, now);
					timer_events[index].active = false;
				}
			}
			break;
		case Signalname::MOTOR_SLOW:
			now = std::chrono::steady_clock::now();
			speed = Speed::slow;
			head_saved = i;
			for(int j = 0; j < SIZE; j++) {
				int index = (head_saved + j) % SIZE;
				if(!timer_events[index].finished && timer_events[index].active && timer_events[index].speed == Speed::fast) {
					setModifiedTimerEvent(timer_events[index], stopped ? false : true, now);
					timer_events[index].active = false;
				}
			}
			break;
		case Signalname::SIGNAL_DUMMY:
			break;
		default:
			LOG_ERROR << __FUNCTION__ <<": Timer does not handle signal: " << (int)signal.name <<endl;
			exit(EXIT_FAILURE);
			break;
		}
	}

}

void Timer::killTimer(Signalname signalname){

	int head_saved = i;

	for(int j = 0; j < SIZE; j++) {
		int index = (head_saved + j) % SIZE;

		if (timer_events[index].signal.name == signalname && timer_events[index].active){
			timer_events[index].active = false;
			return;
		}
	}

}


void Timer::checkIfAvailableSpace(){
	if (i >= SIZE-1) {
		i = 0;
	}
	if (!timer_events[i].finished){
		LOG_ERROR<<__FUNCTION__<<"Trying to overwrite unfinished timer which was expected to be finished.";
		//TODO FAIL
	}
}
void Timer::initialize(){
	for (int j = 0;j < SIZE; j++){
		timer_events[j] = TimerEvent();
	}
}

void Timer::setNewTimerEvent(Signalname signal, unsigned int time){
	checkIfAvailableSpace();
	if (speed == Speed::slow){
		time = time / slow_factor;
	}
	timer_events[i] = TimerEvent(
							std::chrono::milliseconds(time),
							Signal(signal),
							controller_channel,
							speed);
	if (not stopped) {
		later(&fire_timer, std::ref(timer_events[i]));
	}
	else {
		timer_events[i].started = false;
	}
	i++;
}

void Timer::setModifiedTimerEvent(TimerEvent old, bool run, std::chrono::steady_clock::time_point now){
	checkIfAvailableSpace();
	std::chrono::steady_clock::duration duration;
	std::chrono::steady_clock::duration duration_passed(std::chrono::milliseconds::zero());
	if( not stopped ) {
		duration_passed = now - old.begin;
	}

	if(speed == Speed::fast && old.speed == Speed::slow){
		duration = (old.duration - duration_passed) * (int)(slow_factor*1000);
		duration /= 1000;

	}
	else if (speed == Speed::slow && old.speed == Speed::fast){
		duration = (old.duration - duration_passed) * 1000 / (int)(slow_factor * 1000);
	}
	else {
		duration = old.duration - duration_passed;
	}

	timer_events[i] = TimerEvent(
								duration,
								Signal(old.signal),
								controller_channel,
								speed);
	if (run) {
		later(&fire_timer, std::ref(timer_events[i]));
	}
	else {
		timer_events[i].started = false;
	}
	i++;
}

void Timer::pauseAll(){
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	int head_saved = i;

	for(int j = 0; j < SIZE; j++) {
		int index = (head_saved + j) % SIZE;
		if (timer_events[index].active && (!timer_events[index].finished)) {
			setModifiedTimerEvent(timer_events[index], false, now);
			timer_events[index].active = false;
		}
	}
}

void Timer::startAll(){
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	int head_saved = i;

	for(int j = 0; j < SIZE; j++) {
		int index = (head_saved + j) % SIZE;
		if(!timer_events[index].started){
			timer_events[index].begin = now;
			timer_events[index].started = true;
			later(&fire_timer, std::ref(timer_events[index]));
		}
	}

}

void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


} /* namespace logicLayer */
