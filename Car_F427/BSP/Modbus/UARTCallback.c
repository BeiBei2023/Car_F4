/*
 * UARTCallback.c
 *
 *  Created on: May 27, 2020
 *      Author: Alejandro Mera
 */

#include "UARTCallback.h"

uint8_t USART3_RX_BUFFER[256];

uint8_t RcUartBuffer[25]; // 正确声明
// 声明结构体
extern ModbusData Modbus;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

#define Sprint_port &huart3
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

//实现日志打印函数




/**
 * @brief UART接收事件回调函数
 *
 * 该函数在UART外设接收到数据时被调用。它处理接收到的数据，并根据不同的UART实例执行特定的操作。
 *
 * @param huart UART_HandleTypeDef结构体的指针，包含UART外设的配置信息。
 * @param Size 接收到的数据大小。
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  // 切换红色LED状态，用于指示接收事件的发生
  HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
  // 打印接收到的数据大小，用于调试目的
  log_d("HAL_UARTEx_RxEventCallback Size: %d", Size);

  // 检查UART实例是否为USART1
  if (huart->Instance == USART1)
  {
    // 清除标志位
    //  __HAL_UART_CLEAR_IDLEFLAG(huart); // 清除IDLE标志位
    //  使用DMA和IDLE中断接收数据，缓冲区大小为25
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RcUartBuffer, sizeof(RcUartBuffer));

    // 解码接收到的SBus帧
    sbus_decode_frame(RcUartBuffer);
  }

  // 检查UART实例是否为USART6
  if (huart->Instance == USART6)
  {

    // 设置接收标志位，表示数据已接收
    Modbus.Rx_Flag = 1;
    // __HAL_UART_CLEAR_IDLEFLAG(huart); // 清除IDLE标志位
    // 使用DMA和IDLE中断重新开始接收数据，缓冲区大小为256
    HAL_UARTEx_ReceiveToIdle_DMA(&huart6, USART3_RX_BUFFER, sizeof(USART3_RX_BUFFER));

    // 将接收到的数据从缓冲区复制到Modbus数据结构中
    memcpy(Modbus.u8Data, USART3_RX_BUFFER, Size);
  }
}


void USER_USART_INIT(void)
{
  log_d("USER_USART6_INIT");
  // 初始化USART1和USART6的接收中断
  __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);                                                      // 使能USART6接收中断
  int8_t state = HAL_UARTEx_ReceiveToIdle_DMA(&huart6, USART3_RX_BUFFER, sizeof(USART3_RX_BUFFER)); // 开始接收数据
                                                                                                    // 打印这个的返回的信息
  log_d("huart6.RxState: %d", state);

  log_d("USER_USART1_INIT");

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);                                       // 用IDLE和中断接收
  state = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RcUartBuffer, sizeof(RcUartBuffer)); // 用IDLE和中断接收
                                                                                     // 打印这个的返回的信息
  log_d("huart1.RxState: %d", state);
  log_d("USER_USART_INIT completed");
}
