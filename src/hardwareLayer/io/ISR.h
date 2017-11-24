/**
 * ISR.h
 *
 *  Created on: 20.10.2017
 *      Author: abj240
 *
 */

#ifndef ISR_H_
#define ISR_H_

#include <sys/siginfo.h>
#include "AsyncChannel.h"

namespace hardwareLayer {
namespace io {


class ISR {
public:

	/**
	 * @brief  - registers an Interrupt Handler on the specified channel
	 * @before - GEME computer is connected and running (?)
	 * @after  - Interrupt Handler is working and ready to receive interrupts
	 * @param  - The channel ID the Interrupt Handler is listening on
	 * @retval - none
	 */
	static void registerISR(AsyncChannel& chan, char msgType);

	/**
	 * @brief  - Detaches Interrupt Handler and signals thread function to end
	 * @before - GEME computer is connected and running (?)
	 * @after  - Interrupt has stopped working
	 * @param  - none
	 * @retval - none
	 */
	static void unregisterISR(void);

private:
	/*
	 * @brief getter for pending Interrupts
	 * @ret pending interrupt register
	 */
	static int getPendingIntFlags();

	/*
	 * @brief clears all pending Interrupts
	 */
	static void clearAllPendingIntFlag();

	/*
	 * @brief enables interrupts by mask
	 * @param mask
	 */
	static void enableInterrupts(int mask);

	/*
	 * @brief disables all interrupt
	 */
	static void disableInterrupts();

	ISR();
	~ISR();

	static const struct sigevent* mainISR(void* arg, int id);

	static int isrId;
	static struct sigevent isrEvent;
};

} /* io */
} /* hardwareLayer */
#endif /* ISR_H_ */
