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

enum class ProfileState {NL1, NL2, NL3, S1, S2, S3};
enum class Profile {HOLED, FLAT, NORMAL};

struct ItemType{
	Profile profile = Profile::FLAT;
	bool metal = false;
	int code = -1;
	bool inDetection = true;
	float height = 0;
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
	 *  @brief allowed tolerance
	 */
	static int delta;

	/**
	 *  @brief reference value that holds expected height value of valid item
	 *  !!!needs to be set via calibration - value of each cb differs very strong
	 */
	static int normalLevel;

	/**
	 * @brief ground level
	 */
	static int groundLevel;

	/**
	 * @brief measure cycle
	 */
	static void measureProfil(bool&, hardwareLayer::HardwareLayer*);

	/**
	 *
	 */
	static int mapToBinary(int);

	/**
	 *	@brief shows if typeId is in measurement or not
	 *	default false
	 */
	static float mmPerUnit;

	/**
	 *	@brief
	 */
	static float validHeightReference;

	static void switchToState(int, ProfileState*, ProfileState, int*, int*, int* );
	static void switchToState(int, ProfileState*, ProfileState, int* );

	static void setUnitToMm();
	static void setHoleLevel();

private:
	hardwareLayer::HardwareLayer* hal_;




	/**
	 *	@brief shows if typeId is in measurement or not
	 *	default false
	 */
	bool inMeasurement;



	/**
	 * @brief convert sensor units to mm
	 */
	float toMm( int );

};

} /* namespace logicLayer */
#endif /* TYPEIDENTIFICATION_H_ */
