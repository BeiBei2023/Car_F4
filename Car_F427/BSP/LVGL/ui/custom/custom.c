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
#include "wheel.h"
#include "QMI8658A.h"
#include "widgets_init.h"
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

// 时间改变的标志
uint8_t time_changed = 0;

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
    // 自动刷新到第一页的主页面
    time_changed = 1; // 可以改变时间
}

// 提取重复的界面更新逻辑为通用函数
static void update_label_text(lv_obj_t *label, const char *text)
{
    if (label != NULL)
    {
        lv_label_set_text(label, text);
    }
}

// 提取 LED 颜色更新逻辑为通用函数
static void update_led_color(lv_obj_t *led, uint8_t condition, lv_color_t color_true, lv_color_t color_false)
{
    lv_color_t color = condition ? color_true : color_false;
    change_led_color(led, color);
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
    // 更新界面上的电压标签
    update_label_text(guider_ui.Voltage_voltage_label, voltage_str);
}

// 温湿度显示的更新
void ui_humiture_update(void)
{
    static char humidity_str[16];

    // 更新湿度显示
    snprintf(humidity_str, sizeof(humidity_str), "%.2f", custom_aht20_data.Humidity);

    update_label_text(guider_ui.Humidity_humidity_label, humidity_str);
}

// 温度显示的更新
void ui_temperature_update(void)
{
    static char temperature_str[16];
    // 更新温度显示
    snprintf(temperature_str, sizeof(temperature_str), "%.2f", custom_aht20_data.Temperature);
    update_label_text(guider_ui.Temperature_temperature_label, temperature_str);
}

void change_led_color(lv_obj_t *led, lv_color_t color)
{
    lv_led_set_color(led, color); // 设置 LED 的颜色
}

// 定义全局变量存储 IO 状态
static uint8_t io_status[8] = {0}; // 存储 8 个 IO 口的状态

// 读取 IO 口状态并存储到全局变量
static void read_io_status(void)
{
    GPIO_TypeDef *io_port[8] = {OUT1_GPIO_Port, OUT2_GPIO_Port, OUT3_GPIO_Port, OUT4_GPIO_Port, OUT5_GPIO_Port, OUT6_GPIO_Port, OUT7_GPIO_Port, OUT8_GPIO_Port};
    uint16_t io_pin[8] = {OUT1_Pin, OUT2_Pin, OUT3_Pin, OUT4_Pin, OUT5_Pin, OUT6_Pin, OUT7_Pin, OUT8_Pin};

    for (int i = 0; i < 8; i++)
    {
        io_status[i] = HAL_GPIO_ReadPin(io_port[i], io_pin[i]); // 读取 IO 状态
    }
}

// 更新 LED 状态
void ui_led_update(void)
{
    lv_color_t red_color = lv_color_hex(0xFF0000);   // 红色
    lv_color_t green_color = lv_color_hex(0x00FF00); // 绿色

    lv_obj_t *led_objects[8] = {
        guider_ui.IO_State_led_1,
        guider_ui.IO_State_led_2,
        guider_ui.IO_State_led_3,
        guider_ui.IO_State_led_4,
        guider_ui.IO_State_led_5,
        guider_ui.IO_State_led_6,
        guider_ui.IO_State_led_7,
        guider_ui.IO_State_led_8};

    for (int i = 0; i < 8; i++)
    {
        lv_color_t led_color = (io_status[i] == GPIO_PIN_SET) ? green_color : red_color; // 根据状态设置颜色
        change_led_color(led_objects[i], led_color);                                     // 更新 LED 颜色
    }
}

