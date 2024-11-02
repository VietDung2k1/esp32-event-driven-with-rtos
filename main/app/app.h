/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __APP_H__
#define __APP_H__

#include "eos.h"
#include "eos_log.h"

/*****************************************************************************/
/*  Global define variable
 */
/*****************************************************************************/
#define APP_FW_MODEL   "ESP32-C3"
#define APP_FW_VERSION "1.0.0"
#ifdef DEBUG
#define APP_FW_TITLE APP_FW_MODEL "-DEBUG"
#else
#define APP_FW_TITLE APP_FW_MODEL "-PROD"
#endif

/*****************************************************************************/
/*  LIFE task define
 */
/*****************************************************************************/
/* define timer */
#define AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL		(1000)

/* define signal */
enum {
	AC_LIFE_SYSTEM_CHECK = EOS_USER_DEFINE_SIG,
};

/*****************************************************************************/
/*  DEMO task define
 */
/*****************************************************************************/
/* define timer */
#define AC_DEMO_TASK_TIMER_INTERVAL					(10000)
/* define signal */
enum {
	AC_DEMO_TASK_POST = EOS_USER_DEFINE_SIG,
	AC_DEMO_TASK_POST_AND_DATA,
};

#endif