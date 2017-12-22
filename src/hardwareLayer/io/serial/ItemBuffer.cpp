/*
 * ItemBuffer.cpp
 *
 *  Created on: 21.11.2017
 *      Author: abx724
 */

#include "ItemBuffer.h"

namespace hardwareLayer {
namespace io {
namespace serial {

ItemBuffer::ItemBuffer() {
	// TODO Auto-generated constructor stub
}

ItemBuffer::~ItemBuffer() {
	// TODO Auto-generated destructor stub
}

void ItemBuffer::pushItem(logicLayer::Item item){
	items.push_back(item);
}

logicLayer::Item ItemBuffer::pullItem() {

	logicLayer::Item item = items.front();
	while(item.getId()==0){
		item = items.front();
		items.erase(items.begin());
	}

	items.erase(items.begin());
	return item;
}

int ItemBuffer::size(){
	return items.size();
}

} /* namespace itembuffer */
} /* namespace io */
} /* namespace hardwareLayer */
