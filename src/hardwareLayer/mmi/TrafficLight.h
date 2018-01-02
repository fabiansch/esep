/*
 * TrafficLights.h
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_

#include "GPIO.h"
#include <functional>
#include <thread>
#include <chrono>
#include "Blink.h"


namespace hardwareLayer {
namespace mmi {

class TrafficLight {
public:
	/**
	* @brief creates singleton instance on first call, otherwise returns instance
	* @return singleton instance
	*/
	static TrafficLight& instance();

	/**
	* @brief turns the green light on
	*/
	void greenLightOn();

	/**
	* @brief turns the yellow light on
	*/
	void yellowLightOn();

	/**
	* @brief turns the red light on
	*/
	void redLightOn();

	/**
	* @brief turns the green light off
	*/
	void greenLightOff();

	/**
	* @brief turns the yellow light off
	*/
	void yellowLightOff();

	/**
	* @brief turns the red light off
	*/
	void redLightOff();

	/**
	* @brief makes the green light blink
	*/
	void blinkGreen(Speed speed);

	/**
	* @brief makes the yellow light blink
	*/
	void blinkYellow(Speed speed);

	/**
	* @brief makes the red light blink
	*/
	void blinkRed(Speed speed);

	/*
	 * @brief lock or unlock green light
	 * @params lock = true: green light of till called again with lock = true
	 */
	void lockGreen(bool lock);

private:
	TrafficLight();
	TrafficLight(TrafficLight const&);
	TrafficLight& operator=(TrafficLight const&);
	virtual ~TrafficLight();

	enum class LightStatus { off, blinkSlow, blinkFast, on };

	LightStatus greenStatus;
	bool greenLocked;


	mmi::Blink blink;
	std::thread thread;

};
} /* namespace mmi */
} /* namespace hardwareLayer */

#endif /* TRAFFICLIGHT_H_ */
