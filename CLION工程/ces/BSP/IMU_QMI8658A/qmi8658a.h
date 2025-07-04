/**
 * ==================================================
 *  @file qmi8658a.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-08 下午6:39
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef QMI8658A_H
#define QMI8658A_H

#include "main.h"
#include "elog.h"
#include "kalman.h" //  卡尔曼滤波
#include <math.h>
#include "cmsis_os2.h"



extern I2C_HandleTypeDef hi2c2;
// QMI8658A I2C address
#define QMI8658A_I2C_ADDR (0x6A << 1) // 7-bit I2C address for QMI8658A

// QMI8658寄存器地址
enum qmi8658_reg
{
    QMI8658_WHO_AM_I, // 0x00
    QMI8658_REVISION_ID,
    QMI8658_CTRL1,
    QMI8658_CTRL2,
    QMI8658_CTRL3,
    QMI8658_CTRL4,
    QMI8658_CTRL5,
    QMI8658_CTRL6,
    QMI8658_CTRL7,
    QMI8658_CTRL8,
    QMI8658_CTRL9,
    QMI8658_CATL1_L,
    QMI8658_CATL1_H,
    QMI8658_CATL2_L,
    QMI8658_CATL2_H,
    QMI8658_CATL3_L,
    QMI8658_CATL3_H,
    QMI8658_CATL4_L,
    QMI8658_CATL4_H,
    QMI8658_FIFO_WTM_TH,
    QMI8658_FIFO_CTRL,
    QMI8658_FIFO_SMPL_CNT,
    QMI8658_FIFO_STATUS,
    QMI8658_FIFO_DATA,
    QMI8658_STATUSINT = 45,
    QMI8658_STATUS0,
    QMI8658_STATUS1,
    QMI8658_TIMESTAMP_LOW,
    QMI8658_TIMESTAMP_MID,
    QMI8658_TIMESTAMP_HIGH,
    QMI8658_TEMP_L,
    QMI8658_TEMP_H,
    QMI8658_AX_L,
    QMI8658_AX_H,
    QMI8658_AY_L,
    QMI8658_AY_H,
    QMI8658_AZ_L,
    QMI8658_AZ_H,
    QMI8658_GX_L,
    QMI8658_GX_H,
    QMI8658_GY_L,
    QMI8658_GY_H,
    QMI8658_GZ_L,
    QMI8658_GZ_H,
    QMI8658_COD_STATUS = 70,
    QMI8658_dQW_L = 73,
    QMI8658_dQW_H,
    QMI8658_dQX_L,
    QMI8658_dQX_H,
    QMI8658_dQY_L,
    QMI8658_dQY_H,
    QMI8658_dQZ_L,
    QMI8658_dQZ_H,
    QMI8658_dVX_L,
    QMI8658_dVX_H,
    QMI8658_dVY_L,
    QMI8658_dVY_H,
    QMI8658_dVZ_L,
    QMI8658_dVZ_H,
    QMI8658_TAP_STATUS = 89,
    QMI8658_STEP_CNT_LOW,
    QMI8658_STEP_CNT_MIDL,
    QMI8658_STEP_CNT_HIGH,
    QMI8658_RESET = 96
};
// 传感器的数据保存结构体
typedef struct
{
    int16_t acc_x;  // 加速度X
    int16_t acc_y;  // 加速度Y
    int16_t acc_z;  // 加速度Z
    int16_t gyr_x;  // 角速度X
    int16_t gyr_y;  // 角速度Y
    int16_t gyr_z;  // 角速度Z
    float AngleX;   // 角度X
    float AngleY;   // 角度Y
    float AngleZ;   // 角度Z

    uint8_t is_calibrated ; // 新增字段表示是否已校准
} QMI8658A_Data_t;

// 实例化
extern QMI8658A_Data_t qmi8658a_data; // 用于存储传感器数据

// 添加三个卡尔曼滤波器实例（分别用于 X/Y/Z 轴）
extern KalmanTypeDef kalmanX, kalmanY, kalmanZ;


uint8_t QMI8658A_Init(void); // 初始化函数
void QMI8658A_Read_AccAndGry(QMI8658A_Data_t *p);
void QMI8658A_fetch_angleFromAcc(QMI8658A_Data_t *p);

void imu_data_task(void *argument);

#endif //QMI8658A_H
