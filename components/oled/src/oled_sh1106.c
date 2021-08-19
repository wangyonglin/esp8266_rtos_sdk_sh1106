#include <oled_sh1106.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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
#include <oledfont.h>

static const char *TAG = "oled_sh1106";
uint8_t OLED_GRAM[144][8];
oled_sh1106_config_t *config = NULL;
void oled_sh1106_system_init(oled_sh1106_config_t *obj)
{
    if (obj == NULL)
        return;
    config = obj;
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << config->dc) | (1ULL << config->rst);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    spi_config_t spi_config;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: true, WRITE_STATUS: false, READ_STATUS: false, WRITE_BUFFER: false, READ_BUFFER: false
    spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;
    // Cancel hardware cs
    spi_config.interface.cs_en = 0;
    // MISO pin is used for DC
    spi_config.interface.miso_en = 0;
    // CPOL: 1, CPHA: 1
    spi_config.interface.cpol = 1;
    spi_config.interface.cpha = 1;
    // Set SPI to master mode
    // 8266 Only support half-duplex
    spi_config.mode = SPI_MASTER_MODE;
    // Set the SPI clock frequency division factor
    spi_config.clk_div = SPI_40MHz_DIV;
    // Register SPI event callback function
    spi_config.event_cb = NULL;
    ESP_ERROR_CHECK(spi_init(HSPI_HOST, &spi_config));
}

void oled_sh1106_command(uint8_t data)
{
    uint32_t buf = data << 24 | data << 16 | data << 8 | data; // In order to improve the transmission efficiency, it is recommended that the external incoming data is (uint32_t *) type data, do not use other type data.
    spi_trans_t trans = {0};
    trans.mosi = &buf;
    trans.bits.mosi = 8;
    gpio_set_level(config->dc, 0);
    spi_trans(HSPI_HOST, &trans);
}
void oled_sh1106_data(uint8_t data)
{
    uint32_t buf = data << 24 | data << 16 | data << 8 | data; // In order to improve the transmission efficiency, it is recommended that the external incoming data is (uint32_t *) type data, do not use other type data.
    spi_trans_t trans = {0};
    trans.mosi = &buf;
    trans.bits.mosi = 8;
    gpio_set_level(config->dc, 1);
    spi_trans(HSPI_HOST, &trans);
}

