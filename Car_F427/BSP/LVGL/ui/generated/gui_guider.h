/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *main;
	bool main_del;
	lv_obj_t *main_img_1;
	lv_obj_t *main_btn_1;
	lv_obj_t *main_btn_1_label;
	lv_obj_t *Voltage;
	bool Voltage_del;
	lv_obj_t *Voltage_btn_3;
	lv_obj_t *Voltage_btn_3_label;
	lv_obj_t *Voltage_btn_2;
	lv_obj_t *Voltage_btn_2_label;
	lv_obj_t *Voltage_cont_2;
	lv_obj_t *Voltage_label_1;
	lv_obj_t *Voltage_voltage_label;
	lv_obj_t *Voltage_label_3;
	lv_obj_t *Temperature;
	bool Temperature_del;
	lv_obj_t *Temperature_btn_5;
	lv_obj_t *Temperature_btn_5_label;
	lv_obj_t *Temperature_btn_4;
	lv_obj_t *Temperature_btn_4_label;
	lv_obj_t *Temperature_cont_2;
	lv_obj_t *Temperature_label_1;
	lv_obj_t *Temperature_temperature_label;
	lv_obj_t *Humidity;
	bool Humidity_del;
	lv_obj_t *Humidity_btn_7;
	lv_obj_t *Humidity_btn_7_label;
	lv_obj_t *Humidity_btn_6;
	lv_obj_t *Humidity_btn_6_label;
	lv_obj_t *Humidity_cont_2;
	lv_obj_t *Humidity_label_1;
	lv_obj_t *Humidity_humidity_label;
	lv_obj_t *IO_State;
	bool IO_State_del;
	lv_obj_t *IO_State_btn_9;
	lv_obj_t *IO_State_btn_9_label;
	lv_obj_t *IO_State_btn_8;
	lv_obj_t *IO_State_btn_8_label;
	lv_obj_t *IO_State_cont_2;
	lv_obj_t *IO_State_led_1;
	lv_obj_t *IO_State_led_2;
	lv_obj_t *IO_State_led_3;
	lv_obj_t *IO_State_led_4;
	lv_obj_t *IO_State_led_8;
	lv_obj_t *IO_State_led_7;
	lv_obj_t *IO_State_led_6;
	lv_obj_t *IO_State_led_5;
	lv_obj_t *IO_State_label_1;
	lv_obj_t *IO_State_label_2;
	lv_obj_t *IO_State_label_3;
	lv_obj_t *IO_State_label_4;
	lv_obj_t *IO_State_label_5;
	lv_obj_t *IO_State_label_6;
	lv_obj_t *IO_State_label_7;
	lv_obj_t *IO_State_label_8;
	lv_obj_t *SBUS;
	bool SBUS_del;
	lv_obj_t *SBUS_btn_11;
	lv_obj_t *SBUS_btn_11_label;
	lv_obj_t *SBUS_btn_10;
	lv_obj_t *SBUS_btn_10_label;
	lv_obj_t *SBUS_cont_1;
	lv_obj_t *SBUS_label_1;
	lv_obj_t *SBUS_label_2;
	lv_obj_t *SBUS_label_3;
	lv_obj_t *SBUS_label_4;
	lv_obj_t *SBUS_sbus_x_value;
	lv_obj_t *SBUS_sbus_connect_led;
	lv_obj_t *SBUS_sbus_y_value;
	lv_obj_t *SBUS_sbus_w_value;
	lv_obj_t *SBUS_sbus_work_led;
	lv_obj_t *SBUS_label_5;
	lv_obj_t *SBUS_label_6;
	lv_obj_t *IMU;
	bool IMU_del;
	lv_obj_t *IMU_btn_11;
	lv_obj_t *IMU_btn_11_label;
	lv_obj_t *IMU_btn_10;
	lv_obj_t *IMU_btn_10_label;
	lv_obj_t *IMU_cont_1;
	lv_obj_t *IMU_label_1;
	lv_obj_t *IMU_label_2;
	lv_obj_t *IMU_label_3;
	lv_obj_t *IMU_label_4;
	lv_obj_t *IMU_imu_x_value;
	lv_obj_t *IMU_imu_y_value;
	lv_obj_t *IMU_imu_z_value;
	lv_obj_t *CLOCK;
	bool CLOCK_del;
	lv_obj_t *CLOCK_btn_11;
	lv_obj_t *CLOCK_btn_11_label;
	lv_obj_t *CLOCK_btn_10;
	lv_obj_t *CLOCK_btn_10_label;
	lv_obj_t *CLOCK_cont_1;
	lv_obj_t *CLOCK_label_1;
	lv_obj_t *CLOCK_digital_clock_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_main(lv_ui *ui);
void setup_scr_Voltage(lv_ui *ui);
void setup_scr_Temperature(lv_ui *ui);
void setup_scr_Humidity(lv_ui *ui);
void setup_scr_IO_State(lv_ui *ui);
void setup_scr_SBUS(lv_ui *ui);
void setup_scr_IMU(lv_ui *ui);
void setup_scr_CLOCK(lv_ui *ui);
LV_IMG_DECLARE(_555_alpha_160x80);

LV_FONT_DECLARE(lv_font_yousheTittle_30)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_yousheTittle_35)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_11)
LV_FONT_DECLARE(lv_font_yousheTittle_16)
LV_FONT_DECLARE(lv_font_yousheTittle_13)
LV_FONT_DECLARE(lv_font_yousheTittle_22)


#ifdef __cplusplus
}
#endif
#endif
