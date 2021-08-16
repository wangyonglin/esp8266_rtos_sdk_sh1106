#include <OLED_1in3_c.h>
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
void OLED_Init(void)
{
    OLED_1li3_SystemInit();
    OLED_1in3_DriverInit();
    vTaskDelay(500 / portTICK_RATE_MS);
    OLED_1in3_ClearAll();
}
void OLED_String(uint8_t x, uint8_t y, const char *chr, fontsize_t size)
{
    OLED_1in3_String(x, y, (uint8_t *)chr, size, 1);
}
void OLED_Integer(uint8_t x, uint8_t y, int32_t num, fontsize_t size)
{
    char chr[128] = {0};
    sprintf(chr, "%d", num);
    OLED_1in3_String(x, y, (uint8_t *)chr, size, 1);
}
void OLED_Refresh()
{
    OLED_1li3_Refresh();
}