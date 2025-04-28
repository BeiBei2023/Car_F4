#include "st7735s.h"

/* vim: set ai et ts=4 sw=4: */



#define DELAY 0x80

// based on Adafruit ST7735 library for Arduino
static const uint8_t
    init_cmds1[] = {           // Init for 7735R, part 1 (red or green tab)
        15,                    // 15 commands in list:
        ST7735_SWRESET, DELAY, //  1: Software reset, 0 args, w/delay
        150,                   //     150 ms delay
        ST7735_SLPOUT, DELAY,  //  2: Out of sleep mode, 0 args, w/delay
        255,                   //     500 ms delay
        ST7735_FRMCTR1, 3,     //  3: Frame rate ctrl - normal mode, 3 args:
        0x01, 0x2C, 0x2D,      //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR2, 3,     //  4: Frame rate control - idle mode, 3 args:
        0x01, 0x2C, 0x2D,      //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR3, 6,     //  5: Frame rate ctrl - partial mode, 6 args:
        0x01, 0x2C, 0x2D,      //     Dot inversion mode
        0x01, 0x2C, 0x2D,      //     Line inversion mode
        ST7735_INVCTR, 1,      //  6: Display inversion ctrl, 1 arg, no delay:
        0x07,                  //     No inversion
        ST7735_PWCTR1, 3,      //  7: Power control, 3 args, no delay:
        0xA2,
        0x02,             //     -4.6V
        0x84,             //     AUTO mode
        ST7735_PWCTR2, 1, //  8: Power control, 1 arg, no delay:
        0xC5,             //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
        ST7735_PWCTR3, 2, //  9: Power control, 2 args, no delay:
        0x0A,             //     Opamp current small
        0x00,             //     Boost frequency
        ST7735_PWCTR4, 2, // 10: Power control, 2 args, no delay:
        0x8A,             //     BCLK/2, Opamp current small & Medium low
        0x2A,
        ST7735_PWCTR5, 2, // 11: Power control, 2 args, no delay:
        0x8A, 0xEE,
        ST7735_VMCTR1, 1, // 12: Power control, 1 arg, no delay:
        0x0E,
        ST7735_INVOFF, 0, // 13: Don't invert display, no args, no delay
        ST7735_MADCTL, 1, // 14: Memory access control (directions), 1 arg:
        ST7735_ROTATION,  //     row addr/col addr, bottom to top refresh
        ST7735_COLMOD, 1, // 15: set color mode, 1 arg, no delay:
        0x05},            //     16-bit color

#if (defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128))
    init_cmds2[] = {     // Init for 7735R, part 2 (1.44" display)
        2,               //  2 commands in list:
        ST7735_CASET, 4, //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,      //     XSTART = 0
        0x00, 0x7F,      //     XEND = 127
        ST7735_RASET, 4, //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,      //     XSTART = 0
        0x00, 0x7F},     //     XEND = 127
#endif                   // ST7735_IS_128X128

#ifdef ST7735_IS_160X80
    init_cmds2[] = {      // Init for 7735S, part 2 (160x80 display)
        3,                //  3 commands in list:
        ST7735_CASET, 4,  //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x4F,       //     XEND = 79
        ST7735_RASET, 4,  //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,       //     XSTART = 0
        0x00, 0x9F,       //     XEND = 159
        ST7735_INVON, 0}, //  3: Invert colors
#endif

    init_cmds3[] = {                                                                                                         // Init for 7735R, part 3 (red or green tab)
        4,                                                                                                                   //  4 commands in list:
        ST7735_GMCTRP1, 16,                                                                                                  //  1: Gamma Adjustments (pos. polarity), 16 args, no delay:
        0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, ST7735_GMCTRN1, 16,  //  2: Gamma Adjustments (neg. polarity), 16 args, no delay:
        0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, ST7735_NORON, DELAY, //  3: Normal display on, no args, w/delay
        10,                                                                                                                  //     10 ms delay
        ST7735_DISPON, DELAY,                                                                                                //  4: Main screen turn on, no args w/delay
        100};                                                                                                                //     100 ms delay

static void ST7735_Select()
{
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
}

void ST7735_Unselect()
{
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_Reset()
{
  HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET);
}

static void ST7735_WriteCommand(uint8_t cmd)
{
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);

  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd),HAL_MAX_DELAY);

}

