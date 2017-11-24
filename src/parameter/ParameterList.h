/*
 * ParameterList.h
 *
 *  Created on: Nov 9, 2017
 *      Author: stammtisch
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include <vector>
#include "ParameterBase.h"

class ParameterBase;

class ParameterList {
public:
	static ParameterList& instance();
	virtual ~ParameterList();

	void showParameters();

	std::vector<ParameterBase*> parameters;

private:
	ParameterList();
	ParameterList(ParameterList const&);
	ParameterList& operator=(ParameterList const&);
};

#endif /* PARAMETERLIST_H_ */
