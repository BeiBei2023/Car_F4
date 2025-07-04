/**
 * ==================================================
 *  @file modbus_config.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午7:59
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef MODBUS_CONFIG_H
#define MODBUS_CONFIG_H


// Modbus异常码定义
#define MODBUS_EXCEPTION_ILLEGAL_FUNCTION       0x01    //  功能码错误
#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS  0x02     //  数据地址错误
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE    0x03     //  数据值错误
#define MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE  0x04     //  从设备故障


// 定义逻辑地址范围
#define MODBUS_COILS_START 1  // 线圈状态起始地址（逻辑地址 10001）
#define MODBUS_COILS_END 9999 // 线圈状态结束地址
#define MODBUS_COIL_COUNT (MODBUS_COILS_END - MODBUS_COILS_START +1)

#define MODBUS_DISCRETE_INPUT_START 10001 // 离散量输入起始地址（逻辑地址 00001）
#define MODBUS_DISCRETE_INPUT_END 19999   // 离散量输入结束地址

#define MODBUS_INPUT_REGISTER_START 30001 // 输入寄存器起始地址（逻辑地址 30001）
#define MODBUS_INPUT_REGISTER_END 39999   // 输入寄存器结束地址
#define MODBUS_INPUT_REGISTER_COUNT (MODBUS_INPUT_REGISTER_END - MODBUS_INPUT_REGISTER_START + 1)

#define MODBUS_HOLDING_REGISTER_START 40001 // 保持寄存器起始地址（逻辑地址 40001）
#define MODBUS_HOLDING_REGISTER_END 49999   // 保持寄存器结束地址
#define MODBUS_HOLDING_REGISTER_COUNT  (MODBUS_HOLDING_REGISTER_END - MODBUS_HOLDING_REGISTER_START + 1)

// Modbus功能码定义
#define MODBUS_FUNC_READ_COILS 0x01               // 读线圈
#define MODBUS_FUNC_READ_DISCRETE_INPUTS 0x02     // 读离散输入
#define MODBUS_FUNC_READ_HOLDING_REGISTERS 0x03   // 读保持寄存器
#define MODBUS_FUNC_READ_INPUT_REGISTERS 0x04     // 读输入寄存器
#define MODBUS_FUNC_WRITE_SINGLE_COILS 0x05       // 写单个线圈
#define MODBUS_FUNC_WRITE_SINGLE_REGISTER 0x06    // 写单个保持寄存器
#define MODBUS_FUNC_WRITE_MULTIPLE_COILS 0x0F     // 写多个线圈
#define MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS 0x10 // 写多个寄存器





#endif //MODBUS_CONFIG_H
