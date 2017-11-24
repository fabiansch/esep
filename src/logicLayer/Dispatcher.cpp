/*
 * Dummy.cpp
 *
 *  Created on: 17.11.2017
 *      Author: abx724
 */

#include "Header.h"
#include "Dispatcher.h"


namespace logicLayer {

Dispatcher::Dispatcher(
		Channel& controller,
		Channel& typeIdent
) :
controller_(controller),
typeIdent_(typeIdent),
t_( std::ref(*this) ),
running(true)
{

}

Dispatcher::~Dispatcher() {
	cout << "call Dispatcher's deconstructor " << endl;
	terminate();
}


void Dispatcher::operator ()(){
	Signal sig;
	while(running){
		sig.name = Signalname::SIGNAL_DUMMY;
		controller_ << sig;
		typeIdent_ << sig;
		std::cout << "dispatcher sended signal ... " << endl;
		WAIT(1000);
	}
}

void Dispatcher::terminate(){
	running = false;
	t_.join();
}

} /* namespace logicLayer */
