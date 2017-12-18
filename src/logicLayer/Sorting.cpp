/*
 * Sorting.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Signals.h"
#include "Sorting.h"
#include "Item.h"

namespace logicLayer {

static Sorting sorting;

static bool SLIDE_CB1_FULL = false;
static bool SLIDE_CB2_FULL = false;

bool Sorting::amIWanted(Item* item) {

	bool result = false;

	//CASE 1 of 4 : both cb slides are full
	if( SLIDE_CB1_FULL && SLIDE_CB2_FULL ){
		//throw error & stop cb's
	}

	//CASE 2 of 4 : this cb has to pass through everything
	if( SLIDE_CB1_FULL && !SLIDE_CB2_FULL ){
		result = sorting.checkOrder(item);
		//mark pending sorting flag to determine sorting out on next cb, but only if item is undesired
		if(!result){
			item->setPendingSortout(true);
		}
		result = true;
	}

	//CASE 3 of 4 : this cb have to sort everything out
	if( !SLIDE_CB1_FULL && SLIDE_CB2_FULL ){
		result = sorting.checkOrder(item);
	}

	//CASE 4 of 4 : both slides are free , sorting out defined favorites
	if( !SLIDE_CB1_FULL && !SLIDE_CB2_FULL ){
		result = sorting.checkOrder(item);
		//if false need check against preference list
		if(!result){
			result = (bool) item->getId() % 2; // <-- to keep it at begin simple - toggle sorting
			//if item will be let through, mark as pending Sortout
			if( result ){
				item->setPendingSortout(true);
			}
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
				orderState = Order::BOM2;
			}
		break;
		case Order::BOM2:
			if( passedItem->getType().profile == Profile::HOLED && !passedItem->getType().metal ){
				result = true;
				orderState = Order::BMM;
			}
		break;
		case Order::BMM:
			if( passedItem->getType().profile == Profile::HOLED && passedItem->getType().metal ){
				result = true;
				orderState = Order::BOM1;
			}
		break;
	}

	return result;
}

} /* namespace logicLayer */
