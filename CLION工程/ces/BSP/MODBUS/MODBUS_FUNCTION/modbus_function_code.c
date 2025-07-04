
/**
 * ==================================================
 *  @file modbus_function_code.c
 *  @brief TODO Modbus功能码对应逻辑
 *  @author wangb
 *  @date 2025-06-05 下午3:25
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "modbus_function_code.h"


/**
 * 发送异常响应
 *
 * 当Modbus服务器在执行功能码对应的操作时遇到错误，会使用此函数发送异常响应给客户端
 * 异常响应包括服务器ID、带有异常标识的功能码和异常代码，以及CRC校验码
 *
 * @param functionCode 功能码，用于标识Modbus协议中的不同功能
 * @param exceptionCode 异常代码，用于标识发生的具体异常
 */
void SendExceptionResponse(uint8_t functionCode, uint8_t exceptionCode) {
    // 设置响应数据包的服务器ID
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    // 设置带有异常标识的功能码，通过在功能码上设置最高位来标识这是一个异常响应
    Modbus.TX_Data[1] = functionCode | 0x80; // 异常标识位
    // 设置异常代码，告知客户端具体的异常类型
    Modbus.TX_Data[2] = exceptionCode;

    // 计算CRC校验码，确保数据的完整性
    uint16_t crc = calcCRC(Modbus.TX_Data, 3); // 计算 CRC
    // 将CRC校验码的高位和低位分别存入发送数据包中
    Modbus.TX_Data[3] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[4] = crc & 0xFF;
    // 设置发送数据包的长度
    Modbus.u8TX_Data_Len = 5;

    // 调用函数发送组装好的数据包
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}









//根据不同功能码，做出对应的动作
void ProcessFunctionCode(void) {
    if (Modbus.CRC_OK == 1) {
        // 在进入 switch 前添加
        if (Modbus.u8SrverID != Modbus.MODBUS_SEVER_ID && Modbus.u8SrverID != 0) {
            MODBUS_ERROR("Invalid server ID: %d", Modbus.u8SrverID);
            return;
        }

        switch (Modbus.u8FunctionCode) {
            case MODBUS_FUNC_READ_COILS: // 读取线圈
                HandleReadCoils();
                break;

            case MODBUS_FUNC_READ_DISCRETE_INPUTS: // 读取离散输入
                //HandleReadDiscreteInputs();
                break;

            case MODBUS_FUNC_READ_HOLDING_REGISTERS: // 读取保持寄存器
                HandleReadHoldingRegisters();
                break;

            case MODBUS_FUNC_READ_INPUT_REGISTERS: // 读取输入寄存器
                HandleReadInputRegisters();
                break;

            case MODBUS_FUNC_WRITE_SINGLE_COILS: // 写入单个线圈
                HandleWriteSingleCoil();
                break;

            case MODBUS_FUNC_WRITE_SINGLE_REGISTER: // 写入单个寄存器
                HandleWriteSingleRegister();
                break;

            case MODBUS_FUNC_WRITE_MULTIPLE_COILS: // 写入多个线圈
               HandleWriteMultipleCoils();
                break;

            case MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS: // 写入多个寄存器
                HandleWriteMultipleRegisters();
                break;

            default:
                MODBUS_ERROR("Unsupported function code: 0x%02X", Modbus.u8FunctionCode);
                SendExceptionResponse(Modbus.u8FunctionCode, MODBUS_EXCEPTION_ILLEGAL_FUNCTION);
                Modbus.CRC_OK = 0;
                break;
        }
    }
}