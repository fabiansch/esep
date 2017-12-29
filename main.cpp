#include "Header.h"
#include <iostream>

#include "HardwareLayer.h"
#include "LogicLayer.h"
#include "TestController.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

void evaluateIfCb_1();
void printStartMessage();

SCENARIO( "light barriers can be interrupted and can be freed" ) {

    int max_delta = 600; // ms

    GIVEN( "a sortingmachine in RUN state" ) {
        hardwareLayer::HardwareLayer hal;
        logicLayer::LogicLayer lol(hal);
        time_input_to_height = 1000;
        logicLayer::TestController testController;
        lol.getTimer().setControllerChannel(&testController.getChannel());

        hal.getSignalGenerator().pushBackOnSignalBuffer(
                Signal(Signalname::RUN));

        WHEN( "in: START_TIMERS_HEIGHT" ) {
            lol.getTimer().getChannel() << Signal(Signalname::START_TIMERS_HEIGHT);
            THEN( "to TestController was nothing sent yet" ) {
                REQUIRE( testController.receivedSignals.size() == 0 );

                WHEN( "waited for time_input_to_height" ) {
                    WAIT(time_input_to_height);

                    THEN( "out: TIMEFRAME_HEIGHT_ENTER" ) {
                        if( testController.receivedSignals.size() > 0 ) {
                            Signal s1 = testController.receivedSignals.front();
                            testController.receivedSignals.erase(testController.receivedSignals.begin());
                            Signal s2 = Signal(Signalname::TIMEFRAME_HEIGHT_ENTER);
                            REQUIRE( s1.name == s2.name );
                        } else {
                            REQUIRE(false);
                        }
                        WHEN( "in: TIMEFRAME_HEIGHT_LEAVE_KILL" ) {
                            lol.getTimer().getChannel() << Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL);

                            WHEN( "max_delta was waited" ) {
                                WAIT(max_delta);

                                THEN( "not out: TIMEFRAME_HEIGHT_LEAVE" ) {
                                    REQUIRE(testController.receivedSignals.size() == 0 );
                                }
                            }
                        }
                        WHEN( "max_delta was waited" ) {
                            WAIT(max_delta);

                            THEN( "out: TIMEFRAME_HEIGHT_LEAVE" ) {
                                if( testController.receivedSignals.size() > 0 ) {
                                    Signal s1 = testController.receivedSignals.front();
                                    testController.receivedSignals.erase(testController.receivedSignals.begin());
                                    Signal s2 = Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE);
                                    REQUIRE( s1.name == s2.name );
                                } else {
                                    REQUIRE(false);
                                }
                            }
                        }
                        WHEN( "START_TIMERS_HEIGHT" ) {
                            lol.getTimer().getChannel() << Signal(Signalname::START_TIMERS_HEIGHT);

                            WHEN( "in TIMEFRAME_HEIGHT_LEAVE_KILL" ) {
                                lol.getTimer().getChannel() << Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE_KILL);

                                WHEN( "time_input_to_height + max_delta was waited" ) {
                                    WAIT(time_input_to_height + max_delta);

                                    THEN( "not out: TIMEFRAME_HEIGHT_LEAVE" ) {
                                        if( testController.receivedSignals.size() > 0 ) {
                                            Signal s1 = testController.receivedSignals.front();
                                            Signal s2 = Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE);
                                            REQUIRE( s1.name != s2.name );
                                        } else {
                                            REQUIRE(true);
                                        }

                                        THEN( "out: TIMEFRAME_HEIGHT_ENTER" ) {
                                            if( testController.receivedSignals.size() > 0 ) {
                                                Signal s1 = testController.receivedSignals.front();
                                                testController.receivedSignals.erase(testController.receivedSignals.begin());
                                                Signal s2 = Signal(Signalname::TIMEFRAME_HEIGHT_ENTER);
                                                REQUIRE( s1.name == s2.name );
                                            } else {
                                                REQUIRE(false);
                                            }

                                            THEN( "out: TIMEFRAME_HEIGHT_LEAVE" ) {
                                                if( testController.receivedSignals.size() > 0 ) {
                                                    Signal s1 = testController.receivedSignals.front();
                                                    testController.receivedSignals.erase(testController.receivedSignals.begin());
                                                    Signal s2 = Signal(Signalname::TIMEFRAME_HEIGHT_LEAVE);
                                                    REQUIRE( s1.name == s2.name );
                                                } else {
                                                    REQUIRE(false);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// int main(int argc, char *argv[])
// {
// 	LOG_SET_LEVEL(DEBUG);
// 	LOG_SCOPE;

// 	bool restart = false;
// 	do {
// 		printStartMessage();
// 		cb_this.parameterList.initParameters();
// 		evaluateIfCb_1();

// 		hardwareLayer::HardwareLayer hal;
// 		logicLayer::LogicLayer loLay(hal);

// 		restart = loLay.getMenu().isRestart();
// 	} while (restart);

// 	cout << "===================== Shutting down Sortingmachine =========================" << endl;

// 	return EXIT_SUCCESS;
// }


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
