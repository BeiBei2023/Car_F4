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
//#include "cmsis_os.h"
//#include "adc_v.h"
//#include "elog.h"
/*********************
 *      DEFINES
 *********************/
// extern lv_ui guider_ui;       // 全局变量，用于存储界面元素
// float current_voltage = 0.0f; // 全局变量存储当前电压
//                               /**********************
//                                *      TYPEDEFS
//                                **********************/

// /**********************
//  *  STATIC PROTOTYPES
//  **********************/

// /**********************
//  *  STATIC VARIABLES
//  **********************/
/**
 * Create a demo application
 */
void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

// // 更新电压显示的函数
// void update_voltage_display(void)
// {
//     // 获取当前电压值
//     if (adc_v.v_in == 0.0f) // 如果电压值为0，可能是未初始化或读取失败
//     {
//         // log_d("ADC voltage value is zero, check ADC initialization.");
//         return; // 返回，避免显示错误的电压值
//     }

//     char voltage_str[16];
//     // 直接从全局变量获取最新ADC转换结果
//     current_voltage = adc_v.v_in; // 使用你的ADC结构体

//     // 格式化显示值 (保留2位小数)
//     snprintf(voltage_str, sizeof(voltage_str), "%.2f", current_voltage);

//     // 更新Label显示
//     if (guider_ui.Voltage_voltage_label != NULL) //
//     {
//         lv_label_set_text(guider_ui.Voltage_voltage_label, voltage_str);
//         log_d("Voltage updated: %s", voltage_str);
//     }
// }

// void ui_data_update(void *argument)
// {
//     for (;;)
//     {
//         if (lv_scr_act() == guider_ui.Voltage)
//         {

//             update_voltage_display(); // 更新电压显示
//         }

//         osDelay(500);
//     }
// }