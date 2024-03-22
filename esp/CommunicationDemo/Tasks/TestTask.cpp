/*
 * TestTask.cpp
 *
 *  Created on: 5 Oct 2023
 *      Author: a1
 */

#include "TestTask.h"
#include "task_helper.h"
#include "esp_log.h"
//#include <iostream>

#include "esp32/spiram.h"

using namespace comdemo;

static const char *TAG = "TestTask";

DEFINE_CALLBACK(TestTask, run)
TestTask::TestTask(Queue<uint8_t>& queue, I2CPort& i2cPort) :
	m_queue(queue),
	m_i2cPort(i2cPort)
{
//	auto d = esp_spiram_test();
//	size_t psram_size = esp_spiram_get_size();
//	printf("PSRAM size: %d bytes\n", psram_size);

	INSTALL_CALLBACK("Test task", 2048, this, 10, &m_handle);
}

TestTask::~TestTask() {
	// FIXME: it's better to use an atomic flag in this case, because deleting a task
	//        from another thread is a bad idea. Maybe even wait (with timeout) for it to be deleted.
//	if (m_handle)
//		vTaskDelete(m_handle);

}

void TestTask::run() {
	for (;;) {
		std::vector<uint8_t> data(10, 0);
		auto bytesRead = m_i2cPort.slaveReadBuffer(data, 10);

		if (bytesRead > 0) {
			ESP_LOGI(TAG, "bytesRead: %d", bytesRead);
			for (size_t i = 0; i < data.size(); ++i) {
				ESP_LOGI(TAG, "data[%d]: %d", i, data[i]);
			}
		}

		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