void oled_sh1106_driver_init(void)
{

    gpio_set_level(config->rst, 0);
    vTaskDelay(200 / portTICK_RATE_MS);
    gpio_set_level(config->rst, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    oled_sh1106_command(0xAE); //--turn off oled panel

    oled_sh1106_command(0x02); //---set low column address
    oled_sh1106_command(0x10); //---set high column address

    oled_sh1106_command(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_sh1106_command(0x81); //--set contrast control register
    oled_sh1106_command(0xA0); //--Set SEG/Column Mapping a0/a1
    oled_sh1106_command(0xC0); //Set COM/Row Scan Direction
    oled_sh1106_command(0xA6); //--set normal display a6/a7
    oled_sh1106_command(0xA8); //--set multiplex ratio(1 to 64)
    oled_sh1106_command(0x3F); //--1/64 duty
    oled_sh1106_command(0xD3); //-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    oled_sh1106_command(0x00); //-not offset
    oled_sh1106_command(0xd5); //--set display clock divide ratio/oscillator frequency
    oled_sh1106_command(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    oled_sh1106_command(0xD9); //--set pre-charge period
    oled_sh1106_command(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_sh1106_command(0xDA); //--set com pins hardware configuration
    oled_sh1106_command(0x12);
    oled_sh1106_command(0xDB); //--set vcomh
    oled_sh1106_command(0x40); //Set VCOM Deselect Level
    oled_sh1106_command(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_sh1106_command(0x02); //
    oled_sh1106_command(0xAa); // Disable Entire Display On (0xa4/0xa5)
    oled_sh1106_command(0xA6); // Disable Inverse Display On (0xa6/a7)
    oled_sh1106_command(0xC8);
    oled_sh1106_command(0xA1);
    oled_sh1106_clear_all();
    oled_sh1106_command(0xAF); /*display ON*/
}

void oled_sh1106_clear_all(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        bzero(OLED_GRAM, sizeof(OLED_GRAM));
        oled_sh1106_command(0xb0 + i); //设置页地址（0~7）
        oled_sh1106_command(0x02);     //设置显示位置—列低地址
        oled_sh1106_command(0x10);     //设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            oled_sh1106_data(OLED_GRAM[n][i]);
    } //更新显示
}


uint8_t reverse(uint8_t temp)
{
    temp = ((temp & 0x55) << 1) | ((temp & 0xaa) >> 1);
    temp = ((temp & 0x33) << 2) | ((temp & 0xcc) >> 2);
    temp = ((temp & 0x0f) << 4) | ((temp & 0xf0) >> 4);
    return temp;
}


void oled_sh1106_update(const uint8_t *image)
{

    uint16_t page, column, temp;

    for (page = 0; page < 8; page++)
    {
        /* set page address */
        oled_sh1106_command(0xB0 + page);
        /* set low column address */
        oled_sh1106_command(0x02);
        /* set high column address */
        oled_sh1106_command(0x10);

        /* write data */
        for (column = 0; column < 128; column++)
        {
            temp = image[(7 - page) + column * 8];
            oled_sh1106_data(temp);
        }
    }
}



void oled_sh1106_pos(uint8_t x, uint8_t y)
{
    x += 2;
    oled_sh1106_command(0xb0 + y);
    oled_sh1106_command(((x & 0xf0) >> 4) | 0x10);
    oled_sh1106_command((x & 0x0f));
}
void oled_sh1106_refresh(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        oled_sh1106_command(0xb0 + i); //设置行起始地址
        oled_sh1106_command(0x02);     //设置低列起始地址
        oled_sh1106_command(0x10);     //设置高列起始地址
        for (n = 0; n < 128; n++)
            oled_sh1106_data(OLED_GRAM[n][i]);
    }
}
void oled_sh1106_point(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t)
    {
        OLED_GRAM[x][i] |= n;
    }
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

void oled_sh1106_circle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        oled_sh1106_point(x + a, y - b, 1);
        oled_sh1106_point(x - a, y - b, 1);
        oled_sh1106_point(x - a, y + b, 1);
        oled_sh1106_point(x + a, y + b, 1);

        oled_sh1106_point(x + b, y + a, 1);
        oled_sh1106_point(x + b, y - a, 1);
        oled_sh1106_point(x - b, y - a, 1);
        oled_sh1106_point(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

void oled_sh1106_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; //画线起点坐标
    uCol = y1;
    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_x;
    }
    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        oled_sh1106_point(uRow, uCol, mode); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void oled_sh1106_picture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode)
{
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint8_t x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++)
    {
        for (i = 0; i < sizex; i++)
        {
            temp = BMP[j];
            j++;
            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    oled_sh1106_point(x, y, mode);
                else
                    oled_sh1106_point(x, y, !mode);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}
void oled_sh1106_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';                                              //计算偏移后的值
    for (i = 0; i < size2; i++)
    {
        if (size1 == 8)
        {
            temp = asc2_0806[chr1][i];
        } //调用0806字体
        else if (size1 == 12)
        {
            temp = asc2_1206[chr1][i];
        } //调用1206字体
        else if (size1 == 16)
        {
            temp = asc2_1608[chr1][i];
        } //调用1608字体
        else if (size1 == 24)
        {
            temp = asc2_2412[chr1][i];
        } //调用2412字体
        else
            return;
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                oled_sh1106_point(x, y, mode);
            else
                oled_sh1106_point(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}
void oled_sh1106_string(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size, uint8_t mode)
{
    while ((*chr >= ' ') && (*chr <= '~')) //判断是不是非法字符!
    {
        oled_sh1106_char(x, y, *chr, size, mode);
        if (size == 8)
            x += 6;
        else
            x += size / 2;
        chr++;
    }
}
int oled_sh1106_format(uint8_t x, uint8_t y, uint8_t fontsize, uint8_t mode, const char *format, ...)
{
    char buffer[512] = {0};
    bzero(buffer, sizeof(buffer));
    va_list args;
    int rc;
    va_start(args, format);
    rc = vsprintf(buffer, format, args);
    va_end(args);
    ESP_LOGI(TAG, buffer);
    oled_sh1106_string(x, y, (uint8_t *)buffer, fontsize, mode);

    return rc;
}
void oled_sh1106_angle(uint32_t x, uint32_t y, float angle, uint32_t radius, uint8_t mode)
{
    int x0, y0;
    double k = angle * (3.1415926535 / 180);
    x0 = cos(k) * radius;
    y0 = sin(k) * radius;
    oled_sh1106_point(x - x0, y - y0, mode);
}
void oled_sh1106_angle_line(uint32_t x, uint32_t y, float angle, uint32_t radius, uint32_t segment, uint8_t mode)
{
    int i;
    int x0, y0;
    double k = angle * (3.1415926535 / 180);
    for (i = radius - segment; i < radius; i++)
    {
        x0 = cos(k) * i;
        y0 = sin(k) * i;
        oled_sh1106_point(x - x0, y - y0, mode);
    }
}


