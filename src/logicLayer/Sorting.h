/*
 * Sorting.h
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */




#ifndef SORTING_H_
#define SORTING_H_

enum class Order{ BOM1, BOM2, BMM};

namespace hardwareLayer{
	class HardwareLayer;
}

namespace logicLayer {


class Item;

class Sorting
{
  public:
	static bool amIWanted(Item*);

	static Sorting& instance() {
		static Sorting instance;
		return instance;
	}

	Order getOrderState() const {
		return orderState;
	}

	void setOrderState(Order orderState = Order::BOM1) {
		this->orderState = orderState;
	}

  private:
	Order orderState = Order::BOM1;
	bool checkOrder(Item*);
	bool checkAgainstCB1Preferences(Item*);
	bool checkAgainstCB2Preferences(Item*);
	void updateCB1sSortingUnit(  hardwareLayer::HardwareLayer& );
};

} /* namespace logicLayer */
#endif /* SORTING_H_ */
