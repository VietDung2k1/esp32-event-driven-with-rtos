/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __EOS_H__
#define __EOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

#define EOS_TASK_QUEUE_LENGTH 10
#define EOS_USER_DEFINE_SIG	  10

typedef enum {	  // The size of the task stack specified as the NUMBER OF BYTES
	EOS_STACK_DEPTH_LEVEL_1 = 2048,
	EOS_STACK_DEPTH_LEVEL_2 = 4096,
	EOS_STACK_DEPTH_LEVEL_3 = 8192,
	EOS_STACK_DEPTH_LEVEL_4 = 16384,
	EOS_STACK_DEPTH_LEVEL_5 = 35980,
} eos_stack_depth_t;

typedef enum {	  // Component config -> PThreads -> Default task priority (5)
	EOS_PRIORITY_LEVEL_1 = 1,
	EOS_PRIORITY_LEVEL_2,
	EOS_PRIORITY_LEVEL_3,
	EOS_PRIORITY_LEVEL_MAX,	   // Default task priority - 1
} eos_priority_t;

typedef struct EOS_TASK {
	uint16_t id;
	void (*code)(void *);	 // Tasks must be implemented to never return
	eos_stack_depth_t stack_size;
	eos_priority_t priority;
	const char *const name;
	void *queue_handle;
} eos_task_t;

typedef struct EOS_MSG {
	uint16_t id;
	uint32_t len;
	void *data;
} eos_msg_t;

typedef struct EOS_TIMER_MSG {
	uint16_t task_id;
	uint16_t msg_id;
	uint32_t msg_len;
	void *msg_data;
	bool timer_auto_reload;
} eos_timer_msg_t;

/* function using to init task */
extern void eos_task_init(void);
extern void eos_create_task(uint16_t task_id);
extern void eos_create_queue(uint16_t task_id);

/* function using to make sure that all task is initialed */
extern void eos_wait_all_task_started(void);

/* function exchange messages */
extern bool eos_task_post_msg(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len);
extern bool eos_task_receive_msg(uint16_t task_id, uint16_t *msg_id, void **msg_data, uint32_t *msg_len);
extern void eos_free_msg(void *msg_data);

/* function timer */
extern TimerHandle_t eos_timer_set(uint16_t task_id, uint16_t msg_id, void *msg_data, uint32_t msg_len, uint32_t timer_period,
								   bool timer_auto_reload);
extern void eos_timer_remove(TimerHandle_t timer_handle);

#ifdef __cplusplus
}
#endif

#endif