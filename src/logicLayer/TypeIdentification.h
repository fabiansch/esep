/*
 * TypeIdentification.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef TYPEIDENTIFICATION_H_
#define TYPEIDENTIFICATION_H_


#include "SignalReceiver.h"

namespace logicLayer {

class TypeIdentification: public SignalReceiver {
public:
	TypeIdentification();
	virtual ~TypeIdentification();
	void operator()();
};

} /* namespace logicLayer */
#endif /* TYPEIDENTIFICATION_H_ */
