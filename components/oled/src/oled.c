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
#include <math.h>
#include <OLED_1in3_c.h>
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

/* 开机动画 */
void Boot_Animation(void)
{
    static uint8_t x = 0, y = 0;
    for (x = 63; x >= 18; x--)
    {
        OLED_1li3_DrawPoint(108 - 0.7 * x, x, 1); //画斜线 斜率≈√3/3
        OLED_1li3_DrawPoint(17 + 0.7 * x, x, 1);
        y = 64 - x;
        OLED_1li3_DrawPoint(64 - 0.7 * y, y, 1);
        OLED_1li3_DrawPoint(64 + 0.7 * y, y, 1);
        vTaskDelay(100 / portTICK_RATE_MS);
        OLED_1li3_Refresh(); //更新显示到OLED
    }

    for (x = 30; x <= 94; x++)
    {
        OLED_1li3_DrawPoint(125 - x, 47, 1);
        OLED_1li3_DrawPoint(x, 18, 1);
        vTaskDelay(100 / portTICK_RATE_MS);
        OLED_1li3_Refresh(); //更新显示到OLED
    }

    OLED_1in3_String(60, 20, (uint8_t *)"E", XL, 1);
    OLED_1li3_Refresh(); //更新显示到OLED
    vTaskDelay(100 / portTICK_RATE_MS);
}
