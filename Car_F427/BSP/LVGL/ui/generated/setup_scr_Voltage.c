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



void setup_scr_Voltage(lv_ui *ui)
{
    //Write codes Voltage
    ui->Voltage = lv_obj_create(NULL);
    lv_obj_set_size(ui->Voltage, 160, 100);
    lv_obj_set_scrollbar_mode(ui->Voltage, LV_SCROLLBAR_MODE_OFF);

    //Write style for Voltage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Voltage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Voltage, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Voltage, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_btn_3
    ui->Voltage_btn_3 = lv_btn_create(ui->Voltage);
    ui->Voltage_btn_3_label = lv_label_create(ui->Voltage_btn_3);
    lv_label_set_text(ui->Voltage_btn_3_label, "N");
    lv_label_set_long_mode(ui->Voltage_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Voltage_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Voltage_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Voltage_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->Voltage_btn_3, 135, 1);
    lv_obj_set_size(ui->Voltage_btn_3, 22, 35);
    lv_obj_add_flag(ui->Voltage_btn_3, LV_OBJ_FLAG_CLICKABLE);

    //Write style for Voltage_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Voltage_btn_3, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Voltage_btn_3, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Voltage_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Voltage_btn_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Voltage_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Voltage_btn_3, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Voltage_btn_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_btn_3, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Voltage_btn_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Voltage_btn_3, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Voltage_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Voltage_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_btn_2
    ui->Voltage_btn_2 = lv_btn_create(ui->Voltage);
    ui->Voltage_btn_2_label = lv_label_create(ui->Voltage_btn_2);
    lv_label_set_text(ui->Voltage_btn_2_label, "B");
    lv_label_set_long_mode(ui->Voltage_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Voltage_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Voltage_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Voltage_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->Voltage_btn_2, 135, 38);
    lv_obj_set_size(ui->Voltage_btn_2, 22, 35);
    lv_obj_add_flag(ui->Voltage_btn_2, LV_OBJ_FLAG_CLICKABLE);

    //Write style for Voltage_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Voltage_btn_2, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Voltage_btn_2, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Voltage_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Voltage_btn_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Voltage_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Voltage_btn_2, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Voltage_btn_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_btn_2, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Voltage_btn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Voltage_btn_2, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Voltage_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Voltage_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_cont_2
    ui->Voltage_cont_2 = lv_obj_create(ui->Voltage);
    lv_obj_set_pos(ui->Voltage_cont_2, 0, 0);
    lv_obj_set_size(ui->Voltage_cont_2, 134, 80);
    lv_obj_set_scrollbar_mode(ui->Voltage_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for Voltage_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Voltage_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Voltage_cont_2, 186, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Voltage_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Voltage_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Voltage_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Voltage_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Voltage_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_label_1
    ui->Voltage_label_1 = lv_label_create(ui->Voltage);
    lv_label_set_text(ui->Voltage_label_1, "输入电压");
    lv_label_set_long_mode(ui->Voltage_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Voltage_label_1, 0, 0);
    lv_obj_set_size(ui->Voltage_label_1, 134, 29);

    //Write style for Voltage_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Voltage_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Voltage_label_1, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Voltage_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Voltage_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Voltage_label_1, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Voltage_label_1, lv_color_hex(0x00f5ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Voltage_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_voltage_label
    ui->Voltage_voltage_label = lv_label_create(ui->Voltage);
    lv_label_set_text(ui->Voltage_voltage_label, "0.00");
    lv_label_set_long_mode(ui->Voltage_voltage_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Voltage_voltage_label, 0, 32);
    lv_obj_set_size(ui->Voltage_voltage_label, 103, 42);

    //Write style for Voltage_voltage_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Voltage_voltage_label, lv_color_hex(0xffb900), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Voltage_voltage_label, &lv_font_yousheTittle_35, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Voltage_voltage_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Voltage_voltage_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Voltage_voltage_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Voltage_voltage_label, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Voltage_label_3
    ui->Voltage_label_3 = lv_label_create(ui->Voltage);
    lv_label_set_text(ui->Voltage_label_3, "V");
    lv_label_set_long_mode(ui->Voltage_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Voltage_label_3, 106, 32);
    lv_obj_set_size(ui->Voltage_label_3, 29, 44);

    //Write style for Voltage_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Voltage_label_3, lv_color_hex(0xffb900), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Voltage_label_3, &lv_font_yousheTittle_35, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Voltage_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Voltage_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Voltage_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Voltage_label_3, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Voltage_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Voltage.


    //Update current screen layout.
    lv_obj_update_layout(ui->Voltage);

    //Init events for screen.
    events_init_Voltage(ui);
}
