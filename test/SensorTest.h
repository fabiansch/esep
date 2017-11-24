/*
 * SensorTest.h
 *
 *  Created on: 16.11.2017
 *      Author: abj240
 */

#ifndef SENSORTEST_H_
#define SENSORTEST_H_

#include "Signals.h"
#include "HardwareLayer.h"
#include "Header.h"
#include "Item.h"
#include <iostream>
#include <string>

namespace logicLayer {

class SensorTest {
private:
	struct State {//top-level state
		virtual void sensor_test_start(){			testFailed(__FUNCTION__);}
		virtual void sensor_test_successful(uint8_t sender){	testFailed(__FUNCTION__);}
		virtual void lb_input_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_input_freed(){				testFailed(__FUNCTION__);}
		virtual void sensor_height_match(){			testFailed(__FUNCTION__);}
		virtual void sensor_height_not_match(){		testFailed(__FUNCTION__);}
		virtual void sensor_metal_match(){			testFailed(__FUNCTION__);}
		virtual void sensor_metal_not_match(){		testFailed(__FUNCTION__);}
		virtual void sensor_switch_is_open(){		testFailed(__FUNCTION__);}
		virtual void sensor_switch_is_closed(){		testFailed(__FUNCTION__);}
		virtual void lb_height_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_height_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_switch_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_switch_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_slide_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_slide_freed(){				testFailed(__FUNCTION__);}
		virtual void lb_output_interrupted(){		testFailed(__FUNCTION__);}
		virtual void lb_output_freed(){				testFailed(__FUNCTION__);}

		void testFailed(string functionname) {
			cout<<"Test failed: "<<name()<<", caused of "<<functionname<<endl;
			hal->motorStop();
			hal->greenLightOff();
			hal->blinkRed(Speed::fast);
			new (this) LB_INPUT_Test;
		}

		std::string name() const { return typeid(*this).name(); }

		hardwareLayer::HardwareLayer* hal;
		Item* testItem;
	} *statePtr;   // a pointer to current state. Used for polymorphism.






	//============================ LB_INPUT_Test =======================================
	struct LB_INPUT_Test : public State {
		virtual void lb_input_interrupted() {
			cout<<__FUNCTION__<<endl;
			hal->motorRotateClockwise();
			hal->motorFast();
			hal->motorStart();
		}
		virtual void lb_input_freed() {
			cout<<__FUNCTION__<<endl;

			//hal.blinkGreen(Speed::slow);

			cout<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_Test;
			cout<<name()<<endl;
		}
//		virtual void sensor_test_start(){
//			cout<<__FUNCTION__<<endl;
//			hal->motorRotateClockwise();
//			hal->motorFast();
//			hal->motorStart();
//		}
	};

	//============================ SENSOR_HEIGHT_MATCH_Test =======================================
	struct SENSOR_HEIGHT_MATCH_Test : public State {
		virtual void sensor_height_match() {
			cout<<__FUNCTION__<<endl;

			testItem->heightAbsolute = hal->getHeight();
			cout<<"ABSOLUTE HOEHE: "<<testItem->heightAbsolute<<endl;

			cout<<name()<<" => ";
			new (this) SENSOR_HEIGHT_NOT_MATCH_Test;
			cout<<name()<<endl;
		}
	};

	//============================ SENSOR_HEIGHT_NOT_MATCH_Test =======================================
	struct SENSOR_HEIGHT_NOT_MATCH_Test : public State {

