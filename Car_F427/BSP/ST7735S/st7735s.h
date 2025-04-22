
#ifndef _ST7735S_H
#define _ST7735S_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "fonts.h"
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define ST7735_SPI_PORT hspi1
extern SPI_HandleTypeDef ST7735_SPI_PORT;

#define ST7735_RES_Pin       LCD_RESET_Pin
#define ST7735_RES_GPIO_Port LCD_RESET_GPIO_Port
#define ST7735_CS_Pin        LCD_CS_Pin
#define ST7735_CS_GPIO_Port  LCD_CS_GPIO_Port
#define ST7735_DC_Pin        LCD_DC_Pin
#define ST7735_DC_GPIO_Port  LCD_DC_GPIO_Port

// AliExpress/eBay 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY)
*/

// AliExpress/eBay 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, rotate left
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (0)
*/

// WaveShare ST7735S-based 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, rotate left
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_RGB)
*/

// 1.44" display, default orientation
/*#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 2
#define ST7735_YSTART 3
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
*/
// 1.44" display, rotate right
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 3
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, rotate left
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, upside down
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_BGR)
*/

// mini 160x80 display (it's unlikely you want the default orientation)
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 26
#define ST7735_YSTART 1
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160 
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
*/

// mini 160x80, rotate left

#define ST7735_IS_160X80 1
#define ST7735_XSTART 1
#define ST7735_YSTART 26
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MX| ST7735_MADCTL_MV | ST7735_MADCTL_BGR)


// mini 160x80, rotate right 
/*
 * 定义ST7735显示屏的相关参数和配置
 
#define ST7735_IS_160X80 1 // 定义显示屏的分辨率是否为160x80
#define ST7735_XSTART 1   // 定义显示屏X轴的起始位置
#define ST7735_YSTART 26  // 定义显示屏Y轴的起始位置
#define ST7735_WIDTH  160 // 定义显示屏的宽度
#define ST7735_HEIGHT 80  // 定义显示屏的高度

 * 定义显示屏的旋转配置，包括垂直镜像、水平镜像和颜色顺序
 * ST7735_MADCTL_MY: 垂直镜像
 * ST7735_MADCTL_MV: 水平镜像
 * ST7735_MADCTL_BGR: 颜色顺序为BGR

#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/
/****************************/

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_GAMSET  0x26
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

/**
 * @brief 定义了可用的伽马校准值的枚举类型
 * 
 * 这个枚举类型 GammaDef 定义了一系列常量，代表了不同的伽马校准值。
 * 伽马校准用于调整显示设备的输出，以确保颜色的准确还原。
 * 每个枚举值都对应于一个特定的伽马值，用于在代码中方便地选择和应用不同的伽马校准设置。
 */
typedef enum {
    GAMMA_10 = 0x01, // 代表伽马值为1.0的校准设置
    GAMMA_25 = 0x02, // 代表伽马值为2.5的校准设置
    GAMMA_22 = 0x04, // 代表伽马值为2.2的校准设置
    GAMMA_18 = 0x08  // 代表伽马值为1.8的校准设置
} GammaDef;

#ifdef __cplusplus
extern "C" {
#endif

// call before initializing any SPI devices
// 解释：在初始化任何SPI设备之前调用此函数，以确保SPI通信的正确性
void ST7735_Unselect();

// 初始化ST7735显示屏
// 解释：在使用ST7735显示屏之前，必须调用此函数进行初始化设置
void ST7735_Init(void);

// 在指定位置绘制一个像素点
// 参数：x - 像素点的X坐标；y - 像素点的Y坐标；color - 像素点的颜色
// 解释：此函数用于在显示屏上的特定位置绘制一个单一颜色的像素点
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

// 在指定位置写入字符串
// 参数：x - 字符串起始X坐标；y - 字符串起始Y坐标；str - 要写入的字符串；font - 字体定义；color - 字符颜色；bgcolor - 背景颜色
// 解释：此函数用于在显示屏上的指定位置写入字符串，可以指定字体、字符颜色和背景颜色
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

// 填充指定矩形区域
// 参数：x - 矩形起始X坐标；y - 矩形起始Y坐标；w - 矩形宽度；h - 矩形高度；color - 填充颜色
// 解释：此函数用于填充显示屏上的一个矩形区域，可以指定填充颜色
void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

// 快速填充指定矩形区域
// 参数：x - 矩形起始X坐标；y - 矩形起始Y坐标；w - 矩形宽度；h - 矩形高度；color - 填充颜色
// 解释：此函数是填充矩形区域的快速版本，用于提高填充速度
void ST7735_FillRectangleFast(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

// 填充整个屏幕为指定颜色
// 参数：color - 填充颜色
// 解释：此函数用于将整个显示屏填充为单一颜色
void ST7735_FillScreen(uint16_t color);

// 快速填充整个屏幕为指定颜色
// 参数：color - 填充颜色
// 解释：此函数是填充整个屏幕的快速版本，用于提高填充速度
void ST7735_FillScreenFast(uint16_t color);

// 在指定位置绘制图像
// 参数：x - 图像起始X坐标；y - 图像起始Y坐标；w - 图像宽度；h - 图像高度；data - 图像数据
// 解释：此函数用于在显示屏上的指定位置绘制图像，图像数据以数组形式提供
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);

// 反转显示屏颜色
// 参数：invert - 是否反转颜色（true/false）
// 解释：此函数用于反转显示屏上的颜色，可用于显示反转效果
void ST7735_InvertColors(bool invert);

// 设置显示屏的伽马曲线
// 参数：gamma - 伽马曲线定义
// 解释：此函数用于设置显示屏的伽马曲线，以调整显示效果
void ST7735_SetGamma(GammaDef gamma);

#ifdef __cplusplus
}
#endif
                                              /* DISPLAY_USING_TOUCHGFX */

#endif //_ST7735S_H
