/* The IRQ handler
 *
 * The IRQ-Handler checks the DIO48 IRQ-Statusregister.
 * If PortB IRQ is signaled the IRQ-Statusregister is cleared and
 * the event SIGEV_INTR is returned. Elsewise nothing is done and
 * the event SIGEV_NONE is returned.
 *
 */

#include "ISR.h"
#include <sys/neutrino.h>
#include "DIO48.h"
#include "GPIO.h"

#define HW_INTERRUPT 11

constexpr uint8_t UPPER_BYTE = 0xf0;
constexpr uint8_t FULL_BYTE = 0xff;

using namespace std;

namespace hardwareLayer {
namespace io {

int ISR::isrId;
struct sigevent ISR::isrEvent;

ISR::ISR() {
	GPIO::instance().gainAccess();
}

int ISR::getPendingIntFlags() {
	return GPIO::instance().read(DIO_CHG_STATE_IRQ_STATUS);
}

void ISR::disableInterrupts() {
	GPIO::instance().clearBits(DIO_CHG_STATE_IRQ_ENABLE, FULL_BYTE);
	GPIO::instance().setBits(DIO_CHG_STATE_IRQ_ENABLE, FULL_BYTE);
}

void ISR::enableInterrupts(int mask) {
	GPIO::instance().clearBits(DIO_CHG_STATE_IRQ_ENABLE, FULL_BYTE);
	GPIO::instance().setBits(DIO_CHG_STATE_IRQ_ENABLE, ~(mask));
}

// no debug message allowed, otherwise isr crashes
void ISR::clearAllPendingIntFlag() {
	GPIO::instance().clearBits(DIO_CHG_STATE_IRQ_STATUS, FULL_BYTE);
}


const struct sigevent* ISR::mainISR(void* arg, int id) {
	int interrupt_service_register;
	struct sigevent* event = (struct sigevent*) arg;

	// determine the source of the interrupt by reading the Interrupt Status Register
	interrupt_service_register =
			GPIO::instance().read(DIO_CHG_STATE_IRQ_STATUS) &
			(DIO_GROUP0_PB_STATE_CHANGED | DIO_GROUP0_PC_HI_STATE_CHANGED);

	ISR::clearAllPendingIntFlag();

	/* 	no interrupt? */
	if (interrupt_service_register == 0) return nullptr; /* then no event */

	int mask = ((GPIO::instance().read(PORT::C) & UPPER_BYTE) << 8) |
				(GPIO::instance().read(PORT::B) & FULL_BYTE);

	event->sigev_value.sival_int = mask;
	return event;
}


void ISR::registerISR(AsyncChannel& chan, char msgType){
    // disable IRQ
    ISR::disableInterrupts();

    // clear any possible pending irq
    ISR::clearAllPendingIntFlag();

	// initialize event structure
    SIGEV_PULSE_INIT(&isrEvent, chan.getConnectionId(), SIGEV_PULSE_PRIO_INHERIT, (int)msgType, 0);

    // attach mainISR handler to hardware interrupt
    isrId = InterruptAttach(HW_INTERRUPT, ISR::mainISR, &isrEvent, sizeof(isrEvent), 0);
    if (isrId == -1) {
        LOG_ERROR<<"Could not attach mainISR handler to hw interrupt"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // enable IRQ for PortB and PortC
    ISR::enableInterrupts(DIO_GROUP0_PB_IRQ_ENABLE | DIO_GROUP0_PC_IRQ_ENABLE);
}

void ISR::unregisterISR(){

    // detach interrupt handler
    if( InterruptDetach(isrId) < 0 ){
        LOG_ERROR<<"Could not detach interrupt handler"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // disable IRQ
    ISR::disableInterrupts();

    // clear any possible pending irq
    ISR::clearAllPendingIntFlag();
}

} /* io */
} /* hardwareLayer */
