/*
 * Header.h
 *
 *  Created on: 18.10.2017
 *      Author: abj240
 */
#include "Signals.h"
#include "Logger.h"
#include "Parameter.h"

#include <cstdint>


#ifndef HEADER_H_
#define HEADER_H_


#define CHECK_NUMBER 949494;

#define WAIT(x) (std::this_thread::sleep_for(std::chrono::milliseconds(x)))
#define NOW std::chrono::steady_clock::now()

typedef uint8_t port_t;

#endif /* HEADER_H_ */
