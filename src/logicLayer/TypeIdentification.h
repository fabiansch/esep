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

enum class Profile {HOLED, FLAT};

struct itemType{
	Profile profile = Profile::FLAT;
	bool metal = false;
};

namespace logicLayer {

class TypeIdentification: public SignalReceiver {
public:
	TypeIdentification(hardwareLayer::HardwareLayer&);
	virtual ~TypeIdentification();
	void operator()();
	Profile heightMapping( int );
private:
	hardwareLayer::HardwareLayer& hal_;

	/**
	 *  @brief allowed tolerance
	 */
	int deltaHeight;

	/**
	 *	@brief reference value to check against - need to calibrate
	 */
	int validHeightReference;

	/**
	 *
	 */
};

} /* namespace logicLayer */
#endif /* TYPEIDENTIFICATION_H_ */
