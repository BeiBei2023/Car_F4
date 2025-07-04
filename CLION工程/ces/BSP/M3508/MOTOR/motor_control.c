
/**
 * ==================================================
 *  @file motor_control.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-07-03 下午5:41
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "motor_control.h"
#include "motor_command.h"
#include "pid.h"
#include "sbus.h"
#include "cmsis_os2.h"
#include "motor_target_speed.h"


// 定义特殊值，避免硬编码
#define FLAG_INVALID (0xFFFFFFFFU) // 用无符号值表示无效状态

void motor_task(void *argument)
{

    /*PID初始化，用于四个电机的PID控制*/

    for (;;)
    {
        int16_t target_speeds[CAN_MOTOR_NUM] = {g_robot.motor_rpm[0],
                                                g_robot.motor_rpm[1],
                                                g_robot.motor_rpm[2],
                                                g_robot.motor_rpm[3]}; // 4个电机的目标速度

        set_motor_target_speed(target_speeds); // 设置目标速度，限幅

        // motor_pid_update();
        //四个电机的PID参数应该是不一样的
        for (int i = 0; i < CAN_MOTOR_NUM; i++) {
            pid_calc(&pid_speed[i],
                     motor_data[i].motor_omega,
                     motor_data[i].motor_target_speed);
        }

        if (sbus_ch_data.flags == 0 && sbus_ch_data.channels[5] == 1000)
        {
            //遥控器是连接的，并且打开了油门
            send_motor_speed_commands(&hcan1,
                                      pid_speed[0].pos_out,
                                      pid_speed[1].pos_out,
                                      pid_speed[2].pos_out,
                                      pid_speed[3].pos_out);
        }
        else if (sbus_ch_data.flags == 4 || sbus_ch_data.channels[5] == 2000)
        {
            //遥控器是断开的，或者关闭油门
            send_motor_speed_commands(&hcan1,
                                      0,
                                      0,
                                      0,
                                      0);
        }

        osDelay(10); // 每10ms循环一次（约100Hz控制频率）

    }
}