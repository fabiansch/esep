/*
 * LogicLayer.cpp
 *
 *  Created on: 26.10.2017
 *      Author: abj240
 */

#include "LogicLayer.h"


namespace logicLayer {

LogicLayer::LogicLayer(hardwareLayer::HardwareLayer& hal) :
hal(hal),
controller_(*new Controller(hal)),
typeIdent_(*new TypeIdentification),
timer_(*new Timer),
calibration_(*new Calibration),
errorHandler_(*new ErrorHandler),
sorting_(*new Sorting),
dispatcher_(*new Dispatcher(
		hal,
		controller_.getChannel(),
		typeIdent_.getChannel()
		))
, sensorTest(hal)
{
}

LogicLayer::~LogicLayer() {
	cout << "call lol's deconstructor " << endl;
	delete &dispatcher_;
	cout << "before typeIdent" << endl;
	delete &typeIdent_;
	cout << "after typeIdent" << endl;
	delete &controller_;
	cout << "after controller" << endl;
}

} /* namespace logicLayer */
