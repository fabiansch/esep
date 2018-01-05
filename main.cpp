#include "Header.h"
#include <iostream>

#include "HardwareLayer.h"
#include "LogicLayer.h"

using namespace std;

void evaluateIfCb_1();
void printStartMessage();
void resetNonCalibrationParameters();

int main(int argc, char *argv[])
{
	LOG_SET_LEVEL(DEBUG);
	LOG_SCOPE;

	bool restart = false;
	do {
		printStartMessage();
		if(restart) {
			resetNonCalibrationParameters();
		} else {
			cb_this.parameterList.initParameters();
		}
		evaluateIfCb_1();

		hardwareLayer::HardwareLayer hal;
		logicLayer::LogicLayer loLay(hal);

		restart = loLay.getMenu().isRestart();
	} while (restart);

	cout << "================= Shutting down Sortingmachine done =================" << endl;

	return EXIT_SUCCESS;
}


void evaluateIfCb_1() {
	//============== ASSIGNMENT OF CONVEYOR BELT ROLE ==============================
	cb_first = CB_FIRST;
	cb_all = CB_ALL;
	string textInput;
	do {
		cout << "\n\nAre you master conveyor belt? (yes/no):" << endl;
		cin >> textInput;
	} while (!(textInput.compare("yes") ^ textInput.compare("no")));
	if (!textInput.compare("yes")) {
		cb_this = cb_first;
	}
	cin.get(); // get rid of extra return
}

void printStartMessage() {
			cout<< "	  ____                                           _              _ " <<endl;
			cout<< "	 / ___|___  _ ____   _____ _   _  ___  _ __     / \\   _ __   __| |" <<endl;
			cout<< "	| |   / _ \\| '_ \\ \\ / / _ \\ | | |/ _ \\| '__|   / _ \\ | '_ \\ / _` |" <<endl;
			cout<< "	| |__| (_) | | | \\ V /  __/ |_| | (_) | |     / ___ \\| | | | (_| |" <<endl;
			cout<< "	 \\____\\___/|_| |_|\\_/ \\___|\\__, |\\___/|_|    /_/   \\_\\_| |_|\\__,_|" <<endl;
			cout<< "	                           |___/                                  " <<endl;
			cout<< "	 ____             _   _               ____            _                 " <<endl;
			cout<< "	/ ___|  ___  _ __| |_(_)_ __   __ _  / ___| _   _ ___| |_ ___ _ __ ___  " <<endl;
			cout<< "	\\___ \\ / _ \\| '__| __| | '_ \\ / _` | \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ " <<endl;
			cout<< "	 ___) | (_) | |  | |_| | | | | (_| |  ___) | |_| \\__ \\ ||  __/ | | | | |" <<endl;
			cout<< "	|____/ \\___/|_|   \\__|_|_| |_|\\__, | |____/ \\__, |___/\\__\\___|_| |_| |_|" <<endl;
			cout<< " 	                              |___/         |___/                       " <<endl;
}

void resetNonCalibrationParameters() {
	auto time_output_to_input_temp 	= time_output_to_input;
	auto time_input_to_height_temp  = time_input_to_height;
	auto time_height_to_switch_temp = time_height_to_switch;
	auto time_switch_to_output_temp = time_switch_to_output;
	auto time_switch_to_slide_temp 	= time_switch_to_slide;
	auto time_lb_slide_freed_temp 	= time_lb_slide_freed;
	auto time_cb_unit_total_temp 	= time_cb_unit_total;
	auto slow_factor_temp 			= slow_factor;
	auto height_conveyor_belt_temp 	= height_conveyor_belt;
	auto height_item_temp 			= height_item;

	cb_this.parameterList.initParameters();

	time_output_to_input	= time_output_to_input_temp;
	time_input_to_height	= time_input_to_height_temp;
	time_height_to_switch	= time_height_to_switch_temp;
	time_switch_to_output	= time_switch_to_output_temp;
	time_switch_to_slide	= time_switch_to_slide_temp;
	time_lb_slide_freed		= time_lb_slide_freed_temp;
	time_cb_unit_total		= time_cb_unit_total_temp;
	slow_factor				= slow_factor_temp;
	height_conveyor_belt	= height_conveyor_belt_temp;
	height_item				= height_item_temp;

}
