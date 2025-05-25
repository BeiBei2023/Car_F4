/*
 * Modbus.h
 *
 *  Created on: May 5, 2020
 *      Author: Alejandro Mera
 */

#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include "timers.h"
#include "semphr.h"
#include <string.h>
#include "elog.h"

#include <stdint.h>
#include "aht20.h"

extern UART_HandleTypeDef huart3;

// Modbus功能码定义
#define MODBUS_FUNC_READ_COILS 0x01               // 读线圈
#define MODBUS_FUNC_READ_DISCRETE_INPUTS 0x02     // 读离散输入
#define MODBUS_FUNC_READ_HOLDING_REGISTERS 0x03   // 读保持寄存器
#define MODBUS_FUNC_READ_INPUT_REGISTERS 0x04     // 读输入寄存器
#define MODBUS_FUNC_WRITE_SINGLE_COILS 0x05       // 写单个线圈
#define MODBUS_FUNC_WRITE_SINGLE_REGISTER 0x06    // 写单个保持寄存器
#define MODBUS_FUNC_WRITE_MULTIPLE_COILS 0x0F     // 写多个线圈
#define MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS 0x10 // 写多个寄存器

// 定义逻辑地址范围

#define MODBUS_COILS_START 1  // 线圈状态起始地址（逻辑地址 10001）
#define MODBUS_COILS_END 9999 // 线圈状态结束地址

#define MODBUS_DISCRETE_INPUT_START 10001 // 离散量输入起始地址（逻辑地址 00001）
#define MODBUS_DISCRETE_INPUT_END 19999   // 离散量输入结束地址

#define MODBUS_INPUT_REGISTER_START 30001 // 输入寄存器起始地址（逻辑地址 30001）
#define MODBUS_INPUT_REGISTER_END 39999   // 输入寄存器结束地址

#define MODBUS_HOLDING_REGISTER_START 40001 // 保持寄存器起始地址（逻辑地址 40001）
#define MODBUS_HOLDING_REGISTER_END 49999   // 保持寄存器结束地址

// 全局变量声明
#define HOLDING_REGISTER_COUNT 20
extern uint16_t HoldingRegisters[HOLDING_REGISTER_COUNT];

#define INPUT_REGISTER_COUNT 50
extern uint16_t InputRegisters[INPUT_REGISTER_COUNT];

#define COILS_COUNT 100
extern uint8_t Coils[COILS_COUNT / 8];

#define DISCRETE_INPUT_COUNT 50
extern uint8_t DiscreteInputs[DISCRETE_INPUT_COUNT / 8];

// modbus 的数据结构体
typedef struct
{
    uint8_t MODBUS_SEVER_ID; // 从站地址

    uint8_t Rx_Flag;        // 接收标志位
    uint8_t u8Data[256];    // 数据缓冲区，存放接收到的数据
    uint8_t u8SrverID;      // 从站地址
    uint8_t u8FunctionCode; // 功能码

    uint16_t u16CoilsAddress; // 线圈地址
    uint16_t u16CoilsNumber;  // 线圈数量

    // 特殊处理多个线圈
    uint8_t u8byteCount; // 字节数
    uint16_t u16CoilsData[256]; // 线圈数据

    uint16_t u16CRC;            // CRC 校验码
    uint16_t u16CRC_Calculated; // 计算出的 CRC 校验码

    uint8_t RX_Data[256];  // 接收数据缓冲区
    uint8_t TX_Data[256];  // 发送数据缓冲区
    uint8_t u8TX_Data_Len; // 发送数据长度

} ModbusData;

// 声明结构体
extern ModbusData Modbus;

void Modbus_Init(uint8_t device_address); // 可以设置设备地址

uint16_t calcCRC(uint8_t *Buffer, uint8_t u8length);
void Modbus_ExtractData(void);
uint16_t GetHoldingRegisterIndex(uint16_t logicalAddress);
uint16_t ReadSingleHoldingRegister(uint16_t logicalAddress);

#endif /* __MODBUS_H_ */