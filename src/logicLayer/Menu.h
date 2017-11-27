/*
 * Menu.h
 *
 *  Created on: 24.11.2017
 *      Author: aca311
 */

#ifndef MENU_H_
#define MENU_H_

#include "Channel.h"

namespace logicLayer {

class Menu {
public:
	Menu(Channel<Signal>&);
	virtual ~Menu();
	void computeInput();
	bool isRestart();

private:
	Channel<Signal>& controller_;
	bool restart;
};
}

#endif /* MENU_H_ */
