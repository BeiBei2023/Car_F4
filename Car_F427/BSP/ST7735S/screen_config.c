#include "screen_config.h"

// extern FontDef Font_16x26;

/**** 文字区域配置 ****/
TextRegion voltage_regions[] = {
    {31, 43, &Font_16x26, ST7735_BLACK, ST7735_WHITE, ""} // 电压显示区域
};

TextRegion temp_regions[] = {
    {9, 55, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""}, // 温度
    {91, 55, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""} // 湿度
};

/**** 界面配置 ****/
ScreenConfig screens[] = {
    {// 电压界面
     .bg_image = gImage_dianya,
     .bg_width = 160,
     .bg_height = 80,
     .regions = voltage_regions,
     .region_count = 1},
    {// 温湿度界面
     .bg_image = gImage_wenshidu,
     .bg_width = 160,
     .bg_height = 80,
     .regions = temp_regions,
     .region_count = 2}};

/**** 全局管理器实例 ****/
ScreenManager screen_mgr;
