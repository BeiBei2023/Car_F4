
/**
 * ==================================================
 *  @file holding_register.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午2:56
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "holding_register.h"

uint16_t holdingRegisters[MODBUS_HOLDING_REGISTER_COUNT] = {0};    //  保持寄存器

/**
 * 初始化 Holding Registers,保持寄存器的初始值
 *
 * 此函数用于在程序启动时设置 Holding Registers 的默认值
 * 这些寄存器存储了系统的关键运行参数，如温度、PWM 占空比、风扇速度等
 * 通过设置合理的默认值，可以确保系统在初始状态下处于安全和稳定的状态
 */
void InitHoldingRegisters(void) {
    // 初始化默认值
    holdingRegisters[SERVER_ID] = Modbus.u8SrverID; // 设置服务器ID
    // holdingRegisters[TEMP_SENSOR_REG_INDEX] = 10;      // 初始温度 0°C
    // holdingRegisters[CURRENT_REG_INDEX]      = 500;    // 5A
}

/**更改值的方法*****************************************/
/*
int currentTemp = ReadTemperatureSensor();
if (IsRegisterValueValid(TEMP_SENSOR_REG_INDEX, (uint16_t)currentTemp)) {
    holdingRegisters[TEMP_SENSOR_REG_INDEX] = (uint16_t)currentTemp;
}
 */

/**
 * 检查寄存器的值是否有效
 *
 * 此函数根据寄存器索引和值来判断该值是否在允许的范围内
 * 它用于确保特定寄存器的值不会导致系统错误或无效状态
 *
 * @param regIndex 寄存器索引，用于识别特定的寄存器
 * @param value 要写入寄存器的值
 * @return bool 表示值是否有效的布尔值，true表示有效，false表示无效
 */
bool IsRegisterValueValid(uint16_t regIndex, uint16_t value) {

    // if (regIndex == TEMP_SENSOR_REG_INDEX) {
    //
    //     //原始物理值 × 100，并转换为 uint16_t 类型
    //
    //     // 支持温度范围：-40.00℃ ~ 85.00℃
    //     float temp = (float)value / 100.0f;
    //     return temp >= -40.0f && temp <= 85.0f;
    // }

    return true; // 默认接受所有值
}






/**
 * 设置保持寄存器的值
 *
 * 此函数用于将指定的值写入到保持寄存器中它首先检查索引是否超出寄存器数量的范围，
 * 然后检查给定的值是否对指定的寄存器有效如果这两个检查都通过，则将值写入寄存器，
 * 并返回true表示成功；否则，返回false表示失败
 *
 * @param index 保持寄存器的索引
 * @param value 要写入保持寄存器的值
 * @return 写入操作是否成功
 */
bool SetHoldingRegister(uint16_t index, uint16_t value) {
    // 检查索引是否超出寄存器数量的范围
    if (index >= MODBUS_HOLDING_REGISTER_COUNT) return false;
    // 检查给定的值是否对指定的寄存器有效
    if (!IsRegisterValueValid(index, value)) return false;

    // 将值写入指定的保持寄存器
    holdingRegisters[index] = value;
    // 返回true表示写入成功
    return true;
}

/**
 * 读取保持寄存器的值
 *
 * 此函数用于获取Modbus协议中的保持寄存器的值保持寄存器是用来存储用户数据的，
 * 可以通过Modbus请求来读取或写入这些寄存器在调用此函数时，需要指定寄存器的索引，
 * 如果索引超出了保持寄存器数组的范围，则返回一个特殊的错误值
 *
 * @param index 保持寄存器的索引，表示要读取的寄存器位置
 * @return 如果索引超出范围，则返回0xFFFF；否则返回对应索引的保持寄存器的值
 */
uint16_t GetHoldingRegister(uint16_t index) {
    // 检查索引是否超出了保持寄存器数组的范围
    if (index >= MODBUS_HOLDING_REGISTER_COUNT) return 0xFFFF;
    // 返回指定索引的保持寄存器的值
    return holdingRegisters[index];
}



/**
 * 处理写单个寄存器的请求
 *
 * 解析Modbus请求，验证地址和数据的合法性，更新寄存器，并发送响应
 * 如果地址或数据不合法，则发送相应的异常响应
 */
