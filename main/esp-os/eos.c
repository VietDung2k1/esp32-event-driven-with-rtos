/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "eos.h"
#include "eos_log.h"
#include "task_list.h"
#include "sys_io.h"

#define TAG "EOS"

static SemaphoreHandle_t sem_task_started = NULL;
static uint16_t table_task_size				  = EOS_TASK_END_ID;

void eos_task_init(void) {
	printf("\r\n"
		   "    _/_/_/_/  _/_/_/_/  _/_/_/      _/_/      _/_/_/   \r\n"
		   "   _/        _/        _/    _/  _/    _/  _/          \r\n"
		   "  _/_/_/_/  _/_/_/_/  _/_/_/    _/    _/    _/_/       \r\n"
		   " _/              _/  _/        _/    _/        _/      \r\n"
		   "_/_/_/_/  _/_/_/_/  _/          _/_/    _/_/_/         \r\n"
		   "Author: VietDung\r\n"
		   "Build: %s\r\n",
		   __DATE__);

	EOS_LOG_INFO(TAG, "table task size: %u", table_task_size);
	sem_task_started = xSemaphoreCreateCounting(table_task_size, table_task_size);
}

void eos_create_task(uint16_t task_id) {
	SYS_ASSERT(task_id < table_task_size);

	SYS_ASSERT(pdFAIL != xTaskCreate(table_task[task_id].code, table_task[task_id].name, table_task[task_id].stack_size,
									 &table_task[task_id].id, table_task[task_id].priority, NULL));

	EOS_LOG_INFO(TAG, "create task name: %s, id: %u, stack size: %u, priority: %u, code: %p", table_task[task_id].name,
				 table_task[task_id].id, table_task[task_id].stack_size, table_task[task_id].priority, table_task[task_id].code);
}

void eos_create_queue(uint16_t task_id) {
	SYS_ASSERT(task_id < table_task_size);

	table_task[task_id].queue_handle = xQueueCreate(EOS_TASK_QUEUE_LENGTH, sizeof(eos_msg_t));
	SYS_ASSERT(table_task[task_id].queue_handle != NULL);
	EOS_LOG_INFO(TAG, "create queue task name: %s, queue: %p", table_task[task_id].name, table_task[task_id].queue_handle);
}

