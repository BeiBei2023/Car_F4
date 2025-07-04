/**
 * ==================================================
 *  @file qmi8658a.c
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


#include "qmi8658a.h"

#include "cmsis_os.h"


QMI8658A_Data_t qmi8658a_data; // 用于存储传感器数据
// 定义三个卡尔曼滤波器实例
KalmanTypeDef kalmanX, kalmanY, kalmanZ;
static uint32_t last_time_ms = 0; // 上次采集时间
static uint8_t is_first_run = 1;
extern osSemaphoreId_t imu_calibration_done_semaphoreHandle; // 获取传感器校准完成信号量
//偏移校准
float gyro_offset_x = 0.0f;
float gyro_offset_y = 0.0f;
float gyro_offset_z = 0.0f;


static uint8_t QMI8658A_Read(uint8_t reg, uint8_t *data, uint16_t len);

static uint8_t QMI8658A_Write(uint8_t reg, uint8_t *data, uint16_t len);

// IIC读取函数
static uint8_t QMI8658A_Read(uint8_t reg, uint8_t *data, uint16_t len) {
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, QMI8658A_I2C_ADDR, reg, 1, data, len, 1000);
    return (status == HAL_OK) ? 0 : 1;
}

// IIC写入函数
static uint8_t QMI8658A_Write(uint8_t reg, uint8_t *data, uint16_t len) {
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, QMI8658A_I2C_ADDR, reg, 1, data, len, 1000);
    return (status == HAL_OK) ? 0 : 1;
}

#define LOG_TAG "QMI8658A"

void QMI8658A_CalibrateGyro(QMI8658A_Data_t *p) {
    static uint8_t is_gyro_calibrated = 0;

    if (is_gyro_calibrated) return;

    elog_info("QMI8658A", "开始陀螺仪偏移校准...");

    for (int i = 0; i < 100; i++) {
        QMI8658A_Read_AccAndGry(p);
        gyro_offset_x += (int32_t)p->gyr_x;
        gyro_offset_y += (int32_t)p->gyr_y;
        gyro_offset_z += (int32_t)p->gyr_z;
        osDelay(10);
    }

    gyro_offset_x /= 100;
    gyro_offset_y /= 100;
    gyro_offset_z /= 100;

    elog_info("QMI8658A", "陀螺仪偏移校准完成: X:%d Y:%d Z:%d",
              (int)gyro_offset_x, (int)gyro_offset_y, (int)gyro_offset_z);

    p->is_calibrated = 1;
    is_gyro_calibrated = 1;
}


uint8_t QMI8658A_Init(void) {


    elog_info(LOG_TAG, "QMI8658A_Init");
    uint8_t who_am_i;
    uint8_t version;

    uint8_t reset_status;
    uint8_t reset_val = 0xB0;
    uint8_t ctrl1_val = 0x40;
    uint8_t ctrl7_val = 0x03;
    uint8_t ctrl2_val = 0x95;
    uint8_t ctrl3_val = 0xD5;


    QMI8658A_Read(QMI8658_WHO_AM_I, &who_am_i, 1);

    // 检查设备ID是否正确
    while (who_am_i != 0x05) {
        osDelay(1000);
        QMI8658A_Read(QMI8658_WHO_AM_I, &who_am_i, 1);
    }

    elog_info(LOG_TAG, "Device ID is correct: 0x%02X", who_am_i);

    QMI8658A_Read(QMI8658_REVISION_ID, &version, 1);
    elog_info(LOG_TAG, "Revision ID: 0x%02X", version);

    QMI8658A_Write(QMI8658_RESET, &reset_val, 1); // 复位传感器
    osDelay(100); // 等待传感器复位完成
    QMI8658A_Read(0x4D, &reset_status, 1);
    if (reset_status != 0x80) {
        elog_error(LOG_TAG, "Reset failed, status: 0x%02X", reset_status);
        return 0; // 返回0表示初始化失败
    }
    elog_info(LOG_TAG, "Reset status: 0x%02X", reset_status);

    QMI8658A_Write(QMI8658_CTRL1, &ctrl1_val, 1); // CTRL1 设置地址自动增加
    QMI8658A_Write(QMI8658_CTRL7, &ctrl7_val, 1); // CTRL7 允许加速度和陀螺仪
    QMI8658A_Write(QMI8658_CTRL2, &ctrl2_val, 1); // CTRL2 设置ACC 4g 250Hz
    QMI8658A_Write(QMI8658_CTRL3, &ctrl3_val, 1); // CTRL3 设置GRY 512dps 250Hz

    elog_info(LOG_TAG, "初始化完成");

    osDelay(200);

    elog_info("卡尔曼滤波", "Kalman 初始化开始:");
    last_time_ms = HAL_GetTick(); // 初始化时间戳
    KalmanFilter_Init(&kalmanX);
    KalmanFilter_Init(&kalmanY);
    KalmanFilter_Init(&kalmanZ);
    elog_info("卡尔曼滤波", "Kalman 滤波初始化完成:");
    //校准
    QMI8658A_CalibrateGyro(&qmi8658a_data);

    // 测试读取角度值（有校准）
    QMI8658A_fetch_angleFromAcc(&qmi8658a_data); // 读取加速度和陀螺仪数据

    elog_info(LOG_TAG, "TEST--> angle_x = %.1f  angle_y = %.1f angle_z = %.1f",
              qmi8658a_data.AngleX, qmi8658a_data.AngleY, qmi8658a_data.AngleZ);

   osSemaphoreRelease(imu_calibration_done_semaphoreHandle);


    return 1; // 返回1表示初始化成功
}


// 读加速度和陀螺仪的寄存器值
void QMI8658A_Read_AccAndGry(QMI8658A_Data_t *p) {
    uint8_t status, data_ready = 0;
    int16_t buf[6];

    QMI8658A_Read(QMI8658_STATUS0, &status, 1); // 读状态寄存器
    if (status & 0x03) // 判断加速度和陀螺仪数据是否可读
        data_ready = 1;
    if (data_ready == 1) {
        // 如果数据可读
        data_ready = 0;
        QMI8658A_Read(QMI8658_AX_L, (uint8_t *) buf, 12); // 读加速度和陀螺仪值
        p->acc_x = buf[0]; // 加速度X
        p->acc_y = buf[1]; // 加速度Y
        p->acc_z = buf[2]; // 加速度Z
        p->gyr_x = buf[3]; // 陀螺仪X
        p->gyr_y = buf[4]; // 陀螺仪Y
        p->gyr_z = buf[5]; // 陀螺仪Z
    }
}

// 获取XYZ轴的倾角值
void QMI8658A_fetch_angleFromAcc(QMI8658A_Data_t *p) {
    float temp;
    uint32_t current_time = HAL_GetTick();
    float dt = (current_time - last_time_ms) / 1000.0f;
    last_time_ms = current_time;

    QMI8658A_Read_AccAndGry(p);

    // 计算加速度角度
    temp = (float) p->acc_x / sqrt((float) p->acc_y * p->acc_y + (float) p->acc_z * p->acc_z + 1e-6f);
    float angle_acc_x = atan(temp) * 57.29578f;

    temp = (float) p->acc_y / sqrt((float) p->acc_x * p->acc_x + (float) p->acc_z * p->acc_z + 1e-6f);
    float angle_acc_y = atan(temp) * 57.29578f;

    temp = sqrt((float) p->acc_x * p->acc_x + (float) p->acc_y * p->acc_y) / (p->acc_z + 1e-6f);
    float angle_acc_z = atan(temp) * 57.29578f;



    // 使用校准后的陀螺仪值进行积分预测
    float gx = ((float) (p->gyr_x - gyro_offset_x) / 32768.0f) * 512.0f;
    float gy = ((float) (p->gyr_y - gyro_offset_y) / 32768.0f) * 512.0f;
    float gz = ((float) (p->gyr_z - gyro_offset_z) / 32768.0f) * 512.0f;

    kalmanX.x += gx * dt;
    kalmanY.x += gy * dt;
    kalmanZ.x += gz * dt;

    p->AngleX = KalmanFilter_Update(&kalmanX, angle_acc_x);
    p->AngleY = KalmanFilter_Update(&kalmanY, angle_acc_y);
    p->AngleZ = KalmanFilter_Update(&kalmanZ, angle_acc_z);
}

extern osMutexId_t imu_data_mutexHandle; //互斥锁
void imu_data_task(void *argument) {

    for (;;) {

        if (  osSemaphoreAcquire(imu_calibration_done_semaphoreHandle,osWaitForever) == osOK ) {
            if (osMutexAcquire(imu_data_mutexHandle,osWaitForever) == osOK) {
                // 获取 IMU 数据并写入全局结构体
                QMI8658A_fetch_angleFromAcc(&qmi8658a_data);
                osMutexRelease(imu_data_mutexHandle);
            }
        }

    }
}

