/*
 * Menu.h
 *
 *  Created on: 24.11.2017
 *      Author: aca311
 */

#ifndef MENU_H_
#define MENU_H_

#include "Channel.h"
#include "HardwareLayer.h"

namespace logicLayer {

class Menu {
public:
	Menu(hardwareLayer::HardwareLayer& hal, Channel<Signal>&);
	virtual ~Menu();
	void computeInput();
	bool isRestart();
	static void printInfo();

private:
	hardwareLayer::HardwareLayer& hal;
	Channel<Signal>& controller_;
	bool restart;
	void printOptions();
	void initSensorTest();
};
}

#endif /* MENU_H_ */
