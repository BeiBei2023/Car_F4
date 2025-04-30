        #include "motor_control.h"
        
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
                pid_calc(&pid_speed[0],
                         motor_data[0].motor_omega,
                         motor_data[0].motor_target_speed);
        
                pid_calc(&pid_speed[1],
                         motor_data[1].motor_omega,
                         motor_data[1].motor_target_speed);
        
                pid_calc(&pid_speed[2],
                         motor_data[2].motor_omega,
                         motor_data[2].motor_target_speed);
        
                pid_calc(&pid_speed[3],
                         motor_data[3].motor_omega,
                         motor_data[3].motor_target_speed);
        
                if (sbus_ch_data.flags == 0 && sbus_ch_data.channels[5] == 1000)
                {
                    send_motor_speed_commands(&hcan1,
                                              pid_speed[0].pos_out,
                                              pid_speed[1].pos_out,
                                              pid_speed[2].pos_out,
                                              pid_speed[3].pos_out);
                }
                // 修复比较逻辑，使用无符号值
                if (sbus_ch_data.flags == 4 || sbus_ch_data.channels[5] == 2000 )
                {
                    send_motor_speed_commands(&hcan1,
                                              0,
                                              0,
                                              0,
                                              0);
                }
        
                osDelay(1); // 每秒循环一次
            }
        }