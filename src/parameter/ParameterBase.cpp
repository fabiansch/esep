/*
 * ParameterBase.cpp
 *
 *  Created on: Nov 9, 2017
 *      Author: stammtisch
 */

#include "ParameterBase.h"

ParameterBase::ParameterBase() :
name("name has not been declared"),
parameterList(ParameterList::instance())
{

}

ParameterBase::ParameterBase(std::string name) :
name(name),
parameterList(ParameterList::instance())
{
}

ParameterBase::~ParameterBase()
{

}

