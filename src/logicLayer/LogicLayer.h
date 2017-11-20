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

#include "Observer.h"
#include "HardwareLayer.h"



namespace logicLayer {

class LogicLayer : public Observer {
public:
	LogicLayer(hardwareLayer::HardwareLayer&);
	virtual ~LogicLayer();
	void notify();
private:
	hardwareLayer::HardwareLayer& hal;

	Controller& controller_;
	TypeIdentification& typeIdent_;
	Timer& timer_;
	Calibration& calibration_;
	ErrorHandler& errorHandler_;
	Sorting& sorting_;

	Dispatcher& dispatcher_;
};

} /* namespace logicLayer */

#endif /* LOGICLAYER_H_ */
