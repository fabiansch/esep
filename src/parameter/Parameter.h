/*
 * Parameter.h
 *
 *  Created on: Nov 9, 2017
 *      Author: stammtisch
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "ParameterBase.h"
#include "Logger.h"
#include <iostream>
#include <cstdint>


using namespace std;

constexpr uint8_t CB_FIRST = 	0b00000001;
constexpr uint8_t CB_ALL = 		0b11111111;

template <class T>
class Parameter : public ParameterBase {
public:
	explicit Parameter(string name) :
	Parameter(0, name)
	{
	}

	explicit Parameter(T value, string name) :
	ParameterBase(name),
	value(value)
	{
		parameterList.parameters.push_back(this);
	}

	virtual ~Parameter()
	{
	}

	operator T() const
	{
		return value;
	}

	const Parameter<T>& operator=(const Parameter<T>& other) {
		if(other == *this) {
			return *this;
		}
		value = other.value;
		return *this;
	}

//	const Parameter<T>& operator=(const uint8_t& other) {
//		value = other;
//		return *this;
//	}

	const Parameter<T>& operator=(const unsigned int& other) {
		value = other;
		return *this;
	}


	void showValue()
	{



		cout<<name<<": "<<(int)value<<endl;
	}

	T value;
};

extern Parameter<uint8_t> cb_this;
extern Parameter<uint8_t> cb_next;
extern Parameter<uint8_t> cb_first;
extern Parameter<uint8_t> cb_last;
extern Parameter<uint8_t> cb_all;
extern Parameter<uint8_t> cb_available;
extern Parameter<unsigned int> time_output_to_input;
extern Parameter<unsigned int> time_input_to_height;
extern Parameter<unsigned int> time_height_to_switch;
extern Parameter<unsigned int> time_switch_to_output;
extern Parameter<unsigned int> time_switch_to_slide;
extern Parameter<unsigned int> time_lb_slide_freed;
extern Parameter<unsigned int> time_cb_unit_total;
extern Parameter<double> slow_factor;
extern Parameter<double> height_conveyor_belt; //int or double?
extern Parameter<double> height_item; //int or double?

#endif /* PARAMETER_H_ */
