/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __SYS_IO_H__
#define __SYS_IO_H__

#include "driver/gpio.h"

#define BLINK_GPIO (gpio_num_t)(GPIO_NUM_8)

#ifdef __cplusplus
extern "C" {
#endif

extern void esp32c3_weact_init();
extern void toggle_led_life();
extern uint32_t millis();
extern void delay_us(uint32_t us);
extern void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif