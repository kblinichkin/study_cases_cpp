/*
 * Queue
 *
 *  Created on: 5 Oct 2023
 *      Author: a1
 */

#ifndef WRAPPERS_QUEUE_H_
#define WRAPPERS_QUEUE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace comdemo {

template <typename T>
class Queue {
	QueueHandle_t m_handle;
public:
	Queue(UBaseType_t len) :
		m_handle(xQueueCreate(len, sizeof(T)))
	{}

	~Queue() {
		if (m_handle)
			vQueueDelete(m_handle);
	}

	bool send(const T& val, uint32_t msToWait) {
		BaseType_t retval = xQueueSend(m_handle, &val, pdMS_TO_TICKS(msToWait));
		ESP_ERROR_CHECK_WITHOUT_ABORT(retval);

		return retval == pdTRUE;
	}

	bool receive(T& val, uint32_t msToWait) {
		BaseType_t retval = xQueueReceive(m_handle, &val, pdMS_TO_TICKS(msToWait));
		ESP_ERROR_CHECK_WITHOUT_ABORT(retval);

		return retval == pdTRUE;
	}
};
}


#endif /* WRAPPERS_QUEUE_H_ */
