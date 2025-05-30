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



void setup_scr_SBUS(lv_ui *ui)
{
    //Write codes SBUS
    ui->SBUS = lv_obj_create(NULL);
    lv_obj_set_size(ui->SBUS, 160, 100);
    lv_obj_set_scrollbar_mode(ui->SBUS, LV_SCROLLBAR_MODE_OFF);

    //Write style for SBUS, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->SBUS, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->SBUS, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->SBUS, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_btn_11
    ui->SBUS_btn_11 = lv_btn_create(ui->SBUS);
    ui->SBUS_btn_11_label = lv_label_create(ui->SBUS_btn_11);
    lv_label_set_text(ui->SBUS_btn_11_label, "N");
    lv_label_set_long_mode(ui->SBUS_btn_11_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->SBUS_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->SBUS_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->SBUS_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->SBUS_btn_11, 135, 1);
    lv_obj_set_size(ui->SBUS_btn_11, 22, 35);
    lv_obj_add_flag(ui->SBUS_btn_11, LV_OBJ_FLAG_CLICKABLE);

    //Write style for SBUS_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->SBUS_btn_11, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->SBUS_btn_11, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->SBUS_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->SBUS_btn_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->SBUS_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->SBUS_btn_11, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->SBUS_btn_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_btn_11, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_btn_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_btn_11, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_btn_10
    ui->SBUS_btn_10 = lv_btn_create(ui->SBUS);
    ui->SBUS_btn_10_label = lv_label_create(ui->SBUS_btn_10);
    lv_label_set_text(ui->SBUS_btn_10_label, "B");
    lv_label_set_long_mode(ui->SBUS_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->SBUS_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->SBUS_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->SBUS_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->SBUS_btn_10, 135, 38);
    lv_obj_set_size(ui->SBUS_btn_10, 22, 35);
    lv_obj_add_flag(ui->SBUS_btn_10, LV_OBJ_FLAG_CLICKABLE);

    //Write style for SBUS_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_btn_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->SBUS_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->SBUS_btn_10, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->SBUS_btn_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_btn_10, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_btn_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_btn_10, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_btn_10, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->SBUS_btn_10, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->SBUS_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_cont_1
    ui->SBUS_cont_1 = lv_obj_create(ui->SBUS);
    lv_obj_set_pos(ui->SBUS_cont_1, 0, 0);
    lv_obj_set_size(ui->SBUS_cont_1, 134, 80);
    lv_obj_set_scrollbar_mode(ui->SBUS_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for SBUS_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->SBUS_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->SBUS_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->SBUS_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->SBUS_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->SBUS_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_label_1
    ui->SBUS_label_1 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_1, "S.BUS");
    lv_label_set_long_mode(ui->SBUS_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_1, 0, 0);
    lv_obj_set_size(ui->SBUS_label_1, 134, 29);

    //Write style for SBUS_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_1, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_1, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->SBUS_label_1, lv_color_hex(0x00f5ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->SBUS_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_label_2
    ui->SBUS_label_2 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_2, "X");
    lv_label_set_long_mode(ui->SBUS_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_2, 5, 17);
    lv_obj_set_size(ui->SBUS_label_2, 30, 35);

    //Write style for SBUS_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_2, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_2, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_label_3
    ui->SBUS_label_3 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_3, "Y");
    lv_label_set_long_mode(ui->SBUS_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_3, 50, 17);
    lv_obj_set_size(ui->SBUS_label_3, 30, 35);

    //Write style for SBUS_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_3, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_3, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_3, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_label_4
    ui->SBUS_label_4 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_4, "W");
    lv_label_set_long_mode(ui->SBUS_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_4, 95, 17);
    lv_obj_set_size(ui->SBUS_label_4, 30, 35);

    //Write style for SBUS_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_4, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_4, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_4, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_sbus_x_value
    ui->SBUS_sbus_x_value = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_sbus_x_value, "00.00");
    lv_label_set_long_mode(ui->SBUS_sbus_x_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_sbus_x_value, 0, 61);
    lv_obj_set_size(ui->SBUS_sbus_x_value, 42, 12);

    //Write style for SBUS_sbus_x_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_sbus_x_value, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_sbus_x_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_sbus_x_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_sbus_x_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_sbus_x_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_sbus_connect_led
    ui->SBUS_sbus_connect_led = lv_led_create(ui->SBUS);
    lv_led_set_brightness(ui->SBUS_sbus_connect_led, 255);
    lv_led_set_color(ui->SBUS_sbus_connect_led, lv_color_hex(0xff0027));
    lv_obj_set_pos(ui->SBUS_sbus_connect_led, 5, 9);
    lv_obj_set_size(ui->SBUS_sbus_connect_led, 10, 10);

    //Write codes SBUS_sbus_y_value
    ui->SBUS_sbus_y_value = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_sbus_y_value, "00.00");
    lv_label_set_long_mode(ui->SBUS_sbus_y_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_sbus_y_value, 43, 61);
    lv_obj_set_size(ui->SBUS_sbus_y_value, 42, 12);

    //Write style for SBUS_sbus_y_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_sbus_y_value, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_sbus_y_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_sbus_y_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_sbus_y_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_sbus_y_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_sbus_w_value
    ui->SBUS_sbus_w_value = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_sbus_w_value, "00.00");
    lv_label_set_long_mode(ui->SBUS_sbus_w_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_sbus_w_value, 86, 61);
    lv_obj_set_size(ui->SBUS_sbus_w_value, 42, 12);

    //Write style for SBUS_sbus_w_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_sbus_w_value, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_sbus_w_value, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_sbus_w_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_sbus_w_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_sbus_w_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_sbus_work_led
    ui->SBUS_sbus_work_led = lv_led_create(ui->SBUS);
    lv_led_set_brightness(ui->SBUS_sbus_work_led, 255);
    lv_led_set_color(ui->SBUS_sbus_work_led, lv_color_hex(0xff0027));
    lv_obj_set_pos(ui->SBUS_sbus_work_led, 119, 9);
    lv_obj_set_size(ui->SBUS_sbus_work_led, 10, 10);

    //Write codes SBUS_label_5
    ui->SBUS_label_5 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_5, "W");
    lv_label_set_long_mode(ui->SBUS_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_5, 119, 9);
    lv_obj_set_size(ui->SBUS_label_5, 11, 11);

    //Write style for SBUS_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_5, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes SBUS_label_6
    ui->SBUS_label_6 = lv_label_create(ui->SBUS);
    lv_label_set_text(ui->SBUS_label_6, "L");
    lv_label_set_long_mode(ui->SBUS_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->SBUS_label_6, 5, 9);
    lv_obj_set_size(ui->SBUS_label_6, 11, 11);

    //Write style for SBUS_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->SBUS_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->SBUS_label_6, &lv_font_yousheTittle_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->SBUS_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->SBUS_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->SBUS_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of SBUS.


    //Update current screen layout.
    lv_obj_update_layout(ui->SBUS);

    //Init events for screen.
    events_init_SBUS(ui);
}
