/*
 * TypeIdentification.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "TypeIdentification.h"

namespace logicLayer {

TypeIdentification::TypeIdentification(hardwareLayer::HardwareLayer& hal) :
		hal_(hal)
		, validHeightReference(3600) //<-- !need to be parameter from calibration
		, deltaHeight(200)
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

TypeIdentification::~TypeIdentification() {
	LOG_SCOPE
}

void TypeIdentification::operator()(){
	LOG_SCOPE
	Signal sig;
	while(running){
		sig << channel_;
		switch (sig.name) {
			case Signalname::LB_HEIGHT_INTERRUPTED:
				heightMapping(hal_.getHeight());
			break;
			case Signalname::SENSOR_METAL_MATCH:
				cout << "metal recognized" << endl;
			break;
			case Signalname::LB_SLIDE_INTERRUPTED:
				//build type and write to static memory
			break;
			default:

			break;
		}
	}

}

Profile TypeIdentification::heightMapping(int height){
	cout << height << endl;
	cout << validHeightReference << endl;
	if( abs(height - validHeightReference ) < deltaHeight ){
		cout << "HOLED" << endl;
		return Profile::HOLED;
	}
	else{
		cout << "FLAT" << endl;
		return Profile::FLAT;
	}
	return Profile::FLAT;
}


} /* namespace logicLayer */
