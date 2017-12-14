/*
 * TypeIdentification.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "TypeIdentification.h"
#include "HardwareLayer.h"

enum class ProfileState {NL1, NL2, NL3, S1, S2, S3};

namespace logicLayer {

vector<ItemType> TypeIdentification::typeScans = vector<ItemType>();

TypeIdentification::TypeIdentification(hardwareLayer::HardwareLayer* hal) :
		hal_(hal)
		, deltaHeight(200)
		, validHeightReference(3600) //<-- !need to be parameter from calibration
		, ignoreInterrupt(false)
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
				createScan();
			break;
			case Signalname::LB_HEIGHT_FREED:
				ignoreInterrupt = false;
			break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				 //stop mesurement
				hal_->motorFast();
				typeScans.front().inDetection = false;


				typeScans.front().profile = heightMapping(hal_->getHeight());
			break;
			case Signalname::SENSOR_METAL_MATCH:

				typeScans.front().metal = true;
			break;
			case Signalname::LB_SWITCH_INTERRUPTED:

				cout << "metal: " << typeScans.front().metal << endl;
				if(  typeScans.front().profile  == Profile::HOLED){
					cout << "profile: HOLED" << endl;
				}
				else if( typeScans.front().profile  == Profile::NORMAL){
					cout << "profile: NORMAL" << endl;
				}
				else if(  typeScans.front().profile  == Profile::FLAT){
					cout << "profile: FLAT" << endl;
				}

			break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				if(!ignoreInterrupt){

					cout << "in messung" << endl;
					ignoreInterrupt = true;

					if(typeScans.front().profile!=Profile::HOLED){
						typeScans.front().profile = Profile::NORMAL;
					}

					//start mesurement
					//start thread - periodically read height -> e.g. 20 ms
					hal_->motorSlow();
					{
						new std::thread(measureProfil, std::ref( typeScans.front().inDetection ), hal_ );
					}
				}
			break;
			default:

			break;
		}
	}

}

void TypeIdentification::measureProfil(bool& running, hardwareLayer::HardwareLayer* hal){
	ProfileState state = ProfileState::NL1;
	int nl = 2450;
	int delta = 100;
	int validCount = 0;
	int avgValue = 0;

	int x = 4;

	while(running){
		int height = hal->getHeight();
		cout << "EVAL:" << height << endl;

		switch ( state ) {
			case ProfileState::NL1:
				if(abs(height - nl) > delta ){
					cout << "JA1" << endl;
					validCount++;
					avgValue += height;
				}

				//transist to next state after x valid values
				if(validCount == x){
					state = ProfileState::S1;
					//average values & map to 0 | 1
					avgValue /= x;
					mapToBinary( avgValue );

					//clean up
					validCount = 0;
					avgValue = 0;
				}

			break;
			case ProfileState::S1:
				if(abs(height - nl) < delta ){
					validCount++;
				}

				//transist to next state after x valid values
				if(validCount == x){
					state = ProfileState::NL2;
					//clean up
					validCount = 0;
				}
			break;
			case ProfileState::NL2:
				if(abs(height - nl) > delta ){
					cout << "JA2" << endl;
					validCount++;
					avgValue += height;
				}

				//transist to next state after x valid values
				if(validCount == x){
					state = ProfileState::S2;
					//average values & map to 0 | 1
					avgValue /= x;
					mapToBinary( avgValue );

					//clean up
					validCount = 0;
					avgValue = 0;
				}
			break;
			case ProfileState::S2:
				if(abs(height - nl) < delta ){
					validCount++;
				}

				//transist to next state after x valid values
				if(validCount == x){
					state = ProfileState::NL3;
					//clean up
					validCount = 0;
				}
			break;
			case ProfileState::NL3:
				if(abs(height - nl) > delta ){
					cout << "JA3" << endl;
					validCount++;
					avgValue += height;
				}

				//transist to next state after x valid values
				if(validCount == x){
					state = ProfileState::S3;
					//average values & map to 0 | 1
					avgValue /= x;
					mapToBinary( avgValue );

					//clean up
					validCount = 0;
					avgValue = 0;
				}
			break;
			case ProfileState::S3:
				if(abs(height - nl) < delta ){
					validCount++;
				}

				//transist to next state after x valid values
				if(validCount == x){
					//clean up
					validCount = 0;
				}
			break;
		}

		WAIT(10);
	}
}

Profile TypeIdentification::heightMapping(int height){
	cout << height << endl;
	cout << validHeightReference << endl;
	if( abs(height - validHeightReference ) < deltaHeight ){
		return Profile::HOLED;
	}

	return typeScans.front().profile;
}


ItemType* TypeIdentification::createScan(){
	ItemType it;
	typeScans.push_back(it);
	return &typeScans.back();
}

int TypeIdentification::mapToBinary( int height ){
	int binaryValue = 0;

	int nl  = 2450; // need to be parameter from calibration
	int low = 2800; // need to be parameter from calibration
	int delta = 50; // need to be figured out by empirical investigation

	if( abs(height-low) < delta ){
		binaryValue = 1;
	}
	cout << "Binary:" << binaryValue << endl;
	return binaryValue;
}

} /* namespace logicLayer */
