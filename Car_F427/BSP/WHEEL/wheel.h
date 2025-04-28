#ifndef __WHEEL_H_
#define __WHEEL_H_

/* USER CODE BEGIN Includes */
#include "main.h"
#include "cmsis_os.h"
#include "sbus.h"
#include "math.h"




/* 遥控器通道定义 */
#define SBUS_CH_VX       0  // 通道2控制前后速度
#define SBUS_CH_VY       1   // 通道1控制横向速度
#define SBUS_CH_OMEGA    3   // 通道4控制旋转
#define SBUS_CH_GEAR     4   // 通道5为三档拨位开关
#define SBUS_CH_WORK     5     /// 启动,停止

/* 机械参数（单位：米） */
/*
根据实际尺寸测量以下参数：
    a：小车中心到轮子的纵向距离（半长）
    b：小车中心到轮子的横向距离（半宽）
    r：轮子半径（单位：米）
*/
#define MECANUM_A        0.20f  //小车中心到轮子的纵向距离（半长）
#define MECANUM_B        0.22f  //小车中心到轮子的横向距离（半宽）
#define WHEEL_RADIUS     0.08f  //轮子半径（单位：米）




/* 全局状态结构体 */
typedef struct {
    /* 处理后的遥控数据 */
    float vx;           // 归一化后的前后速度 [-1.0, 1.0]
    float vy;           // 归一化后的横向速度
    float omega;        // 归一化后的旋转速度
    uint8_t work;        //归一化，-1时停止，1启动
    int8_t speed_mode; // 0-低速 1-中速 2-高速 
    
    
    /* 电机控制数据 */
    float motor_rpm[4]; // 四个电机的目标转速（RPM）
} RobotState;

extern RobotState g_robot;  // 机器人状态

// 全局互斥量声明
extern osMutexId_t mutexHandle;

/* USER CODE END Includes */


void task_remote_attr(void *argument);
void task_led_attr(void *argument);



#endif // !__WHEEL_H_
