/*
 * InterruptHandler.h
 *
  *  Created on: 20.10.2017
 *      Author: abj240
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include "Signals.h"
#include <thread>
#include <vector>
#include "Observable.h"
#include <chrono>
#include <thread>
#include <mutex>

namespace hardwareLayer {
namespace io {

/*
 * brief: SPair's high member holds signal for edge high
 * 		  SPair's low member holds signal for edge low
 */
struct SPair {
	SPair(Signalname high, Signalname low) :
	high(high),
	low(low)
	{
		LOG_SCOPE
	}
	Signalname high;
	Signalname low;
};

/*
 * brief: SensorEvent holds data for one specific event.
 *
 * bitmask specifies specific bit for this event that consists of two signals.
 * name holds a string of the signal
 * signalPair.high holds signal for higher edge change of bit
 * siganlPair.low holds signal for lower edge  change of bit
 */
struct SensorEvent {
	SensorEvent(const int bitmask, const std::string name, const int chatterProtectionTime, const SPair signalPair) :
	bitmask(bitmask),
	name(name),
	chatterProtectionTime(chatterProtectionTime),
	lastTimeTriggered(std::chrono::steady_clock::now()),
	signalPair(signalPair)
	{
		LOG_SCOPE
	}
	~SensorEvent(){
		LOG_SCOPE
	}
	const int bitmask;
	const std::string name;
	const int chatterProtectionTime;
	std::chrono::steady_clock::time_point lastTimeTriggered;
	const SPair signalPair;
};

class SignalGenerator : public Observable {
public:
	SignalGenerator();
	virtual ~SignalGenerator();

	/*
	 *@brief: functor for thread that listens on AsyncChannel with a blocked wait
	 */
	void operator()();

	/*
	 *@brief: terminates thread that listens on AsyncChannel within the next message
	 *@brief: that gets read after calling this method
	 */
	void terminate();

	/*
	 *@brief: returns nextSignal in signalBuffer.
	 *
	 *If signalBuffer is empty it returns Signal(cb_this,cb_this,SIGNAL_BUFFER_EMPTY)
	 */
	Signal nextSignal();

	/*
	 *@brief: test Method to print Event Vector
	 *
	 */
	static void printEvents();

	/*
	 *@brief: threadsafely pushs back Signal on signalBuffer.
	 *@param: signal Signal
	 */
	void pushBackOnSignalBuffer(Signal signal);

	/*
	 *@brief threadsafely clears signalBuffer
	 */
	void clearSignalBuffer();

	/*
	 * @brief performs poll on sensors and generates signal when change has occurred and signal's chatter protection has passed
	 */
	void pollOnSensors();

	/*
	 * @brief: clears E_STOP_PUSHED in stored_bitmask
	 */
	void clearEStoppPushed();

	// sensor events for higher byte of PORT C
	static SensorEvent BUTTON_START;
	static SensorEvent BUTTON_STOP;
	static SensorEvent BUTTON_RESET;
	static SensorEvent BUTTON_E_STOP;
	// sensor events for PORT B
	static SensorEvent LIGHT_BARRIER_INPUT;
	static SensorEvent LIGHT_BARRIER_HEIGHT;
	static SensorEvent SENSOR_HEIGHT_MATCH;
	static SensorEvent LIGHT_BARRIER_SWITCH;
	static SensorEvent SENSOR_METAL_MATCH;
	static SensorEvent LIGHT_BARRIER_SLIDE;
	static SensorEvent SENSOR_SWITCH_OPEN;
	static SensorEvent LIGHT_BARRIER_OUTPUT;

private:
	/*
	 *@brief: initializes and returns vector<SensorEvent> events
	 */
	static std::vector< SensorEvent> init_events();

	/*
	*@brief: deals with the Clatter of the Switch and the Buttons; Sensors just get triggered once, if they get triggered multiple times in a short defined timeframe
	*@param: Signal signal
	*@return: true if no clatter
	*/
	bool noChatterOn(SensorEvent& event);


	/*
	 * events holds all sensor events
	 */
	static std::vector<SensorEvent> events;

	static void chatter_timer(SignalGenerator* signalGenerator, SensorEvent* event);


	std::thread signal_generator_th;
	bool running;
	int stored_mask;
	std::vector<Signal> signalBuffer;
	std::thread chatter_timer_th;
	std::mutex mutex;
};

} /* namespace io */
} /* namespace hardwareLayer */
#endif /* INTERRUPTHANDLER_H_ */
