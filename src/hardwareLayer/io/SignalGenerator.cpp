/*
 * InterruptHandler.cpp
 *
 *  Created on: 20.10.2017
 *      Author: abj240
 */

#include "SignalGenerator.h"
#include "Header.h"
#include "AsyncChannel.h"
#include "ISR.h"
#include "GPIO.h"
#include "Motor.h"
#include <iostream>
#include <ctime>



constexpr int MAGIC_NUMBER = 15;

namespace hardwareLayer {
namespace io {

const int NO_CHATTER_TIME = 0;
const int CHATTER_TIME = 40;
const int CHATTER_TIME_ESTOP = 750;
const int CHATTER_TIME_SWITCH = 100;


SensorEvent SignalGenerator::BUTTON_START(		0b00010000<<8, "BUTTON_START", CHATTER_TIME , SPair(				Signalname::BUTTON_START_PUSHED,
																													Signalname::BUTTON_START_PULLED));
SensorEvent SignalGenerator::BUTTON_STOP( 		0b00100000<<8, "BUTTON_STOP", CHATTER_TIME , SPair(					Signalname::BUTTON_STOP_PULLED,
																													Signalname::BUTTON_STOP_PUSHED));
SensorEvent SignalGenerator::BUTTON_RESET( 		0b01000000<<8, "BUTTON_RESET", CHATTER_TIME, SPair(					Signalname::BUTTON_RESET_PUSHED,
																													Signalname::BUTTON_RESET_PULLED));
SensorEvent SignalGenerator::BUTTON_E_STOP(		0b10000000<<8, "BUTTON_E_STOP", CHATTER_TIME_ESTOP, SPair(	  		Signalname::BUTTON_E_STOP_PULLED,
																						  	  	  	  	  	  	 	Signalname::BUTTON_E_STOP_PUSHED));
SensorEvent SignalGenerator::LIGHT_BARRIER_INPUT( 	0b00000001, "LIGHT_BARRIER_INPUT", CHATTER_TIME, SPair( 		Signalname::LB_INPUT_FREED,
																						  	  	  	  	  	  	  	Signalname::LB_INPUT_INTERRUPTED));
SensorEvent SignalGenerator::LIGHT_BARRIER_HEIGHT(	0b00000010, "LIGHT_BARRIER_HEIGHT", NO_CHATTER_TIME, SPair(		Signalname::LB_HEIGHT_FREED,
																						  	  	  	  	  	  	  	Signalname::LB_HEIGHT_INTERRUPTED));
SensorEvent SignalGenerator::SENSOR_HEIGHT_MATCH(	0b00000100, "SENSOR_HEIGHT_MATCH", NO_CHATTER_TIME, SPair( 		Signalname::SENSOR_HEIGHT_MATCH,
																													Signalname::SENSOR_HEIGHT_NOT_MATCH));
SensorEvent SignalGenerator::LIGHT_BARRIER_SWITCH(	0b00001000, "LIGHT_BARRIER_SWITCH", NO_CHATTER_TIME, SPair(		Signalname::LB_SWITCH_FREED,
																													Signalname::LB_SWITCH_INTERRUPTED));
SensorEvent SignalGenerator::SENSOR_METAL_MATCH(	0b00010000, "SENSOR_METAL_MATCH", NO_CHATTER_TIME, SPair(  		Signalname::SENSOR_METAL_MATCH,
																						  	  	  	  	  	  	  	Signalname::SENSOR_METAL_NOT_MATCH));
SensorEvent SignalGenerator::SENSOR_SWITCH_OPEN(	0b00100000, "SENSOR_SWITCH_OPEN", CHATTER_TIME_SWITCH, SPair(	Signalname::SENSOR_SWITCH_IS_OPEN,
																													Signalname::SENSOR_SWITCH_IS_CLOSED));
SensorEvent SignalGenerator::LIGHT_BARRIER_SLIDE(	0b01000000, "LIGHT_BARRIER_SLIDE", NO_CHATTER_TIME, SPair( 		Signalname::LB_SLIDE_FREED,
																						  	  	  	  	  	  	  	Signalname::LB_SLIDE_INTERRUPTED));
SensorEvent SignalGenerator::LIGHT_BARRIER_OUTPUT(	0b10000000, "LIGHT_BARRIER_OUTPUT", CHATTER_TIME, SPair(		Signalname::LB_OUTPUT_FREED,
																								  	  	  	  	  	Signalname::LB_OUTPUT_INTERRUPTED));

std::vector< SensorEvent>  SignalGenerator::events = init_events();

SignalGenerator::SignalGenerator()
: running(true)
, chatter_timer_th(std::thread(chatter_timer, this, nullptr))
{
	LOG_SCOPE
	mutex.lock();
	init_events();
	GPIO::instance().gainAccess();
	stored_mask = GPIO::instance().read(PORT::C)<<8 | GPIO::instance().read(PORT::B);
	ISR::registerISR(AsyncChannel::instance(), MAGIC_NUMBER);
	signal_generator_th = std::thread(std::ref(*this));
}

SignalGenerator::~SignalGenerator() {
	LOG_SCOPE
	terminate();
	AsyncChannel::instance().sendMessage({0,stored_mask});
	chatter_timer_th.join();
	signal_generator_th.join();
	ISR::unregisterISR();
}

void SignalGenerator::operator()() {
	LOG_SCOPE

	while (running) {
		AsyncMsg message;
		message = AsyncChannel::instance().nextMessage();
		int current_mask = (int)message.value;
		int change = current_mask xor stored_mask;
		for(auto &event : events) {
			if (change & event.bitmask) { // change happend on event?
				if (noChatterOn(event)) {
					if (event.bitmask & current_mask) { 	// low -> high
						pushBackOnSignalBuffer(Signal(event.signalPair.high));
					} else {								// high -> low
						pushBackOnSignalBuffer(Signal(event.signalPair.low));
					}
					stored_mask = current_mask;
				}
			}
		}
	}
}

void SignalGenerator::terminate() {
	LOG_SCOPE
	running = false;
}

Signal SignalGenerator::nextSignal() {
	LOG_SCOPE
	Signal signal(Signalname::SIGNAL_BUFFER_EMPTY);
	if (not signalBuffer.empty()) {
		signal = signalBuffer.front();
		signalBuffer.erase(signalBuffer.begin());
	}
	return signal;
}

bool SignalGenerator::noChatterOn(SensorEvent& event) {
	using namespace std::chrono;
	steady_clock::time_point timeNow = steady_clock::now();
	auto timeSinceLastInterrupt = duration_cast <milliseconds> (timeNow - event.lastTimeTriggered);
	if (timeSinceLastInterrupt.count() > event.chatterProtectionTime) {
		event.lastTimeTriggered = timeNow;
		if (event.chatterProtectionTime > 0) {
			chatter_timer_th.detach();
			chatter_timer_th = std::thread(chatter_timer, this, &event);
		}
		return true;
	}
	else {
		return false;
	}
}

void SignalGenerator::chatter_timer(SignalGenerator* signalGenerator, SensorEvent* event) {
	if (event != nullptr) {
		WAIT(event->chatterProtectionTime);
		signalGenerator->pollOnSensors();
	} else {
		LOG_WARNING<<__FUNCTION__<<": event hold nullptr"<<endl;
	}
}

void SignalGenerator::pollOnSensors() {
	int sensorValues;
	sensorValues = ((GPIO::instance().read(PORT::C)&0xf0)<<8) |
					(GPIO::instance().read(PORT::B)&0xff);
	AsyncChannel::instance().sendMessage(AsyncMsg(0,sensorValues));

}

void SignalGenerator::pushBackOnSignalBuffer(Signal signal) {
	LOG_SCOPE
	mutex.lock();
	signalBuffer.push_back(signal);
	LOG_DEBUG << "signal pushed";
	notify_observers();
	mutex.unlock();
}

void SignalGenerator::clearSignalBuffer() {
	LOG_SCOPE

	mutex.lock();
	signalBuffer.clear();
	mutex.unlock();
	if(signalBuffer.size() != 0){
		LOG_ERROR<<__FUNCTION__<<": could not clear signalBuffer"<<endl;
	}
}

std::vector<SensorEvent> SignalGenerator::init_events() {
	LOG_SCOPE
	std::vector<SensorEvent> events;
	events.push_back(BUTTON_START);
	events.push_back(BUTTON_STOP);
	events.push_back(BUTTON_RESET);
	events.push_back(BUTTON_E_STOP);
	events.push_back(LIGHT_BARRIER_INPUT);
	events.push_back(LIGHT_BARRIER_HEIGHT);
	events.push_back(LIGHT_BARRIER_SWITCH);
	events.push_back(LIGHT_BARRIER_SLIDE);
	events.push_back(LIGHT_BARRIER_OUTPUT);
	events.push_back(SENSOR_HEIGHT_MATCH);
	events.push_back(SENSOR_METAL_MATCH);
	events.push_back(SENSOR_SWITCH_OPEN);
	return events;
}

void SignalGenerator::printEvents() {
	for (SensorEvent event : events) {
		std::time_t  t = std::chrono::steady_clock::to_time_t(event.lastTimeTriggered);
		cout<<event.name<<endl;
		cout<<"bitmask: "			<<(int)event.bitmask<<endl;
		cout<<"cpt: "				<<(int)event.chatterProtectionTime<<endl;
		cout<<"time: " 				<< std::ctime(&t)<<endl;
		cout<<"signalpair high: "	<<(int)event.signalPair.high<<endl;
		cout<<"signal low: "		<<(int)event.signalPair.low<<endl;
	}
}

void SignalGenerator::clearEStoppPushed() {
	stored_mask |= BUTTON_E_STOP.bitmask;
}

void SignalGenerator::register_observer(Observer* o) {
	Observable::register_observer(o);
	mutex.unlock();
	actuators::Motor::instance(std::ref(*this)).lock(false);
}

void SignalGenerator::unregister_observer(Observer* o) {
	actuators::Motor::instance(std::ref(*this)).lock(true);
	mutex.lock();
	Observable::unregister_observer(o);
}

} /* namespace io */
} /* namespace hardwareLayer */
