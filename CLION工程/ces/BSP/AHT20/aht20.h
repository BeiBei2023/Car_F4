/**
 * ==================================================
 *  @file aht20.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午3:09
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef AHT20_H
#define AHT20_H
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>
#include "elog.h"
#include "cmsis_os2.h"

 extern I2C_HandleTypeDef hi2c2;
#define AHT20_ADDRESS 0x70

typedef struct {
    float Temperature;
    float Humidity;
} AHT20_Data_t;

// 初始化AHT20
void AHT20_Init();

// 获取温度和湿度
void AHT20_Read(AHT20_Data_t *data);

#endif //AHT20_H
