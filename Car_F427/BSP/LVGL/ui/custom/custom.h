/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "gui_guider.h"

    extern float current_voltage; // 当前电压值（全局可访问）

    void custom_init(lv_ui *ui);
    void update_voltage_display(void);                      // 更新电压显示的函数
    void ui_humiture_update(void);                          // 湿度显示的更新
    void ui_temperature_update(void);                       // 温度显示的更新
    void change_led_color(lv_obj_t *led, lv_color_t color); // 改变LED颜色的函数
    void ui_led_update(void);                               // 更新LED状态显示的函数
    void bind_remote_data(void);                            // 绑定远程数据
    void update_gyroscope_data(void);                       // 更新陀螺仪数据的函数
    void ui_data_update(void *argument);                    // 更新传感器数据
    void sensor_data_update(void *argument);                // 传感器数据更新
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
