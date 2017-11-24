/*
 * GPIO.h
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "Header.h"

#ifndef GPIO_H_
#define GPIO_H_

#include <mutex>

enum class PORT {
    A,
    B,
    C
};

namespace hardwareLayer {
namespace io {


class GPIO {
public:
	/**
	* @brief creates singleton instance on first call, otherwise returns instance
	* @return singleton instance
	*/
	static GPIO& instance();
	virtual ~GPIO();

	void gainAccess();
	/**
	* @brief reads an 8-bit value from the specified port
	*/
	port_t read(PORT port);
	/**
	* @brief reads an 8-bit value from the specified address
	*/
	uint8_t read(uint8_t address);
	/**
	* @brief sets bits from bitmask on the specified port
	*/
	void setBits(PORT port, port_t bitmask);
	/**
	* @brief sets bits from bitmask on the specified address
	*/
	void setBits(uint8_t address, uint8_t bitmask);
	/**
	* @brief clears bits from bitmask on the specified port
	*/
	void clearBits(PORT port, port_t bitmask);
	/**
	* @brief clears bits from bitmask on the specified address
	*/
	void clearBits(uint8_t address, port_t bitmask);

protected:
	GPIO();
	GPIO(GPIO const&);
	GPIO& operator=(GPIO const&);
	/**
	* @brief sets 8-bit value val on the specified address
	*/
	virtual void write(  uint8_t address, uint8_t val);


	std::mutex gpio_mutex;
};

} /* namespace gpio */
} /* namespace hardwareLayer */
#endif /* GPIO_H_ */
