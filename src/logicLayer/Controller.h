/*
 * Controller.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "SignalReceiver.h"
#include "SensorTest.h"

namespace logicLayer {

class Controller: public SignalReceiver {
public:
	Controller(hardwareLayer::HardwareLayer& hal);
	virtual ~Controller();

	virtual void operator()();

private:
	SensorTest sensorTest;
};

} /* namespace logicLayer */
#endif /* CONTROLLER_H_ */
