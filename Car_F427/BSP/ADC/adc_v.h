#ifndef __ADC_V_H
#define __ADC_V_H


#include "main.h"
#include "adc.h"
#include "string.h"
#include "stdio.h"
#include <stdint.h>

extern ADC_HandleTypeDef hadc1;

/**
 * @brief 结构体，存放读取到的电压值
 * 
 */
typedef struct
{
    float v_in;     //输入电压
} ADC_V_t;

extern ADC_V_t adc_v;

#endif // ! __ADC_V_H
