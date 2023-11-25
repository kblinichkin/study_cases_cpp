/*
 * I2CPort.h
 *
 *  Created on: 5 Oct 2023
 *      Author: a1
 */

#ifndef WRAPPERS_I2CPORT_H_
#define WRAPPERS_I2CPORT_H_

#include "freertos/FreeRTOS.h"

#include <vector>

namespace comdemo {

class I2CPort {
public:
	// TODO: add suport of master mode here as well
	I2CPort();
	~I2CPort();

	// FIXME: I2CPort::buffer slaveReadBuffer(size_t bytesToRead, uint32_t msToWait);
	int slaveReadBuffer(std::vector<uint8_t>& data, uint32_t msToWait);
	// FIXME: int slaveReadBuffer(I2CPort::buffer buffer, uint32_t msToWait);
	int slaveWriteBuffer(const std::vector<uint8_t>& data, uint32_t msToWait);
};

} /* namespace comdemo */

#endif /* WRAPPERS_I2CPORT_H_ */
