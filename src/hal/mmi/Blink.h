/*
 * Blink.h
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#ifndef BLINK_H_
#define BLINK_H_

namespace hal {
namespace mmi {

class Blink {
public:
	/**
	* @brief blink period in ms
	*/
	Blink();
	virtual ~Blink();
	void operator()();
	void add(int bitmask, bool fast);

private:
	int bitmaskSlow;
	int bitmaskFast;
};

} /* namespace mmi */
} /* namespace hal */
#endif /* BLINK_H_ */