		virtual void sensor_height_not_match() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) LB_HEIGHT_INTERRUPT_Test;
			cout<<name()<<endl;
		}
	};

	//============================ LB_HEIGHT_INTERRUPT_Test =======================================
	struct LB_HEIGHT_INTERRUPT_Test : public State {
		virtual void sensor_height_match() {
			cout<<__FUNCTION__<<endl;
			cout<<name()<<" => ";
			new (this) SENSOR_HEIGHT_NOT_MATCH_Test;
			cout<<name()<<endl;
		}
		virtual void lb_height_interrupted() {
			cout<<__FUNCTION__<<endl;

			testItem->heightCenter = hal->getHeight();
			cout<<"HOEHE DER MITTE: "<<testItem->heightCenter<<endl;

			if(cb_this == cb_last && hal->getItemBufferSize() > 0) {
				Item passedItem = hal->getPassedItem();
				if( abs(passedItem.heightCenter - testItem->heightCenter) < 200) {
					cout<<"DIFFERENZ IST NICHT ALLZU GROSS."<<endl;
				} else {
					cout<<"DIFFERECE TOO HIGH"<<endl;
					testFailed(__FUNCTION__);
				}
			}
			cout<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_2_Test;
			cout<<name()<<endl;
		}
	};

	//============================ SENSOR_HEIGHT_MATCH_2_Test =======================================
	struct SENSOR_HEIGHT_MATCH_2_Test : public State {
		virtual void sensor_height_match() {
			cout<<__FUNCTION__<<endl;
		}
		virtual void sensor_height_not_match() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) LB_HEIGHT_FREE_Test;
			cout<<name()<<endl;
		}
	};

	//============================ LB_HEIGHT_FREE_Test =======================================
	struct LB_HEIGHT_FREE_Test : public State {
		virtual void sensor_height_match() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) SENSOR_HEIGHT_MATCH_2_Test;
			cout<<name()<<endl;
		}
		virtual void lb_height_freed() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) SENSOR_METAL_MATCH_Test;
			cout<<name()<<endl;
		}
	};

	//============================ SENSOR_METAL_MATCH_Test =======================================
	struct SENSOR_METAL_MATCH_Test : public State {
		virtual void sensor_metal_match() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) LB_SWITCH_INTERRUPT_Test;
			cout<<name()<<endl;
		}
	};

	//============================ LB_SWITCH_INTERRUPT_Test =======================================
	struct LB_SWITCH_INTERRUPT_Test : public State {
		virtual void lb_switch_interrupted() {
			cout<<__FUNCTION__<<endl;
			cout<<name()<<" => ";
			new (this) SENSOR_SWITCH_IS_OPEN_test;
			cout<<name()<<endl;
			hal->switchPointOpen();
		}
	};

	//============================ SENSOR_SWITCH_IS_OPEN_test =======================================
	struct SENSOR_SWITCH_IS_OPEN_test : public State {
		virtual void sensor_switch_is_open() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) SENSOR_METAL_NOT_MATCH_Test;
			cout<<name()<<endl;
		}
	};

	//============================ SENSOR_METAL_NOT_MATCH_Test =======================================
	struct SENSOR_METAL_NOT_MATCH_Test : public State {
		// this transition is temporary while shatter protection is not implemented
		virtual void sensor_switch_is_closed() {
			cout<<__FUNCTION__<<"======================================= THIS IS ONLY ALLOWED TO HAPPEN TEMPORARY WHILEST SHATTER PROTECTION IS NOT IMPLEMENTED. ==================="<<endl;

			cout<<name()<<" => ";
			new (this) SENSOR_SWITCH_IS_OPEN_test;
			cout<<name()<<endl;
		}

		virtual void sensor_metal_not_match() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) LB_SWITCH_FREE_Test;
			cout<<name()<<endl;
		}
	};

	//============================ LB_SWITCH_FREE_Test =======================================
	struct LB_SWITCH_FREE_Test : public State {
		virtual void lb_switch_freed() {
			cout<<__FUNCTION__<<endl;

			cout<<name()<<" => ";
			new (this) LB_OUTPUT_Test;
			cout<<name()<<endl;
		}
	};

	//============================ LB_OUTPUT_Test =======================================
	struct LB_OUTPUT_Test : public State {
		virtual void lb_output_interrupted() {
			cout<<__FUNCTION__<<endl;
			hal->switchPointClose();
			if(cb_this == cb_last) {
				cout<<"WE ARE LAST"<<endl;
								hal->motorStop();
				cout<<"please put item on master's input again."<<endl;
			} else {
				cout<<"WE ARE NOT LAST"<<endl;
				hal->sendSerial(Signal(cb_this, cb_next, Signalname::SENSOR_TEST_START));
				hal->sendItemViaSerial(testItem);
			}
		}
		virtual void lb_output_freed() {
			cout<<__FUNCTION__<<endl;
			cout<<name()<<" successfully"<<endl;
			if(cb_this != cb_last) {
				cout<<"Test continues on next conveyer belt. When item reaches last conveyer belt's output, please put it on input of master again."<<endl;
			}
			cout<<name()<<" => ";
			new (this) LB_SLIDE_Test;
			cout<<name()<<endl;
		}
		virtual void sensor_switch_is_closed(){}
	};

	//============================ LB_SLIDE_Test =======================================
	struct LB_SLIDE_Test : public State {
		virtual void lb_input_interrupted() {
			hal->motorRotateClockwise();
			hal->motorFast();
			hal->motorStart();
		}
		virtual void lb_input_freed() {}
		virtual void lb_height_interrupted() {}
		virtual void lb_height_freed() {}
		virtual void lb_switch_interrupted() {}
		virtual void lb_switch_freed() {}
		virtual void sensor_height_match(){	}
		virtual void sensor_height_not_match(){}
		virtual void sensor_metal_match(){	}
		virtual void sensor_metal_not_match(){}
		virtual void lb_slide_interrupted() {
			cout<<__FUNCTION__<<endl;
			hal->motorStop();
		}
		virtual void lb_slide_freed() {
			cout<<__FUNCTION__<<endl;
			cout<<name()<<" successfully"<<endl;
			hal->sendSerial(Signal(cb_this,cb_1,Signalname::SENSOR_TEST_SUCCESSFUL));

			if(cb_this != cb_1) {
				cout<<name()<<" => ";
				new (this) LB_INPUT_Test;;
				cout<<name()<<endl;
			} else {
				cout<<name()<<" => ";
				new (this) OTHER_CBs_Test;;
				cout<<name()<<endl;
			}
		}
	};

	//============================ OTHER_CBs_Test =======================================
	struct OTHER_CBs_Test : public State {
		virtual void sensor_test_successful(uint8_t sender) {
			cout<<"Test was successful on conveyer belt: "<<(int)sender<<endl;
			if(sender == cb_last) {
				cout<<"### SENSOR TEST finished ###"<<endl;
				cout<<"hit enter to go on"<<endl;
				new (this) LB_INPUT_Test;
			}
		}
		virtual void sensor_test_unsuccessful(uint8_t sender) {
			cout<<"Test UNsuccessful on conveyer belt: "<<(int)sender<<endl;
			cout<<"Please restart test."<<endl;
			cout<<name()<<" => ";
			new (this) LB_INPUT_Test;
			cout<<name()<<endl;
		}
	};


	LB_INPUT_Test stateMember;//The memory for the state is part of context object

	hardwareLayer::HardwareLayer& hal;
	Item testItem;

