//
// Created by wangb on 25-6-3.
//

#ifndef UART_CALLBACK_H
#define UART_CALLBACK_H


#include  "main.h"
#include  "stdio.h"
#include  "string.h"
#include "../MODBUS/MODBUS_BASE/modbus.h"
#include "cmsis_os2.h"
#include "sbus.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

//创建数组，用于存放Modbus数据
extern uint8_t Modbus_Data_Array[256];
//创建数组，用于存放SBUS数据
extern uint8_t SBUS_Data_Array[SBUS_FRAME_LENGTH];

void USER_UART_INIT(void);

#endif //UART_CALLBACK_H
