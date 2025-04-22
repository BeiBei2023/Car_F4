#include "adc_v.h"


uint16_t adc_v_get_value = 0;

ADC_V_t adc_v;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adc_v_get_value = HAL_ADC_GetValue(&hadc1); // 获取ADC1的值
    adc_v.v_in = (adc_v_get_value/4096.0f*3.3f)*11.0f;  // 获取电压值
}









