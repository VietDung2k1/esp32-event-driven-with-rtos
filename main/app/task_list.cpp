/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "task_list.h"
#include "task_shell.h"
#include "task_device.h"

eos_task_t table_task[] = {
	{TASK_SHELL_ID, 	task_shell, 	EOS_STACK_DEPTH_LEVEL_2, 	EOS_PRIORITY_LEVEL_1, 	"_shell", 	NULL},
	{TASK_DEVICE_ID, 	task_device, 	EOS_STACK_DEPTH_LEVEL_2, 	EOS_PRIORITY_LEVEL_1, 	"_device", 	NULL},
};