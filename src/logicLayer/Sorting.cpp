/*
 * Sorting.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Signals.h"
#include "Sorting.h"

namespace logicLayer {

Sorting::Sorting()
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Sorting::~Sorting() {
	LOG_SCOPE
}

/**
 * @override
 */
void Sorting::operator ()(){

}

} /* namespace logicLayer */
