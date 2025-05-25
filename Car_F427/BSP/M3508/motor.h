#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "pid.h"
#include "drv_can.h"

#include "can.h"
#include <math.h>
/*
核心控制逻辑

控制目标
系统的目标是通过PID闭环控制实现对电机转速的精准控制，而非直接控制电流或位置。

数据流关系

输入：
用户设定的期望转速（目标值）。
电调通过CAN总线上报的实际转速（反馈值）。

输出：
单片机计算的转矩电流指令值（范围：-16384 ~ 16384），该值通过CAN报文发送给电调，控制电机输出扭矩。
PID控制层级

速度环PID（单片机实现）：
输入误差 = 期望转速 - 实际转速 → 输出转矩电流指令。
电流环PID（电调内部实现）：
根据转矩电流指令调节电机电流，最终驱动电机转动。

*/

typedef volatile struct
{

    int16_t motor_angle; // 转子机械角度值范围:0~ 8191(对应转子机械角度为0~360°)
    int16_t motor_omega; // 转子转速值的单位为:RPM
    int16_t motor_torque;
    int16_t motor_temperture; // 电机温度的单位为:℃

    int16_t motor_omega_original;   // 转子转速值的单位为:RPM

    int16_t motor_angle_last; // 转子机械角度值范围:0~ 8191(对应转子机械角度为0~360°)
    int16_t motor_omega_last; // 转子转速值的单位为:RPM
    int16_t motor_torque_last;
    int16_t motor_temperture_last; // 电机温度的单位为:℃

    int16_t motor_diff;     // 电机角度变化值
    int16_t round_count;    // 电机圈数
    int16_t motor_poeition; // 电机总的角度位置信息
    int16_t motor_angle_acutruly;

    int16_t motor_target_angle; // 设定的电机目标角度（位置）
    int16_t motor_target_speed; // 设定的电机目标转速

} MotorRxData_t;

#define CAN_MOTOR_NUM 4 // 电机数量

/* 全局电机数据实例，volatile保证实时更新 */
// 电调通信的CAN标识符，两个标识符(0200和0x1FF)各自对应控制4个电调，我们使用0X200
#define CAN_ID_MOTOR_CONTROL 0x200

extern const uint32_t motor_id[CAN_MOTOR_NUM];
extern MotorRxData_t motor_data[CAN_MOTOR_NUM]; // 存放实时的电机数据

void related_initialization_of_motors(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function);
void set_motor_target_speed(int16_t new_target_speed[CAN_MOTOR_NUM]);
//void motor_pid_update(void);
uint8_t send_motor_speed_commands(CAN_HandleTypeDef *hcan,
                                  int16_t motor1_speed,
                                  int16_t motor2_speed,
                                  int16_t motor3_speed,
                                  int16_t motor4_speed);

MotorRxData_t *get_motor_data(uint8_t motor_id);

#endif // !__MOTOR_H_
