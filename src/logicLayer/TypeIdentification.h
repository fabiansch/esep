/*
 * TypeIdentification.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef TYPEIDENTIFICATION_H_
#define TYPEIDENTIFICATION_H_

#include "HardwareLayer.h"
#include "SignalReceiver.h"

namespace logicLayer {

class TypeIdentification: public SignalReceiver {
public:
	TypeIdentification(hardwareLayer::HardwareLayer&);
	virtual ~TypeIdentification();
	void operator()();
private:
	hardwareLayer::HardwareLayer& hal_;
};

} /* namespace logicLayer */
#endif /* TYPEIDENTIFICATION_H_ */
