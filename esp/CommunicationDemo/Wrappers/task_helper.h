/*
 * task.h
 *
 *  Created on: 3 Oct 2023
 *      Author: Kirill Blinichkin
 */

#ifndef WRAPPERS_TASK_HELPER_H_
#define WRAPPERS_TASK_HELPER_H_

//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

/*#define DECLARE_CALLBACK() \
	friend void Run(void* data);*/

#define DEFINE_CALLBACK(ClassName, RunFuncName) \
	extern "C" void Run(void* data) { \
		reinterpret_cast<ClassName*>(data)->RunFuncName(); \
		vTaskDelete(NULL); \
	}

#define INSTALL_CALLBACK(TaskNameStr, StackDepth, ThisPtr, Priority, HandleRef) \
	do { \
		ESP_ERROR_CHECK_WITHOUT_ABORT( \
			!(pdPASS == xTaskCreate(&Run, TaskNameStr, \
					static_cast<configSTACK_DEPTH_TYPE>(StackDepth), \
					ThisPtr, static_cast<UBaseType_t>(Priority), HandleRef)) \
		); \
	} while(0);

#endif /* WRAPPERS_TASK_HELPER_H_ */
