/*
 * SwitchPoint.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "SwitchPoint.h"
#include "GPIO.h"
#include "Header.h"

constexpr int SWITCHPOINT_OPEN = 0b00010000;

namespace hal {
namespace actuators {

	SwitchPoint *SwitchPoint::_instance = nullptr;

	SwitchPoint *SwitchPoint::instance() {
		if (_instance == nullptr) {
			_instance = new SwitchPoint;
		}
		return _instance;
	}

	SwitchPoint::SwitchPoint() {
		LOG_SCOPE;
	}

	SwitchPoint::~SwitchPoint() {
		LOG_SCOPE;
	}

	void SwitchPoint::open(){

		io::GPIO::instance()->setBits(PORT::A,  SWITCHPOINT_OPEN);
	}

	void SwitchPoint::close(){

		io::GPIO::instance()->clearBits(PORT::A,  SWITCHPOINT_OPEN);
	}


	} /* namespace actuators */
} /* namespace hal */
