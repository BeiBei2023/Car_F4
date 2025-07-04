
/**
 * ==================================================
 *  @file system_voltage.c
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


#include "system_voltage.h"

#include "elog.h"
ADC_V_t adc_v;
float adc_v_get_value = 0;
uint16_t adc_buffer[ADC_BUFFER_SIZE];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc == &hadc1) {
        // 处理前半个缓冲区
        for (int i = 0; i < ADC_BUFFER_SIZE / 2; i++) {
            adc_v.v_in= ((float) adc_buffer[i] / 4096.0f * 3.3f) * 11.0f;
           //log_d("电压值 %.2f V", adc_v.v_in);
        }

    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc == &hadc1) {
        // 处理后半个缓冲区
        for (int i = ADC_BUFFER_SIZE / 2; i < ADC_BUFFER_SIZE; i++) {
            adc_v.v_in = ((float)adc_buffer[i] / 4096.0f * 3.3f) * 11.0f;
            //log_d("电压值 %.2f V", adc_v.v_in);
        }

    }
}
