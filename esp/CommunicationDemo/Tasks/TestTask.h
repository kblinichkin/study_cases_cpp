/*
 * TestTask.h
 *
 *  Created on: 5 Oct 2023
 *      Author: a1
 */

#ifndef TASKS_TESTTASK_H_
#define TASKS_TESTTASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Queue.h"
#include "I2CPort.h"

namespace comdemo {

class TestTask {
public:
	TestTask(Queue<uint8_t>& queue, I2CPort& i2cPort);

	~TestTask();

	void run();

private:
	TaskHandle_t m_handle;

	Queue<uint8_t>& m_queue;
	I2CPort& m_i2cPort;
};

} /* namespace comdemo */

#endif /* TASKS_TESTTASK_H_ */
