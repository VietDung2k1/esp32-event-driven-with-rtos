/**
 ****************************************************************************** 
 * @author: VietDung 
 * @date:   31/10/2024 
 ****************************************************************************** 
 **/
#ifndef __SYS_LOG_H__ 
#define __SYS_LOG_H__ 

#include "esp_system.h" 
#include "freertos/FreeRTOS.h"
#include <stdio.h> 
#include <stdint.h> 

#define UNUSED(x) (void)(x) 
#define EOS_LOG_ENABLE 

#define ESCAPE          "\033" 
#define RED_COLOR       "[0;31m" 
#define YELLOW_COLOR    "[0;33m" 
#define GREEN_COLOR     "[0;32m" 
#define CYAN_COLOR      "[0;36m" 
#define RESET_COLOR     "[0m" 

#define SYS_PRINTF printf 

static inline void get_current_time(char *buffer, size_t size) {
    TickType_t tickCount = xTaskGetTickCount();
    uint32_t milliseconds = tickCount * portTICK_PERIOD_MS;
    uint32_t seconds = milliseconds / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;

    snprintf(buffer, size, "[%02lu:%02lu:%02lu]", (unsigned long)(hours % 24), (unsigned long)(minutes % 60), (unsigned long)(seconds % 60));
}

#ifdef EOS_LOG_ENABLE 
#ifdef RELEASE 
#define SYS_LOGE(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE RED_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGW(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE YELLOW_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGI(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE GREEN_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGD(tag, format, ...) 	(void)0
#define SYS_LOGV(tag, format, ...) 	(void)0
#else 
#define DEBUG 
#define SYS_LOGE(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE RED_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGW(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE YELLOW_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGI(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE GREEN_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGD(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE CYAN_COLOR "%s [" tag "] " format "\n" ESCAPE RESET_COLOR, time_buf, ##__VA_ARGS__); } 
#define SYS_LOGV(tag, format, ...) 	{ char time_buf[20]; get_current_time(time_buf, sizeof(time_buf)); SYS_PRINTF(ESCAPE RESET_COLOR "%s [" tag "] " format "\n", time_buf, ##__VA_ARGS__); } 
#endif 
#endif 

#define SYS_ASSERT(cond)                                     \
    do                                                       \
    {                                                        \
        if (!(cond))                                         \
        {                                                    \
            SYS_LOGE(ESCAPE RED_COLOR "[ASSERT]", "%s:%d\n" ESCAPE RESET_COLOR, __FILE__, __LINE__); \
            esp_restart();			                         \
        }                                                    \
    } while (0); 

#endif
