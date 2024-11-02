/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "app.h"
#include "sys_io.h"
#include "task_list.h"

#define TAG "APP"

extern "C" void app_main(void) {
	/*********************
	 * hardware configure *
	 **********************/
	esp32c3_weact_init();

	/*********************
	 * software configure *
	 **********************/
	eos_task_init();

	/* Shell: Debug and Config */
	eos_create_task(TASK_SHELL_ID);

	/* Device: IO Pins */
	eos_create_queue(TASK_DEVICE_ID);
	eos_create_task(TASK_DEVICE_ID);

}