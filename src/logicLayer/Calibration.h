/*
 * Calibration.h
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "SignalReceiver.h"

namespace logicLayer {

class Calibration: public SignalReceiver {
public:
	Calibration();
	virtual ~Calibration();
	void operator()();
};

} /* namespace logicLayer */
#endif /* CALIBRATION_H_ */
