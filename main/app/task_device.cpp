/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "app.h"
#include "sys_io.h"

#include "eos.h"
#include "eos_log.h"
#include "task_list.h"
#include "task_shell.h"
#include "task_device.h"
#include <stdlib.h>

#define TAG "DEVICE"

extern void task_device(void *pvParameters) {
	eos_wait_all_task_started();
	EOS_LOG_INFO(TAG, "STARTED");

	uint16_t msg_id	 = 0;
	void *msg_data	 = NULL;
	uint32_t msg_len = 0;
	
	eos_timer_set(TASK_DEVICE_ID, AC_LIFE_SYSTEM_CHECK, NULL, 0, AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, true);

	while (eos_task_receive_msg(TASK_DEVICE_ID, &msg_id, &msg_data, &msg_len)) {
		switch (msg_id) {
		case AC_LIFE_SYSTEM_CHECK: {
			EOS_LOG_INFO(TAG, "AC_LIFE_SYSTEM_CHECK");
			toggle_led_life();
		} break;

		default:
			break;
		}
		eos_free_msg(msg_data);
	}
}