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

void ItemBuffer::pushItem(Item item){
	items.push_back(item);
}

} /* namespace itembuffer */
} /* namespace io */
} /* namespace hardwareLayer */
