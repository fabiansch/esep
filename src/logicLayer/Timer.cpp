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
	speed = Speed::fast;
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
	int j;
	int head_saved;
	std::chrono::steady_clock::time_point now;
	while(running) {

		signal << channel_;

		switch (signal.name) {
		case Signalname::START_TIMERS_INPUT:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			setTimerEvent(Signalname::TIMEFRAME_INPUT_LEAVE, time_output_to_input + 2000, true);
			break;
		case Signalname::START_TIMERS_HEIGHT:
			cout<<"Timer got START_TIMERS_HEIGHT"<<endl;
			setTimerEvent(Signalname::TIMEFRAME_HEIGHT_ENTER, time_input_to_height - 500, true);
			setTimerEvent(Signalname::TIMEFRAME_HEIGHT_LEAVE, time_input_to_height + 500, true);
			break;
		case Signalname::START_TIMERS_SWITCH:
			cout<<"Timer got START_TIMERS_INPUT"<<endl;
			setTimerEvent(Signalname::TIMEFRAME_SWITCH_ENTER,time_height_to_switch - 500, true);
			setTimerEvent(Signalname::TIMEFRAME_SWITCH_LEAVE,time_height_to_switch + 500, true);
			break;
		case Signalname::START_TIMERS_SLIDE:
			cout<<"Timer got START_TIMERS_SLIDE"<<endl;
			setTimerEvent(Signalname::TIMEFRAME_SLIDE_ENTER,1000, true);
			setTimerEvent(Signalname::TIMEFRAME_SLIDE_LEAVE,time_switch_to_slide + 2000, true);
			break;
		case Signalname::START_TIMERS_OUTPUT:
			cout<<"Timer got START_TIMERS_OUTPUT"<<endl;
			setTimerEvent(Signalname::TIMEFRAME_OUTPUT_ENTER,time_switch_to_output - 500, true);
			setTimerEvent(Signalname::TIMEFRAME_OUTPUT_LEAVE,time_switch_to_output + 500, true);
			break;
		case Signalname::MOTOR_STOP:
			pauseAll();
			break;
		case Signalname::MOTOR_START:
			startAll();
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
			cout<<"MOTOR_FAST"<<endl;
			now = std::chrono::steady_clock::now();
			speed = Speed::fast;
			head_saved = i;
			for(int j = 0; j < SIZE; j++) {
				int index = (head_saved + j) % SIZE;

				if(timer_events[index].active && timer_events[index].speed == Speed::fast){

						std::chrono::steady_clock::duration d1((((timer_events[index].duration-(now - timer_events[index].begin))*1000) * ((int)(slow_factor*1000))));
						cout<<"time" <<std::chrono::duration_cast<std::chrono::milliseconds>(d1).count()<<endl;

						setTimerEvent(timer_events[index].signal.name, d1 ,true);



						timer_events[index].active = false;
					}
				}
						break;
		case Signalname::MOTOR_SLOW: //TODO St�rtzt ab
			cout<<"MOTOR_SLOW"<<endl;
			now = std::chrono::steady_clock::now();
			speed = Speed::slow;
			head_saved = i;

			for(int j = 0; j < SIZE; j++) {
				int index = (head_saved + j) % SIZE;

				if(timer_events[index].active && timer_events[index].speed == Speed::fast){

						std::chrono::steady_clock::duration d1((((timer_events[index].duration-(now - timer_events[index].begin))*1000) / ((int)(slow_factor*1000))));
						cout<<"time" <<std::chrono::duration_cast<std::chrono::milliseconds>(d1).count()<<endl;

						setTimerEvent(timer_events[index].signal.name, d1 ,true);



						timer_events[index].active = false;
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
	int j = i;
	do{
		if (timer_events[j].signal.name == signalname && timer_events[j].active){
			timer_events[j].active = false;
			cout<<"TIMER KILLED"<<endl;
		}
		j++;
		if (j == sizeof(timer_events)/sizeof(TimerEvent)-1){
			j = 0;
		}

	}while(j!=i);
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

void Timer::setTimerEvent(Signalname signal, unsigned int milliseconds, bool start){
	checkIfAvailableSpace();

	timer_events[i] = TimerEvent(
							std::chrono::milliseconds(milliseconds),
							Signal(signal),
							controller_channel);
	if (start){
		later(&fire_timer, std::ref(timer_events[i]));
	}
	i++;
}

void Timer::setTimerEvent(Signalname signal, std::chrono::steady_clock::duration d, bool start) {
	setTimerEvent(signal, std::chrono::duration_cast<std::chrono::milliseconds>(d).count(), start);
}

void Timer::pauseAll(){
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	int j = i;
	int head_saved = i;
	do{
		if (timer_events[j].active){ //active prev
			setTimerEvent(timer_events[j].signal.name, (timer_events[j].duration - (now - timer_events[j].begin)) ,false);
			timer_events[j].active = false;
			timer_events[j].started = false;
			//timer_events[j].active = false; // prev not used
		}
		j++;
		if (j == (sizeof(timer_events)/sizeof(TimerEvent))){
			j = 0;
		}
	} while (j != head_saved);
	//			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//			for(auto& event : timer_events) {
	//				if(event.finished == false) {
	//					event.duration = event.duration - (now - event.begin);
	//					cout<<"TIMER STOPPED"<<endl;
	//					if(event.active) {
	//						event.started = false;
	//					}
	//					event.finished = true;
	//				}
	//				event.active = false;
	//			}
	//
	//			break;
}

void Timer::startAll(){
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	int j = i;
	do{
		if(!timer_events[j].started){
			timer_events[j].begin = now;
			timer_events[j].started = true;
			timer_events[j].active = true;
			later(&fire_timer, std::ref(timer_events[j]));
		}
		if (j ==  0){
			j = sizeof(timer_events)/sizeof(TimerEvent);
		}
		j--;
	}while (j != i);
}





void Timer::setControllerChannel(Channel<Signal>* controller) {
	controller_channel = controller;
}


} /* namespace logicLayer */
