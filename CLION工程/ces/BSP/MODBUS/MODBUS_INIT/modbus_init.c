
/**
 * ==================================================
 *  @file modbus_init.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-05 下午3:21
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "modbus_init.h"



// 在 modbus.c 中新增辅助函数
static const char *GetUsartName(UART_HandleTypeDef *huart) {
    if (!huart || !huart->Instance) return "NULL";

    // ✅ 通过 Instance 指针匹配 USART 名称
    if (huart->Instance == USART1) return "USART1";
    if (huart->Instance == USART2) return "USART2";
    if (huart->Instance == USART3) return "USART3";
    if (huart->Instance == UART4) return "UART4";
    if (huart->Instance == UART5) return "UART5";
    if (huart->Instance == USART6) return "USART6";
    if (huart->Instance == UART7) return "UART7";
    if (huart->Instance == UART8) return "UART8";

    return "UNKNOWN";
}


static void LogUsartConfig(UART_HandleTypeDef *huart) {
    elog_info("Modbus", "[Modbusd-串口 配置详情]");
    elog_info("Modbus", "  名称: %s", GetUsartName(huart));
    elog_info("Modbus", "  波特率: %lu bps", huart->Init.BaudRate);
    elog_info("Modbus", "  数据位: %d", huart->Init.WordLength == UART_WORDLENGTH_8B ? 8 : 9);
    elog_info("Modbus", "  停止位: %d", huart->Init.StopBits + 1);
}


/**
 * @brief 初始化Modbus设备
 *
 * 该函数用于设置Modbus设备的地址，并选择用于通信的UART端口
 * 通过这两个步骤，可以将设备配置为Modbus网络中的一个节点
 *
 * @param device_address 设备的从站地址，范围为1到247，根据Modbus协议规定
 * @param huart 指向UART_HandleTypeDef的指针，用于选择通信端口
 */
void Modbus_Init(uint8_t device_address, UART_HandleTypeDef *huart) {
    Modbus.MODBUS_SEVER_ID = device_address;
    elog_info("Modbus", "设置——>设备地址成功，地址为: %02x", Modbus.MODBUS_SEVER_ID);
    Modbus.huart = huart;
    elog_info("Modbus", "设置——>端口选择成功，端口为: %s", GetUsartName(huart));
    LogUsartConfig(huart);
    InitHoldingRegisters(); //  初始化保持寄存器的数值
    InitInputRegisters();   //  初始化输入寄存器的数值
    InitCoils();           //  初始化线圈寄存器的数值
}

