/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#include "sys_io.h"
#include "sys_log.h"
#include "esp_timer.h"
#include "driver/usb_serial_jtag.h"
#include "nvs_flash.h"

static uint8_t s_led_state = 0;

void esp32c3_weact_init(void) {
    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      SYS_ASSERT(nvs_flash_erase() == ESP_OK);
      ret = nvs_flash_init();
    }
    SYS_ASSERT(ret == ESP_OK);    
	
	/*Config led life*/
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output*/
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	/* Configure USB SERIAL JTAG */
	usb_serial_jtag_driver_config_t usb_serial_jtag_config = {
		.tx_buffer_size = 254,
		.rx_buffer_size = 254,
	};
	SYS_ASSERT(usb_serial_jtag_driver_install(&usb_serial_jtag_config) != ESP_FAIL);
}


void toggle_led_life(void) {
    gpio_set_level(BLINK_GPIO, s_led_state);
    s_led_state = !s_led_state;
}

uint32_t millis() {
	return (uint32_t)(esp_timer_get_time()/1000);
}

void delay_us(uint32_t us) {
	int64_t next = esp_timer_get_time() + us;
	while (esp_timer_get_time() < next) {
		;
	}
}

void delay_ms(uint32_t ms) {
	for (uint32_t c = 0; c < ms; c++) {
		delay_us(1000);
	}
}