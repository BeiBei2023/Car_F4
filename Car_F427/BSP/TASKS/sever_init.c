#include "sever_init.h"
#include "pid.h"
#include "callback.h"
#include "st7735s.h"
#include "fonts.h"
#include "aht20.h"
#include "button_config.h"
#include "screen_config.h"

void init(void)
{

    Button_Init();
    /*-------------------初始化电机，绑定回调函数------------------------- */

    related_initialization_of_motors(motor_can, CAN_Motor_Call_Back); // 初始化电机

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
    /*-----------------------------------------------------------------------*/

    ST7735_Init();

    // 初始化界面管理器
    ScreenManager_Init(&screen_mgr, screens, 5);
    ScreenManager_Switch(&screen_mgr, 0);

    AHT20_Init();
    HAL_Delay(1000);
    EMLOG(LOG_INFO, "init ok");
}
