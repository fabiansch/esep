/*
 * Serial.h
 *
 *  Created on: 16.10.2017
 *      Author: aca311
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <string>
#include <Signals.h>
#include "Message.h"


namespace hardwareLayer {
namespace io {
namespace serial {

class Interface {
	public:
		Interface(std::string dev_send, std::string dev_read);
		Interface(std::string dev_send, std::string dev_read, int baud);
		~Interface();

		/*
		 *  @brief sends Signal to serial interface
		 *  @return the number of written bytes or -1 if error occured.
		 */
		int send(Message& message);

		/*
		 *@brief: receives Message from serial interface in a threadsafe manner.
		 *@brief: waits till sizeof(Message) bytes received or timeout happened.
		 *@return: 0 or -1 when error occured.
		 */
		int recv(Message *msg);

		/*
		 *@brief: performs flush on serial interface in a threadsafe manner.
		 *@return: 0 or -1 when error occured.
		 */
		int flush(void);

	private:
		/*
		 * @brief init serial interface
		 * @param int baud - baudrate
		 * @return 0 or negativ when error(s) occured.
		 *
		 * with following options:
		 * input and output with @param baud
		 * 8N1 : (8 data bits, no parity, 1 stop bit)
		 * ignore modem status lines
		 *
		 */
		int init(int baud);
		Interface(const Interface& other);
		Interface& operator=(const Interface& other);

		int fd_read;
		int fd_send;
	};


} /* namespace serial */
} /* namespace io */
} /* namespace hal */

#endif /* INTERFACE_H_ */