void HandleWriteSingleRegister(void) {
    // 获取寄存器地址和要写入的值
    uint16_t address = Modbus.u16RegAddress;
    uint16_t value = Modbus.u16WriteValue;



    // 转换为逻辑地址
    uint16_t logicalAddress = address + MODBUS_HOLDING_REGISTER_START;

    // 转换为数组索引
    uint16_t startIndex = logicalAddress - MODBUS_HOLDING_REGISTER_START;


    // 地址范围验证
    if (startIndex  > MODBUS_HOLDING_REGISTER_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_READ_HOLDING_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 数据有效性验证
    if (!IsRegisterValueValid(startIndex, value)) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_SINGLE_REGISTER,
                           MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // 更新寄存器
    // 使用封装接口安全写入
    bool success = SetHoldingRegister(startIndex, value);
    if (!success) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_SINGLE_REGISTER, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // 构造响应帧（回显写入的地址和值）
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_SINGLE_REGISTER;
    Modbus.TX_Data[2] = (address >> 8) & 0xFF;
    Modbus.TX_Data[3] = address & 0xFF;

    Modbus.TX_Data[4] = (value >> 8) & 0xFF;
    Modbus.TX_Data[5] = value & 0xFF;

    // 计算CRC校验码并添加到响应帧中
    uint16_t crc = calcCRC(Modbus.TX_Data, 6);
    Modbus.TX_Data[6] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[7] = crc & 0xFF;

    // 设置响应帧长度并发送响应
    Modbus.u8TX_Data_Len = 8;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}

/**
 * 处理写多个保持寄存器的请求
 *
 * 解析Modbus请求，验证地址和数据的合法性，更新多个寄存器，并发送响应
 * 如果地址或数据不合法，则发送相应的异常响应
 */
void HandleWriteMultipleRegisters(void) {

    uint16_t startAddress = Modbus.u16RegAddress; // 协议地址（偏移地址）
    uint16_t regCount = Modbus.u16RegNumber;     // 寄存器数量
    uint8_t byteCount = Modbus.u8ByteCount;       // 数据字节数

    // 转换为逻辑地址和数组索引
    uint16_t logicalStartAddress = startAddress + MODBUS_HOLDING_REGISTER_START;
    uint16_t startIndex = logicalStartAddress - MODBUS_HOLDING_REGISTER_START;

    // 地址范围验证
    if (startIndex + regCount > MODBUS_HOLDING_REGISTER_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 数据长度验证（必须是 regCount × 2）
    if (byteCount != regCount * 2) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // 写入每个寄存器并验证
    for (uint16_t i = 0; i < regCount; i++) {
        uint16_t value = Modbus.u8CoilBytes[i]<< 8 | Modbus.u8CoilBytes[i+1];

        // 数据有效性验证
        if (!IsRegisterValueValid(startIndex + i, value)) {
            SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);

            return;
        }

        // 使用封装接口安全写入
        bool success = SetHoldingRegister(startIndex + i, value);
        if (!success) {
            SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS, MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE);
            return;
        }
    }

    // 构造响应帧（回显起始地址和写入数量）
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS;
    Modbus.TX_Data[2] = (startAddress >> 8) & 0xFF;
    Modbus.TX_Data[3] = startAddress & 0xFF;
    Modbus.TX_Data[4] = (regCount >> 8) & 0xFF;
    Modbus.TX_Data[5] = regCount & 0xFF;

    // 计算CRC校验码
    uint16_t crc = calcCRC(Modbus.TX_Data, 6);
    Modbus.TX_Data[6] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[7] = crc & 0xFF;

    Modbus.u8TX_Data_Len = 8;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);


}





/**
 * 处理读取保持寄存器请求
 *
 * 本函数负责响应Modbus协议中的读取保持寄存器请求它从设备的保持寄存器中读取数据，
 * 并构造相应的响应帧发送给请求方
 */
void HandleReadHoldingRegisters(void) {
    uint16_t startAddress = Modbus.u16RegAddress;   // 协议地址（偏移地址，如 0 表示 40001）
    uint16_t regCount = Modbus.u16RegNumber;        // 寄存器数量

    // 转换为逻辑地址
    uint16_t logicalAddress = startAddress + MODBUS_HOLDING_REGISTER_START;

    // 转换为数组索引
    uint16_t startIndex = logicalAddress - MODBUS_HOLDING_REGISTER_START;

    // 地址范围验证
    if (startIndex + regCount > MODBUS_HOLDING_REGISTER_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_READ_HOLDING_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 构造响应帧
    Modbus.TX_Data[0] = Modbus.u8SrverID;   // 设备站号
    Modbus.TX_Data[1] = MODBUS_FUNC_READ_HOLDING_REGISTERS; // 功能码
    Modbus.TX_Data[2] = regCount * 2;       // 字节数

    // 从数组中读取数据并打包到 TX 缓冲区
    for (uint16_t i = 0; i < regCount; i++) {
        uint16_t regValue = holdingRegisters[startIndex + i]; // 使用 startIndex
        Modbus.TX_Data[3 + i * 2]     = (regValue >> 8) & 0xFF; // 高字节先发
        Modbus.TX_Data[3 + i * 2 + 1] = regValue & 0xFF;         // 低字节
    }

    // 计算响应长度和 CRC 校验
    uint16_t responseLength = 3 + regCount * 2;
    uint16_t crc = calcCRC(Modbus.TX_Data, responseLength);
    Modbus.TX_Data[responseLength]     = (crc >> 8) & 0xFF;
    Modbus.TX_Data[responseLength + 1] = crc & 0xFF;

    // 发送响应
    Modbus.u8TX_Data_Len = responseLength + 2;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}




