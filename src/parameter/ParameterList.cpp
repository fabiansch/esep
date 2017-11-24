/*
 * ParameterList.cpp
 *
 *  Created on: Nov 9, 2017
 *      Author: stammtisch
 */

#include "ParameterList.h"
#include <iostream>

ParameterList& ParameterList::instance() {
	static ParameterList parameterList;
	return parameterList;
}

ParameterList::ParameterList() {

}

ParameterList::~ParameterList() {

}

void ParameterList::showParameters() {
	for(auto parameter : parameters) {
		parameter->showValue();
	}
	std::cout<<std::endl;
}
