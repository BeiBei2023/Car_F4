
/**
 * ==================================================
 *  @file input_register.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午5:02
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "input_register.h"

uint16_t inputRegisters[MODBUS_INPUT_REGISTER_COUNT] = {0};


void InitInputRegisters(void) {
    // 初始化默认值
    inputRegisters[TEMP_SENSOR_REG_INDEX] = 1000;      // 初始温度 10°C
    inputRegisters[VOLTAGE_SENSOR_REG_INDEX] = 100;   // 默认电压 23.0V

}


uint16_t GetInputRegister(uint16_t index) {
    if (index >= MODBUS_INPUT_REGISTER_COUNT) return 0xFFFF;
    return inputRegisters[index];
}


bool IsInputRegisterValid(uint16_t regIndex, uint16_t value) {

    if (regIndex == TEMP_SENSOR_REG_INDEX) {

        //原始物理值 × 100，并转换为 uint16_t 类型

        // 支持温度范围：-40.00℃ ~ 85.00℃
        float temp = (float)value / 100.0f;
        return temp >= -40.0f && temp <= 85.0f;
    }

    if (regIndex == VOLTAGE_SENSOR_REG_INDEX) {
        float  voltage = (float)value / 100.0f;
        // 支持电压范围：0.00V ~ 100.00V
        return voltage >= 0.0f && voltage <= 24.0f;
    }
    return true; // 默认接受所有值
}



void HandleReadInputRegisters(void) {
    uint16_t startAddress = Modbus.u16RegAddress;     // 协议地址（偏移地址）
    uint16_t regCount = Modbus.u16RegNumber;           // 寄存器数量

    // 转换为逻辑地址和数组索引
    uint16_t logicalAddress = startAddress + MODBUS_INPUT_REGISTER_START;
    uint16_t startIndex = logicalAddress - MODBUS_INPUT_REGISTER_START;

    // 地址范围验证
    if (startIndex + regCount > MODBUS_INPUT_REGISTER_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_READ_INPUT_REGISTERS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 构造响应帧
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_READ_INPUT_REGISTERS;
    Modbus.TX_Data[2] = regCount * 2; // 字节数

    // 从数组中读取数据并打包到 TX 缓冲区
    for (uint16_t i = 0; i < regCount; i++) {
        uint16_t regValue = GetInputRegister(startIndex + i);
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