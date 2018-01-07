/*
 * Sorting.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Signals.h"
#include "Sorting.h"
#include "Item.h"
#include "HardwareLayer.h"

namespace logicLayer {

//static Sorting sorting;


bool Sorting::amIWanted(Item* item) {

	bool result = false;

	//CASE 1 of 4 : both cb slides are full
	if( this_slide_full && other_slide_full ){
		result = instance().checkOrder(item);
	}

	//CASE 2 of 4 : this cb has to pass through everything on cb1
	if( this_slide_full && !other_slide_full  ){
		result = instance().checkOrder(item);

		//need to do only on first cb
		if( cb_this == cb_sorting_1 ){
			//mark pending sorting flag to determine sorting out on next cb, but only if item is undesired
			if(!result){
				item->setPendingSortout(true);
			}
			result = true; // passing through all items
		}

	}

	//CASE 3 of 4 : this cb have to sort everything out
	if( !this_slide_full && other_slide_full  ){
		result = instance().checkOrder(item);
	}

	//CASE 4 of 4 : both slides are free , sorting out defined favorites
	if( !this_slide_full && !other_slide_full  ){

		//if false need to check against preference list
		if( cb_this == cb_sorting_1 ){
			result = instance().checkOrder(item); //check at first if item is desired
			if(!result){
				result = instance().checkAgainstCB1Preferences(item);
			}
		}
		else if(cb_this == cb_sorting_2){
			result = instance().checkOrder(item);
		}
		//if item will be let through, mark as pending Sortout
		if( result ){
			item->setPendingSortout(true);
		}

	}

	return result;
}

bool Sorting::checkOrder(Item* passedItem){

	bool result = false;

	switch (orderState) {
		case Order::BOM1:
			if( passedItem->getType().profile == Profile::HOLED && !passedItem->getType().metal ){
				result = true;
				previousState = orderState;
				orderState = Order::BOM2;
			}
		break;
		case Order::BOM2:
			if( passedItem->getType().profile == Profile::HOLED && !passedItem->getType().metal ){
				result = true;
				previousState = orderState;
				orderState = Order::BMM;
			}
		break;
		case Order::BMM:
			if( passedItem->getType().profile == Profile::HOLED && passedItem->getType().metal ){
				result = true;
				previousState = orderState;
				orderState = Order::BOM1;
			}
		break;
	}

	return result;
}

bool Sorting::checkAgainstCB1Preferences(Item* item){
	bool result = true;

	if( item->getType().code == 1){
		result= false;
	}
	else if( item->getType().code == 5){
		result= false;
	}
	else if( item->getType().profile == Profile::FLAT){
		result= false;
	}

	return result;
}

bool Sorting::checkAgainstCB2Preferences(Item* item){
	bool result = true;

	if( item->getType().code == 2){
		result= false;
	}
	else if( item->getType().code == 4){
		result= false;
	}

	return result;
}

void Sorting::informCB1SortingUnit( hardwareLayer::HardwareLayer* hal ){
	switch ( instance().getOrderState() ) {
		case Order::BOM1:
			hal->sendSerial( Signal(cb_this, cb_sorting_1, Signalname::SORTING_BOM1 ) );
		break;
		case Order::BOM2:
			hal->sendSerial( Signal(cb_this, cb_sorting_1, Signalname::SORTING_BOM2 ) );
		break;
		case Order::BMM:
			hal->sendSerial( Signal(cb_this, cb_sorting_1, Signalname::SORTING_BMM ) );
		break;
	}
}


} /* namespace logicLayer */
