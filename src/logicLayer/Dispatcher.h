/*
 * Dummy.h
 *
 *  Created on: 17.11.2017
 *      Author: abx724
 */

#ifndef DUMMY_H_
#define DUMMY_H_

#include "Channel.h"
#include "HardwareLayer.h"
#include <thread>

namespace logicLayer {

class Dispatcher : public Observer {
public:
	Dispatcher(
			hardwareLayer::HardwareLayer&,
			Channel&,
			Channel&
	);
	virtual ~Dispatcher();
	void notify();
private:

	hardwareLayer::HardwareLayer& hal;
	Channel& controller_;
	Channel& typeIdent_;


};

} /* namespace logicLayer */
#endif /* DUMMY_H_ */
