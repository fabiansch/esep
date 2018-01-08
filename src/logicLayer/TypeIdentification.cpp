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
float TypeIdentification::holeLevel = 0;


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
						if( typeScans.front().height_cb_1 == 0){
							typeScans.front().height_cb_1 = toMm( height );
						}
					}
					else{
						if( typeScans.front().height_cb_2 == 0){
							typeScans.front().height_cb_2 = toMm( height );
						}
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
						typeScans.front().profile = Profile::NORMAL;


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


	int code  = 0;


	int height = 0;

	int expectedValues = 5;
	int deltaEdge = 100;
	int deltaSpace = 100;
	int edgeCount = 0;
	int spaceCount = 0;
	int validValueCount = 0;

	int edgeAvg = 0;


	//profile recognition via following state machine
	while( *running ){
		height = hal->getHeight();


		switch ( state ) {
			case ProfileState::EDGE:
				if(height >= (height_item+deltaSpace) ){
					validValueCount++;
				}

				if(validValueCount==expectedValues){
					if( height <= (height_conveyor_belt - deltaSpace) ){
						state = ProfileState::SPACE;
					}
					else{
						state = ProfileState::FINAL;
					}

					validValueCount = 0;
					spaceCount++;


					switch (spaceCount) {
						case 2:
							code |= mapToBinary(height) << 2;
							break;
						case 3:
							code |= mapToBinary(height) << 1;
							break;
						case 4:
							code |= mapToBinary(height);
							typeScans.front().code = code;

							break;
					}

				}


			break;
			case ProfileState::SPACE:
				if(height <= (height_item+deltaEdge) ){
					validValueCount++;
					edgeAvg += height;
				}

				if(validValueCount==expectedValues){
					state = ProfileState::EDGE;
					validValueCount = 0;
					edgeCount++;


					//get height of first edge
					if( edgeCount == 1 ){

						//set height
						if(cb_this == cb_sorting_1){
							typeScans.front().height_cb_1 = toMm( edgeAvg/expectedValues );
						}
						else{
							typeScans.front().height_cb_2 = toMm( edgeAvg/expectedValues );
						}
					}

					edgeAvg = 0;

				}
			break;
			case ProfileState::CENTER:

			break;
			case ProfileState::FINAL:
				*running = false;
			break;
		}

		WAIT( 5 );

	}
}

Profile TypeIdentification::heightMapping(int height){
	if( abs(height - holeLevel ) < delta ){
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

	return binaryValue;
}


float TypeIdentification::toMm( int measuredHeight ){
	return mmPerUnit * ( height_conveyor_belt - measuredHeight );
}

void TypeIdentification::setUnitToMm(){
	mmPerUnit = (float) 25 / ( height_conveyor_belt - height_item ); //25 --> valid item height in mm
}

void TypeIdentification::setHoleLevel(){
	holeLevel = height_conveyor_belt - ( ( height_conveyor_belt-height_item) / 25 ) * 8 ; //<-- !need to be parameter from calibration ( 8mm hole height )
}



} /* namespace logicLayer */
