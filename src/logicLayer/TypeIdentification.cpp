/*
 * TypeIdentification.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "TypeIdentification.h"
#include "HardwareLayer.h"

namespace logicLayer {

vector<ItemType> TypeIdentification::typeScans = vector<ItemType>();
int TypeIdentification::delta = 200;
float TypeIdentification::mmPerUnit = 0;
float TypeIdentification::validHeightReference = 0;


TypeIdentification::TypeIdentification(hardwareLayer::HardwareLayer* hal) :
		hal_(hal)
		, inMeasurement(false)
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
				 //stop mesurement
				inMeasurement = false;
				typeScans.front().inDetection = false;
				hal_->motorFast();
			break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				 //stop mesurement thread
			{
				new std::thread(measureProfil, std::ref( typeScans.front().inDetection ), hal_ );
			}

				typeScans.front().profile = heightMapping(hal_->getHeight());

				//set height
				if(cb_this == cb_sorting_1){
					typeScans.front().height_cb_1 = toMm( hal_->getHeight() );
				}
				else{
					typeScans.front().height_cb_2 = toMm( hal_->getHeight() );
				}

			break;
			case Signalname::SENSOR_METAL_MATCH:

				typeScans.front().metal = true;
			break;
			case Signalname::LB_SWITCH_INTERRUPTED:
					//silence is golden
			break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				if(!inMeasurement){
					inMeasurement = true;

					if(typeScans.front().profile!=Profile::HOLED){
						typeScans.front().profile = Profile::NORMAL;
					}

					//start mesurement
					//start thread - periodically read height -> e.g. 20 ms
					hal_->motorSlow();

				}
			break;
			default:

			break;
		}
	}

}

void TypeIdentification::measureProfil(bool& running, hardwareLayer::HardwareLayer* hal){
	ProfileState state = ProfileState::NL1;
	int validCount = 0;
	int avgValue = 0;

	int code_lsb = 0;
	int code_cb = 0;
	int code_msb = 0;
	int height = 0;

	//profile recognition via following state machine
	while(running && (height <= 4000) ){
		height = hal->getHeight();
		cout << "EVAL:" << height << endl;

		switch ( state ) {
			case ProfileState::NL1:
				cout << "NL1" << endl;
				switchToState(height, &state, ProfileState::S1, &avgValue, &validCount, &code_lsb);
			break;
			case ProfileState::S1:
				cout << "S1" << endl;
				switchToState(height, &state, ProfileState::NL2, &validCount);
			break;
			case ProfileState::NL2:
				cout << "NL2" << endl;
				switchToState(height, &state, ProfileState::S2, &avgValue, &validCount, &code_cb);
			break;
			case ProfileState::S2:
				cout << "S2" << endl;
				switchToState(height, &state, ProfileState::NL3, &validCount);
			break;
			case ProfileState::NL3:
				cout << "NL3" << endl;
				switchToState(height, &state, ProfileState::S3, &avgValue, &validCount, &code_msb);
			break;
			case ProfileState::S3:
				cout << "S3" << endl;
				int code = code_lsb | (code_cb << 1) | (code_msb << 2);
				TypeIdentification::typeScans.front().code = code;

			break;
		}

		WAIT(2);
	}
}

Profile TypeIdentification::heightMapping(int height){
	cout << height << endl;
	cout << validHeightReference << endl;
	if( abs(height - validHeightReference ) < delta ){
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

	int low = height_item + (5 / mmPerUnit); // need to be parameter from calibration
	int delta = 75; // need to be figured out by empirical investigation

	if( height > low-delta ){
		binaryValue = 1;
	}
	cout << "Binary:" << binaryValue << " - low: " << low << "avg: " << height <<  endl;
	return binaryValue;
}

void TypeIdentification::switchToState(int measuredHeight, ProfileState* currentState, ProfileState switchToState, int* avgValue, int* count, int* code ){


	//int amountOfNeededValues = (*currentState != ProfileState::NL1) ? 3 : 2;
	int amountOfNeededValues = 5;
	int delta = 100;

	if( abs( measuredHeight - height_item ) > delta  && measuredHeight > height_item ){
		*count = *count + 1;

		*avgValue = *avgValue + measuredHeight;
		cout << "Count: " << *count << " Sum: " << *avgValue << endl;
	}
	else{
		*count = 0;
		*avgValue = 0;
	}

	//switch to next state after "count" valid values
	if( *count == amountOfNeededValues ){

		//average values & map to 0 | 1
		*avgValue = *avgValue / amountOfNeededValues;
		*code = mapToBinary( *avgValue );

		//clean up
		*count = 0;
		*avgValue   = 0;

		*currentState = switchToState; // needs to be last
	}
}

void TypeIdentification::switchToState(int measuredHeight, ProfileState* currentState, ProfileState switchToState, int* count ){

	int amountOfNeededValues = 3;

	if( abs( measuredHeight - height_item ) < 100 ){
		*count = *count + 1;
	}

	//switch to next state after "count" valid values
	if(*count == amountOfNeededValues){
		*currentState = switchToState;
		//clean up
		*count = 0;
	}
}

float TypeIdentification::toMm( int measuredHeight ){
	return mmPerUnit * ( height_conveyor_belt - measuredHeight );
}

void TypeIdentification::setUnitToMm(){
	mmPerUnit = (float) 25 / ( height_conveyor_belt - height_item ); //25 --> valid item height in mm
	cout << "Höhe Band : " << height_conveyor_belt << endl;
	cout << "Höhe Item : " << height_item << endl;
	cout << "mm/unit : " << mmPerUnit << endl;
}

void TypeIdentification::setHoleLevel(){
	validHeightReference = height_conveyor_belt - ( 8 / mmPerUnit ); //<-- !need to be parameter from calibration ( 8mm hole height )
}



} /* namespace logicLayer */
