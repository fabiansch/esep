/*
 * Test.h
 *
 *  Created on: 23.10.2017
 *      Author: abx724
 */

#ifndef TEST_H_
#define TEST_H_

#include "GPIO.h"
#include <string>
#include "HardwareLayer.h"


namespace logicLayer{
namespace test{

class Test {
public:
	Test( hardwareLayer::HardwareLayer* hal);
	virtual ~Test();

	/**
	 *  @brief test actuators
	 */
	void actuatorsTest();

	/**
	 *  @brief test mmi
	 */
	void mmiTest();

	/**
	 *@brief test buttons
	 *@brief just working without LogicLayer started. LogicLayer is using
	 *@brief SensorTest for testing other sensors
	 */
	void buttonsTest();

	/**
	*  @brief test singleton thread safeness
	*/
	void singletonThreadSafeTest();

	/**
	 *  @brief test thread safeness in GPIO class
	 */
	void threadSafenessInGpioTest();
private:

	hardwareLayer::HardwareLayer* _hal;

	/**
	 *
	 */
	static void writeSomethingElse(hardwareLayer::io::GPIO *gpio, int difference);

	/**
	 *	@brief go to next test
	 */
	bool nextTest(string functionName);

	void buttonTestHelper(hardwareLayer::io::SensorEvent signalbitmask, Signalname eventTriggerStart, Signalname eventTriggerEnd);

};


} /* test */
} /* logicLayer */

#endif /* TEST_H_ */


