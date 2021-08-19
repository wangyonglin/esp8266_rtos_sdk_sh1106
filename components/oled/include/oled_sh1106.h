#ifndef __oled_sh1106_h
#define __oled_sh1106_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct
{
    uint8_t mosi;
    uint8_t miso;
    uint8_t clk;
    uint8_t cs;
} oled_sh1106_spi_t;
typedef struct
{
    oled_sh1106_spi_t spi;
    uint8_t cpha;
    uint8_t cpol;
    uint8_t rst;
    uint8_t dc;
} oled_sh1106_config_t;

#define OLED_SH1106_SPI_CPHA 1
#define OLED_SH1106_SPI_CPOL 1
#define OLED_SH1106_SPI_MOSI 13
#define OLED_SH1106_SPI_CLK 14
#define OLED_SH1106_SPI_CS 15
#define OLED_SH1106_RST 4
#define OLED_SH1106_DC 12
    //function
    void oled_sh1106_system_init(oled_sh1106_config_t *config);
void oled_sh1106_driver_init(void);
void oled_sh1106_clear_all(void);
void oled_sh1106_Update(const uint8_t *Image);
void oled_sh1106_command(uint8_t data);
void oled_sh1106_data(uint8_t data);
void oled_sh1106_pos(uint8_t x, uint8_t y);
void oled_sh1106_refresh(void);
void oled_sh1106_point(uint8_t x, uint8_t y, uint8_t t);

void oled_sh1106_circle(uint8_t x, uint8_t y, uint8_t r);
void oled_sh1106_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode);
void oled_sh1106_picture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode);
void oled_sh1106_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode);
void oled_sh1106_strint(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size, uint8_t mode);
void oled_sh1106_angle(uint32_t x, uint32_t y, float angle, uint32_t radius, uint8_t mode);
void oled_sh1106_angle_line(uint32_t x, uint32_t y, float angle, uint32_t radius, uint32_t segment, uint8_t mode);
int oled_sh1106_format(uint8_t x, uint8_t y, uint8_t fontsize, uint8_t mode, const char *format, ...);
void oled_sh1106_update(const uint8_t *image);
#endif
