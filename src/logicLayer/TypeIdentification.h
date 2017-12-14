/*
 * TypeIdentification.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef TYPEIDENTIFICATION_H_
#define TYPEIDENTIFICATION_H_

#include "SignalReceiver.h"


namespace hardwareLayer{
	class HardwareLayer;
}

enum class Profile {HOLED, FLAT, NORMAL};

struct ItemType{
	Profile profile = Profile::FLAT;
	bool metal = false;
	bool inDetection = true;
};

namespace logicLayer {


class TypeIdentification: public SignalReceiver {
public:
	TypeIdentification(hardwareLayer::HardwareLayer*);
	virtual ~TypeIdentification();
	void operator()();
	Profile heightMapping( int );

	static ItemType* createScan();
	static vector<ItemType> typeScans;
	/**
	 * @brief measure cycle
	 */
	static void measureProfil(bool&, hardwareLayer::HardwareLayer*);

	/**
	 *
	 */
	static int mapToBinary(int);

private:
	hardwareLayer::HardwareLayer* hal_;







	/**
	 *  @brief allowed tolerance
	 */
	int deltaHeight;

	/**
	 *	@brief reference value to check against - need to calibrate
	 */
	int validHeightReference;

	/**
	 *	@brief
	 */
	bool ignoreInterrupt;

};

} /* namespace logicLayer */
#endif /* TYPEIDENTIFICATION_H_ */
