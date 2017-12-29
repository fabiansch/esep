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
, controller_(hal, timer_.getChannel())
,calibration_(hal)
, dispatcher_(
		hal,
		controller_.getChannel(),
		timer_.getChannel()
		)
// , menu_(hal, controller_.getChannel())
{
	LOG_SCOPE
	timer_.setControllerChannel(&controller_.getChannel());
	// menu_.computeInput();
}

LogicLayer::~LogicLayer() {
	LOG_SCOPE
}

// Menu& LogicLayer::getMenu(){
// 	return menu_;
// }

Controller& LogicLayer::getController() {
	return controller_;
}

} /* namespace logicLayer */
