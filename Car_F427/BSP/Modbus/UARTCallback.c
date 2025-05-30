/*
 * UARTCallback.c
 *
 *  Created on: May 27, 2020
 *      Author: Alejandro Mera
 */

#include "UARTCallback.h"

ModbusData Modbus;
uint8_t USART6_RX_BUFFER[256];

uint8_t RcUartBuffer[25]; // 正确声明

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

#define Sprint_port &huart6
int fputc(int ch, FILE *f)
{
  // 采用轮询方式发送1字节数据，超时时间设置为无限等待
  HAL_UART_Transmit(Sprint_port, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

int fgetc(FILE *f)
{
  uint8_t ch;
  // 采用轮询方式接收 1字节数据，超时时间设置为无限等待
  HAL_UART_Receive(Sprint_port, (uint8_t *)&ch, 1, 1000);
  return ch;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

  if (huart->Instance == USART3)
  {

    Modbus.Rx_Flag = 1; // 设置接收标志位

    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, USART6_RX_BUFFER, 256); // 重新开始接收数据

    memcpy(Modbus.u8Data, USART6_RX_BUFFER, Size); // 将接收到的数据存入结构体
  }
  if (huart->Instance == USART1)
  {

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RcUartBuffer, 25); // 用IDLE和中断接收

    sbus_decode_frame(RcUartBuffer);

    //__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  }
}

void USER_USART_INIT(void)
{
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);                  // 使能USART6接收中断
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, USART6_RX_BUFFER, 256); // 开始接收数据

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);             // 用IDLE和中断接收
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RcUartBuffer, 25); // 用IDLE和中断接收
}

// 串口接收错误中断回调函数
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART6)
  {
    if (huart->ErrorCode == HAL_UART_ERROR_ORE)
    {
      __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);                  // 使能USART6接收中断
      HAL_UARTEx_ReceiveToIdle_DMA(&huart3, USART6_RX_BUFFER, 256); // 重新开启接收
    }
  }
}
