#ifndef __OLED_H
#define __OLED_H
#include <stdlib.h>
#include <stdint.h>
typedef enum {
	S =8,
	L =12,
	XL=16,
	XXL=24
}fontsize_t;
void OLED_Init(void);
void OLED_String(uint8_t x, uint8_t y, const char *chr, fontsize_t size);
void OLED_Integer(uint8_t x, uint8_t y, int32_t num, fontsize_t size);
void OLED_Refresh();

void Boot_Animation(void);
#endif