// 将遥控器的数值绑定到界面元素上
void bind_remote_data(void)
{
    const lv_color_t green_color = lv_color_hex(0x00FF00); // 绿色
    const lv_color_t red_color = lv_color_hex(0xFF0000);   // 红色

    if (sbus_ch_data.flags == 0) // 遥控器已连接
    {
        // 更新连接指示灯为绿色
        change_led_color(guider_ui.SBUS_sbus_connect_led, green_color);

        // 更新使能指示灯颜色
        update_led_color(guider_ui.SBUS_sbus_work_led, sbus_ch_data.channels[5] == 1000, green_color, red_color);

        // 更新遥控器数据到界面
        char sbus_x_str[16], sbus_y_str[16], sbus_w_str[16];
        snprintf(sbus_x_str, sizeof(sbus_x_str), "%.2f", g_robot.x);
        snprintf(sbus_y_str, sizeof(sbus_y_str), "%.2f", g_robot.y);
        snprintf(sbus_w_str, sizeof(sbus_w_str), "%.2f", g_robot.w);

        update_label_text(guider_ui.SBUS_sbus_x_value, sbus_x_str);
        update_label_text(guider_ui.SBUS_sbus_y_value, sbus_y_str);
        update_label_text(guider_ui.SBUS_sbus_w_value, sbus_w_str);
    }
    else if (sbus_ch_data.flags == 4) // 遥控器未连接
    {

        // 更新连接指示灯为红色
        change_led_color(guider_ui.SBUS_sbus_connect_led, red_color);

        // 清空界面显示
        update_label_text(guider_ui.SBUS_sbus_x_value, "N/A");
        update_label_text(guider_ui.SBUS_sbus_y_value, "N/A");
        update_label_text(guider_ui.SBUS_sbus_w_value, "N/A");
    }
}

// 更新陀螺仪的显示数据
QMI8658A_Data_t lvgl_imu_data; // 用于存储陀螺仪数据
void update_gyroscope_data(void)
{
    QMI8658A_fetch_angleFromAcc(&lvgl_imu_data); // 从陀螺仪获取角度数据
    char imu_x_str[16], imu_y_str[16], imu_z_str[16];
    snprintf(imu_x_str, sizeof(imu_x_str), "%.2f", lvgl_imu_data.AngleX);
    snprintf(imu_y_str, sizeof(imu_y_str), "%.2f", lvgl_imu_data.AngleY);
    snprintf(imu_z_str, sizeof(imu_z_str), "%.2f", lvgl_imu_data.AngleZ);

    // 更新界面上的标签
    update_label_text(guider_ui.IMU_imu_x_value, imu_x_str);
    update_label_text(guider_ui.IMU_imu_y_value, imu_y_str);
    update_label_text(guider_ui.IMU_imu_z_value, imu_z_str);
}

extern int CLOCK_digital_clock_1_hour_value;
extern int CLOCK_digital_clock_1_min_value;
extern int CLOCK_digital_clock_1_sec_value;

void set_digital_clock_time(int hour, int min, int sec)
{

    CLOCK_digital_clock_1_hour_value = hour;
    CLOCK_digital_clock_1_min_value = min;
    CLOCK_digital_clock_1_sec_value = sec;

    CLOCK_digital_clock_1_timer(NULL); // 手动刷新显示
}

void ui_data_update(void *argument)
{
    lv_obj_t *current_screen = NULL; // 存储当前活动屏幕

    for (;;)
    {
        // 获取当前活动屏幕
        current_screen = lv_scr_act();

        // 根据当前屏幕执行对应的更新操作
        if (current_screen == guider_ui.Voltage)
        {
            update_voltage_display(); // 更新电压显示
        }
        else if (current_screen == guider_ui.Humidity)
        {
            ui_humiture_update(); // 更新湿度显示
        }
        else if (current_screen == guider_ui.Temperature)
        {
            ui_temperature_update(); // 更新温度显示
        }
        else if (current_screen == guider_ui.IO_State)
        {
            ui_led_update(); // 更新IO状态显示
        }
        else if (current_screen == guider_ui.SBUS)
        {
            bind_remote_data(); // 绑定遥控器数据到界面
        }
        else if (current_screen == guider_ui.IMU)
        {
            update_gyroscope_data(); // 更新陀螺仪数据
        }

        // 延迟以避免频繁更新
        osDelay(100);
    }
}
void sensor_data_update(void *argument)
{
    for (;;)
    {
        AHT20_Read(&custom_aht20_data); // 读取AHT20传感器数据
        read_io_status();               // 读取 IO 状态
        log_d("LVGL IS RUNNING...");
        if (time_changed == 1)
        {
            set_digital_clock_time(12, 0, 0); // 设置数字时钟时间为12:00:00
            time_changed = 0;                 // 重置时间改变标志
        }

        osDelay(1000); // 每秒更新一次
    }
}