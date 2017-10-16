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
	Blink(bool fast);
	virtual ~Blink();
	void operator()();
	void add(int bitmask);

private:
	bool fast;
	int bitmask;
};

} /* namespace mmi */
} /* namespace hal */
#endif /* BLINK_H_ */
