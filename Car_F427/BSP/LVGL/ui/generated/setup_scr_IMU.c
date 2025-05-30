/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_IMU(lv_ui *ui)
{
    //Write codes IMU
    ui->IMU = lv_obj_create(NULL);
    lv_obj_set_size(ui->IMU, 160, 100);
    lv_obj_set_scrollbar_mode(ui->IMU, LV_SCROLLBAR_MODE_OFF);

    //Write style for IMU, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->IMU, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IMU, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IMU, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_btn_11
    ui->IMU_btn_11 = lv_btn_create(ui->IMU);
    ui->IMU_btn_11_label = lv_label_create(ui->IMU_btn_11);
    lv_label_set_text(ui->IMU_btn_11_label, "N");
    lv_label_set_long_mode(ui->IMU_btn_11_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->IMU_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->IMU_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->IMU_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->IMU_btn_11, 135, 1);
    lv_obj_set_size(ui->IMU_btn_11, 22, 35);
    lv_obj_add_flag(ui->IMU_btn_11, LV_OBJ_FLAG_CLICKABLE);

    //Write style for IMU_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->IMU_btn_11, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IMU_btn_11, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IMU_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->IMU_btn_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IMU_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IMU_btn_11, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IMU_btn_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_btn_11, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_btn_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_btn_11, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_btn_10
    ui->IMU_btn_10 = lv_btn_create(ui->IMU);
    ui->IMU_btn_10_label = lv_label_create(ui->IMU_btn_10);
    lv_label_set_text(ui->IMU_btn_10_label, "B");
    lv_label_set_long_mode(ui->IMU_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->IMU_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->IMU_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->IMU_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->IMU_btn_10, 135, 38);
    lv_obj_set_size(ui->IMU_btn_10, 22, 35);
    lv_obj_add_flag(ui->IMU_btn_10, LV_OBJ_FLAG_CLICKABLE);

    //Write style for IMU_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_btn_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IMU_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IMU_btn_10, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IMU_btn_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_btn_10, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_btn_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_btn_10, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_btn_10, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IMU_btn_10, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IMU_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_cont_1
    ui->IMU_cont_1 = lv_obj_create(ui->IMU);
    lv_obj_set_pos(ui->IMU_cont_1, 0, 0);
    lv_obj_set_size(ui->IMU_cont_1, 134, 80);
    lv_obj_set_scrollbar_mode(ui->IMU_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for IMU_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IMU_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IMU_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IMU_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IMU_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IMU_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_label_1
    ui->IMU_label_1 = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_label_1, "IMU倾角");
    lv_label_set_long_mode(ui->IMU_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_label_1, 0, 0);
    lv_obj_set_size(ui->IMU_label_1, 134, 29);

    //Write style for IMU_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_label_1, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_label_1, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IMU_label_1, lv_color_hex(0x00f5ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IMU_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_label_2
    ui->IMU_label_2 = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_label_2, "X");
    lv_label_set_long_mode(ui->IMU_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_label_2, 5, 17);
    lv_obj_set_size(ui->IMU_label_2, 30, 35);

    //Write style for IMU_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_label_2, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_label_2, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_label_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_label_3
    ui->IMU_label_3 = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_label_3, "Y");
    lv_label_set_long_mode(ui->IMU_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_label_3, 50, 17);
    lv_obj_set_size(ui->IMU_label_3, 30, 35);

    //Write style for IMU_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_label_3, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_label_3, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_label_3, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_label_4
    ui->IMU_label_4 = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_label_4, "Z");
    lv_label_set_long_mode(ui->IMU_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_label_4, 95, 17);
    lv_obj_set_size(ui->IMU_label_4, 30, 35);

    //Write style for IMU_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_label_4, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_label_4, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_label_4, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_imu_x_value
    ui->IMU_imu_x_value = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_imu_x_value, "00.00");
    lv_label_set_long_mode(ui->IMU_imu_x_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_imu_x_value, 0, 61);
    lv_obj_set_size(ui->IMU_imu_x_value, 42, 12);

    //Write style for IMU_imu_x_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_imu_x_value, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_imu_x_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_imu_x_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_imu_x_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_imu_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_imu_y_value
    ui->IMU_imu_y_value = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_imu_y_value, "00.00");
    lv_label_set_long_mode(ui->IMU_imu_y_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_imu_y_value, 43, 61);
    lv_obj_set_size(ui->IMU_imu_y_value, 42, 12);

    //Write style for IMU_imu_y_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_imu_y_value, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_imu_y_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_imu_y_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_imu_y_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_imu_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IMU_imu_z_value
    ui->IMU_imu_z_value = lv_label_create(ui->IMU);
    lv_label_set_text(ui->IMU_imu_z_value, "00.00");
    lv_label_set_long_mode(ui->IMU_imu_z_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IMU_imu_z_value, 86, 61);
    lv_obj_set_size(ui->IMU_imu_z_value, 42, 12);

    //Write style for IMU_imu_z_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IMU_imu_z_value, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IMU_imu_z_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IMU_imu_z_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IMU_imu_z_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IMU_imu_z_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of IMU.


    //Update current screen layout.
    lv_obj_update_layout(ui->IMU);

    //Init events for screen.
    events_init_IMU(ui);
}
