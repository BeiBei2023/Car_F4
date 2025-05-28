#include "adc_v.h"


uint16_t adc_v_get_value = 0;

ADC_V_t adc_v;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adc_v_get_value = HAL_ADC_GetValue(&hadc1); // 获取ADC1的值
    adc_v.v_in = (adc_v_get_value/4096.00f*3.30f)*11.00f;  // 获取电压值
    //将电压值传入屏幕的全局变量
    current_voltage = adc_v.v_in; // 更新全局电压变量
}