static void ST7735_WriteData(uint8_t *buff, size_t buff_size)
{
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);

  HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size,HAL_MAX_DELAY);
}

static void ST7735_ExecuteCommandList(const uint8_t *addr)
{
  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = *addr++;
  while (numCommands--)
  {
    uint8_t cmd = *addr++;
    ST7735_WriteCommand(cmd);

    numArgs = *addr++;
    // If high bit set, delay follows args
    ms = numArgs & DELAY;
    numArgs &= ~DELAY;
    if (numArgs)
    {
      ST7735_WriteData((uint8_t *)addr, numArgs);
      addr += numArgs;
    }

    if (ms)
    {
      ms = *addr++;
      if (ms == 255)
        ms = 500;
      HAL_Delay(ms);
    }
  }
}

/**
 * 设置ST7735显示屏的地址窗口
 * 
 * 本函数通过发送特定的命令和数据到ST7735显示屏，来设置显示区域的地址窗口
 * 这包括设置列地址和行地址，并准备写入RAM
 * 
 * @param x0 显示窗口左上角的X坐标
 * @param y0 显示窗口左上角的Y坐标
 * @param x1 显示窗口右下角的X坐标
 * @param y1 显示窗口右下角的Y坐标
 */
static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  // column address set
  ST7735_WriteCommand(ST7735_CASET);
  // 设置列地址范围，考虑到显示区域的起始偏移
  uint8_t data[] = {0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART};
  ST7735_WriteData(data, sizeof(data));

  // row address set
  ST7735_WriteCommand(ST7735_RASET);
  // 设置行地址范围，考虑到显示区域的起始偏移
  data[1] = y0 + ST7735_YSTART;
  data[3] = y1 + ST7735_YSTART;
  ST7735_WriteData(data, sizeof(data));

  // write to RAM
  ST7735_WriteCommand(ST7735_RAMWR);
  // 准备写入RAM，以在设置的地址窗口内显示内容
}

/**
 * @brief 初始化ST7735液晶显示器
 * 
 * 本函数执行ST7735液晶显示器的初始化步骤，包括复位、执行初始化命令列表，
 * 以及初始化完成后填充屏幕为黄色。这些操作确保了显示器能够正确配置并进入
 * 可显示状态。
 */
void ST7735_Init()
{
  // 选择ST7735设备，准备进行初始化
  ST7735_Select();
  
  // 复位ST7735显示器，确保从一个已知的状态开始
  ST7735_Reset();
  
  // 执行第一组初始化命令，配置显示器的基本设置
  ST7735_ExecuteCommandList(init_cmds1);
  
  // 执行第二组初始化命令，进一步配置显示器的显示模式等
  ST7735_ExecuteCommandList(init_cmds2);
  
  // 执行第三组初始化命令，完成显示器的初始化配置
  ST7735_ExecuteCommandList(init_cmds3);
  
  // 取消选择ST7735设备，初始化命令执行完毕
  ST7735_Unselect();
  
  // 初始化完成后，快速填充整个屏幕为黑色，作为初始化完成的标志
  ST7735_FillScreenFast(ST7735_RED);

  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
}

/**
 * 在ST7735液晶显示屏上绘制一个像素点。
 * 
 * 此函数通过接收x和y坐标以及颜色值来在指定位置绘制一个像素点。
 * 它首先检查给定的坐标是否超出了显示屏的边界，如果超出则直接返回。
 * 然后，它会选择显示屏，并设置绘制区域的窗口地址，接着发送颜色数据，
 * 最后取消选择显示屏。
 * 
 * @param x 横坐标，表示像素点在显示屏上的水平位置。
 * @param y 纵坐标，表示像素点在显示屏上的垂直位置。
 * @param color 像素点的颜色值，采用RGB565格式。
 */
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  // 检查坐标是否超出显示屏边界
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;

  // 选择显示屏以进行操作
  ST7735_Select();

  // 设置绘制像素点的窗口地址
  ST7735_SetAddressWindow(x, y, x + 1, y + 1);

  // 准备颜色数据，分为高字节和低字节
  uint8_t data[] = {color >> 8, color & 0xFF};

  // 发送颜色数据到显示屏
  ST7735_WriteData(data, sizeof(data));

  // 取消选择显示屏，结束操作
  ST7735_Unselect();
}

