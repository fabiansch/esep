#ifndef TESTCONTROLLER_H_
#define TESTCONTROLLER_H_

#include "SignalReceiver.h"

namespace logicLayer {

class TestController : public SignalReceiver {
public:
  TestController();
  virtual void operator()();

  std::vector<Signal> receivedSignals;
};

} /* namespace logicLayer */
#endif /* TESTCONTROLLER_H_ */
