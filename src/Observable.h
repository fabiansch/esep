/*
 * Observable.h
 *
 *  Created on: 30.10.2017
 *      Author: abx852
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <vector>
#include "Observer.h"
#include "Logger.h"

class Observable {
public:
	void register_observer(Observer* o) {
		//check if observer already exists
		if (observers.size() > 0) {
			for (auto &observer : observers) {
				if (observer == o) {
					LOG_ERROR<<__FUNCTION__<<" tried to register observer for second time.";
					exit(EXIT_FAILURE);
				}
			}
		}
		observers.push_back(o);

	}
	void notify_observers() {
		for (auto &observer : observers) {
			observer->notify();
		}
	}
private:
	std::vector<Observer*> observers;
};

#endif /* OBSERVABLE_H_ */
