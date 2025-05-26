#include "QMI8658A.h"
#include <math.h>

QMI8658A_Data_t qmi8658a_data; // 用于存储传感器数据

static uint8_t QMI8658A_Read(uint8_t reg, uint8_t *data, uint16_t len);
static uint8_t QMI8658A_Write(uint8_t reg, uint8_t *data, uint16_t len);

// IIC读取函数
static uint8_t QMI8658A_Read(uint8_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, QMI8658A_I2C_ADDR, reg, 1, data, len, 1000);
    return (status == HAL_OK) ? 0 : 1;
}

// IIC写入函数
static uint8_t QMI8658A_Write(uint8_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, QMI8658A_I2C_ADDR, reg, 1, data, len, 1000);
    return (status == HAL_OK) ? 0 : 1;
}

// 初始化QMI8658A传感器
uint8_t QMI8658A_Init(void)
{
    // id 校验
    uint8_t who_am_i;
    // 版本号
    uint8_t version;

    // 复位检测
    uint8_t reset_status;
    uint8_t reset_val = 0xB0;
    uint8_t ctrl1_val = 0x40;
    uint8_t ctrl7_val = 0x03;
    uint8_t ctrl2_val = 0x95;
    uint8_t ctrl3_val = 0xD5;

    QMI8658A_Read(QMI8658_WHO_AM_I, &who_am_i, 1);

    // 检查设备ID是否正确
    while (who_am_i != 0x05)
    {
        HAL_Delay(1000);                               // 等待100毫秒
        QMI8658A_Read(QMI8658_WHO_AM_I, &who_am_i, 1); // 重新读取设备ID
    }

    log_i("QMI8658A: Device ID is correct: 0x%02X", who_am_i);

    QMI8658A_Read(QMI8658_REVISION_ID, &version, 1);
    log_i("QMI8658A: Revision ID: 0x%02X", version);

    QMI8658A_Write(QMI8658_RESET, &reset_val, 1); // 复位传感器
    HAL_Delay(100);                               // 等待传感器复位完成
    QMI8658A_Read(0x4D, &reset_status, 1);
    if (reset_status != 0x80)
    {
        log_e("QMI8658A: Reset failed, status: 0x%02X", reset_status);
        return 0; // 返回0表示初始化失败
    }
    log_i("QMI8658A: Reset status: 0x%02X", reset_status);

    QMI8658A_Write(QMI8658_CTRL1, &ctrl1_val, 1); // CTRL1 设置地址自动增加
    QMI8658A_Write(QMI8658_CTRL7, &ctrl7_val, 1); // CTRL7 允许加速度和陀螺仪
    QMI8658A_Write(QMI8658_CTRL2, &ctrl2_val, 1); // CTRL2 设置ACC 4g 250Hz
    QMI8658A_Write(QMI8658_CTRL3, &ctrl3_val, 1); // CTRL3 设置GRY 512dps 250Hz

    log_i("QMI8658A: Initialized");

    HAL_Delay(200);

    // 测试
    QMI8658A_fetch_angleFromAcc(&qmi8658a_data); // 读取加速度和陀螺仪数据

    log_i("QMI8658A:TEST--> angle_x = %.1f  angle_y = %.1f angle_z = %.1f", qmi8658a_data.AngleX, qmi8658a_data.AngleY, qmi8658a_data.AngleZ);

    return 1; // 返回1表示初始化成功
}

// 读加速度和陀螺仪的寄存器值
void QMI8658A_Read_AccAndGry(QMI8658A_Data_t *p)
{
    uint8_t status, data_ready = 0;
    int16_t buf[6];

    QMI8658A_Read(QMI8658_STATUS0, &status, 1); // 读状态寄存器
    if (status & 0x03)                          // 判断加速度和陀螺仪数据是否可读
        data_ready = 1;
    if (data_ready == 1)
    { // 如果数据可读
        data_ready = 0;
        QMI8658A_Read(QMI8658_AX_L, (uint8_t *)buf, 12); // 读加速度和陀螺仪值
        p->acc_x = buf[0];                               // 加速度X
        p->acc_y = buf[1];                               // 加速度Y
        p->acc_z = buf[2];                               // 加速度Z
        p->gyr_x = buf[3];                               // 陀螺仪X
        p->gyr_y = buf[4];                               // 陀螺仪Y
        p->gyr_z = buf[5];                               // 陀螺仪Z
    }
}

// 获取XYZ轴的倾角值
void QMI8658A_fetch_angleFromAcc(QMI8658A_Data_t *p)
{
    float temp;

    QMI8658A_Read_AccAndGry(p); // 读取加速度和陀螺仪的寄存器值
    // 根据寄存器值 计算倾角值 并把弧度转换成角度
    temp = (float)p->acc_x / sqrt(((float)p->acc_y * (float)p->acc_y + (float)p->acc_z * (float)p->acc_z));
    p->AngleX = atan(temp) * 57.29578f; // 180/π=57.29578
    temp = (float)p->acc_y / sqrt(((float)p->acc_x * (float)p->acc_x + (float)p->acc_z * (float)p->acc_z));
    p->AngleY = atan(temp) * 57.29578f; // 180/π=57.29578
    temp = sqrt(((float)p->acc_x * (float)p->acc_x + (float)p->acc_y * (float)p->acc_y)) / (float)p->acc_z;
    p->AngleZ = atan(temp) * 57.29578f; // 180/π=57.29578
}
