#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp8266/gpio_struct.h"
#include "esp8266/spi_struct.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_libc.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#include <oled.h>
#include <bmp.h>

void app_main(void)
{

	OLED_Init();

	for (;;)
	{
		OLED_String(0,0,"wangyonglin-300",S);
		OLED_Integer(0,32,-123,L);
		OLED_Refresh();
		vTaskDelay(5000 / portTICK_RATE_MS);
	}
}
