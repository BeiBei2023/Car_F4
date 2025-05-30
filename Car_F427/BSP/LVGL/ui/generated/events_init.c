/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void main_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Voltage, guider_ui.Voltage_del, &guider_ui.main_del, setup_scr_Voltage, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_main (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->main_btn_1, main_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void Voltage_btn_3_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Temperature, guider_ui.Temperature_del, &guider_ui.Voltage_del, setup_scr_Temperature, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void Voltage_btn_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.main, guider_ui.main_del, &guider_ui.Voltage_del, setup_scr_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_Voltage (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Voltage_btn_3, Voltage_btn_3_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Voltage_btn_2, Voltage_btn_2_event_handler, LV_EVENT_ALL, ui);
}

static void Temperature_btn_5_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Humidity, guider_ui.Humidity_del, &guider_ui.Temperature_del, setup_scr_Humidity, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void Temperature_btn_4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Voltage, guider_ui.Voltage_del, &guider_ui.Temperature_del, setup_scr_Voltage, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_Temperature (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Temperature_btn_5, Temperature_btn_5_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Temperature_btn_4, Temperature_btn_4_event_handler, LV_EVENT_ALL, ui);
}

static void Humidity_btn_7_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.IO_State, guider_ui.IO_State_del, &guider_ui.Humidity_del, setup_scr_IO_State, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void Humidity_btn_6_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Temperature, guider_ui.Temperature_del, &guider_ui.Humidity_del, setup_scr_Temperature, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_Humidity (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Humidity_btn_7, Humidity_btn_7_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Humidity_btn_6, Humidity_btn_6_event_handler, LV_EVENT_ALL, ui);
}

static void IO_State_btn_9_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.SBUS, guider_ui.SBUS_del, &guider_ui.IO_State_del, setup_scr_SBUS, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void IO_State_btn_8_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Humidity, guider_ui.Humidity_del, &guider_ui.IO_State_del, setup_scr_Humidity, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_IO_State (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->IO_State_btn_9, IO_State_btn_9_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->IO_State_btn_8, IO_State_btn_8_event_handler, LV_EVENT_ALL, ui);
}

static void SBUS_btn_11_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.IMU, guider_ui.IMU_del, &guider_ui.SBUS_del, setup_scr_IMU, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void SBUS_btn_10_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.IO_State, guider_ui.IO_State_del, &guider_ui.SBUS_del, setup_scr_IO_State, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_SBUS (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->SBUS_btn_11, SBUS_btn_11_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->SBUS_btn_10, SBUS_btn_10_event_handler, LV_EVENT_ALL, ui);
}

static void IMU_btn_11_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.CLOCK, guider_ui.CLOCK_del, &guider_ui.IMU_del, setup_scr_CLOCK, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void IMU_btn_10_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.SBUS, guider_ui.SBUS_del, &guider_ui.IMU_del, setup_scr_SBUS, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_IMU (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->IMU_btn_11, IMU_btn_11_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->IMU_btn_10, IMU_btn_10_event_handler, LV_EVENT_ALL, ui);
}

static void CLOCK_btn_11_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.main, guider_ui.main_del, &guider_ui.CLOCK_del, setup_scr_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void CLOCK_btn_10_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.IMU, guider_ui.IMU_del, &guider_ui.CLOCK_del, setup_scr_IMU, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
        break;
    }
    default:
        break;
    }
}

void events_init_CLOCK (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->CLOCK_btn_11, CLOCK_btn_11_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->CLOCK_btn_10, CLOCK_btn_10_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
