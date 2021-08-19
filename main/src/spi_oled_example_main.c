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
#include <bmp.h>
#include <oled_sh1106.h>
#define compass_x 96
#define compass_y 32
oled_sh1106_config_t oled_cfg = {
	.cpha = OLED_SH1106_SPI_CPHA,
	.cpol = OLED_SH1106_SPI_CPOL,
	.spi.clk = OLED_SH1106_SPI_CLK,
	.spi.cs = OLED_SH1106_SPI_CS,
	.spi.mosi = OLED_SH1106_SPI_MOSI,
	.dc = OLED_SH1106_DC,
	.rst = OLED_SH1106_RST,
};
void app_main(void)
{
	oled_sh1106_system_init(&oled_cfg);
	oled_sh1106_driver_init();
	for (int i = 0; i < 360; i++)
		oled_sh1106_angle(compass_x, compass_y, i, 32, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 0, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 45, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 90, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 135, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 180, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 225, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 270, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 315, 32, 4, 1);
	oled_sh1106_angle_line(compass_x, compass_y, 360, 32, 4, 1);
	oled_sh1106_refresh();

	for (int n = 0; n < 360; n++)
	{
		oled_sh1106_angle_line(compass_x, compass_y, n-1, 28, 26, 0);
		oled_sh1106_angle_line(compass_x, compass_y, n, 28, 26, 1);
		oled_sh1106_refresh();
	}

	vTaskDelay(5000 / portTICK_RATE_MS);
}
