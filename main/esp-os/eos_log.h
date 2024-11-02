/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __EOS_LOG_H__
#define __EOS_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sys_log.h"

#ifdef EOS_LOG_ENABLE
#define EOS_LOG_ERROR	SYS_LOGE
#define EOS_LOG_WARNING SYS_LOGW
#define EOS_LOG_INFO	SYS_LOGI
#define EOS_LOG_DEBUG	SYS_LOGD
#else
#define EOS_LOG_ERROR(...)	 (void)0
#define EOS_LOG_WARNING(...) (void)0
#define EOS_LOG_INFO(...)	 (void)0
#define EOS_LOG_DEBUG(...)	 (void)0
#endif

#ifdef __cplusplus
}
#endif

#endif