/**
 * 在ST7735液晶显示屏上写入一个字符
 *
 * @param x 字符显示的起始X坐标
 * @param y 字符显示的起始Y坐标
 * @param ch 要显示的字符
 * @param font 字体定义结构体，包含字体的宽度、高度和数据
 * @param color 字符的颜色
 * @param bgcolor 背景颜色
 */
static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor)
{
  // 定义循环变量和临时变量
  uint32_t i, b, j;

  // 设置显示窗口，即字符显示的区域
  ST7735_SetAddressWindow(x, y, x + font->width - 1, y + font->height - 1);

  // 遍历字符的每一行
  for (i = 0; i < font->height; i++)
  {
    // 获取当前行的字形数据
    b = font->data[(ch - 32) * font->height + i];
    // 遍历当前行的每一列
    for (j = 0; j < font->width; j++)
    {
      // 检查当前列是否为前景色
      if ((b << j) & 0x8000)
      {
        // 是前景色，则发送颜色数据到显示屏
        uint8_t data[] = {color >> 8, color & 0xFF};
        ST7735_WriteData(data, sizeof(data));
      }
      else
      {
        // 不是前景色，则发送背景色数据到显示屏
        uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
        ST7735_WriteData(data, sizeof(data));
      }
    }
  }
}

/*
Simpler (and probably slower) implementation:

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color) {
    uint32_t i, b, j;

    for(i = 0; i < font->height; i++) {
        b = font->data[(ch - 32) * font->height + i];
        for(j = 0; j < font->width; j++) {
            if((b << j) & 0x8000)  {
                ST7735_DrawPixel(x + j, y + i, color);
            }
        }
    }
}
*/

void ST7735_WriteString(uint16_t x, uint16_t y, const char *str, FontDef* font, uint16_t color, uint16_t bgcolor)
{
  ST7735_Select();

  while (*str)
  {
    if (x + font->width >= ST7735_WIDTH)
    {
      x = 0;
      y += font->height;
      if (y + font->height >= ST7735_HEIGHT)
      {
        break;
      }

      if (*str == ' ')
      {
        // skip spaces in the beginning of the new line
        str++;
        continue;
      }
    }

    ST7735_WriteChar(x, y, *str, font, color, bgcolor);
    x += font->width;
    str++;
  }

  ST7735_Unselect();
}

/**
 * 在ST7735显示屏上绘制填充矩形
 * 
 * 此函数通过指定的坐标、宽度、高度和颜色，在ST7735显示屏上绘制一个填充矩形
 * 它首先执行裁剪操作，以确保绘制的矩形不会超出显示屏的边界
 * 然后设置矩形的绘制区域，并通过SPI接口发送相应的像素数据
 * 
 * @param x 矩形左上角的x坐标
 * @param y 矩形左上角的y坐标
 * @param w 矩形的宽度
 * @param h 矩形的高度
 * @param color 用于填充矩形的颜色
 */
void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  // clipping
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;
  if ((x + w - 1) >= ST7735_WIDTH)
    w = ST7735_WIDTH - x;
  if ((y + h - 1) >= ST7735_HEIGHT)
    h = ST7735_HEIGHT - y;

  // Select the ST7735 display for communication
  ST7735_Select();
  // Set the drawing area for the rectangle
  ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

  // Prepare the color data to be sent
  uint8_t data[] = {color >> 8, color & 0xFF};
  // Set the data/command pin to data mode
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  // Send the color data for each pixel in the rectangle
  for (y = h; y > 0; y--)
  {
    for (x = w; x > 0; x--)
    {

      HAL_SPI_Transmit(&ST7735_SPI_PORT, data, sizeof(data),HAL_MAX_DELAY);
    }
  }

  // Unselect the ST7735 display after drawing is complete
  ST7735_Unselect();
}