public:
	SensorTest(hardwareLayer::HardwareLayer& hal)
	: statePtr(&stateMember) // assigning start state
	, hal(hal)
	{
		statePtr->hal = &hal;
		statePtr->testItem = &testItem; // connecting state->testItem with the SensorTest::testItem

		hal.blinkGreen(Speed::slow);

		if(cb_this == cb_1) {
		cout<<"### SENSOR TEST started ###"<<endl;
		cout<<"Please put item (metal above) on master's input."<<endl;
		}
	}

	virtual ~SensorTest(){};

	std::string nameOf(State *state) const { return typeid(*state).name(); }

	virtual void sensor_test_start(){		statePtr->sensor_test_start();}
	virtual void sensor_test_successful(uint8_t sender){	statePtr->sensor_test_successful(sender);}

	virtual void lb_input_interrupted(){	statePtr->lb_input_interrupted();}
	virtual void lb_input_freed(){			statePtr->lb_input_freed();}

	virtual void sensor_height_match(){			statePtr->sensor_height_match();}
	virtual void sensor_height_not_match(){		statePtr->sensor_height_not_match();}

	virtual void lb_height_interrupted(){	statePtr->lb_height_interrupted();}
	virtual void lb_height_freed(){			statePtr->lb_height_freed();}

	virtual void sensor_metal_match(){			statePtr->sensor_metal_match();}
	virtual void sensor_metal_not_match(){		statePtr->sensor_metal_not_match();}

	virtual void sensor_switch_is_open(){		statePtr->sensor_switch_is_open();}
	virtual void sensor_switch_is_closed(){		statePtr->sensor_switch_is_closed();}

	virtual void lb_switch_interrupted(){	statePtr->lb_switch_interrupted();}
	virtual void lb_switch_freed(){			statePtr->lb_switch_freed();}

	virtual void lb_slide_interrupted(){	statePtr->lb_slide_interrupted();}
	virtual void lb_slide_freed(){			statePtr->lb_slide_freed();}

	virtual void lb_output_interrupted(){	statePtr->lb_output_interrupted();}
	virtual void lb_output_freed(){			statePtr->lb_output_freed();}
};

} /* namespace logicLayer */
#endif /* SENSORTEST_H_ */
