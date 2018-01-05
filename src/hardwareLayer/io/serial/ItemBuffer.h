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

	void pushItem(logicLayer::Item);
	logicLayer::Item pullItem();
	int size();
	void reset();

private :
	vector<logicLayer::Item> items;
};

} /* namespace itembuffer */
} /* namespace io */
} /* namespace hardwareLayer */

#endif /* ITEMBUFFER_H_ */
