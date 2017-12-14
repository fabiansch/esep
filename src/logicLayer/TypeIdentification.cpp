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
			case Signalname::LB_INPUT_FREED:
				//create ItemType first
				currentScan_ = createScan();
			break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				 //stop mesurement
				currentScan_->profile = heightMapping(hal_.getHeight());
			break;
			case Signalname::SENSOR_METAL_MATCH:

				currentScan_->metal = true;
			break;
			case Signalname::LB_SWITCH_INTERRUPTED:
				//build type and write to static memory
				cout << "metal: " << currentScan_->metal << endl;
				if(  currentScan_->profile  == Profile::HOLED){
					cout << "profile: HOLED" << endl;
				}
				else if(  currentScan_->profile  == Profile::NORMAL){
					cout << "profile: NORMAL" << endl;
				}
				else if(  currentScan_->profile  == Profile::FLAT){
					cout << "profile: FLAT" << endl;
				}

			break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				currentScan_->profile = Profile::NORMAL;
				//start mesurement
				//start thread - periodically read height -> e.g. 20 ms
			break;
			default:

			break;
		}
	}

}

//deprecated
Profile TypeIdentification::heightMapping(int height){
	cout << height << endl;
	cout << validHeightReference << endl;
	if( abs(height - validHeightReference ) < deltaHeight ){
		return Profile::HOLED;
	}

	return currentScan_->profile;
}


ItemType* TypeIdentification::createScan(){
	ItemType it;
	typeScans.push_back(it);
	return &typeScans.back();
}

} /* namespace logicLayer */
