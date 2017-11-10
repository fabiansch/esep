/*
 * WatchDog.cpp
 *
 *  Created on: 30.10.2017
 *      Author: abx724
 */

#include "Header.h"
#include "WatchDog.h"
#include <thread>

namespace hardwareLayer {
namespace io {
namespace serial {

WatchDog::WatchDog(Serial& serial, SignalGenerator& sgen) :
		serial_(serial),
		sgen_(sgen),
		otherDogisAlive(false){}

WatchDog::~WatchDog() {
	// TODO Auto-generated destructor stub
}


void WatchDog::operator()(){
	while(true){

		struct Message msg;
		msg.signal.name   	= Signalname::SERIAL_ARE_YOU_ALIVE;
		msg.signal.sender 	= 0;
		msg.signal.receiver = 1;
		msg.payload.das 	= 42;
		msg.payload.dies	= 42;
		msg.payload.jenes 	= 42;


		//ask other machine if it i alive
		serial_.send(msg);

		WAIT(1000);


		//check if other machine has send keep alive signal -
		//if not error signal
		if(!otherDogisAlive){
			std::cout << "!!!machine is not connected" << std::endl;
			Signal sig;
			sig.name = Signalname::CONNECTION_LOST;
			sgen_.pushBackOnSignalBuffer(sig);
		}

		setOtherDogIsAlive(false);

	}
}

void WatchDog::setOtherDogIsAlive(bool isAlive){

	otherDogisAlive = isAlive;
	if ( isAlive ) {
		Signal sig;
		sig.name = Signalname::CONNECTION_CONNECTED;
		sgen_.pushBackOnSignalBuffer(sig);
	}

}

void WatchDog::sendImAlive(){

	struct Message msg;
	msg.signal.name   	= Signalname::SERIAL_IM_ALIVE;
	msg.signal.sender 	= 0;
	msg.signal.receiver = 1;
	msg.payload.das 	= 42;
	msg.payload.dies	= 42;
	msg.payload.jenes 	= 42;

	serial_.send(msg);

}

} /* namespace serial */
} /* namespace io */
} /* namespace hal */
