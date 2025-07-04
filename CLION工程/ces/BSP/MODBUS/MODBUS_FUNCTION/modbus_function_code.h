/**
 * ==================================================
 *  @file modbus_function_code.h
 *  @brief TODO Modbus功能码对应逻辑
 *  @author wangb
 *  @date 2025-06-05 下午3:25
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef MODBUS_FUNCTION_CODE_H
#define MODBUS_FUNCTION_CODE_H

#include "modbus.h"
#include <stdint.h>
#include "coil.h"
#include "input_register.h"
#include "holding_register.h"
#include "modbus_config.h"




#define HOLDING_REG(index) \
((index) < HOLDING_REGISTER_COUNT ? holdingRegisters[index] : 0xFFFF)

// 定义宏HOLDING_REG以安全地访问holdingRegisters数组
// 参数index: 寄存器的索引
// 如果index在有效范围内，则返回对应索引的寄存器值
// 如果index超出有效范围，则返回0xFFFF作为错误指示



void SendExceptionResponse(uint8_t functionCode, uint8_t exceptionCode);
void ProcessFunctionCode(void);
#endif //MODBUS_FUNCTION_CODE_H
