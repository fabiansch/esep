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

Parameter<bool> this_cb_busy("this conveyer belt busy");
Parameter<bool> next_cb_busy("next conveyer belt busy");


// ====== TIMES FROM CALIBRATION FOR RUN MODE ==========

Parameter<unsigned int> time_cb_unit_total(		"duration time between LB_INPUT_FREED and LB_OUTPUT_INT");

Parameter<unsigned int> time_output_to_input(	"duration time between ITEM_ARRIVED and LB_INPUT_INT. Value is only stored, if cb_this != cb_first");
Parameter<unsigned int> time_input_to_height(	"duration time between LB_INPUT_FREED and LB_HEIGHT_INT");
Parameter<unsigned int> time_height_to_switch(	"duration time between LB_HEIGHT_FREED and LB_SWITCH_INT");
Parameter<unsigned int> time_switch_to_output(	"duration time between LB_SWITCH_FREED and LB_OUTPUT_INT");
Parameter<unsigned int> time_switch_to_slide(	"duration time between LB_SWITCH_FREED and LB_SLIDE_INT");
Parameter<unsigned int> time_lb_slide_freed(		"duration time between LB_SWITCH_INT and LB_SWITCH_FREED");
Parameter<float> slow_factor(					"factor, which is used to calculate slow speed");
Parameter<float> height_conveyor_belt(			"Height of conveyor belt for calibration purposes");
Parameter<float> height_item(					"Height of item for calibration purposes");
