/**
 * ==================================================
 *  @file modbus.c
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

#include "modbus.h"


ModbusData Modbus; // 全局变量定义

/********功能代码**********************************************************************/

void Modbus_Send(uint8_t *data, uint8_t length) {
    HAL_UART_Transmit_DMA(Modbus.huart, data, length); // 使用实际数据长度
}


// 站号验证函数
static bool CheckServerID(void) {
    Modbus.u8SrverID = Modbus.u8Data[0];
    if (Modbus.u8SrverID != Modbus.MODBUS_SEVER_ID &&
        Modbus.u8SrverID != 0) {
        MODBUS_ERROR("Server ID 不匹配: %d", Modbus.u8SrverID);
        return false;
    }
    return true;
}

// CRC验证函数
static bool ValidateCRC(void) {
    Modbus.u16CRC = (Modbus.u8Data[Modbus.data_length - 2] << 8) |
                    Modbus.u8Data[Modbus.data_length - 1];
    Modbus.u16CRC_Calculated = calcCRC(Modbus.u8Data, Modbus.data_length - 2);

    if (Modbus.u16CRC != Modbus.u16CRC_Calculated) {
        MODBUS_ERROR("CRC error: 接收 %04X vs 计算 %04X",
                     Modbus.u16CRC, Modbus.u16CRC_Calculated);
        return false;
    }
    return true;
}



void ParseFunctionCode(void) {
    // 提取功能码等数据
    Modbus.u8FunctionCode = Modbus.u8Data[1];
    Modbus.u16RegAddress = (Modbus.u8Data[2] << 8) | Modbus.u8Data[3];

    // 功能码处理
    if (Modbus.u8FunctionCode == MODBUS_FUNC_READ_COILS ||
        Modbus.u8FunctionCode == MODBUS_FUNC_READ_DISCRETE_INPUTS ||
        Modbus.u8FunctionCode == MODBUS_FUNC_READ_HOLDING_REGISTERS ||
        Modbus.u8FunctionCode == MODBUS_FUNC_READ_INPUT_REGISTERS) {
        // 检查最小长度
        if (Modbus.data_length < 8) {
            MODBUS_ERROR("Function %02X: Data too short", Modbus.u8FunctionCode);
            return;
        }

        // 读取寄存器数量
        Modbus.u16RegNumber = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5];
    } else if (Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_SINGLE_COILS ||
               Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_SINGLE_REGISTER) {
        if (Modbus.data_length >= 8) {
            //共用数据提取逻辑
            Modbus.u16WriteValue = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5];
            // log_d("数据：%2x", Modbus.u8Data[4]);
            // log_d("数据：%2x", Modbus.u8Data[5]);
            // MODBUS_LOG("Function %02X: Address %04X, Value %04X",Modbus.u8FunctionCode,Modbus.u16RegAddress,Modbus.u16WriteValue);

            //线圈特有验证
            if (Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_SINGLE_COILS &&
                Modbus.u16WriteValue != 0x0000 && Modbus.u16WriteValue != 0xFF00) {
                MODBUS_ERROR("Invalid coil value: 0x%04X (必须为 0x0000 或 0xFF00)",
                             Modbus.u16WriteValue);
                return;
            }
        } else {
            MODBUS_ERROR("Function %02X: 缺少值字段", Modbus.u8FunctionCode);
            return;
        }
    } else if (Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_MULTIPLE_COILS ||
               Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS) {
        // 检查最小长度（需要至少7字节 + 字节数）
        if (Modbus.data_length < 8) {
            MODBUS_ERROR("Function %02X: Data too short", Modbus.u8FunctionCode);
            return;
        }
        //  读取寄存器数量
        Modbus.u16RegNumber = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5];

        //  字节数
        Modbus.u8ByteCount = Modbus.u8Data[6];


        //  判断长度
        if (Modbus.data_length != (9 + Modbus.u8ByteCount)) {
            MODBUS_ERROR("Function %02X: Length mismatch (expected %d, got %d)",
                         Modbus.u8FunctionCode, 9 + Modbus.u8ByteCount, Modbus.data_length);
            return;
        }

        // 边界检查
        if (Modbus.u8ByteCount > sizeof(Modbus.u8CoilBytes)) {
            MODBUS_ERROR("Byte count %d exceeds buffer size %d",
                         Modbus.u8ByteCount, sizeof(Modbus.u8CoilBytes));
            return;
        }

        // 数据拷贝
        memcpy(Modbus.u8CoilBytes, &Modbus.u8Data[7], Modbus.u8ByteCount);
    } else {
        MODBUS_ERROR("Unsupported function code: %02X", Modbus.u8FunctionCode);
        return;
    }

    // 所有检查通过后，设置就绪标志
    Modbus.CRC_OK = 1;
}




void Modbus_Data(void) {
    // 使用实际数据长度
    uint8_t data_len = Modbus.data_length;

    // 初始化 CRC_OK
    Modbus.CRC_OK = 0;

    // 数据长度检查（最小8字节）
    if (data_len < 8) {
        MODBUS_ERROR("Invalid data length: %d", data_len);
        return;
    }

    // 提取站号并验证
    if (!CheckServerID()) {
        return;
    }

    // 验证CRC校验
    if (!ValidateCRC()) {
        return;
    }

    // 解析功能码及数据
    ParseFunctionCode();

    //  处理功能码
    ProcessFunctionCode();
}


/***RTOS任务解析*************************************************************************/

extern osSemaphoreId_t modbus_data_semaphoreHandle;
extern osMutexId_t modbus_buffer_mutexHandle;

void Modbus_Data_Parsing(void *argument) {
    for (;;) {


        if (osSemaphoreAcquire(modbus_data_semaphoreHandle, osWaitForever) == osOK) {

            // 处理接收到的SBUS数据
            if (osMutexAcquire(modbus_buffer_mutexHandle, osWaitForever) == osOK) {

                memcpy(Modbus.u8Data,Modbus.u8Data_backup,sizeof(Modbus.u8Data_backup));
                osMutexRelease(modbus_buffer_mutexHandle);    // 释放锁
            }
            // 解码SBUS数据
            Modbus_Data();
        }
    }
}
