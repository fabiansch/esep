/*
 * Parameter.cpp
 *
 *  Created on: 09.11.2017
 *      Author: abj240
 */

#include "Parameter.h"
#include "Signals.h"

// all parameters need to call standard constructor here

Parameter<uint8_t> cb_this(		"this conveyer belt");
Parameter<uint8_t> cb_next(		"next conveyer belt");
Parameter<uint8_t> cb_first(	"first conveyer belt");
Parameter<uint8_t> cb_last(		"last conveyer belt");
Parameter<uint8_t> cb_all(		"all conveyer belts");
Parameter<uint8_t> cb_available("available conveyer belts");

