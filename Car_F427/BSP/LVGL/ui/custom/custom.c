/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "cmsis_os.h"
#include "adc_v.h"
#include "elog.h"
#include "aht20.h"
#include "main.h"
/*********************
 *      DEFINES
 *********************/
extern lv_ui guider_ui;       // 全局变量，用于存储界面元素
float current_voltage = 0.0f; // 全局变量存储当前电压
                              /**********************
                               *      TYPEDEFS
                               **********************/
// 温度湿度
AHT20_Data_t custom_aht20_data; // 用于存储AHT20传感器数据

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
/**
 * Create a demo application
 */
void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

// 更新电压显示的函数
void update_voltage_display(void)
{
    // 获取当前电压值
    if (adc_v.v_in == 0.0f) // 如果电压值为0，可能是未初始化或读取失败
    {
        // log_d("ADC voltage value is zero, check ADC initialization.");
        return; // 返回，避免显示错误的电压值
    }

    char voltage_str[16];
    // 直接从全局变量获取最新ADC转换结果
    current_voltage = adc_v.v_in; // 使用你的ADC结构体

    // 格式化显示值 (保留2位小数)
    snprintf(voltage_str, sizeof(voltage_str), "%.2f", current_voltage);

    // 更新Label显示
    if (guider_ui.Voltage_voltage_label != NULL) //
    {
        lv_label_set_text(guider_ui.Voltage_voltage_label, voltage_str);
        // log_d("Voltage updated: %s", voltage_str);
    }
}

// 温湿度显示的更新
void ui_humiture_update(void)
{
    static char humidity_str[16];

    // 更新湿度显示
    snprintf(humidity_str, sizeof(humidity_str), "%.2f", custom_aht20_data.Humidity);

    if (guider_ui.Humidity_humidity_label != NULL)
    {
        lv_label_set_text(guider_ui.Humidity_humidity_label, humidity_str);
        // log_d("Humidity updated: %s", humidity_str);
    }
}

// 温度显示的更新
void ui_temperature_update(void)
{
    static char temperature_str[16];
    // 更新温度显示
    snprintf(temperature_str, sizeof(temperature_str), "%.2f", custom_aht20_data.Temperature);

    if (guider_ui.Temperature_temperature_label != NULL)
    {
        lv_label_set_text(guider_ui.Temperature_temperature_label, temperature_str);
        // log_d("Temperature updated: %s", temperature_str);
    }
}

void change_led_color(lv_obj_t *led, lv_color_t color)
{
    lv_led_set_color(led, color); // 设置 LED 的颜色
}

// 判断8个IO口的状态，改变LED颜色，高电平为红色，低电平为绿色
void ui_led_update(void)
{
    // 创建一个数组，存放IO口的状态
    uint8_t io_status[8];
    // 创建一个数组，存放LED的颜色
    lv_color_t led_color[8];
    // 定义颜色
    lv_color_t red_color = lv_color_hex(0xFF0000);   // 红色
    lv_color_t green_color = lv_color_hex(0x00FF00); // 绿色

    // 初始化数组
    for (int i = 0; i < 8; i++)
    {
        io_status[i] = 0;           // 默认IO口状态为0
        led_color[i] = green_color; // 默认颜色为绿色
    }

    // 创建一个列表，存放IO口对应的端口
    GPIO_TypeDef* io_port[8] = {OUT1_GPIO_Port, OUT2_GPIO_Port, OUT3_GPIO_Port, OUT4_GPIO_Port, OUT5_GPIO_Port, OUT6_GPIO_Port, OUT7_GPIO_Port, OUT8_GPIO_Port};

    // 创建一个列表，存放IO口对应的引脚
    uint16_t io_pin[8] = {OUT1_Pin, OUT2_Pin, OUT3_Pin, OUT4_Pin, OUT5_Pin, OUT6_Pin, OUT7_Pin, OUT8_Pin};
    // 遍历8个IO口
    for (int i = 0; i < 8; i++)
    {
        // 读取IO口状态
        io_status[i] = HAL_GPIO_ReadPin(io_port[i], io_pin[i]);

        // 根据IO口状态设置LED颜色
        if (io_status[i] == GPIO_PIN_SET) // 高电平
        {
            led_color[i] = green_color; // 设置为红色
        }
        else // 低电平
        {
            led_color[i] = red_color; // 设置为绿色
        }
    }

    // 更新LED颜色
    change_led_color(guider_ui.IO_State_led_1, led_color[0]);
    change_led_color(guider_ui.IO_State_led_2, led_color[1]);
    change_led_color(guider_ui.IO_State_led_3, led_color[2]);
    change_led_color(guider_ui.IO_State_led_4, led_color[3]);
    change_led_color(guider_ui.IO_State_led_5, led_color[4]);
    change_led_color(guider_ui.IO_State_led_6, led_color[5]);
    change_led_color(guider_ui.IO_State_led_7, led_color[6]);
    change_led_color(guider_ui.IO_State_led_8, led_color[7]);


}

void ui_data_update(void *argument)
{
    for (;;)
    {

        // 判断是否在这个界面
        if (lv_scr_act() == guider_ui.Voltage)
        {

            update_voltage_display(); // 更新电压显示
        }
        if (lv_scr_act() == guider_ui.Humidity)
        {

            ui_humiture_update(); // 更新湿度显示
        }
        if (lv_scr_act() == guider_ui.Temperature)
        {

            ui_temperature_update(); // 更新温度显示
        }
        if (lv_scr_act() == guider_ui.IO_State)
        {

            ui_led_update(); // 更新IO状态显示
        }

        osDelay(500);
    }
}

void sensor_data_update(void *argument)
{
    for (;;)
    {
        AHT20_Read(&custom_aht20_data); // 读取AHT20传感器数据
        osDelay(2000);                  // 每秒更新一次
    }
}