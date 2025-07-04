/**
 * ==================================================
 *  @file motor_control.h
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



#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/**************电机通用的固定参数**********************/
#define M3508_MOTOR_POSSITION_PID_MAX_OUT 16000 // 3508电机最大输出值
#define M3508_MOTOR_POSSITION_PID_MAX_IOUT 2000  // 3508电机最大积分输出值

/*************电机1的参数*******************/
#define M3508_MOTOR_1_POSSITION_PID_KP 5.55f    // 3508电机1的PID参数 比例
#define M3508_MOTOR_1_POSSITION_PID_KI 0.002f    // 3508电机1的PID参数 积分
#define M3508_MOTOR_1_POSSITION_PID_KD 0.001f    // 3508电机1的PID参数 微分
/**************电机2的参数*****************/
#define M3508_MOTOR_2_POSSITION_PID_KP 5.55f    // 3508电机2的PID参数 比例
#define M3508_MOTOR_2_POSSITION_PID_KI 0.002f    // 3508电机2的PID参数 积分
#define M3508_MOTOR_2_POSSITION_PID_KD 0.001f    // 3508电机2的PID参数 微分
/**************电机3的参数***************/
#define M3508_MOTOR_3_POSSITION_PID_KP 5.55f    // 3508电机3的PID参数 比例
#define M3508_MOTOR_3_POSSITION_PID_KI 0.002f    // 3508电机3的PID参数 积分
#define M3508_MOTOR_3_POSSITION_PID_KD 0.001f    // 3508电机3的PID参数 微分
/**************电机4的参数**************/
#define M3508_MOTOR_4_POSSITION_PID_KP 5.55f    // 3508电机4的PID参数 比例
#define M3508_MOTOR_4_POSSITION_PID_KI 0.002f    // 3508电机4的PID参数 积分
#define M3508_MOTOR_4_POSSITION_PID_KD 0.001f    // 3508电机4的PID参数 微分





void motor_task(void *argument);


#endif //MOTOR_CONTROL_H
