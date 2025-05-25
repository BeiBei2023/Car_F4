#ifndef PID_H_
#define PID_H_

/**
 * @file pid.h
 * @author wangbeihong
 * @brief 使用 CAN 控制时,电调对电机是电流闭环控制,所以需要在程序中从电调获取电机转速并增加电机转速 PID 调节控制.
 * @version 0.1
 * @date 2025-04-04
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "stm32f4xx_hal.h"


enum
{
    LLAST = 0, // 上上次
    LAST = 1,  // 上次
    NOW = 2,   // 本次

    POSITION_PID, // 位置式
    DELTA_PID,    // 增量式
};

typedef struct __pid_t
{
    float p;    // 比例系数
    float i;    // 积分系数
    float d;    // 微分系数

    float set[3]; // 目标值,包含NOW， LAST， LLAST上上次
    float get[3]; // 测量值
    float err[3]; // 误差

    float pout; // p输出
    float iout; // i输出
    float dout; // d输出

    float pos_out;      // 本次位置式输出
    float last_pos_out; // 上次输出
    
    float delta_u;      // 本次增量值
    float delta_out;    // 本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;

    float max_err;  // 最大误差
    float deadband; // err < deadband return     // 死区

    uint32_t pid_mode;    // 工作模式


    uint32_t MaxOutput;     // 输出限幅
    uint32_t IntegralLimit; // 积分限幅

    void (*f_param_init)(struct __pid_t *pid, // PID参数初始化
                         uint32_t pid_mode,
                         uint32_t maxOutput,
                         uint32_t integralLimit,
                         float p,
                         float i,
                         float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d); // pid三个参数修改

} pid_t;

/**
 * 初始化PID控制器结构体
 *
 * @param pid        指向PID结构体的指针
 * @param mode       控制器的工作模式（如自动/手动模式或控制类型）
 * @param maxout     控制器的最大输出限幅值
 * @param intergral_limit  积分项的积分限幅值，用于防止积分饱和
 * @param kp         比例增益系数
 * @param ki         积分增益系数
 * @param kd         微分增益系数
 */
void PID_struct_init(
    pid_t *pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,

    float kp,
    float ki,
    float kd);

/**
 * 执行PID控制计算
 *
 * @param pid    PID控制器结构体指针
 * @param fdb    系统反馈值
 * @param ref    参考输入值
 * @return       计算得到的PID控制输出值
 */
float pid_calc(pid_t *pid, float fdb, float ref);
void pid_reset(pid_t *pid, float kp, float ki, float kd);



/* 横滚轴PID控制器标识符 */
extern pid_t pid_rol;

/* 俯仰轴PID控制器标识符 */
extern pid_t pid_pit;

/* 偏航轴PID控制器标识符 */
extern pid_t pid_yaw;

/* 俯仰角速度PID控制器标识符 */
extern pid_t pid_pit_omg;

/* 偏航角速度PID控制器标识符 */
extern pid_t pid_yaw_omg;

/* 四个速度相关PID控制器的数组 */
extern pid_t pid_speed[4];

/* 偏航角度PID控制器标识符 */
extern pid_t pid_yaw_alfa;

/* 底盘角度PID控制器标识符 */
extern pid_t pid_chassis_angle;

/* 推杆相关PID控制器标识符 */
extern pid_t pid_poke;

/* 推杆角速度PID控制器标识符 */
extern pid_t pid_poke_omg;

/* IMU温度PID控制器标识符 */
extern pid_t pid_imu_tmp;

/* 大缓冲偏航校准PID控制器标识符 */
extern pid_t pid_cali_bby;

/* 大缓冲俯仰校准PID控制器标识符 */
extern pid_t pid_cali_bbp;

/* 角速度PID控制器标识符 */
extern pid_t pid_angular_speed;

/* 位置PID控制器标识符 */
extern pid_t pid_position;

#endif // PID_H_
