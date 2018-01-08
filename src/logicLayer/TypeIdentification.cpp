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
	std::thread measure_t;
	bool forceEnding = true;

	while(running){
		sig << channel_;
		switch (sig.name) {
			case Signalname::LB_INPUT_FREED:
				//create ItemType first
				createScan();
			break;
			case Signalname::LB_HEIGHT_FREED:
				if( typeScans.size() > 0){ //only start if there is an type object on vector
					 //stop mesurement
					inMeasurement = false;
					typeScans.front().inDetection = false;
					hal_->motorFast();
				}
			break;
			case Signalname::LB_HEIGHT_INTERRUPTED:
				if( typeScans.size() > 0){ //only start if there is an type object on vector

					int height = hal_->getHeight();
					typeScans.front().profile = heightMapping( height );

					//set height
					if(cb_this == cb_sorting_1){
						typeScans.front().height_cb_1 = toMm( height );
					}
					else{
						typeScans.front().height_cb_2 = toMm( height );
					}


				}

			break;
			case Signalname::SENSOR_METAL_NOT_MATCH:
				if( typeScans.size() > 0){ //only start if there is an type object on vector
					typeScans.front().metal = false;
				}
				break;
			case Signalname::SENSOR_METAL_MATCH:
				if( typeScans.size() > 0){ //only start if there is an type object on vector
					typeScans.front().metal = true;
				}
			break;
			case Signalname::LB_SWITCH_INTERRUPTED:
					//silence is golden
			break;
			case Signalname::SENSOR_HEIGHT_MATCH:
				if( typeScans.size() > 0){ //only start if there is an type object on vector
					if(!inMeasurement){
						inMeasurement = true;

						hal_->motorSlow();

						 //start mesurement thread

						if(measure_t.joinable()){
							forceEnding = false;
							measure_t.join();

						}
						forceEnding = true;
						measure_t = std::thread(measureProfil, &forceEnding, hal_ );


					}
				}
			break;
			default:

			break;
		}
	}

}

void TypeIdentification::measureProfil(bool* running, hardwareLayer::HardwareLayer* hal){

	ProfileState state = ProfileState::SPACE;

	int code_lsb = 0;
	int code_cb = 0;
	int code_msb = 0;

	int code  = 0;
	int code2 = 0;

	int height = 0;

	int expectedValues = 10;
	int deltaEdge = 100;
	int deltaSpace = 100;
	int edgeCount = 0;
	int spaceCount = 0;
	int validValueCount = 0;

	cout<<"START"<<endl;

	//profile recognition via following state machine
	while( *running ){
		height = hal->getHeight();
		cout << "EVAL:" << height << endl;

		switch ( state ) {
			case ProfileState::EDGE:
				if(height >= (height_item+deltaSpace) ){
					validValueCount++;
				}

				if(validValueCount==expectedValues){
					if( height <= (height_conveyor_belt - 100) ){
						state = ProfileState::SPACE;
					}
					else{
						state = ProfileState::FINAL;
					}

					validValueCount = 0;
					spaceCount++;
					cout << "Zwischenraum " << spaceCount << endl;
					if(spaceCount<=3){
						code |= mapToBinary(height) << spaceCount-1;
					}
					else{
						code2 |= mapToBinary(height) << spaceCount-4;
					}

				}


			break;
			case ProfileState::SPACE:
				if(height <= (height_item+deltaEdge) ){
					validValueCount++;
				}

				if(validValueCount==expectedValues){
					state = ProfileState::EDGE;
					validValueCount = 0;
					edgeCount++;
					cout << "Kante " << edgeCount << endl;
				}
			break;
			case ProfileState::CENTER:

			break;
			case ProfileState::FINAL:
				*running = false;
				cout << "### FINAL " << endl;
				cout << code << endl;
				cout << code2 << endl;
			break;
		}

		WAIT( 5 );

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

	int zero  = height_item + (height_item/25)*2; // need to be parameter from calibration
	int delta = 100; // need to be figured out by empirical investigation

	if( height > zero + delta ){
		binaryValue = 1;
	}
	cout << "Binary:" << binaryValue << " - zero: " << zero << "height: " << height <<  endl;
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
