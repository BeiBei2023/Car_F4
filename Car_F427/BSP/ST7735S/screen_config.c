#include "screen_config.h"

// extern FontDef Font_16x26;

/**** 文字区域配置 ****/
TextRegion voltage_regions[] = {
    {40, 45, &Font_16x26, ST7735_BLACK, ST7735_WHITE, ""} // 电压显示区域
};

TextRegion temp_regions[] = {
    {15, 43, &Font_16x26, ST7735_BLACK, ST7735_WHITE, ""}, // 温度
    {89, 43, &Font_16x26, ST7735_BLACK, ST7735_WHITE, ""}  // 湿度
};

TextRegion motor_speed[] = {
    {29, 6, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""},   // 温度
    {107, 6, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""},  // 温度
    {29, 54, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""},  // 温度
    {107, 54, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""}, // 温度
};

/**** 界面配置 ****/
ScreenConfig screens[] =
    {
        {
            // 首页，学校名字
            .bg_image = gImage_school_name,
            .bg_width = 160,
            .bg_height = 80,
            .regions = NULL,
            .region_count = 0,
        },
        {// 电压界面
         .bg_image = gImage_shurudianya,
         .bg_width = 160,
         .bg_height = 80,
         .regions = voltage_regions,
         .region_count = 1},
        {// 温湿度界面
         .bg_image = gImage_wenshidu,
         .bg_width = 160,
         .bg_height = 80,
         .regions = temp_regions,
         .region_count = 2},
        {
            // 访问github界面
            .bg_image = gImage_github,
            .bg_width = 160,
            .bg_height = 80,
            .regions = NULL,
            .region_count = 0,
        },
        {// 电机速度
         .bg_image = gImage_motor_speed,
         .bg_width = 160,
         .bg_height = 80,
         .regions = motor_speed,
         .region_count = 4},

};

/**** 全局管理器实例 ****/
ScreenManager screen_mgr;
