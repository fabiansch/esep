/*
 * Sorting.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Signals.h"
#include "Sorting.h"

namespace logicLayer {

bool Sorting::amIWanted(Item* item) {
	return item->getId() % 2;
}

} /* namespace logicLayer */
