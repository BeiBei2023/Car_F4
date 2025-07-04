/**
 * ==================================================
 *  @file modbus_init.h
 *  @brief TODO 初始化Modbus设备
 *  @author wangb
 *  @date 2025-06-05 下午3:21
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef MODBUS_INIT_H
#define MODBUS_INIT_H

#include "../MODBUS_BASE/modbus.h"
#include "coil.h"

#include "holding_register.h"
#include "input_register.h"

void Modbus_Init(uint8_t device_address, UART_HandleTypeDef *huart);

#endif //MODBUS_INIT_H
