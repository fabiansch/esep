/*
 * Serial.cpp
 *
 *  Created on: 19.05.2017
 *      Author: aca216
 */

#include "Interface.h"

#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <cerrno>


using namespace std;

namespace hardwareLayer {
namespace io {
namespace serial {

struct termios termino_send;
struct termios termino_read;

constexpr int STANDARD_BAUD_RATE = B19200;
constexpr int TIMEOUT_2_SECONDS = 20;

Interface::Interface(std::string dev_send, std::string dev_read) :
Interface(dev_send, dev_read, STANDARD_BAUD_RATE)
{
	LOG_SCOPE
}
Interface::Interface(std::string dev_send, std::string dev_read, int baud) {
	LOG_SCOPE
	// Open File descriptor send
	fd_send = open(dev_send.c_str(), O_RDWR);
	if(this->fd_send == -1) {
		LOG_ERROR << __FUNCTION__ << ": error while opening file descriptor occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	// Open File descriptor read
	fd_read = open(dev_read.c_str(), O_RDWR);
	if(this->fd_read == -1) {
		LOG_ERROR << __FUNCTION__ << ": error while opening file descriptor occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}

	// initialize settings for Serial Interface send
	if(init(baud) < 0) {
		LOG_ERROR << __FUNCTION__ << ": error while initializing serial interface occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
}

Interface::~Interface() {
	LOG_SCOPE
	// Close File descriptor read
	if( close(this->fd_send) < 0 ) {
		LOG_ERROR << __FUNCTION__ << ": error while closing file descriptor WRITE occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	// Close File descriptor read
	if( close(this->fd_read) < 0 ) {
		LOG_ERROR << __FUNCTION__ << ": error while closing file descriptor READ occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
}

int Interface::init(int baud) {
	LOG_SCOPE
	int ret = 0;
    flush();
	ret += tcgetattr(this->fd_send, &termino_send); 			// get the current terminal control settings
	ret += cfsetispeed(&termino_send, baud);					// set input baud rate
	ret += cfsetospeed(&termino_send, baud);					// set output baud rate
	termino_send.c_cflag &= ~CSIZE;								// number of data bits per character
	termino_send.c_cflag &= ~CSTOPB;							// disable two stop bits and use just one
	termino_send.c_cflag &= ~PARENB;							// disable parity
	termino_send.c_cflag |= CS8;								// set 8 data bits
	termino_send.c_cflag |= CREAD;								// enable receiver
	termino_send.c_cflag |= CLOCAL;								// ignore modem status lines
	ret += tcsetattr(this->fd_send, TCSANOW, &termino_send); 	// set terminal control settings immediately

	ret += tcgetattr(this->fd_read, &termino_read); 			// get the current terminal control settings
	ret += cfsetispeed(&termino_read, baud);					// set input baud rate
	ret += cfsetospeed(&termino_read, baud);					// set output baud rate
	termino_read.c_cflag &= ~CSIZE;								// number of data bits per character
	termino_read.c_cflag &= ~CSTOPB;							// disable two stop bits and use just one
	termino_read.c_cflag &= ~PARENB;							// disable parity
	termino_read.c_cflag |= CS8;								// set 8 data bits
	termino_read.c_cflag |= CREAD;								// enable receiver
	termino_read.c_cflag |= CLOCAL;								// ignore modem status lines
	ret += tcsetattr(this->fd_read, TCSANOW, &termino_read); 	// set terminal control settings immediately

	return ret;
}

int Interface::send(Message& message) {
	LOG_SCOPE
	int ret = 0;

	ret = write(this->fd_send, &message, sizeof(Message));

	if(ret == -1) {
		LOG_ERROR << __FUNCTION__ << ": error while writing Message on file descriptor occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
	}
	return ret;
}

int Interface::recv( Message *msg ) {
	LOG_SCOPE
	int ret = readcond(this->fd_read, msg, sizeof(Message), sizeof(Message),0,TIMEOUT_2_SECONDS);
	if(ret == -1) {
		LOG_ERROR << __FUNCTION__ << ": error while reading Message from file descriptor occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
	}
	return ret;
}

int Interface::flush() {
	LOG_SCOPE
	int ret = 0;

	ret = tcflush(this->fd_read, TCIOFLUSH);

	if(ret == -1) {
		LOG_ERROR << __FUNCTION__ << ": error while flushing file descriptor occurred."<<endl;
		LOG_ERROR<<errno<<" "<<strerror(errno)<<endl;
	}

	return ret;
}


} /* namespace serial */
} /* namespace io */
} /* namespace hal */
