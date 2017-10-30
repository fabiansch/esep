/*
 * GpioTesting.h
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */

#ifndef GPIOTESTING_H_
#define GPIOTESTING_H_

#include "GPIO.h"

class GpioTesting: public hal::io::GPIO {
public:
	virtual void write(  PORT port, port_t val) override;
	static GpioTesting& instance(bool test);
	GpioTesting();
	void helloWorld();

private:
	GpioTesting(bool test);
};

#endif /* GPIOTESTING_H_ */
