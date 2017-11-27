/*
 * LogicLayer.cpp
 *
 *  Created on: 26.10.2017
 *      Author: abj240
 */

#include "LogicLayer.h"


namespace logicLayer {

LogicLayer::LogicLayer(hardwareLayer::HardwareLayer& hal)
: hal(hal)
, controller_(hal)
, dispatcher_(
		hal,
		controller_.getChannel(),
		timer_.getChannel()
		)
{
	LOG_SCOPE
}

LogicLayer::~LogicLayer() {
	LOG_SCOPE
}

} /* namespace logicLayer */
