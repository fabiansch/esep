/*
 * Blink.h
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#ifndef BLINK_H_
#define BLINK_H_

#include "Header.h"

namespace hal {
namespace mmi {

class Blink {
public:
	Blink();
	virtual ~Blink();

	/**
	* @brief funktor for using in thread
	* Exiting while loop is possible through terminate method if thread was
	* started with std::ref of Blink object before.
	*/
	void operator()();

	/**
	 * @brief merges parameter bitmask to either bitmaskFast or bitmaskSlow
	 * after removing it from the other one.
	 * @param fast = 1: merging with bitmaskFast
	 * @param fast = 0: merging with bitmaskSlow
	 */
	void add(int bitmask, Speed speed);

	/**
	 * @brief removes bitmask from bitmaskFast and bitmaskSlow
	 */
	void removeBitmask(int bitmask);

	/**
	 * @brief causes termination of while loop
	 */
	void terminate();

private:
	bool running;
	int bitmaskSlow;
	int bitmaskFast;
};

} /* namespace mmi */
} /* namespace hal */
#endif /* BLINK_H_ */
