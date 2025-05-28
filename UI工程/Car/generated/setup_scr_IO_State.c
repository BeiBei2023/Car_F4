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



void setup_scr_IO_State(lv_ui *ui)
{
    //Write codes IO_State
    ui->IO_State = lv_obj_create(NULL);
    lv_obj_set_size(ui->IO_State, 160, 100);
    lv_obj_set_scrollbar_mode(ui->IO_State, LV_SCROLLBAR_MODE_OFF);

    //Write style for IO_State, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->IO_State, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_cont_1
    ui->IO_State_cont_1 = lv_obj_create(ui->IO_State);
    lv_obj_set_pos(ui->IO_State_cont_1, 0, 0);
    lv_obj_set_size(ui->IO_State_cont_1, 160, 80);
    lv_obj_set_scrollbar_mode(ui->IO_State_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for IO_State_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IO_State_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IO_State_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IO_State_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IO_State_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IO_State_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_btn_9
    ui->IO_State_btn_9 = lv_btn_create(ui->IO_State_cont_1);
    ui->IO_State_btn_9_label = lv_label_create(ui->IO_State_btn_9);
    lv_label_set_text(ui->IO_State_btn_9_label, "N");
    lv_label_set_long_mode(ui->IO_State_btn_9_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->IO_State_btn_9_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->IO_State_btn_9, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->IO_State_btn_9_label, LV_PCT(100));
    lv_obj_set_pos(ui->IO_State_btn_9, 135, 1);
    lv_obj_set_size(ui->IO_State_btn_9, 22, 35);
    lv_obj_add_flag(ui->IO_State_btn_9, LV_OBJ_FLAG_CLICKABLE);

    //Write style for IO_State_btn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->IO_State_btn_9, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IO_State_btn_9, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IO_State_btn_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->IO_State_btn_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IO_State_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IO_State_btn_9, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IO_State_btn_9, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_btn_9, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_btn_9, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_btn_9, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_btn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_btn_8
    ui->IO_State_btn_8 = lv_btn_create(ui->IO_State_cont_1);
    ui->IO_State_btn_8_label = lv_label_create(ui->IO_State_btn_8);
    lv_label_set_text(ui->IO_State_btn_8_label, "B");
    lv_label_set_long_mode(ui->IO_State_btn_8_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->IO_State_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->IO_State_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->IO_State_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->IO_State_btn_8, 135, 38);
    lv_obj_set_size(ui->IO_State_btn_8, 22, 35);
    lv_obj_add_flag(ui->IO_State_btn_8, LV_OBJ_FLAG_CLICKABLE);

    //Write style for IO_State_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->IO_State_btn_8, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IO_State_btn_8, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IO_State_btn_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->IO_State_btn_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IO_State_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IO_State_btn_8, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IO_State_btn_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_btn_8, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_btn_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_btn_8, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_cont_2
    ui->IO_State_cont_2 = lv_obj_create(ui->IO_State);
    lv_obj_set_pos(ui->IO_State_cont_2, 0, 0);
    lv_obj_set_size(ui->IO_State_cont_2, 134, 80);
    lv_obj_set_scrollbar_mode(ui->IO_State_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for IO_State_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->IO_State_cont_2, 186, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->IO_State_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->IO_State_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->IO_State_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->IO_State_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_led_1
    ui->IO_State_led_1 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_1, 255);
    lv_led_set_color(ui->IO_State_led_1, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_1, 7, 9);
    lv_obj_set_size(ui->IO_State_led_1, 20, 20);

    //Write codes IO_State_led_2
    ui->IO_State_led_2 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_2, 255);
    lv_led_set_color(ui->IO_State_led_2, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_2, 38, 9);
    lv_obj_set_size(ui->IO_State_led_2, 20, 20);

    //Write codes IO_State_led_3
    ui->IO_State_led_3 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_3, 255);
    lv_led_set_color(ui->IO_State_led_3, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_3, 69, 9);
    lv_obj_set_size(ui->IO_State_led_3, 20, 20);

    //Write codes IO_State_led_4
    ui->IO_State_led_4 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_4, 255);
    lv_led_set_color(ui->IO_State_led_4, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_4, 100, 9);
    lv_obj_set_size(ui->IO_State_led_4, 20, 20);

    //Write codes IO_State_led_8
    ui->IO_State_led_8 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_8, 255);
    lv_led_set_color(ui->IO_State_led_8, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_8, 7, 46);
    lv_obj_set_size(ui->IO_State_led_8, 20, 20);

    //Write codes IO_State_led_7
    ui->IO_State_led_7 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_7, 255);
    lv_led_set_color(ui->IO_State_led_7, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_7, 69, 46);
    lv_obj_set_size(ui->IO_State_led_7, 20, 20);

    //Write codes IO_State_led_6
    ui->IO_State_led_6 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_6, 255);
    lv_led_set_color(ui->IO_State_led_6, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_6, 38, 46);
    lv_obj_set_size(ui->IO_State_led_6, 20, 20);

    //Write codes IO_State_led_5
    ui->IO_State_led_5 = lv_led_create(ui->IO_State);
    lv_led_set_brightness(ui->IO_State_led_5, 255);
    lv_led_set_color(ui->IO_State_led_5, lv_color_hex(0x00a1b5));
    lv_obj_set_pos(ui->IO_State_led_5, 100, 46);
    lv_obj_set_size(ui->IO_State_led_5, 20, 20);

    //Write codes IO_State_label_1
    ui->IO_State_label_1 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_1, "1");
    lv_label_set_long_mode(ui->IO_State_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_1, 7, 11);
    lv_obj_set_size(ui->IO_State_label_1, 17, 14);

    //Write style for IO_State_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_1, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_2
    ui->IO_State_label_2 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_2, "2");
    lv_label_set_long_mode(ui->IO_State_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_2, 38, 11);
    lv_obj_set_size(ui->IO_State_label_2, 17, 14);

    //Write style for IO_State_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_2, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_3
    ui->IO_State_label_3 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_3, "3");
    lv_label_set_long_mode(ui->IO_State_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_3, 69, 11);
    lv_obj_set_size(ui->IO_State_label_3, 17, 14);

    //Write style for IO_State_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_3, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_4
    ui->IO_State_label_4 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_4, "4");
    lv_label_set_long_mode(ui->IO_State_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_4, 100, 11);
    lv_obj_set_size(ui->IO_State_label_4, 17, 14);

    //Write style for IO_State_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_4, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_5
    ui->IO_State_label_5 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_5, "5");
    lv_label_set_long_mode(ui->IO_State_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_5, 7, 49);
    lv_obj_set_size(ui->IO_State_label_5, 17, 14);

    //Write style for IO_State_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_5, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_6
    ui->IO_State_label_6 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_6, "6");
    lv_label_set_long_mode(ui->IO_State_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_6, 38, 49);
    lv_obj_set_size(ui->IO_State_label_6, 17, 14);

    //Write style for IO_State_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_6, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_7
    ui->IO_State_label_7 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_7, "7");
    lv_label_set_long_mode(ui->IO_State_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_7, 69, 49);
    lv_obj_set_size(ui->IO_State_label_7, 17, 14);

    //Write style for IO_State_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_7, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes IO_State_label_8
    ui->IO_State_label_8 = lv_label_create(ui->IO_State);
    lv_label_set_text(ui->IO_State_label_8, "9");
    lv_label_set_long_mode(ui->IO_State_label_8, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->IO_State_label_8, 100, 49);
    lv_obj_set_size(ui->IO_State_label_8, 17, 14);

    //Write style for IO_State_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->IO_State_label_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->IO_State_label_8, &lv_font_yousheTittle_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->IO_State_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->IO_State_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->IO_State_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of IO_State.


    //Update current screen layout.
    lv_obj_update_layout(ui->IO_State);

    //Init events for screen.
    events_init_IO_State(ui);
}