// Fill a rectangle area on the ST7735 LCD screen with a specified color
// This function is optimized for speed, suitable for filling large areas
void ST7735_FillRectangleFast(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  // clipping
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;
  if ((x + w - 1) >= ST7735_WIDTH)
    w = ST7735_WIDTH - x;
  if ((y + h - 1) >= ST7735_HEIGHT)
    h = ST7735_HEIGHT - y;

  ST7735_Select();
  ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

  // Prepare whole line in a single buffer
  uint8_t pixel[] = {color >> 8, color & 0xFF};
  uint8_t *line = malloc(w * sizeof(pixel));
  for (x = 0; x < w; ++x)
    memcpy(line + x * sizeof(pixel), pixel, sizeof(pixel));

  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  for (y = h; y > 0; y--)

    HAL_SPI_Transmit(&ST7735_SPI_PORT, line, w * sizeof(pixel),HAL_MAX_DELAY);

  free(line);
  ST7735_Unselect();
}

/**
 * 填充整个屏幕为指定颜色。
 * 
 * @param color 要填充的颜色值。
 */
void ST7735_FillScreen(uint16_t color)
{
  // 使用FillRectangle函数填充整个屏幕，起始坐标为(0, 0)，覆盖整个屏幕宽度和高度。
  ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

/**
 * ST7735_FillScreenFast函数
 * 
 * 使用指定的颜色填充整个屏幕。该函数通过调用ST7735_FillRectangleFast函数，
 * 设置填充区域为整个屏幕的宽度和高度来实现快速填充屏幕的效果。
 * 
 * @param color 填充屏幕所使用的颜色值。这是一个16位的颜色值，适用于ST7735显示屏。
 */
void ST7735_FillScreenFast(uint16_t color)
{
  // 调用快速填充矩形函数，参数为(起始X坐标, 起始Y坐标, 屏幕宽度, 屏幕高度, 颜色)
  // 用于填充整个屏幕为指定的颜色
  ST7735_FillRectangleFast(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

/**
 * 在ST7735液晶屏上绘制图像
 * 
 * 此函数将根据提供的参数和图像数据，在ST7735液晶屏的指定位置绘制图像
 * 它首先检查图像的绘制区域是否超出液晶屏的边界，如果超出，则直接返回
 * 如果未超出，它将选择液晶屏，设置图像绘制的窗口区域，然后写入图像数据
 * 
 * @param x 图像绘制区域的起始x坐标
 * @param y 图像绘制区域的起始y坐标
 * @param w 图像的宽度
 * @param h 图像的高度
 * @param data 图像数据指针，图像数据是16位的RGB565格式
 */
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
  // 检查图像的起始坐标是否超出液晶屏的边界
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;

  // 检查图像的结束x坐标是否超出液晶屏的宽度
  if ((x + w - 1) >= ST7735_WIDTH)
    return;

  // 检查图像的结束y坐标是否超出液晶屏的高度
  if ((y + h - 1) >= ST7735_HEIGHT)
    return;

  // 选择液晶屏，准备写入数据
  ST7735_Select();

  // 设置图像绘制的窗口区域
  ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

  // 写入图像数据，图像数据是16位的RGB565格式
  ST7735_WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);

  // 取消选择液晶屏
  ST7735_Unselect();
}

/**
 * ST7735_InvertColors函数用于控制ST7735显示屏的颜色反转功能。
 * 
 * @param invert 一个布尔值，决定是否反转显示屏的颜色。如果invert为true，颜色将反转；如果为false，颜色将保持正常。
 */
void ST7735_InvertColors(bool invert)
{
  // 选择ST7735显示屏以进行通信。
  ST7735_Select();
  
  // 发送命令以反转或恢复正常显示颜色，具体取决于invert参数的值。
  ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
  
  // 取消选择ST7735显示屏，结束通信。
  ST7735_Unselect();
}

/**
 * @brief 设置ST7735显示器的伽马曲线
 * 
 * 本函数通过选择合适的伽马曲线来调整显示器的颜色特性。伽马曲线影响显示器如何呈现颜色的深浅，
 * 对于不同的显示需求，可以选择不同的伽马曲线设置。
 * 
 * @param gamma 伽马曲线定义，指定要设置的伽马曲线
 */
void ST7735_SetGamma(GammaDef gamma)
{
  // 选择ST7735显示器以进行通信
  ST7735_Select();
  
  // 发送伽马设置命令到ST7735显示器
  ST7735_WriteCommand(ST7735_GAMSET);
  
  // 发送具体的伽马曲线数据到显示器
  ST7735_WriteData((uint8_t *)&gamma, sizeof(gamma));
  
  // 通信完成后取消选择ST7735显示器
  ST7735_Unselect();
}
