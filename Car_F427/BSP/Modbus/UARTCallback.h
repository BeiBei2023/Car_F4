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

void HAL_USART_INIT(void);

#endif // !__UARTCALLBACK_H_
