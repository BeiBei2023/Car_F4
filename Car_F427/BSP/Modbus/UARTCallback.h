#ifndef __UARTCALLBACK_H_
#define __UARTCALLBACK_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"
#include "Modbus.h"
#include "elog.h"
#include "main.h"
#include <string.h>
#include <stdio.h> // 包含标准输入输出头文件
#include "sbus.h"

extern uint8_t USART3_RX_BUFFER[256];
extern uint8_t RcUartBuffer[25]; // 正确声明
void USER_USART_INIT(void);

#endif // !__UARTCALLBACK_H_
