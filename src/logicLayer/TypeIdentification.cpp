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
	receiver_ = std::thread(std::ref(*this));
}

TypeIdentification::~TypeIdentification() {
	cout << "STOP" << endl;
}

void TypeIdentification::operator()(){
	Signal sig;
	while(running){

		sig << channel_;
		switch (sig.name) {
			case Signalname::TEST:
				std::cout << "TypeIdentification receives signal." << endl;
				break;
			default:
				break;
		}

		std::cout << "TypeIdentification end of while" << endl;
	}

	std::cout << "after while" << endl;
}

} /* namespace logicLayer */
