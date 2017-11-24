/*
 * ItemBuffer.h
 *
 *  Created on: 21.11.2017
 *      Author: abx724
 */

#ifndef ITEMBUFFER_H_
#define ITEMBUFFER_H_

#include "Item.h"

namespace hardwareLayer {
namespace io {
namespace serial {

class ItemBuffer {
public:
	ItemBuffer();
	virtual ~ItemBuffer();

	void pushItem(Item);
	Item pullItem();
	int size();

private :
	vector<Item> items;
};

} /* namespace itembuffer */
} /* namespace io */
} /* namespace hardwareLayer */

#endif /* ITEMBUFFER_H_ */
