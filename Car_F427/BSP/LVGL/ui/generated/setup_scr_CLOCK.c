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



int CLOCK_digital_clock_1_min_value = 0;
int CLOCK_digital_clock_1_hour_value = 0;
int CLOCK_digital_clock_1_sec_value = 0;
void setup_scr_CLOCK(lv_ui *ui)
{
    //Write codes CLOCK
    ui->CLOCK = lv_obj_create(NULL);
    lv_obj_set_size(ui->CLOCK, 160, 100);
    lv_obj_set_scrollbar_mode(ui->CLOCK, LV_SCROLLBAR_MODE_OFF);

    //Write style for CLOCK, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->CLOCK, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes CLOCK_btn_11
    ui->CLOCK_btn_11 = lv_btn_create(ui->CLOCK);
    ui->CLOCK_btn_11_label = lv_label_create(ui->CLOCK_btn_11);
    lv_label_set_text(ui->CLOCK_btn_11_label, "N");
    lv_label_set_long_mode(ui->CLOCK_btn_11_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->CLOCK_btn_11_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->CLOCK_btn_11, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->CLOCK_btn_11_label, LV_PCT(100));
    lv_obj_set_pos(ui->CLOCK_btn_11, 135, 1);
    lv_obj_set_size(ui->CLOCK_btn_11, 22, 35);
    lv_obj_add_flag(ui->CLOCK_btn_11, LV_OBJ_FLAG_CLICKABLE);

    //Write style for CLOCK_btn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->CLOCK_btn_11, 155, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK_btn_11, lv_color_hex(0x959595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK_btn_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->CLOCK_btn_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->CLOCK_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->CLOCK_btn_11, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->CLOCK_btn_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->CLOCK_btn_11, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->CLOCK_btn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->CLOCK_btn_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->CLOCK_btn_11, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->CLOCK_btn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->CLOCK_btn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes CLOCK_btn_10
    ui->CLOCK_btn_10 = lv_btn_create(ui->CLOCK);
    ui->CLOCK_btn_10_label = lv_label_create(ui->CLOCK_btn_10);
    lv_label_set_text(ui->CLOCK_btn_10_label, "B");
    lv_label_set_long_mode(ui->CLOCK_btn_10_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->CLOCK_btn_10_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->CLOCK_btn_10, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->CLOCK_btn_10_label, LV_PCT(100));
    lv_obj_set_pos(ui->CLOCK_btn_10, 135, 38);
    lv_obj_set_size(ui->CLOCK_btn_10, 22, 35);
    lv_obj_add_flag(ui->CLOCK_btn_10, LV_OBJ_FLAG_CLICKABLE);

    //Write style for CLOCK_btn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->CLOCK_btn_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->CLOCK_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->CLOCK_btn_10, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->CLOCK_btn_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->CLOCK_btn_10, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->CLOCK_btn_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->CLOCK_btn_10, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->CLOCK_btn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->CLOCK_btn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->CLOCK_btn_10, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK_btn_10, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK_btn_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->CLOCK_btn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes CLOCK_cont_1
    ui->CLOCK_cont_1 = lv_obj_create(ui->CLOCK);
    lv_obj_set_pos(ui->CLOCK_cont_1, 0, 0);
    lv_obj_set_size(ui->CLOCK_cont_1, 134, 80);
    lv_obj_set_scrollbar_mode(ui->CLOCK_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for CLOCK_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->CLOCK_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->CLOCK_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->CLOCK_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->CLOCK_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->CLOCK_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->CLOCK_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes CLOCK_label_1
    ui->CLOCK_label_1 = lv_label_create(ui->CLOCK);
    lv_label_set_text(ui->CLOCK_label_1, "实时时钟");
    lv_label_set_long_mode(ui->CLOCK_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->CLOCK_label_1, 0, 1);
    lv_obj_set_size(ui->CLOCK_label_1, 134, 29);

    //Write style for CLOCK_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->CLOCK_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->CLOCK_label_1, &lv_font_yousheTittle_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->CLOCK_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->CLOCK_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->CLOCK_label_1, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK_label_1, lv_color_hex(0x00f5ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->CLOCK_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes CLOCK_digital_clock_1
    static bool CLOCK_digital_clock_1_timer_enabled = false;
    ui->CLOCK_digital_clock_1 = lv_dclock_create(ui->CLOCK, "0:00:00");
    if (!CLOCK_digital_clock_1_timer_enabled) {
        lv_timer_create(CLOCK_digital_clock_1_timer, 1000, NULL);
        CLOCK_digital_clock_1_timer_enabled = true;
    }
    lv_obj_set_pos(ui->CLOCK_digital_clock_1, 0, 37);
    lv_obj_set_size(ui->CLOCK_digital_clock_1, 134, 35);

    //Write style for CLOCK_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->CLOCK_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->CLOCK_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->CLOCK_digital_clock_1, &lv_font_yousheTittle_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->CLOCK_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->CLOCK_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->CLOCK_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->CLOCK_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->CLOCK_digital_clock_1, lv_color_hex(0xff6500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->CLOCK_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->CLOCK_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->CLOCK_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->CLOCK_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->CLOCK_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->CLOCK_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of CLOCK.


    //Update current screen layout.
    lv_obj_update_layout(ui->CLOCK);

    //Init events for screen.
    events_init_CLOCK(ui);
}
