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
#include <OLED_1in3_c.h>
void app_main(void)
{

	OLED_Init();

	//Boot_Animation();
	//	Boot_Animation();
	//vTaskDelay(2000/portTICK_RATE_MS);

	for (int i = 0; i < 180; i++)
		OLED_1in3_Angle(64, 64, i, 48, 1);
	OLED_1in3_AngleLine(64, 64, 0, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 30, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 60, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 90, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 120, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 150, 48, 3, 1);
	OLED_1in3_AngleLine(64, 64, 180, 48, 3, 1);
	OLED_Refresh();
	for (int x = 0; x < 180; x++)
	{
		OLED_1in3_AngleLine(64, 64, x-1, 42, 42, 0);
		OLED_1in3_AngleLine(64, 64, x, 42, 42, 1);	
		OLED_Refresh();
	}
	vTaskDelay(5000/portTICK_RATE_MS);
}
