/**
 * ==================================================
 *  @file updata_registers_value.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午6:09
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef UPDATA_REGISTERS_VALUE_H
#define UPDATA_REGISTERS_VALUE_H

#include "holding_register.h"
#include "input_register.h"
#include "system_voltage.h"
#include "coil.h"

void UpdateRegistersTask(void *argument);   //  保持寄存器的数据更新


#endif //UPDATA_REGISTERS_VALUE_H
