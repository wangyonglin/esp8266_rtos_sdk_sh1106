/*****************************************************************************
* | File      	:   OLED_1in3_c.c
* | Author      :   Waveshare team
* | Function    :   1.3inch OLED Module (C) Drive function
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2020-08-18
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __OLED_1IN3_C_H
#define __OLED_1IN3_C_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/********************************************************************************
function:	
		Define the full screen height length of the display
********************************************************************************/

#define OLED_1IN3_WIDTH 64	  //OLED width
#define OLED_1IN3_HEIGHT 128  //OLED height

#define OLED_HSPI_MOSI 13
#define OLED_HSPI_CLK 14
#define OLED_HSPI_CS 15
#define OLED_HSPI_RST 4
#define OLED_HSPI_DC 12
//function
void OLED_1li3_SystemInit(void);
void OLED_1in3_DriverInit(void);
void OLED_1in3_ClearAll(void);
void OLED_1in3_Update(const uint8_t *Image);
void OLED_1li3_WriteCommand(uint8_t data);
void OLED_1li3_WriteData(uint8_t data);
void OLED_1in3_ReverseColor(uint8_t i);
void OLED_1in3_RotateShow(uint8_t i);
void OLED_1in3_S_Pos(uint8_t x, uint8_t y);
void OLED_1li3_Refresh(void);
void OLED_1li3_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_1li3_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_1li3_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_1li3_Picture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_1in3_Char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_1in3_String(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size,uint8_t mode);
void OLED_1in3_AngleLine(uint32_t x,uint32_t y,float angle,uint32_t radius,uint32_t segment,uint8_t mode);
void OLED_1in3_AngleLineEx(uint32_t x,uint32_t y,float angle,uint32_t radius,uint8_t mode);
void OLED_1in3_Angle(uint32_t x, uint32_t y, float angle, uint32_t radius,uint8_t mode);
#endif
