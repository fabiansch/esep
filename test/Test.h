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
	Test();
	virtual ~Test();

	/**
	 *  @brief test actuators
	 */
	static void actuatorsTest(hardwareLayer::HardwareLayer& hal);

	/**
	 *  @brief test mmi
	 */
	static void mmiTest(hardwareLayer::HardwareLayer& hal);

	/*
	 * @brief adds signal to testSignalBuffer
	 */
	static void testSignalBufferAdd(Signal signal);

	/**
	 *@brief test buttons
	 *@brief just working without LogicLayer started. LogicLayer is using
	 *@brief SensorTest for testing other sensors
	 */
	static void buttonsTest();

	/**
	*  @brief test singleton thread safeness
	*/
	static void singletonThreadSafeTest();

	/**
	 *  @brief test thread safeness in GPIO class
	 */
	static void threadSafenessInGpioTest();

	static void channelTest();
	void handle(Signal signal);
private:
	/**
	 *
	 */
	static void writeSomethingElse(hardwareLayer::io::GPIO *gpio, int difference);

	/**
	 *	@brief go to next test
	 */
	static bool nextTest(string functionName);

	static Signal nextSignal();

	static void buttonTestHelper(hardwareLayer::io::SensorEvent signalbitmask, Signalname eventTriggerStart, Signalname eventTriggerEnd);

	static vector<Signal> testSignalBuffer;
};


} /* test */
} /* logicLayer */

#endif /* TEST_H_ */


