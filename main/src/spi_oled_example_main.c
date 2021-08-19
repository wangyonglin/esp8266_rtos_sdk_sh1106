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
#define compass_x 63
#define compass_y 64

void app_main(void)
{

	OLED_Init();

	//Boot_Animation();
	//	Boot_Animation();
	//vTaskDelay(2000/portTICK_RATE_MS);

	for (int i = 0; i < 180; i++)
		OLED_1in3_Angle(compass_x, compass_y, i, 32, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 0, 32, 3, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 30, 32, 3, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 60, 32, 3, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 90, 32, 3, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 120, 32, 3, 1);
	OLED_1in3_AngleLine(compass_x, compass_y, 150, 32, 3, 1);
	OLED_1in3_Format(26, 56, S, 1, "%d", 0);
	OLED_1in3_Format(58, 24, S, 1, "%d", 90);
	OLED_1in3_Format(96, 56, S, 1, "%d", 180);
	int c = 90;
	OLED_1in3_AngleLine(compass_x, compass_y, c, 26, 26, 1);
	OLED_1in3_Format(0, 0, XL, 1, "[%d]", c);	
	OLED_Refresh();
	vTaskDelay(5000 / portTICK_RATE_MS);
}
