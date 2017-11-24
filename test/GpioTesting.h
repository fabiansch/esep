/*
 * GpioTesting.h
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */

#ifndef GPIOTESTING_H_
#define GPIOTESTING_H_

#include "GPIO.h"

class GpioTesting: public hardwareLayer::io::GPIO {
public:
	virtual void write(uint8_t address, uint8_t val) override;
	static GpioTesting& instance(bool test);
	GpioTesting();
	void helloWorld();

private:
	GpioTesting(bool test);
};

#endif /* GPIOTESTING_H_ */
