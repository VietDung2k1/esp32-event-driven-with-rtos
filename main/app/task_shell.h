/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __TASK_SHELL_H__
#define __TASK_SHELL_H__

#include <stdint.h>
#include "cmd_line.h"

#define SHELL_BUFFER_SIZE (256)

typedef struct SHELL_BUFFER {
	uint8_t index;
	uint8_t data[SHELL_BUFFER_SIZE];
} shell_buffer_t;

extern cmd_line_t lgn_cmd_table[];

extern void task_shell(void *pvParameters);

#endif