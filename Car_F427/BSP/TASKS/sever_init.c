#include "sever_init.h"
#include "pid.h"
#include "callback.h"
#include "st7735s.h"
#include "fonts.h"
#include "aht20.h"
#include "button_config.h"
#include "screen_config.h"
#include "elog.h"
#include "QMI8658A.h"

void init(void)
{

    log_i("init start");
    Button_Init();
    log_i("Button_Init ok");
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
    log_i("PID_struct_init ok");
    /*-----------------------------------------------------------------------*/

    AHT20_Init();



    QMI8658A_Init(); // 初始化QMI8658A传感器


    log_i("all sever init ok");
}
