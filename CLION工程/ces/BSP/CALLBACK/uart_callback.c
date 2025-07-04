//
// Created by wangb on 25-6-3.
//
#include  "uart_callback.h"

#include <stdlib.h>


#ifdef __GNUC__
// 如果是GCC编译器，定义PUTCHAR_PROTOTYPE为int __io_putchar(int ch)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
// 否则，定义PUTCHAR_PROTOTYPE为int fputc(int ch, FILE *f)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

// 定义PUTCHAR_PROTOTYPE函数
PUTCHAR_PROTOTYPE {
    // 使用HAL库的UART传输函数，将字符ch发送到UART3
    HAL_UART_Transmit(&huart3, (uint8_t *) &ch, 1, HAL_MAX_DELAY);
    // 返回字符ch
    return ch;
}

extern ModbusData Modbus;
extern sbus_data sbus_ch_data;

//创建数组，用于存放Modbus数据                          mnnnnnnnnnnnnnn,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
uint8_t Modbus_Data_Array[256];
//创建数组，用于存放SBUS数据
uint8_t SBUS_Data_Array[25];


//用于初始串口相关的函数
void USER_UART_INIT(void) {
    elog_info("串口中断", "初始化UART6");

    // 等待 UART6 就绪
    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY) {
        // 可以添加超时机制避免死循环
    }

    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
    if (HAL_UARTEx_ReceiveToIdle_DMA(&huart6, Modbus_Data_Array, sizeof(Modbus_Data_Array)) != HAL_OK) {
        elog_error("UART6", "DMA接收启动失败");
    }
    elog_info("串口初始化完成", "UART6初始化完成");


    elog_info("串口初始化", "UART1初始化");

    // 等待 UART1 就绪
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) {
        // 可以添加超时机制避免死循环
    }

    // __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    // 接收非空中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //  IDLE中断



    // 清空状态
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    __HAL_UART_CLEAR_PEFLAG(&huart1);
    __HAL_UART_CLEAR_FEFLAG(&huart1);


    if (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE)) {
        log_i("UART1 空闲中断已启用");
    } else {
        log_e("UART1 空闲中断未启用！");
    }

    if (NVIC_GetEnableIRQ(USART1_IRQn)) {
        log_i("UART1 中断已启用");
    } else {
        log_e("UART1 中断未启用！");
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(&huart1, SBUS_Data_Array, 25) != HAL_OK) {
        elog_error("UART1", "DMA接收启动失败");
    }
    elog_info("串口初始化完成", "UART1初始化完成");
}

//Modbus接收数据
extern osSemaphoreId_t modbus_data_semaphoreHandle; //  Modbus数据信号量
extern osMutexId_t modbus_buffer_mutexHandle; //  Modbus数据互斥锁

//SBUS接收数据
extern osSemaphoreId_t sbus_data_semaphoreHandle; //  SBUS数据信号量
extern osMutexId_t sbus_buffer_mutexHandle; //  SBUS数据互斥锁


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
log_d("callback");
    if (huart->Instance == USART1) {
        log_d("收到 SBUS 数据，长度：%d", Size);
        memcpy(sbus_ch_data.frame_data, SBUS_Data_Array, Size);
        sbus_ch_data.length = Size;
       osSemaphoreRelease(sbus_data_semaphoreHandle);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, SBUS_Data_Array, sizeof(SBUS_Data_Array));
    }
   else if (huart->Instance == USART6 && Size > 0) {
        memcpy(Modbus.u8Data_backup, Modbus_Data_Array, Size);
        Modbus.data_length = Size;
        osSemaphoreRelease(modbus_data_semaphoreHandle);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart6, Modbus_Data_Array, 25);
    }
}
