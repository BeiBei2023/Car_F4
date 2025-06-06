#include "aht20.h"
AHT20_Data_t aht20_data; // 用于存储AHT20传感器数据
/**
 * @brief  初始化AHT20
 */
void AHT20_Init()
{
  log_i("AHT20: AHT20_Init start");
  uint8_t readBuffer;
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);
  if ((readBuffer & 0x08) == 0x00)
  {
    log_i("AHT20: Sensor detected, preparing to initialize");
    uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};
    HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
    log_i("AHT20: Initialization command sent");
  }

  HAL_Delay(200); // 等待传感器初始化完成
  log_i("AHT20: Initialization complete, testing sensor");

  // 测试
  AHT20_Read(&aht20_data);
  log_i("AHT20: Test data: Humidity = %f, Temperature = %f", aht20_data.Humidity, aht20_data.Temperature);
}
/**
 * @brief  获取温度和湿度
 * @param  Temperature: 存储获取到的温度
 * @param  Humidity: 存储获取到的湿度
 */
void AHT20_Read(AHT20_Data_t *data)
{
  uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
  uint8_t readBuffer[6] = {0};

  HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
  HAL_Delay(75);
  HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);

  if ((readBuffer[0] & 0x80) == 0x00)
  {
    uint32_t dataHumidity = 0;
    dataHumidity = ((uint32_t)readBuffer[3] >> 4) + ((uint32_t)readBuffer[2] << 4) + ((uint32_t)readBuffer[1] << 12);
    data->Humidity = dataHumidity * 100.0f / (1 << 20);

    uint32_t dataTemperature = 0;
    dataTemperature = (((uint32_t)readBuffer[3] & 0x0F) << 16) + ((uint32_t)readBuffer[4] << 8) + (uint32_t)readBuffer[5];
    data->Temperature = dataTemperature * 200.0f / (1 << 20) - 50;
  }
}
