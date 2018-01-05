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
			Channel<Signal>&,
			Channel<Signal>&
	);
	virtual ~Dispatcher();
	virtual void notify() override;
private:

	hardwareLayer::HardwareLayer& hal;
	Channel<Signal>& controller_;
	Channel<Signal>& timer_;

	void set_other_slide_full(const Signal& signal);
};

} /* namespace logicLayer */
#endif /* DUMMY_H_ */
