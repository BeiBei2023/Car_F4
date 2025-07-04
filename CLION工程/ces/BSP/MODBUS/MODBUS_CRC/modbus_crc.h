/**
 * ==================================================
 *  @file modbus_crc.h
 *  @brief TODO 使用查表法计算CRC16
 *  @author wangb
 *  @date 2025-06-05 下午3:18
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include "stdio.h"
#include  "stdint.h"


uint16_t calcCRC(uint8_t *buffer, uint8_t length);
#endif //MODBUS_CRC_H
