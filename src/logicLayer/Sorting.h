/*
 * Sorting.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#ifndef SORTING_H_
#define SORTING_H_

enum class Order{ BOM1, BOM2, BMM};

namespace logicLayer {

class Item;

class Sorting
{
  public:
	static bool amIWanted(Item*);

  private:
	Order orderState = Order::BOM1;
	bool checkOrder(Item*);
	bool checkAgainstCB1Preferences(Item*);
	bool checkAgainstCB2Preferences(Item*);
};

} /* namespace logicLayer */
#endif /* SORTING_H_ */
