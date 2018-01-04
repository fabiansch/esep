/*
 * LogicLayer.h
 *
 *  Created on: 26.10.2017
 *      Author: abj240
 */

#ifndef LOGICLAYER_H_
#define LOGICLAYER_H_


#include "Controller.h"
#include "TypeIdentification.h"
#include "Timer.h"
#include "Calibration.h"
#include "ErrorHandler.h"
#include "Sorting.h"
#include "Dispatcher.h"
#include "HardwareLayer.h"
#include "Menu.h"


namespace logicLayer {

class LogicLayer {
public:
	LogicLayer(hardwareLayer::HardwareLayer&);
	virtual ~LogicLayer();
	Controller& getController();
	Timer& getTimer() { return timer_; };
	// Menu& getMenu();

private:
	hardwareLayer::HardwareLayer& hal;
	Timer timer_;

	TypeIdentification typeIdent_;

	Controller controller_;
	Calibration calibration_;
	//Sorting sorting_; <-- no need because its static
	Dispatcher dispatcher_;
	// Menu menu_;  // have to be last one

void checkIfEStopIsPushed(hardwareLayer::HardwareLayer& hal);
};

} /* namespace logicLayer */

#endif /* LOGICLAYER_H_ */
