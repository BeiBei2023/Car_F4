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



void setup_scr_Humidity(lv_ui *ui)
{
    //Write codes Humidity
    ui->Humidity = lv_obj_create(NULL);
    lv_obj_set_size(ui->Humidity, 160, 100);
    lv_obj_set_scrollbar_mode(ui->Humidity, LV_SCROLLBAR_MODE_OFF);

    //Write style for Humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Humidity, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Humidity, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Humidity_btn_7
    ui->Humidity_btn_7 = lv_btn_create(ui->Humidity);
    ui->Humidity_btn_7_label = lv_label_create(ui->Humidity_btn_7);
    lv_label_set_text(ui->Humidity_btn_7_label, "N");
    lv_label_set_long_mode(ui->Humidity_btn_7_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Humidity_btn_7_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Humidity_btn_7, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Humidity_btn_7_label, LV_PCT(100));
    lv_obj_set_pos(ui->Humidity_btn_7, 135, 1);
    lv_obj_set_size(ui->Humidity_btn_7, 22, 35);
    lv_obj_add_flag(ui->Humidity_btn_7, LV_OBJ_FLAG_CLICKABLE);

    //Write style for Humidity_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Humidity_btn_7, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Humidity_btn_7, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Humidity_btn_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Humidity_btn_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Humidity_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Humidity_btn_7, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Humidity_btn_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Humidity_btn_7, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Humidity_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Humidity_btn_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Humidity_btn_7, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Humidity_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Humidity_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Humidity_btn_6
    ui->Humidity_btn_6 = lv_btn_create(ui->Humidity);
    ui->Humidity_btn_6_label = lv_label_create(ui->Humidity_btn_6);
    lv_label_set_text(ui->Humidity_btn_6_label, "B");
    lv_label_set_long_mode(ui->Humidity_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Humidity_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Humidity_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Humidity_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->Humidity_btn_6, 135, 38);
    lv_obj_set_size(ui->Humidity_btn_6, 22, 35);
    lv_obj_add_flag(ui->Humidity_btn_6, LV_OBJ_FLAG_CLICKABLE);

    //Write style for Humidity_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Humidity_btn_6, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Humidity_btn_6, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Humidity_btn_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Humidity_btn_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Humidity_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Humidity_btn_6, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Humidity_btn_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Humidity_btn_6, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Humidity_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Humidity_btn_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Humidity_btn_6, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Humidity_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Humidity_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Humidity_cont_2
    ui->Humidity_cont_2 = lv_obj_create(ui->Humidity);
    lv_obj_set_pos(ui->Humidity_cont_2, 0, 0);
    lv_obj_set_size(ui->Humidity_cont_2, 134, 80);
    lv_obj_set_scrollbar_mode(ui->Humidity_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for Humidity_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Humidity_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Humidity_cont_2, 186, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Humidity_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Humidity_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Humidity_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Humidity_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Humidity_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Humidity_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Humidity_label_1
    ui->Humidity_label_1 = lv_label_create(ui->Humidity);
    lv_label_set_text(ui->Humidity_label_1, "内部湿度");
    lv_label_set_long_mode(ui->Humidity_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Humidity_label_1, 0, 0);
    lv_obj_set_size(ui->Humidity_label_1, 134, 29);

    //Write style for Humidity_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Humidity_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Humidity_label_1, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Humidity_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Humidity_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Humidity_label_1, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Humidity_label_1, lv_color_hex(0x00f5ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Humidity_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Humidity_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Humidity_humidity_label
    ui->Humidity_humidity_label = lv_label_create(ui->Humidity);
    lv_label_set_text(ui->Humidity_humidity_label, "00.00");
    lv_label_set_long_mode(ui->Humidity_humidity_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Humidity_humidity_label, 3, 33);
    lv_obj_set_size(ui->Humidity_humidity_label, 132, 42);

    //Write style for Humidity_humidity_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Humidity_humidity_label, lv_color_hex(0x7cff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Humidity_humidity_label, &lv_font_yousheTittle_35, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Humidity_humidity_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Humidity_humidity_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Humidity_humidity_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Humidity_humidity_label, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Humidity_humidity_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Humidity.


    //Update current screen layout.
    lv_obj_update_layout(ui->Humidity);

    //Init events for screen.
    events_init_Humidity(ui);
}
