/*
 * SignalReceiver.h
 *
 *  Created on: 14.11.2017
 *      Author: abx724
 */

#ifndef SIGNALRECEIVER_H_
#define SIGNALRECEIVER_H_

#include <thread>
#include "Channel.h"


namespace logicLayer {


class SignalReceiver {
public:
	SignalReceiver();
	virtual ~SignalReceiver();
	Channel<Signal>& getChannel();
	virtual void operator()() = 0;

protected:
	Channel<Signal> channel_;
	bool running;
	std::thread receiver_;
	void terminate();

private:

};

} /* namespace logicLayer */
#endif /* SIGNALRECEIVER_H_ */
