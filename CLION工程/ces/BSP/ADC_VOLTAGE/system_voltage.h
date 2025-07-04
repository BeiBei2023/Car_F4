/**
 * ==================================================
 *  @file system_voltage.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午4:54
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef SYSTEM_VOLTAGE_H
#define SYSTEM_VOLTAGE_H


#include "main.h"
#include "adc.h"
#include "string.h"
#include "stdio.h"
#include <stdint.h>

// #include "custom.h"  // 确保能访问current_voltage


extern ADC_HandleTypeDef hadc1;

//  ADC1的缓存大小
#define ADC_BUFFER_SIZE  256
extern  uint16_t adc_buffer[ADC_BUFFER_SIZE];

/**
 * @brief 结构体，存放读取到的电压值
 *
 */
typedef struct
{
    float v_in;     //输入电压
} ADC_V_t;

extern ADC_V_t adc_v;

#endif //SYSTEM_VOLTAGE_H
