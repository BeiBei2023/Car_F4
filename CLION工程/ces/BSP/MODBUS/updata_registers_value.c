
/**
 * ==================================================
 *  @file updata_registers_value.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午6:09
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "updata_registers_value.h"

void UpdateRegistersTask(void *argument) {
     AHT20_Data_t modbus_aht2o_data;
    uint16_t voltageADCValue = 0;

    for (;;) {

        /***********************AHT20数值***********************/
        AHT20_Read(&modbus_aht2o_data);

        // 获取原始温度（float 类型）
        float rawTemp = modbus_aht2o_data.Temperature;

        // 转换为 uint16_t 类型，单位 0.01℃（乘以 100 并四舍五入）
        uint16_t currentTemp = (uint16_t)(rawTemp * 100.0f + 0.5f);



        // 验证数据有效性（确保在寄存器允许范围内）
        if (IsRegisterValueValid(TEMP_SENSOR_REG_INDEX, currentTemp)) {
            inputRegisters[TEMP_SENSOR_REG_INDEX] = currentTemp;
        }

        /***********************ADC电压数值***********************/

        float adc_value = adc_v.v_in;
        voltageADCValue = (uint16_t)(adc_value * 100.0f + 0.5f);

        if (IsRegisterValueValid(VOLTAGE_SENSOR_REG_INDEX, voltageADCValue)) {
            inputRegisters[VOLTAGE_SENSOR_REG_INDEX] = voltageADCValue;
        }

        osDelay(10); // 每 10ms 更新一次
    }
}


