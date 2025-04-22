#ifndef __AHT20_H__
#define __AHT20_H__

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>

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

#endif
