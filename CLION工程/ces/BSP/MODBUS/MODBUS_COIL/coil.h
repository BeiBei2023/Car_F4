/**
 * ==================================================
 *  @file coil.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午6:49
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef COIL_H
#define COIL_H

#include <stdint.h>
#include "modbus_function_code.h"
#include "modbus_config.h"


typedef enum {
    LED_RED_COIL_INDEX = 0, // 红灯LED
    LED_GREEN_COIL_INDEX, // 绿色LED
    LED_BLUE_COIL_INDEX, // 蓝色LED

    GPIO_PE11_COIL_INDEX, //  GPIO_PE11
    GPIO_PE9_COIL_INDEX, //  GPIO_PE9
    GPIO_PE14_COIL_INDEX, //  GPIO_PE14
    GPIO_PE13_COIL_INDEX, //  GPIO_PE13
    GPIO_PB5_COIL_INDEX, //  GPIO_PB5
    GPIO_PA6_COIL_INDEX, //  GPIO_PA6
    GPIO_PB1_COIL_INDEX, //  GPIO_PB1
    GPIO_PB0_COIL_INDEX //  GPIO_PB0
} CoilIndex;

/********************************分配端口和引脚****************************************/
#define LED_RED_PORT LED_RED_GPIO_Port
#define LED_RED_PIN LED_RED_Pin

#define LED_GREEN_PORT LED_GREEN_GPIO_Port
#define LED_GREEN_PIN LED_GREEN_Pin

#define LED_BLUE_PORT LED_BLUE_GPIO_Port
#define LED_BLUE_PIN LED_BLUE_Pin

#define GPIO_PE11_PORT OUT1_GPIO_Port
#define GPIO_PE11_PIN OUT1_Pin

#define GPIO_PE9_PORT OUT2_GPIO_Port
#define GPIO_PE9_PIN OUT2_Pin

#define GPIO_PE14_PORT OUT3_GPIO_Port
#define GPIO_PE14_PIN OUT3_Pin

#define GPIO_PE13_PORT OUT4_GPIO_Port
#define GPIO_PE13_PIN OUT4_Pin

#define GPIO_PB5_PORT OUT5_GPIO_Port
#define GPIO_PB5_PIN OUT5_Pin

#define GPIO_PA6_PORT OUT6_GPIO_Port
#define GPIO_PA6_PIN OUT6_Pin

#define GPIO_PB1_PORT OUT7_GPIO_Port
#define GPIO_PB1_PIN OUT7_Pin

#define GPIO_PB0_PORT OUT8_GPIO_Port
#define GPIO_PB0_PIN OUT8_Pin

/***************************************************************************************/



extern uint8_t coilRegisters[MODBUS_COIL_COUNT];

// 初始化线圈寄存器
void InitCoils(void);

// 读取线圈状态（带边界检查）
uint8_t GetCoil(CoilIndex index,bool fromHardware);

// 设置线圈状态（带边界检查和合法性验证）
bool SetCoil(CoilIndex index, uint8_t value);

// 检查线圈值是否合法
bool IsCoilValueValid(CoilIndex index, uint8_t value);

// 处理 Modbus 功能码 0x01（Read Coils）
void HandleReadCoils(void);

// 处理 Modbus 功能码 0x05 / 0x0F（Write Single/Multiple Coils）
void HandleWriteSingleCoil(void);
void HandleWriteMultipleCoils(void);


#endif //COIL_H
