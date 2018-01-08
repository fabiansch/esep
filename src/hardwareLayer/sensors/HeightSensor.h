/*
 * HeightSensor.h
 *
 *  Created on: 30.10.2017
 *      Author: aca311
 */

#ifndef HEIGHTSENSOR_H_
#define HEIGHTSENSOR_H_

#include "Header.h"
#include <mutex>


namespace hardwareLayer {
namespace sensors {

class HeightSensor {
public:
	/**
	* @brief creates singleton instance on first call, otherwise returns instance
	* @return singleton instance
	*/
	static HeightSensor& instance();
	/**
	* @brief reads the height value
	* @return height value
	*/
	uint16_t getHeight(void);

private:
	HeightSensor();
	HeightSensor(HeightSensor const&);
	HeightSensor& operator=(HeightSensor const&);
	~HeightSensor();

	std::mutex mutex_;
};


} /*sensors*/
} /*hardwareLayer*/

#endif /* HEIGHTSENSOR_H_ */