void eos_wait_all_task_started(void) {
	EOS_LOG_DEBUG(TAG, "wait ...");
	SYS_ASSERT(pdFAIL != xSemaphoreTake(sem_task_started, portMAX_DELAY));
	/* Wait until all tasks have started */
	while (uxSemaphoreGetCount(sem_task_started) > 0) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

bool eos_task_post_msg(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len) {
	EOS_LOG_DEBUG(TAG, "task post id: %u, sig: %u, msg: %p, len: %lu", task_id, msg_id, msg_data, msg_len);
	SYS_ASSERT(task_id < table_task_size);

	/* Create message*/
	eos_msg_t eos_msg = {msg_id, msg_len, NULL};
	if (msg_len) {
		eos_msg.data = pvPortMalloc(msg_len);
		SYS_ASSERT(eos_msg.data != NULL);

		memcpy(eos_msg.data, msg_data, msg_len);
		EOS_LOG_DEBUG(TAG, "task post copy msg %u -> %p, len: %lu", eos_msg.id, eos_msg.data, eos_msg.len);
	}

	/* Send message*/
	SYS_ASSERT(pdFAIL != xQueueSend((QueueHandle_t)table_task[task_id].queue_handle, &eos_msg, portMAX_DELAY));
	return true;
}

bool eos_task_post_msg_form_isr(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len) {
	EOS_LOG_DEBUG(TAG, "task post from isr id: %u, sig: %u, msg: %p, len: %lu", task_id, msg_id, msg_data, msg_len);
	SYS_ASSERT(task_id < table_task_size);

	/* Create message*/
	eos_msg_t eos_msg = {msg_id, msg_len, NULL};
	if (msg_len) {
		eos_msg.data = pvPortMalloc(msg_len);
		SYS_ASSERT(eos_msg.data != NULL);

		memcpy(eos_msg.data, msg_data, msg_len);
		EOS_LOG_DEBUG(TAG, "task post copy msg %u -> %p, len: %lu", eos_msg.id, eos_msg.data, eos_msg.len);
	}

	BaseType_t xHigherPriorityTaskWoken;
	/* We have not woken a task at the start of the ISR. */

	xHigherPriorityTaskWoken = pdFALSE;

	/* Send message from ISR */
	SYS_ASSERT(pdFAIL != xQueueSendFromISR((QueueHandle_t)table_task[task_id].queue_handle, &eos_msg, &xHigherPriorityTaskWoken));

	/* Now the buffer is empty we can switch context if necessary. */
	if (xHigherPriorityTaskWoken) {
		/* Actual macro used here is port specific. */
		portYIELD_FROM_ISR();
	}
	return true;
}

bool eos_task_receive_msg(uint16_t task_id, uint16_t *msg_id, void **msg_data, uint32_t *msg_len) {
	SYS_ASSERT(task_id < table_task_size);

	/* Get message form queue*/
	eos_msg_t eos_msg = {0, 0, NULL};

	SYS_ASSERT(pdFAIL != xQueueReceive((QueueHandle_t)table_task[task_id].queue_handle, &eos_msg, portMAX_DELAY));
	/* Copy message details */
	*msg_id	  = eos_msg.id;
	*msg_data = eos_msg.data;
	*msg_len  = eos_msg.len;

	EOS_LOG_DEBUG(TAG, "task receive copy msg %u -> %p, len: %lu", *msg_id, *msg_data, *msg_len);
	return true;
}

void eos_free_msg(void *msg_data) {
	if (msg_data != NULL) {
		EOS_LOG_DEBUG(TAG, "delete msg %p", msg_data);
		vPortFree(msg_data);
	}
}

static void eos_timer_callback_func(TimerHandle_t timer_handle) {
	SYS_ASSERT(timer_handle != NULL);

	/* Get timer message */
	eos_timer_msg_t *eos_timer_msg = (eos_timer_msg_t *)pvTimerGetTimerID(timer_handle);
	SYS_ASSERT(eos_timer_msg != NULL);

	EOS_LOG_DEBUG(TAG, "timer callback id: %u, sig: %u, msg: %p, len: %lu", eos_timer_msg->task_id, eos_timer_msg->msg_id,
				  eos_timer_msg->msg_data, eos_timer_msg->msg_len);

	/* Call task post */
	eos_task_post_msg(eos_timer_msg->task_id, eos_timer_msg->msg_id, eos_timer_msg->msg_data, eos_timer_msg->msg_len);

	/* Stop and delete timer if not auto-reloading */
	if (!eos_timer_msg->timer_auto_reload) {
		xTimerStop(timer_handle, 0);
		xTimerDelete(timer_handle, 0);
		vPortFree(eos_timer_msg->msg_data);
		vPortFree(eos_timer_msg);
	}
}

TimerHandle_t eos_timer_set(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len, uint32_t timer_period,
							bool timer_auto_reload) {
	EOS_LOG_DEBUG(TAG, "timer set id: %u, sig: %u, msg: %p, len: %lu", task_id, msg_id, msg_data, msg_len);
	SYS_ASSERT(task_id < table_task_size);

	/* Create timer message */
	eos_timer_msg_t *eos_timer_msg = (eos_timer_msg_t *)pvPortMalloc(sizeof(eos_timer_msg_t));

	SYS_ASSERT(eos_timer_msg != NULL);

	eos_timer_msg->task_id			 = task_id;
	eos_timer_msg->msg_id			 = msg_id;
	eos_timer_msg->msg_len			 = msg_len;
	eos_timer_msg->msg_data			 = NULL;
	eos_timer_msg->timer_auto_reload = timer_auto_reload;

	if (msg_len > 0) {
		eos_timer_msg->msg_data = pvPortMalloc(msg_len);

		SYS_ASSERT(eos_timer_msg->msg_data != NULL);

		memcpy(eos_timer_msg->msg_data, msg_data, msg_len);
		EOS_LOG_DEBUG(TAG, "timer set copy msg %u -> %p, len: %lu", eos_timer_msg->msg_id, eos_timer_msg->msg_data,
					  eos_timer_msg->msg_len);
	}

	/* Create timer */
	TimerHandle_t eos_timer_handle =
		xTimerCreate("EOS_TIMER", pdMS_TO_TICKS(timer_period), eos_timer_msg->timer_auto_reload ? pdTRUE : pdFALSE,
					 (void *)eos_timer_msg, eos_timer_callback_func);

	/* Start timer */
	if (eos_timer_handle != NULL) {
		SYS_ASSERT(xTimerStart(eos_timer_handle, pdMS_TO_TICKS(100)) != pdFAIL);
		return eos_timer_handle;
	}
	return NULL;
}

void eos_timer_remove(TimerHandle_t timer_handle) {
	SYS_ASSERT(timer_handle != NULL);

	/* Get timer message */
	eos_timer_msg_t *eos_timer_msg = (eos_timer_msg_t *)pvTimerGetTimerID(timer_handle);
	SYS_ASSERT(eos_timer_msg != NULL);

	/* Delete data*/
	vPortFree(eos_timer_msg->msg_data);
	vPortFree(eos_timer_msg);

	/* Delete timer */
	xTimerStop(timer_handle, 0);
	xTimerDelete(timer_handle, 0);
}
