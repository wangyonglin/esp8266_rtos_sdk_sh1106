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
#include <oledfont.h>
static const char *TAG = "OLED_1in3_c";
uint8_t OLED_GRAM[144][8];
void OLED_1li3_SystemInit(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << OLED_HSPI_DC) | (1ULL << OLED_HSPI_RST);
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
    spi_config.clk_div = SPI_10MHz_DIV;
    // Register SPI event callback function
    spi_config.event_cb = NULL;
    ESP_ERROR_CHECK(spi_init(HSPI_HOST, &spi_config));
}

void OLED_1li3_WriteCommand(uint8_t data)
{
    uint32_t buf = data << 24 | data << 16 | data << 8 | data; // In order to improve the transmission efficiency, it is recommended that the external incoming data is (uint32_t *) type data, do not use other type data.
    spi_trans_t trans = {0};
    trans.mosi = &buf;
    trans.bits.mosi = 8;
    gpio_set_level(OLED_HSPI_DC, 0);
    spi_trans(HSPI_HOST, &trans);
}
void OLED_1li3_WriteData(uint8_t data)
{
    uint32_t buf = data << 24 | data << 16 | data << 8 | data; // In order to improve the transmission efficiency, it is recommended that the external incoming data is (uint32_t *) type data, do not use other type data.
    spi_trans_t trans = {0};
    trans.mosi = &buf;
    trans.bits.mosi = 8;
    gpio_set_level(OLED_HSPI_DC, 1);
    spi_trans(HSPI_HOST, &trans);
}
/********************************************************************************
function:
			initialization
********************************************************************************/
void OLED_1in3_DriverInit(void)
{

    gpio_set_level(OLED_HSPI_RST, 0);
    vTaskDelay(200 / portTICK_RATE_MS);
    gpio_set_level(OLED_HSPI_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    OLED_1li3_WriteCommand(0xAE); //--turn off oled panel

    OLED_1li3_WriteCommand(0x02); //---set low column address
    OLED_1li3_WriteCommand(0x10); //---set high column address

    OLED_1li3_WriteCommand(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_1li3_WriteCommand(0x81); //--set contrast control register
    OLED_1li3_WriteCommand(0xA0); //--Set SEG/Column Mapping a0/a1
    OLED_1li3_WriteCommand(0xC0); //Set COM/Row Scan Direction
    OLED_1li3_WriteCommand(0xA6); //--set normal display a6/a7
    OLED_1li3_WriteCommand(0xA8); //--set multiplex ratio(1 to 64)
    OLED_1li3_WriteCommand(0x3F); //--1/64 duty
    OLED_1li3_WriteCommand(0xD3); //-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    OLED_1li3_WriteCommand(0x00); //-not offset
    OLED_1li3_WriteCommand(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_1li3_WriteCommand(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_1li3_WriteCommand(0xD9); //--set pre-charge period
    OLED_1li3_WriteCommand(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_1li3_WriteCommand(0xDA); //--set com pins hardware configuration
    OLED_1li3_WriteCommand(0x12);
    OLED_1li3_WriteCommand(0xDB); //--set vcomh
    OLED_1li3_WriteCommand(0x40); //Set VCOM Deselect Level
    OLED_1li3_WriteCommand(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_1li3_WriteCommand(0x02); //
    OLED_1li3_WriteCommand(0xAa); // Disable Entire Display On (0xa4/0xa5)
    OLED_1li3_WriteCommand(0xA6); // Disable Inverse Display On (0xa6/a7)
    OLED_1li3_WriteCommand(0xC8);
    OLED_1li3_WriteCommand(0xA1);
    OLED_1in3_ClearAll();
    OLED_1li3_WriteCommand(0xAF); /*display ON*/

    //OLED_1in3_ReverseColor(0);
   // OLED_1in3_RotateShow(1);

}

/********************************************************************************
function:
			Clear screen
********************************************************************************/
void OLED_1in3_ClearAll(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_1li3_WriteCommand(0xb0 + i); //设置页地址（0~7）
        OLED_1li3_WriteCommand(0x02);     //设置显示位置—列低地址
        OLED_1li3_WriteCommand(0x10);     //设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            OLED_1li3_WriteData(0x00);
    } //更新显示
}

/********************************************************************************
function:	reverse a byte data
********************************************************************************/
uint8_t reverse(uint8_t temp)
{
    temp = ((temp & 0x55) << 1) | ((temp & 0xaa) >> 1);
    temp = ((temp & 0x33) << 2) | ((temp & 0xcc) >> 2);
    temp = ((temp & 0x0f) << 4) | ((temp & 0xf0) >> 4);
    return temp;
}

/********************************************************************************
function:	Update all memory to OLED
********************************************************************************/
void OLED_1in3_Update(const uint8_t *Image)
{

    uint16_t page, column, temp;

    for (page = 0; page < 8; page++)
    {
        /* set page address */
        OLED_1li3_WriteCommand(0xB0 + page);
        /* set low column address */
        OLED_1li3_WriteCommand(0x02);
        /* set high column address */
        OLED_1li3_WriteCommand(0x10);

        /* write data */
        for (column = 0; column < 128; column++)
        {
            temp = Image[(7 - page) + column * 8];
            OLED_1li3_WriteData(temp);
        }
    }
}

void OLED_1in3_ReverseColor(uint8_t i)
{
    if (i == 0)
    {
        OLED_1li3_WriteCommand(0xA6); //正常显示
    }
    if (i == 1)
    {
        OLED_1li3_WriteCommand(0xA7); //反色显示
    }
}
void OLED_1in3_RotateShow(uint8_t i)
{
    if (i == 0)
    {
        OLED_1li3_WriteCommand(0xC8); //正常显示
        OLED_1li3_WriteCommand(0xA1);
    }
    if (i == 1)
    {
        OLED_1li3_WriteCommand(0xC0); //反转显示
        OLED_1li3_WriteCommand(0xA0);
    }
}

void OLED_1in3_S_Pos(uint8_t x, uint8_t y)
{
    x += 2;
    OLED_1li3_WriteCommand(0xb0 + y);
    OLED_1li3_WriteCommand(((x & 0xf0) >> 4) | 0x10);
    OLED_1li3_WriteCommand((x & 0x0f));
}
void OLED_1li3_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
	   OLED_1li3_WriteCommand(0xb0+i); //设置行起始地址
	   OLED_1li3_WriteCommand(0x02);   //设置低列起始地址
	   OLED_1li3_WriteCommand(0x10);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_1li3_WriteData(OLED_GRAM[n][i]);
  }
}
void OLED_1li3_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

void OLED_1li3_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_1li3_DrawPoint(x + a, y - b,1);
        OLED_1li3_DrawPoint(x - a, y - b,1);
        OLED_1li3_DrawPoint(x - a, y + b,1);
        OLED_1li3_DrawPoint(x + a, y + b,1);
 
        OLED_1li3_DrawPoint(x + b, y + a,1);
        OLED_1li3_DrawPoint(x + b, y - a,1);
        OLED_1li3_DrawPoint(x - b, y - a,1);
        OLED_1li3_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

void OLED_1li3_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_1li3_DrawPoint(uRow,uCol,mode);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void OLED_1li3_Picture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
	uint16_t j=0;
	uint8_t i,n,temp,m;
	uint8_t x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_1li3_DrawPoint(x,y,mode);
					else OLED_1li3_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}
void OLED_1in3_Char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_1li3_DrawPoint(x,y,mode);
			else OLED_1li3_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}
void OLED_1in3_String(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size,uint8_t mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_1in3_Char(x,y,*chr,size,mode);
		if(size==8)x+=6;
		else x+=size/2;
		chr++;
  }
}

