/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "app.h"
#include "cmd_line.h"
#include "task_shell.h"
#include "eos_log.h"
#include "sys_io.h"
#include "esp_system.h"
#include "task_list.h"

#define TAG "SHELL"

/*****************************************************************************/
/*  command function declare
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t *argv);
int32_t shell_ver(uint8_t *argv);
int32_t shell_help(uint8_t *argv);

/*****************************************************************************/
/*  command table
 */
/*****************************************************************************/
cmd_line_t lgn_cmd_table[] = {

	/*************************************************************************/
	/* system command */
	/*************************************************************************/
	{(const int8_t *)"reset", 	shell_reset,	(const int8_t *)"reset terminal"},
	{(const int8_t *)"ver",		shell_ver,	   	(const int8_t *)"version info"	},
	{(const int8_t *)"help",	shell_help,	 	(const int8_t *)"help info"		},

 /* End Of Table */
	{(const int8_t *)0,			(pf_cmd_func)0, (const int8_t *)0				}
};

/*****************************************************************************/
/*  command function definaion
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t *argv) {
	(void)argv;
	EOS_LOG_INFO(TAG, "\033[2J\r");
	esp_restart();
	return 0;
}

int32_t shell_ver(uint8_t *argv) {
	(void)argv;
	EOS_LOG_INFO(TAG, "-Firmware model:   %s\n", APP_FW_MODEL);
	EOS_LOG_INFO(TAG, "-Firmware title    %s\n", APP_FW_TITLE);
	EOS_LOG_INFO(TAG, "-Firmware version: %s\n", APP_FW_VERSION);
#ifdef DEBUG
	EOS_LOG_INFO(TAG, "-Build type:       DEBUG\n");
#else
	EOS_LOG_INFO(TAG, "-Build type:       PROD\n");
#endif
	EOS_LOG_INFO(TAG, "-Uptime:           %lu\n", millis());
	EOS_LOG_INFO(TAG, "-Heap free:        %u\n", xPortGetFreeHeapSize());
	return 0;
}

int32_t shell_help(uint8_t *argv) {
	uint32_t idx = 0;
	switch (*(argv + 5)) {
	default:
		EOS_LOG_INFO(TAG, "\nCOMMANDS INFORMATION:\n\n");
		while (lgn_cmd_table[idx].cmd != (const int8_t *)0) {
			EOS_LOG_INFO(TAG, "%s\n\t%s\n\n", lgn_cmd_table[idx].cmd, lgn_cmd_table[idx].info);
			idx++;
		}
		break;
	}
	return 0;
}