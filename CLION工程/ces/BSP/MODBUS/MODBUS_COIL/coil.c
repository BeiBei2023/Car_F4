/**
 * ==================================================
 *  @file coil.c
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-06 下午6:49
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */


#include "coil.h"

// 定义线圈寄存器数组
uint8_t coilRegisters[MODBUS_COIL_COUNT] = {0};


// 初始化线圈状态
void InitCoils(void) {
    //默认关闭所有
    for (int i = 0; i < MODBUS_COIL_COUNT; i++) {
        SetCoil(i, 0);
    }
}

// 获取线圈状态（带边界检查）
uint8_t GetCoil(CoilIndex index,bool fromHardware) {
    if (index >= MODBUS_COIL_COUNT) return 0xFF;

    if (fromHardware) {
        switch (index) {
            case LED_RED_COIL_INDEX:
                return HAL_GPIO_ReadPin(LED_RED_PORT, LED_RED_PIN);
            case LED_GREEN_COIL_INDEX:
                return HAL_GPIO_ReadPin(LED_GREEN_PORT, LED_GREEN_PIN);
            case LED_BLUE_COIL_INDEX:
                return HAL_GPIO_ReadPin(LED_BLUE_PORT, LED_BLUE_PIN);
            case GPIO_PA6_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PA6_PORT, GPIO_PA6_PIN);
            case GPIO_PB0_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PB0_PORT, GPIO_PB0_PIN);
            case GPIO_PB1_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PB1_PORT, GPIO_PB1_PIN);
            case GPIO_PB5_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PB5_PORT, GPIO_PB5_PIN);
            case GPIO_PE11_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PE11_PORT, GPIO_PE11_PIN);
            case GPIO_PE13_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PE13_PORT, GPIO_PE13_PIN);
            case GPIO_PE14_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PE14_PORT, GPIO_PE14_PIN);
            case GPIO_PE9_COIL_INDEX:
                return HAL_GPIO_ReadPin(GPIO_PE9_PORT, GPIO_PE9_PIN);
            default:
                return 0xFF;
        }
    }
}


