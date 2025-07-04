/**
 * ==================================================
 *  @file modbus.h
 *  @brief 用于Modbus的协议解析，配合STM单片机
 *  @author wangb
 *  @date 2025-06-04 下午7:25
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#ifndef MODBUS_H
#define MODBUS_H

/** Private includes ----------------------------------------------------------*/
#include "main.h"
#include "elog.h"
#include "../MODBUS_CRC/modbus_crc.h"
#include <string.h>
#include "cmsis_os2.h"

#include "../MODBUS_FUNCTION/modbus_function_code.h"






// 新增错误处理宏
#define MODBUS_ERROR(fmt, ...) do { \
log_e("Modbus Error: " fmt, ##__VA_ARGS__); \
Modbus.CRC_OK = 0; \
Modbus.Rx_Flag = 0; \
} while(0)

#define MODBUS_LOG(...) log_d(__VA_ARGS__)

typedef struct {
    uint8_t MODBUS_SEVER_ID; // 从站地址
    UART_HandleTypeDef *huart; // UART 句柄

    /* 接收数据 */
    uint8_t Rx_Flag; // 接收标志位
    uint8_t CRC_OK;
    uint8_t u8Data_backup[256]; // DMA  接收数据缓冲区
    uint8_t u8Data[256]; // 接收缓冲区
    uint8_t data_length; // 实际接收数据长度（可选保留）
    uint8_t u8SrverID; // 当前解析到的从站地址
    uint8_t u8FunctionCode; // 功能码

    /* 寄存器地址与数量 */
    uint16_t u16RegAddress; // 寄存器起始地址
    uint16_t u16RegNumber; // 寄存器数量（读操作）或写入值（单写操作）

    /* 写入特有字段 */
    uint8_t u8ByteCount; // 多写入时的字节数
    uint8_t u8CoilBytes[256]; // 线圈原始字节数据
    uint16_t u16WriteValue;  //  写入值

    /* CRC校验 */
    uint16_t u16CRC; // 接收的CRC值
    uint16_t u16CRC_Calculated; // 计算出的CRC值

    /* 发送数据 */
    uint8_t TX_Data[256]; // 响应数据缓冲区
    uint8_t u8TX_Data_Len; // 发送数据长度
} ModbusData;

// 声明结构体
extern ModbusData Modbus;


/**************函数声明******************************************************************/
void Modbus_Init(uint8_t device_address, UART_HandleTypeDef *huart);
void Modbus_Data(void);
void Modbus_Send(uint8_t *data, uint8_t length);
void Modbus_Data_Parsing(void *argument) ;
#endif //MODBUS_H
