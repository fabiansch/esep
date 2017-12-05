/*
 * Calibration.cpp
 *
 *  Created on: 20.11.2017
 *      Author: abx724
 */

#include "Calibration.h"

namespace logicLayer {

Calibration::Calibration() {
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

Calibration::~Calibration() {
	LOG_SCOPE
}

void Calibration::operator()(){
	Signal signal;
	while(running) {
		signal << channel_;
	}
}

} /* namespace logicLayer */
