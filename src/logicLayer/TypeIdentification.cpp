/*
 * TypeIdentification.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "TypeIdentification.h"

namespace logicLayer {

static vector<ItemType> typeScans;

TypeIdentification::TypeIdentification(hardwareLayer::HardwareLayer& hal) :
		hal_(hal)
		, deltaHeight(200)
		, validHeightReference(3600) //<-- !need to be parameter from calibration
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
				//create ItemType first & stop mesurement
				currentScan_ = createScan();
				currentScan_->profile = heightMapping(hal_.getHeight());
			break;
			case Signalname::SENSOR_METAL_MATCH:
				cout << "metal recognized" << endl;
				currentScan_->metal = true;
			break;
			case Signalname::LB_SLIDE_INTERRUPTED:
				//build type and write to static memory
			break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				//start mesurement
				//start thread - periodically read height -> e.g. 20 ms
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


ItemType* TypeIdentification::createScan(){
	ItemType it;
	typeScans.push_back(it);
	return &typeScans.front();
}

} /* namespace logicLayer */
