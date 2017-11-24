/*
 * ParameterBase.h
 *
 *  Created on: Nov 9, 2017
 *      Author: stammtisch
 */

#ifndef PARAMETERBASE_H_
#define PARAMETERBASE_H_

#include <string>
#include "ParameterList.h"
#include <iostream>
using namespace std;

class ParameterList;

class ParameterBase {
public:
	ParameterBase();
	ParameterBase(std::string name);
	virtual ~ParameterBase();

	virtual void showValue() = 0;

	std::string name;
	ParameterList& parameterList;
};

#endif /* PARAMETERBASE_H_ */
