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

bool Sorting::amIWanted(Item* item) {

	return item->getType().profile == Profile::HOLED;
}

} /* namespace logicLayer */