// 设置线圈状态（带合法性验证）
bool SetCoil(CoilIndex index, uint8_t value) {
    if (index >= MODBUS_COIL_COUNT) return false;
    if (!IsCoilValueValid(index, value)) return false;

    coilRegisters[index] = value;

    // 根据线圈索引绑定到对应的 GPIO
    switch (index) {
        case LED_RED_COIL_INDEX:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case LED_GREEN_COIL_INDEX:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case LED_BLUE_COIL_INDEX:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PE11_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PE11_PORT, GPIO_PE11_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PE13_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PE13_PORT, GPIO_PE13_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PE14_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PE14_PORT, GPIO_PE14_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PE9_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PE9_PORT, GPIO_PE9_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PA6_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PA6_PORT, GPIO_PA6_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PB0_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PB0_PORT, GPIO_PB0_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PB1_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PB1_PORT, GPIO_PB1_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case GPIO_PB5_COIL_INDEX:
            HAL_GPIO_WritePin(GPIO_PB5_PORT, GPIO_PB5_PIN, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        default:
            return false;
    }

    return true;
}

// 校验线圈值是否合法（例如只能是 0 或 1）
bool IsCoilValueValid(CoilIndex index, uint8_t value) {
    (void) index; // 可根据需要扩展不同线圈的规则
    return value == 0 || value == 1;
}


void HandleReadCoils(void) {
    uint16_t startAddress = Modbus.u16RegAddress; // 协议地址（偏移地址）
    uint16_t coilCount = Modbus.u16RegNumber; // 线圈数量

    // 转换为逻辑地址和数组索引
    uint16_t logicalAddress = startAddress + MODBUS_COILS_START;
    uint16_t startIndex = logicalAddress - MODBUS_COILS_START;

    // 地址范围验证
    if (startIndex + coilCount > MODBUS_COIL_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_READ_COILS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 构造响应帧（每个字节表示 8 个线圈的状态）
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_READ_COILS;
    Modbus.TX_Data[2] = (coilCount + 7) / 8; // 字节数

    for (uint16_t i = 0; i < coilCount; i++) {
        uint8_t byteIndex = i / 8;
        uint8_t bitIndex = i % 8;

        if (GetCoil(startIndex + i,false)) {
            Modbus.TX_Data[3 + byteIndex] |= (1 << bitIndex);
        } else {
            Modbus.TX_Data[3 + byteIndex] &= ~(1 << bitIndex);
        }
    }

    // 计算 CRC 并发送响应
    uint16_t crc = calcCRC(Modbus.TX_Data, 3 + Modbus.TX_Data[2]);
    Modbus.TX_Data[3 + Modbus.TX_Data[2]] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[3 + Modbus.TX_Data[2] + 1] = crc & 0xFF;

    Modbus.u8TX_Data_Len = 3 + Modbus.TX_Data[2] + 2;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}

void HandleWriteSingleCoil(void) {
    uint16_t address = Modbus.u16RegAddress;
    uint16_t value = Modbus.u16WriteValue; // 0xFF00 表示 ON，0x0000 表示 OFF

    // 转换为数组索引
    uint16_t logicalAddress = address + MODBUS_COILS_START;
    uint16_t startIndex = logicalAddress - MODBUS_COILS_START;

    // 地址范围验证
    if (startIndex >= MODBUS_COIL_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_SINGLE_COILS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 转换 Modbus 值为 0 或 1
    uint8_t coilValue = (value == 0xFF00) ? 1 : ((value == 0x0000) ? 0 : 0xFF);
    if (coilValue == 0xFF) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_SINGLE_COILS, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // 设置线圈状态
    if (!SetCoil(startIndex, coilValue)) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_SINGLE_COILS, MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE);
        return;
    }

    // 回显写入的地址和值
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_SINGLE_COILS;
    Modbus.TX_Data[2] = (address >> 8) & 0xFF;
    Modbus.TX_Data[3] = address & 0xFF;
    Modbus.TX_Data[4] = (value >> 8) & 0xFF;
    Modbus.TX_Data[5] = value & 0xFF;

    uint16_t crc = calcCRC(Modbus.TX_Data, 6);
    Modbus.TX_Data[6] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[7] = crc & 0xFF;

    Modbus.u8TX_Data_Len = 8;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}


void HandleWriteMultipleCoils(void) {
    uint16_t startAddress = Modbus.u16RegAddress;
    uint16_t coilCount = Modbus.u16RegNumber;
    uint8_t byteCount = Modbus.u8ByteCount;

    // 转换为数组索引
    uint16_t logicalAddress = startAddress + MODBUS_COILS_START;
    uint16_t startIndex = logicalAddress - MODBUS_COILS_START;

    // 地址范围验证
    if (startIndex + coilCount > MODBUS_COIL_COUNT) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_COILS, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // 数据长度验证
    if (byteCount != (coilCount + 7) / 8) {
        SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_COILS, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // 写入每个线圈
    uint8_t *pData = &Modbus.u8Data[7]; // 数据字段起始于第 7 字节
    for (uint16_t i = 0; i < coilCount; i++) {
        uint8_t byteIndex = (i / 8);
        uint8_t bitMask = 1 << (i % 8);
        uint8_t coilValue = (pData[byteIndex] & bitMask) ? 1 : 0;

        if (!SetCoil(startIndex + i, coilValue)) {
            SendExceptionResponse(MODBUS_FUNC_WRITE_MULTIPLE_COILS, MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE);
            return;
        }
    }

    // 构造响应帧
    Modbus.TX_Data[0] = Modbus.u8SrverID;
    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_MULTIPLE_COILS;
    Modbus.TX_Data[2] = (startAddress >> 8) & 0xFF;
    Modbus.TX_Data[3] = startAddress & 0xFF;
    Modbus.TX_Data[4] = (coilCount >> 8) & 0xFF;
    Modbus.TX_Data[5] = coilCount & 0xFF;

    uint16_t crc = calcCRC(Modbus.TX_Data, 6);
    Modbus.TX_Data[6] = (crc >> 8) & 0xFF;
    Modbus.TX_Data[7] = crc & 0xFF;

    Modbus.u8TX_Data_Len = 8;
    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);
}
