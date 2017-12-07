/*
 * TypeIdentification.cpp
 *
 *  Created on: 15.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "TypeIdentification.h"

namespace logicLayer {

TypeIdentification::TypeIdentification()
{
	LOG_SCOPE
	SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

TypeIdentification::~TypeIdentification() {
	LOG_SCOPE
}

void TypeIdentification::operator()(){
	LOG_SCOPE
	Signal sig;
	while(running){

		sig << channel_;
		switch (sig.name) {
			case Signalname::SIGNAL_DUMMY:
				LOG_WARNING<<__FUNCTION__<<": received SIGNAL_DUMMY"<<endl;
				break;
			default:
				break;
		}
	}

}

} /* namespace logicLayer */
