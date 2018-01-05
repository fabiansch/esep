/*
 * Observer.h
 *
 *  Created on: 30.10.2017
 *      Author: abx852
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

class Observer {
	public:
	virtual ~Observer(){}
	virtual void notify() = 0;
};

#endif /* OBSERVER_H_ */
