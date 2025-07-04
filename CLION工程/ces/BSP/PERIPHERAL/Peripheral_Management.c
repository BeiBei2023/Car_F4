
/**
 * ==================================================
 *  @file Peripheral_Management.c
 *  @brief 用于外设的初始化
 *  @author wangb
 *  @date 2025-06-04 下午7:47
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "Peripheral_Management.h"

#include "uart_callback.h"
#include  "elog.h"
#include "../MODBUS/MODBUS_BASE/modbus.h"
#include "../MODBUS/MODBUS_INIT/modbus_init.h"
#include "aht20.h"
#include "adc.h"
#include "system_voltage.h"
#include "qmi8658a.h"
#include "motor_control.h"
#include "motor_command.h"
#include "pid.h"


/** 所有的初始化代码,放在FreeRTOS 任务之后 ****************************************/

void Peripheral_Management_Init(void) {
    easylogger_init();
    elog_info("初始化", "-----------------------------------------STM32F427VGT6 开始启动！----------------------------------------");
    elog_info("初始化","开始吧——————————————————------------------->>>>>>>>>>>>>>>>>>>>>>>");

    elog_info("初始化", "开启全局中断");
    __enable_irq();     //开启全局中断
    elog_info("初始化", "初始化LOG");

    elog_info("ADC","开始初始化ADC！");
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, ADC_BUFFER_SIZE); // 启动DMA
    elog_info("ADC","初始化成功！");



    elog_info("温湿度传感器","开始初始化AHT20！");
    AHT20_Init();
    elog_info("温湿度传感器","初始化成功！");

    elog_info("QMI8658A传感器","开始初始化QMI86548A！");
    QMI8658A_Init();
    elog_info("QMI8658A传感器","初始化成功！");

    elog_info("串口中断","初始化开始！");
    USER_UART_INIT();   //  串口初始化
    elog_info("串口中断","初始化成功！");

    elog_info("Modbus","开始初始化站号和串口！");
    Modbus_Init(0x01, &huart6);
    elog_info("Modbus","初始化成功！");

    //初始化通道值
    sbus_ch_data.channels[0] = 1500; // 前后速度
    sbus_ch_data.channels[1] = 1500; // 横向速度
    sbus_ch_data.channels[3] = 1500; // 旋转速度

    sbus_ch_data.flags = 4;


    /*-------------------初始化电机，绑定回调函数------------------------- */

    related_initialization_of_motors(motor_can, CAN_Motor_Call_Back); // 初始化电机

    log_i("related_initialization_of_motors ok");

    /*-----------------------------------------------------------------*/

    /*-------------------------初始化PID参数---------------------------- */
    PID_struct_init(&pid_speed[0],
                    POSITION_PID, // pid_speed[0].pid_mode = DELTA_PID;
                    M3508_MOTOR_POSSITION_PID_MAX_OUT,
                    M3508_MOTOR_POSSITION_PID_MAX_IOUT,
                    M3508_MOTOR_1_POSSITION_PID_KP,
                    M3508_MOTOR_1_POSSITION_PID_KI,
                    M3508_MOTOR_1_POSSITION_PID_KD);
    PID_struct_init(&pid_speed[1],
                    POSITION_PID, // pid_speed[0].pid_mode = DELTA_PID;
                    M3508_MOTOR_POSSITION_PID_MAX_OUT,
                    M3508_MOTOR_POSSITION_PID_MAX_IOUT,
                    M3508_MOTOR_2_POSSITION_PID_KP,
                    M3508_MOTOR_2_POSSITION_PID_KI,
                    M3508_MOTOR_2_POSSITION_PID_KD);
    PID_struct_init(&pid_speed[2],
                    POSITION_PID, // pid_speed[0].pid_mode = DELTA_PID;
                    M3508_MOTOR_POSSITION_PID_MAX_OUT,
                    M3508_MOTOR_POSSITION_PID_MAX_IOUT,
                    M3508_MOTOR_3_POSSITION_PID_KP,
                    M3508_MOTOR_3_POSSITION_PID_KI,
                    M3508_MOTOR_3_POSSITION_PID_KD);
    PID_struct_init(&pid_speed[3],
                    POSITION_PID, // pid_speed[0].pid_mode = DELTA_PID;
                    M3508_MOTOR_POSSITION_PID_MAX_OUT,
                    M3508_MOTOR_POSSITION_PID_MAX_IOUT,
                    M3508_MOTOR_4_POSSITION_PID_KP,
                    M3508_MOTOR_4_POSSITION_PID_KI,
                    M3508_MOTOR_4_POSSITION_PID_KD);

    elog_info("初始化","-----------------------------------------------设备初始化完成！-------------------------------------------------");
    elog_info("初始化","进入FreeRTOS任务--------------------->>>>>>>>>>>>>>>>>>");
}