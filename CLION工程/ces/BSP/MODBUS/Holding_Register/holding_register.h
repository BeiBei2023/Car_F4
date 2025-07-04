/**
 * ==================================================
 *  @file holding_register.h
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



#ifndef HOLDING_REGISTER_H
#define HOLDING_REGISTER_H

#include <stdint.h>
#include "../MODBUS_FUNCTION/modbus_function_code.h"
#include "aht20.h"


// 寄存器索引定义（与 modbus_function_code.h 中一致）
typedef enum {
    // TEMP_SENSOR_REG_INDEX = 0,  // 温度传感器
    // CURRENT_REG_INDEX         // 电流测量
     SERVER_ID //从站设备地址
} HoldingRegisterIndex;

extern  uint16_t holdingRegisters[MODBUS_HOLDING_REGISTER_COUNT];


void InitHoldingRegisters(void);

void HandleWriteSingleRegister(void);   //  写单个保持寄存器
void HandleReadHoldingRegisters(void);  //  读保持寄存器
void HandleWriteMultipleRegisters(void); //  写多个保持寄存器
bool IsRegisterValueValid(uint16_t regIndex, uint16_t value) ;

#endif //HOLDING_REGISTER_H
