#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "TestTask.h"
#include "Queue.h"
#include "I2CPort.h"

#include "esp_log.h"

static const char *TAG = "Main";

using namespace comdemo;


extern "C" void app_main(void)
{
	I2CPort i2cPort;
	Queue<uint8_t> q(10);
	TestTask task(q, i2cPort);
}

