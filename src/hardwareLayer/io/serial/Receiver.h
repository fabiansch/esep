/*
 * Receiver.h
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Interface.h"
#include "WatchDog.h"
#include "SignalGenerator.h"
#include "ItemBuffer.h"
#include <thread>


namespace hardwareLayer {
namespace io {
namespace serial {

class Receiver {
public:
    Receiver(Interface& serial, WatchDog& dog, SignalGenerator& sgen );
    ~Receiver();
    void operator()();
    void terminate();
    ItemBuffer& getItemBuffer();

private:
	void setNext_cb();
	void evaluateTokenAndSendFeed(const Message& msg);
	void registerOnToken(Message& msg);
	void sendFeed();
	void evaluateFeed(const Message& msg);
	void forwardIfNotMaster(Message& msg);

    Interface& serial_;
    WatchDog& dog_;
    SignalGenerator& sgen_;
    bool running;
	std::thread _th_receiver;
	ItemBuffer itemBuffer_;
};

} /* namespace serial */
} /* namespace io */
} /* namespace hal */


#endif /* RECEIVER_H_ */
