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
Parameter<uint8_t> cb_previous(	"previous conveyer belt");
Parameter<uint8_t> cb_next(		"next conveyer belt");
Parameter<uint8_t> cb_first(	"first conveyer belt");
Parameter<uint8_t> cb_last(		"last conveyer belt");
Parameter<uint8_t> cb_all(		"all conveyer belts");
Parameter<uint8_t> cb_available("available conveyer belts");
Parameter<uint8_t> cb_sorting_1("sorting 1 conveyer belt");
Parameter<uint8_t> cb_sorting_2("sorting 2 conveyer belt");


Parameter<int> items_on_cb("amount of items that currently are on cb_this");
Parameter<bool> item_on_switch("Item on SwitchPoint");
Parameter<bool> item_on_output("Item on Output");

Parameter<bool> next_cb_busy("next conveyer belt busy");

