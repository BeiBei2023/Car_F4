/**
 * ==================================================
 *  @file input_register.h
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



#ifndef INPUT_REGISTER_H
#define INPUT_REGISTER_H

#include <stdint.h>
#include "modbus_function_code.h"
#include "modbus_config.h"

// 寄存器索引定义
typedef enum {
    TEMP_SENSOR_REG_INDEX = 0,      // 温度传感器
    VOLTAGE_SENSOR_REG_INDEX    // 电压传感器

    // 其他只读寄存器...
} InputRegisterIndex;

// 输入寄存器数组（只读）
extern  uint16_t inputRegisters[MODBUS_INPUT_REGISTER_COUNT] ;

// 初始化输入寄存器
void InitInputRegisters(void);

// 读取输入寄存器值（带边界检查）
uint16_t GetInputRegister(uint16_t index);

// 处理 Modbus 功能码 0x04 的请求
void HandleReadInputRegisters(void);
void HandleReadInputRegisters(void); //  处理 Modbus 功能码 0x04 的请求
bool IsInputRegisterValid(uint16_t regIndex, uint16_t value) ;

#endif //INPUT_REGISTER_H
