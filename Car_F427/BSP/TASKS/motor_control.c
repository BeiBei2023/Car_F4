#include "motor_control.h"

void motor_task(void *argument)
{
    /*PID初始化，用于四个电机的PID控制*/

    for (;;)
    {

            motor_pid_update();
            pid_calc(&pid_speed[0], motor_data[0].motor_omega, g_robot.motor_rpm[0]);
            pid_calc(&pid_speed[1], motor_data[1].motor_omega, g_robot.motor_rpm[1]);
            pid_calc(&pid_speed[2], motor_data[2].motor_omega, g_robot.motor_rpm[2]);
            pid_calc(&pid_speed[3], motor_data[3].motor_omega, g_robot.motor_rpm[3]);

            send_motor_speed_commands(&hcan1,
                                      pid_speed[0].pos_out,
                                      pid_speed[1].pos_out,
                                      pid_speed[2].pos_out,
                                      pid_speed[3].pos_out);

        osDelay(2); // 每秒循环一次
    }
}
