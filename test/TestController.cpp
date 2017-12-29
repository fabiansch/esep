#include "TestController.h"

namespace logicLayer {

TestController::TestController()
{
  SignalReceiver::receiver_ = std::thread(std::ref(*this));
}

void TestController::operator()() {
  LOG_SCOPE

  Signal signal;
  while(running) {
    signal << channel_;
    receivedSignals.push_back(signal);
  }
}

} /* namespace logicLayer */
