/*
 * Modbus.c
 *  Modbus RTU Master and Slave library for STM32 CUBE with FreeRTOS
 *  Created on: May 5, 2020
 *      Author: Alejandro Mera
 *      Adapted from https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
 */

#include "Modbus.h"

AHT20_Data_t modbus_sensorData;

uint16_t HoldingRegisters[HOLDING_REGISTER_COUNT] = {0}; // 保持寄存器
uint16_t InputRegisters[INPUT_REGISTER_COUNT] = {0};     // 输入寄存器
uint8_t Coils[COILS_COUNT / 8] = {0};                    // 线圈
uint8_t DiscreteInputs[DISCRETE_INPUT_COUNT / 8] = {0};  // 离散输入

static void UpdateCoilsFromGPIO(void);          // 更新线圈状态
static void SyncCoilsToGPIO(void);              // 同步线圈状态到GPIO引脚
static void UpdateDiscreteInputsFromGPIO(void); // 更新离散输入状态
static void UpdateInputRegisters(void);         // 更新输入寄存器
static void UpdateHoldingRegisters(void);       // 更新保持寄存器

void Modbus_Init(uint8_t device_address) // 可以设置设备地址
{

    Modbus.MODBUS_SEVER_ID = device_address; // 设置从站地址
}

// Modbus的发送函数
static void Modbus_Send(uint8_t *data, uint8_t length)
{
    HAL_UART_Transmit_DMA(&huart3, data, length); // 使用实际数据长度
}

// CRC 查找表
static const uint16_t crcTable[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

// 使用查表法计算 CRC
uint16_t calcCRC(uint8_t *buffer, uint8_t length)
{
    uint16_t crc = 0xFFFF; // 初始化 CRC 寄存器为 0xFFFF

    for (uint8_t i = 0; i < length; i++)
    {
        uint8_t tableIndex = (crc ^ buffer[i]) & 0xFF; // 计算查表索引
        crc = (crc >> 8) ^ crcTable[tableIndex];       // 更新 CRC 值
    }

    // 调换高低位
    return (crc << 8) | (crc >> 8);
}

// CRC16-MODBUS 校验
// uint16_t calcCRC(uint8_t *Buffer, uint8_t u8length)
// {
//     unsigned int temp, temp2, flag;
//     temp = 0xFFFF; // 初始化 CRC 寄存器为 0xFFFF
//     for (unsigned char i = 0; i < u8length; i++)
//     {
//         temp = temp ^ Buffer[i]; // 将数据字节与 CRC 寄存器的低字节异或
//         for (unsigned char j = 1; j <= 8; j++)
//         {
//             flag = temp & 0x0001; // 检查最低位
//             temp >>= 1;           // 右移一位
//             if (flag)
//                 temp ^= 0xA001; // 如果最低位为 1，则与多项式 0xA001 异或
//         }
//     }
//     // 交换高低字节顺序
//     temp2 = temp >> 8;
//     temp = (temp << 8) | temp2;
//     temp &= 0xFFFF; // 确保结果为 16 位
//     return temp;
// }

/**
 * 根据逻辑地址获取线圈的索引
 *
 * @param logicalAddress 线圈的逻辑地址
 * @return 如果逻辑地址有效，则返回对应的线圈索引；否则返回0xFFFF表示地址错误
 *
 * 此函数的目的是将线圈的逻辑地址转换为在内存中的索引位置
 * 它首先检查给定的逻辑地址是否在线圈地址的有效范围内
 * 如果是，它将计算出线圈数组中的正确索引并返回
 * 如果不在有效范围内，则返回特殊值0xFFFF，表示给定的地址无效
 */
uint16_t GetCoilIndex(uint16_t logicalAddress)
{
    // 检查逻辑地址是否在有效范围内
    if (logicalAddress >= MODBUS_COILS_START && logicalAddress <= MODBUS_COILS_END)
    {
        // 逻辑地址减去起始地址，转换为数组索引
        return logicalAddress - MODBUS_COILS_START;
    }
    // 返回无效值，表示地址错误
    return 0xFFFF;
}

/**
 * 将逻辑地址转换为 holding register 的索引
 *
 * @param logicalAddress 逻辑地址，用于标识 modbus 寄存器的地址
 * @return 如果逻辑地址在 holding register 范围内，则返回对应的索引；否则返回 0xFFFF 表示无效地址
 */
uint16_t GetHoldingRegisterIndex(uint16_t logicalAddress)
{

    // 检查逻辑地址是否在 holding register 的有效范围内
    if (logicalAddress >= MODBUS_HOLDING_REGISTER_START && logicalAddress <= MODBUS_HOLDING_REGISTER_END)
    {
        // 逻辑地址减去 holding register 起始地址，转换为数组索引
        return logicalAddress - MODBUS_HOLDING_REGISTER_START;
    }
    // 返回无效值，表示地址错误
    return 0xFFFF;
}

/**
 * 读取单个保持寄存器的值
 *
 * @param logicalAddress 逻辑地址，用于标识特定的保持寄存器
 * @return 如果地址有效，则返回对应保持寄存器的值；否则返回0
 *
 * 此函数首先将逻辑地址转换为保持寄存器数组的索引
 * 如果索引有效且在允许的范围内，则返回对应寄存器的值
 * 否则，记录错误信息并返回0作为默认值
 */
uint16_t ReadSingleHoldingRegister(uint16_t logicalAddress)
{
    // 获取保持寄存器的索引
    uint16_t index = GetHoldingRegisterIndex(logicalAddress);

    log_d("Read Holding Register Index: %d", index);

    // 检查索引是否有效且在允许的范围内
    if (index != 0xFFFF && index < HOLDING_REGISTER_COUNT)
    {
        return HoldingRegisters[index]; // 返回寄存器值
    }

    // 如果地址无效，记录错误信息
    log_e("Invalid Holding Register Address: %d", logicalAddress);

    // 返回默认值0
    return 0;
}

// 提取Modbus数据
void Modbus_ExtractData(void)
{
    if (Modbus.Rx_Flag == 1)
    {
        uint16_t expectedLength = 0;

        // 提取从站地址
        Modbus.u8SrverID = Modbus.u8Data[0];

        if (Modbus.u8SrverID == Modbus.MODBUS_SEVER_ID)
        {
            // 提取功能码
            Modbus.u8FunctionCode = Modbus.u8Data[1];
            // 提取线圈地址
            Modbus.u16CoilsAddress = (Modbus.u8Data[2] << 8) | Modbus.u8Data[3];

            // 提取线圈数量
            Modbus.u16CoilsNumber = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5];

            // 根据功能码解析数据
            if (Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_MULTIPLE_COILS || Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS) // 写多个寄存器,
            {
                Modbus.u8byteCount = Modbus.u8Data[6]; // 提取字节数

                // 写多个线圈或寄存器
                expectedLength = 7 + Modbus.u8byteCount + 2; // 7字节固定部分 + 数据字节数 + CRC校验码2字节

                // 提取并验证 CRC 校验码
                Modbus.u16CRC = (Modbus.u8Data[7 + Modbus.u8byteCount] << 8) | Modbus.u8Data[8 + Modbus.u8byteCount];
                Modbus.u16CRC_Calculated = calcCRC(Modbus.u8Data, 7 + Modbus.u8byteCount);

                // 验证 CRC 校验
                if (Modbus.u16CRC != Modbus.u16CRC_Calculated)
                {
                    log_e("CRC Check Failed: Received CRC: 0x%04X, Calculated CRC: 0x%04X", Modbus.u16CRC, Modbus.u16CRC_Calculated);
                    memset(Modbus.u8Data, 0, expectedLength);
                    ;       // 清空接收缓冲区
                    return; // CRC 校验失败，直接返回
                }

                // 解析数据部分并存储到结构体中
                for (uint8_t i = 0; i < Modbus.u8byteCount; i++)
                {
                    Modbus.u16CoilsData[i] = Modbus.u8Data[7 + i]; // 数据部分从第7字节开始
                }

                log_d("Function Code: %d, Byte Count: %d", Modbus.u8FunctionCode, Modbus.u8byteCount);
            }
            else if (Modbus.u8FunctionCode == MODBUS_FUNC_READ_COILS || Modbus.u8FunctionCode == MODBUS_FUNC_READ_DISCRETE_INPUTS ||
                     Modbus.u8FunctionCode == MODBUS_FUNC_READ_HOLDING_REGISTERS || Modbus.u8FunctionCode == MODBUS_FUNC_READ_INPUT_REGISTERS || Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_SINGLE_COILS || Modbus.u8FunctionCode == MODBUS_FUNC_WRITE_SINGLE_REGISTER) // 其他功能码
            {
                // 读取操作（如读取线圈、离散输入、保持寄存器、输入寄存器）
                expectedLength = 6 + 2; // 6字节固定部分 + CRC校验码2字节

                // 提取并验证 CRC 校验码
                Modbus.u16CRC = (Modbus.u8Data[6] << 8) | Modbus.u8Data[7];
                Modbus.u16CRC_Calculated = calcCRC(Modbus.u8Data, 6);

                // 验证 CRC 校验
                if (Modbus.u16CRC != Modbus.u16CRC_Calculated)
                {
                    log_e("CRC Check Failed: Received CRC: 0x%04X, Calculated CRC: 0x%04X", Modbus.u16CRC, Modbus.u16CRC_Calculated);
                    memset(Modbus.u8Data, 0, expectedLength);
                    ;       // 清空接收缓冲区
                    return; // CRC 校验失败，直接返回
                }
            }
            else
            {
                log_e("Unsupported Function Code: %d", Modbus.u8FunctionCode);
                memset(Modbus.u8Data, 0, expectedLength);
                ;       // 清空接收缓冲区
                return; // 不支持的功能码，直接返回
            }

            // 检查是否超出缓冲区大小
            if (expectedLength > sizeof(Modbus.u8Data))
            {
                log_e("Invalid data length: Expected %d, but buffer size is %d", expectedLength, sizeof(Modbus.u8Data));
                return; // 数据长度超出缓冲区，直接返回
            }

            if (Modbus.u16CRC == Modbus.u16CRC_Calculated)
            {
                log_d("CRC Check OK");

                // 满足CRC校验条件，执行相应的功能码操作
                switch (Modbus.u8FunctionCode)
                {
                case MODBUS_FUNC_READ_COILS: // 读取线圈
                {
                    UpdateCoilsFromGPIO(); // 更新线圈状态
                    log_d("Read Coils");
                    // 提取起始地址和线圈数量
                    uint16_t startAddress = Modbus.u16CoilsAddress + MODBUS_COILS_START;
                    uint16_t coilCount = Modbus.u16CoilsNumber;

                    // 检查起始地址和线圈数量是否有效
                    if (startAddress < MODBUS_COILS_START || startAddress + coilCount > MODBUS_COILS_END)
                    {
                        log_e("Invalid Coil Address or Count");
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 返回错误
                    }

                    // 计算需要返回的字节数
                    uint8_t byteCount = (coilCount + 7) / 8; // 每 8 个线圈占 1 字节
                    // 填充响应数据
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID; // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_READ_COILS; // 功能码
                    Modbus.TX_Data[2] = byteCount;              // 数据字节数

                    // 填充线圈状态
                    for (uint8_t i = 0; i < byteCount; i++)
                    {
                        uint8_t coilByte = 0;
                        for (uint8_t bit = 0; bit < 8; bit++)
                        {
                            uint16_t coilIndex = GetCoilIndex(startAddress + i * 8 + bit);
                            if (coilIndex != 0xFFFF && coilIndex < COILS_COUNT)
                            {
                                if (Coils[coilIndex / 8] & (1 << (coilIndex % 8)))
                                {
                                    coilByte |= (1 << bit);
                                }
                            }
                        }
                        Modbus.TX_Data[3 + i] = coilByte;
                    }

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 3 + byteCount; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 低字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 高字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC 长度
                    // 发送数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    // 发送成功，打印日志发送的数据
                    log_d("Modbus: Send data success.");
                    log_d("Modbus: Server ID: %d", Modbus.MODBUS_SEVER_ID);
                    log_d("Modbus: Function Code: %d", MODBUS_FUNC_READ_COILS);
                    log_d("Modbus: Byte Count: %d", byteCount);
                    for (uint8_t i = 0; i < byteCount; i++)
                    {
                        log_d("Modbus: Coil %d: %d", startAddress + i, Modbus.TX_Data[3 + i]);
                    }
                    log_d("Modbus: CRC: %04X", Modbus.u16CRC_Calculated);
                    log_d("Modbus: Response Sent");

                    break;
                }
                case MODBUS_FUNC_READ_DISCRETE_INPUTS: // 读取离散量输入
                {
                    log_d("Read Discrete Inputs");
                    UpdateDiscreteInputsFromGPIO(); // 更新离散输入状态

                    // 提取起始地址和离散输入数量
                    uint16_t startAddress = Modbus.u16CoilsAddress + MODBUS_DISCRETE_INPUT_START; // 起始地址
                    uint16_t inputCount = Modbus.u16CoilsNumber;                                  // 离散输入数量

                    // 检查起始地址和离散输入数量是否有效
                    if (startAddress < MODBUS_DISCRETE_INPUT_START || startAddress + inputCount > MODBUS_DISCRETE_INPUT_END)
                    {
                        log_e("Invalid Discrete Input Address or Count");
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址或数量无效，直接返回
                    }

                    // 计算需要返回的字节数
                    uint8_t byteCount = (inputCount + 7) / 8; // 每 8 个离散输入占 1 字节

                    // 填充响应数据
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;           // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_READ_DISCRETE_INPUTS; // 功能码
                    Modbus.TX_Data[2] = byteCount;                        // 数据字节数

                    // 填充离散输入状态
                    for (uint8_t i = 0; i < byteCount; i++)
                    {
                        uint8_t inputByte = 0;
                        for (uint8_t bit = 0; bit < 8; bit++)
                        {
                            uint16_t inputIndex = startAddress + i * 8 + bit - MODBUS_DISCRETE_INPUT_START;
                            if (inputIndex < DISCRETE_INPUT_COUNT)
                            {
                                if (DiscreteInputs[inputIndex / 8] & (1 << (inputIndex % 8)))
                                {
                                    inputByte |= (1 << bit); // 设置对应位为 1
                                }
                            }
                        }
                        Modbus.TX_Data[3 + i] = inputByte; // 写入响应数据
                    }

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 3 + byteCount; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 高字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 低字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Read Discrete Inputs Response Sent");
                    break;
                }
                case MODBUS_FUNC_READ_HOLDING_REGISTERS: // 读取保持寄存器
                {
                    log_d("Read Holding Registers");

                    UpdateInputRegisters(); // 更新输入寄存器

                    // 将偏移量转换为逻辑地址
                    uint16_t logicalAddress = Modbus.u16CoilsAddress + MODBUS_HOLDING_REGISTER_START;
                    uint16_t value = ReadSingleHoldingRegister(logicalAddress); // 读取保持寄存器

                    log_d("Holding Register Address: %d", logicalAddress);
                    log_d("Holding Register Value: %d", value);

                    log_d("Response Sent");

                    // 填充响应数据
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;             // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_READ_HOLDING_REGISTERS; // 功能码
                    Modbus.TX_Data[2] = 2;                                  // 数据字节数
                    Modbus.TX_Data[3] = (value >> 8) & 0xFF;                // 数据高字节
                    Modbus.TX_Data[4] = value & 0xFF;                       // 数据低字节

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 5; // 数据部分长度（从站地址 + 功能码 + 数据）
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 低字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 高字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC 长度

                    // 发送数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    break;
                }
                case MODBUS_FUNC_READ_INPUT_REGISTERS: // 读取输入寄存器
                {
                    log_d("Read Input Registers");

                    // 提取起始地址和寄存器数量
                    uint16_t startAddress = Modbus.u16CoilsAddress + MODBUS_INPUT_REGISTER_START; // 起始地址
                    uint16_t registerCount = Modbus.u16CoilsNumber;                               // 寄存器数量

                    // 检查起始地址和寄存器数量是否有效
                    if (startAddress < MODBUS_INPUT_REGISTER_START || startAddress + registerCount > MODBUS_INPUT_REGISTER_END)
                    {
                        log_e("Invalid Input Register Address or Count");
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址或数量无效，直接返回
                    }

                    // 计算需要返回的字节数
                    uint8_t byteCount = registerCount * 2; // 每个寄存器占 2 字节

                    // 填充响应数据
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;           // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_READ_INPUT_REGISTERS; // 功能码
                    Modbus.TX_Data[2] = byteCount;                        // 数据字节数

                    // 填充输入寄存器的值
                    for (uint16_t i = 0; i < registerCount; i++)
                    {
                        uint16_t registerIndex = startAddress + i - MODBUS_INPUT_REGISTER_START; // 计算寄存器索引
                        if (registerIndex < INPUT_REGISTER_COUNT)
                        {
                            uint16_t value = InputRegisters[registerIndex];  // 获取寄存器值
                            Modbus.TX_Data[3 + i * 2] = (value >> 8) & 0xFF; // 高字节
                            Modbus.TX_Data[4 + i * 2] = value & 0xFF;        // 低字节
                        }
                        else
                        {
                            log_e("Invalid Input Register Index: %d", registerIndex);
                            memset(Modbus.u8Data, 0, expectedLength);
                            ;       // 清空接收缓冲区
                            return; // 索引无效，直接返回
                        }
                    }

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 3 + byteCount; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 高字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 低字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Read Input Registers Response Sent");
                    break;
                }
                case MODBUS_FUNC_WRITE_SINGLE_COILS: // 写单个线圈
                {
                    log_d("Write Single Coils");
                    // 提取线圈地址和状态
                    uint16_t coilAddress = Modbus.u16CoilsAddress + MODBUS_COILS_START;
                    uint16_t writeValue = Modbus.u16CoilsNumber;

                    log_d("Coil Address: %d", coilAddress);
                    log_d("Write Value: %d", writeValue);

                    // 检查线圈地址是否有效
                    uint16_t coilIndex = GetCoilIndex(coilAddress);
                    if (coilIndex == 0xFFFF || coilIndex >= COILS_COUNT)
                    {
                        log_e("Invalid Coil Address: %d", coilAddress);
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址无效，直接返回
                    }

                    // 更新线圈状态
                    if (writeValue == 0xFF00) // 设置线圈为 ON
                    {
                        Coils[coilIndex / 8] |= (1 << (coilIndex % 8));
                        log_d("Coil %d set to ON", coilAddress);
                    }
                    else if (writeValue == 0x0000) // 设置线圈为 OFF
                    {
                        Coils[coilIndex / 8] &= ~(1 << (coilIndex % 8));
                        log_d("Coil %d set to OFF", coilAddress);
                    }
                    else
                    {
                        log_e("Invalid Write Value: 0x%04X", writeValue);
                        return; // 写入值无效，直接返回
                    }

                    SyncCoilsToGPIO(); // 同步线圈状态到GPIO引脚
                    // 填充响应数据（与请求相同）
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;         // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_SINGLE_COILS; // 功能码
                    Modbus.TX_Data[2] = Modbus.u8Data[2];               // 线圈地址高字节
                    Modbus.TX_Data[3] = Modbus.u8Data[3];               // 线圈地址低字节
                    Modbus.TX_Data[4] = Modbus.u8Data[4];               // 写入值高字节
                    Modbus.TX_Data[5] = Modbus.u8Data[5];               // 写入值低字节

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 6; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 低字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 高字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Write Single Coil Response Sent");

                    break;
                }
                case MODBUS_FUNC_WRITE_SINGLE_REGISTER: // 写单个寄存器
                {
                    log_d("Write Single Register");

                    // 提取寄存器地址和写入值
                    uint16_t registerAddress = (Modbus.u8Data[2] << 8) | Modbus.u8Data[3]; // 寄存器地址
                    uint16_t writeValue = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5];      // 写入值

                    log_d("Register Address: %d", registerAddress);
                    log_d("Write Value: %d", writeValue);

                    // 检查寄存器地址是否有效
                    uint16_t registerIndex = GetHoldingRegisterIndex(registerAddress);
                    if (registerIndex == 0xFFFF || registerIndex >= HOLDING_REGISTER_COUNT)
                    {
                        log_e("Invalid Register Address: %d", registerAddress);
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址无效，直接返回
                    }

                    // 更新保持寄存器的值
                    HoldingRegisters[registerIndex] = writeValue;
                    log_d("Holding Register[%d] updated to %d", registerIndex, writeValue);

                    // 填充响应数据（与请求相同）
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;            // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_SINGLE_REGISTER; // 功能码
                    Modbus.TX_Data[2] = Modbus.u8Data[2];                  // 寄存器地址高字节
                    Modbus.TX_Data[3] = Modbus.u8Data[3];                  // 寄存器地址低字节
                    Modbus.TX_Data[4] = Modbus.u8Data[4];                  // 写入值高字节
                    Modbus.TX_Data[5] = Modbus.u8Data[5];                  // 写入值低字节

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 6; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 高字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 低字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Write Single Register Response Sent");
                    break;
                }
                case MODBUS_FUNC_WRITE_MULTIPLE_COILS: // 写多个线圈
                {
                    log_d("Write Multiple Coils");
                    uint16_t StartAddress = Modbus.u16CoilsAddress + MODBUS_COILS_START; // 起始地址
                    uint16_t Quantity = Modbus.u16CoilsNumber;                           // 数量
                    uint16_t ByteCount = Modbus.u8byteCount;                             // 字节数

                    log_d("Start Address: %d, Coil Count: %d, Byte Count: %d", StartAddress, Quantity, ByteCount);

                    // 检查起始地址和线圈数量是否有效
                    if (StartAddress < MODBUS_COILS_START || StartAddress + Quantity > MODBUS_COILS_END)
                    {
                        log_e("Invalid Coil Address or Count");
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址或数量无效，直接返回
                    }

                    // 遍历线圈状态字节，更新 Coils 数组
                    for (uint16_t i = 0; i < Quantity; i++)
                    {
                        uint16_t coilIndex = GetCoilIndex(StartAddress + i); // 获取线圈索引
                        if (coilIndex == 0xFFFF || coilIndex >= COILS_COUNT)
                        {
                            log_e("Invalid Coil Index: %d", coilIndex);
                            return; // 索引无效，直接返回
                        }

                        // 获取当前线圈的状态（从数据字节中提取）
                        uint8_t byteIndex = (i / 8); // 数据字节的索引
                        uint8_t bitIndex = i % 8;    // 数据字节中的位索引
                        uint8_t coilState = (Modbus.u16CoilsData[byteIndex] >> bitIndex) & 0x01;

                        // 更新 Coils 数组中的状态
                        if (coilState)
                        {
                            Coils[coilIndex / 8] |= (1 << (coilIndex % 8)); // 设置线圈为 ON
                        }
                        else
                        {
                            Coils[coilIndex / 8] &= ~(1 << (coilIndex % 8)); // 设置线圈为 OFF
                        }
                    }

                    // 同步线圈状态到硬件
                    SyncCoilsToGPIO();

                    // 填充响应数据（与请求的起始地址和线圈数量相同）
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;           // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_MULTIPLE_COILS; // 功能码
                    Modbus.TX_Data[2] = Modbus.u8Data[2];                 // 起始地址高字节
                    Modbus.TX_Data[3] = Modbus.u8Data[3];                 // 起始地址低字节
                    Modbus.TX_Data[4] = Modbus.u8Data[4];                 // 线圈数量高字节
                    Modbus.TX_Data[5] = Modbus.u8Data[5];                 // 线圈数量低字节

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 6; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 低字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 高字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Write Multiple Coils Response Sent");

                    break;
                }
                case MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS: // 写多个寄存器
                {
                    log_d("Write Multiple Registers");

                    // 提取起始地址和寄存器数量
                    uint16_t startAddress = (Modbus.u8Data[2] << 8) | Modbus.u8Data[3];  // 起始地址
                    uint16_t registerCount = (Modbus.u8Data[4] << 8) | Modbus.u8Data[5]; // 寄存器数量
                    uint8_t byteCount = Modbus.u8Data[6];                                // 数据字节数

                    log_d("Start Address: %d, Register Count: %d, Byte Count: %d", startAddress, registerCount, byteCount);

                    // 检查起始地址和寄存器数量是否有效
                    if (startAddress < MODBUS_HOLDING_REGISTER_START ||
                        startAddress + registerCount > MODBUS_HOLDING_REGISTER_END ||
                        byteCount != registerCount * 2)
                    {
                        log_e("Invalid Register Address, Count, or Byte Count");
                        memset(Modbus.u8Data, 0, expectedLength);
                        ;       // 清空接收缓冲区
                        return; // 地址或数量无效，直接返回
                    }

                    // 遍历寄存器数据并更新保持寄存器
                    for (uint16_t i = 0; i < registerCount; i++)
                    {
                        uint16_t registerIndex = startAddress + i - MODBUS_HOLDING_REGISTER_START; // 计算寄存器索引
                        if (registerIndex >= HOLDING_REGISTER_COUNT)
                        {
                            log_e("Invalid Register Index: %d", registerIndex);
                            memset(Modbus.u8Data, 0, expectedLength);
                            ;       // 清空接收缓冲区
                            return; // 索引无效，直接返回
                        }

                        // 提取寄存器值（高字节和低字节）
                        uint16_t value = (Modbus.u8Data[7 + i * 2] << 8) | Modbus.u8Data[8 + i * 2];
                        HoldingRegisters[registerIndex] = value; // 更新保持寄存器
                        log_d("Holding Register[%d] updated to %d", registerIndex, value);
                    }

                    // 填充响应数据（与请求的起始地址和寄存器数量相同）
                    Modbus.TX_Data[0] = Modbus.MODBUS_SEVER_ID;               // 从站地址
                    Modbus.TX_Data[1] = MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS; // 功能码
                    Modbus.TX_Data[2] = Modbus.u8Data[2];                     // 起始地址高字节
                    Modbus.TX_Data[3] = Modbus.u8Data[3];                     // 起始地址低字节
                    Modbus.TX_Data[4] = Modbus.u8Data[4];                     // 寄存器数量高字节
                    Modbus.TX_Data[5] = Modbus.u8Data[5];                     // 寄存器数量低字节

                    // 计算 CRC 校验码
                    Modbus.u8TX_Data_Len = 6; // 数据部分长度
                    Modbus.u16CRC_Calculated = calcCRC(Modbus.TX_Data, Modbus.u8TX_Data_Len);
                    Modbus.TX_Data[Modbus.u8TX_Data_Len] = (Modbus.u16CRC_Calculated >> 8) & 0xFF; // CRC 高字节
                    Modbus.TX_Data[Modbus.u8TX_Data_Len + 1] = Modbus.u16CRC_Calculated & 0xFF;    // CRC 低字节
                    Modbus.u8TX_Data_Len += 2;                                                     // 数据长度加上 CRC

                    // 发送响应数据
                    Modbus_Send(Modbus.TX_Data, Modbus.u8TX_Data_Len);

                    log_d("Write Multiple Registers Response Sent");
                    break;
                }
                }
            }
            else if (Modbus.u16CRC != Modbus.u16CRC_Calculated)
            {
                log_d("CRC Check Error");
                // 俩校验码是什么
                log_d("Received CRC: %04X, Calculated CRC: %04X", Modbus.u16CRC, Modbus.u16CRC_Calculated);
                memset(Modbus.u8Data, 0, expectedLength);
                ; // 清空接收缓冲区

                return; // CRC 校验失败，直接返回
            }
        }
        Modbus.Rx_Flag = 0; // 清除接收标志位
        memset(Modbus.u8Data, 0, expectedLength);
        ; // 清空接收缓冲区

        return; // 返回
    }
}

// 更新线圈状态从GPIO引脚读取的值
static void UpdateCoilsFromGPIO(void)
{
    // 清空线圈状态，准备更新
    memset(Coils, 0, sizeof(Coils));

    // 定义 GPIO 引脚和线圈索引的映射关系
    typedef struct
    {
        GPIO_TypeDef *port; // GPIO 端口
        uint16_t pin;       // GPIO 引脚
        uint8_t coilIndex;  // 线圈索引
    } GPIO_Coil_Map;

    // 映射表：每个 GPIO 引脚对应一个线圈索引
    GPIO_Coil_Map gpioCoilMap[] = {
        {LED_R_GPIO_Port, LED_R_Pin, 0}, // 红色 LED 对应 Coils[0] 的第 0 位
        {LED_G_GPIO_Port, LED_G_Pin, 1}, // 绿色 LED 对应 Coils[0] 的第 1 位
        {LED_B_GPIO_Port, LED_B_Pin, 2}, // 蓝色 LED 对应 Coils[0] 的第 2 位
        {OUT1_GPIO_Port, OUT1_Pin, 8},   // OUT1 对应 Coils[1] 的第 0 位
        {OUT2_GPIO_Port, OUT2_Pin, 9},   // OUT2 对应 Coils[1] 的第 1 位
        {OUT3_GPIO_Port, OUT3_Pin, 10},  // OUT3 对应 Coils[1] 的第 2 位
        {OUT4_GPIO_Port, OUT4_Pin, 11},  // OUT4 对应 Coils[1] 的第 3 位
        {OUT5_GPIO_Port, OUT5_Pin, 12},  // OUT5 对应 Coils[1] 的第 4 位
        {OUT6_GPIO_Port, OUT6_Pin, 13},  // OUT6 对应 Coils[1] 的第 5 位
        {OUT7_GPIO_Port, OUT7_Pin, 14},  // OUT7 对应 Coils[1] 的第 6 位
        {OUT8_GPIO_Port, OUT8_Pin, 15},  // OUT8 对应 Coils[1] 的第 7 位
    };

    // 遍历映射表，更新线圈状态
    for (uint8_t i = 0; i < sizeof(gpioCoilMap) / sizeof(GPIO_Coil_Map); i++)
    {
        GPIO_Coil_Map *map = &gpioCoilMap[i];
        if (HAL_GPIO_ReadPin(map->port, map->pin) == GPIO_PIN_SET)
        {
            Coils[map->coilIndex / 8] |= (1 << (map->coilIndex % 8)); // 设置对应线圈为 1
        }
        else
        {
            Coils[map->coilIndex / 8] &= ~(1 << (map->coilIndex % 8)); // 清除对应线圈为 0
        }
    }

    // 调试日志：输出 Coils 数组的状态
    for (uint8_t i = 0; i < sizeof(Coils); i++)
    {
        log_d("Coils[%d]: 0x%02X", i, Coils[i]);
    }
}

static void SyncCoilsToGPIO(void)
{
    // 定义 GPIO 引脚和线圈索引的映射关系
    typedef struct
    {
        GPIO_TypeDef *port; // GPIO 端口
        uint16_t pin;       // GPIO 引脚
        uint8_t coilIndex;  // 线圈索引
    } GPIO_Coil_Map;

    // 映射表：每个 GPIO 引脚对应一个线圈索引
    GPIO_Coil_Map gpioCoilMap[] = {
        {LED_R_GPIO_Port, LED_R_Pin, 0}, // 红色 LED 对应 Coils[0] 的第 0 位
        {LED_G_GPIO_Port, LED_G_Pin, 1}, // 绿色 LED 对应 Coils[0] 的第 1 位
        {LED_B_GPIO_Port, LED_B_Pin, 2}, // 蓝色 LED 对应 Coils[0] 的第 2 位
        {OUT1_GPIO_Port, OUT1_Pin, 8},   // OUT1 对应 Coils[1] 的第 0 位
        {OUT2_GPIO_Port, OUT2_Pin, 9},   // OUT2 对应 Coils[1] 的第 1 位
        {OUT3_GPIO_Port, OUT3_Pin, 10},  // OUT3 对应 Coils[1] 的第 2 位
        {OUT4_GPIO_Port, OUT4_Pin, 11},  // OUT4 对应 Coils[1] 的第 3 位
        {OUT5_GPIO_Port, OUT5_Pin, 12},  // OUT5 对应 Coils[1] 的第 4 位
        {OUT6_GPIO_Port, OUT6_Pin, 13},  // OUT6 对应 Coils[1] 的第 5 位
        {OUT7_GPIO_Port, OUT7_Pin, 14},  // OUT7 对应 Coils[1] 的第 6 位
        {OUT8_GPIO_Port, OUT8_Pin, 15},  // OUT8 对应 Coils[1] 的第 7 位
    };

    // 遍历映射表，同步线圈状态到 GPIO 引脚
    for (uint8_t i = 0; i < sizeof(gpioCoilMap) / sizeof(GPIO_Coil_Map); i++)
    {
        GPIO_Coil_Map *map = &gpioCoilMap[i];
        HAL_GPIO_WritePin(map->port, map->pin, (Coils[map->coilIndex / 8] & (1 << (map->coilIndex % 8))) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    // 调试日志：输出每个 GPIO 引脚的状态
    for (uint8_t i = 0; i < sizeof(gpioCoilMap) / sizeof(GPIO_Coil_Map); i++)
    {
        GPIO_Coil_Map *map = &gpioCoilMap[i];
        log_d("GPIO Pin: %s, State: %s",
              (map->coilIndex < 8) ? "Coils[0]" : "Coils[1]",
              (Coils[map->coilIndex / 8] & (1 << (map->coilIndex % 8))) ? "ON" : "OFF");
    }
}

static void UpdateDiscreteInputsFromGPIO(void)
{
    typedef struct
    {
        GPIO_TypeDef *port; // GPIO 端口
        uint16_t pin;       // GPIO 引脚
        uint8_t inputIndex; // 离散输入索引
    } GPIO_DiscreteInput_Map;

    // 映射表：每个 GPIO 引脚对应一个离散输入索引
    GPIO_DiscreteInput_Map gpioDiscreteInputMap[] = {
        {OUT1_GPIO_Port, OUT1_Pin, 0}, // 输入1 对应 DiscreteInputs[0] 的第 0 位
        {OUT2_GPIO_Port, OUT2_Pin, 1}, // 输入2 对应 DiscreteInputs[0] 的第 1 位
        {OUT3_GPIO_Port, OUT3_Pin, 2}, // 输入3 对应 DiscreteInputs[0] 的第 2 位
        {OUT4_GPIO_Port, OUT4_Pin, 3}, // 输入4 对应 DiscreteInputs[0] 的第 3 位
        {OUT5_GPIO_Port, OUT5_Pin, 4}, // 输入5 对应 DiscreteInputs[0] 的第 4 位
        {OUT6_GPIO_Port, OUT6_Pin, 5}, // 输入6 对应 DiscreteInputs[0] 的第 5 位
        {OUT7_GPIO_Port, OUT7_Pin, 6}, // 输入7 对应 DiscreteInputs[0] 的第 6 位
        {OUT8_GPIO_Port, OUT8_Pin, 7}, // 输入8 对应 DiscreteInputs[0] 的第 7 位

    };
    // 清空离散输入状态，准备更新
    memset(DiscreteInputs, 0, sizeof(DiscreteInputs));

    // 遍历映射表，更新离散输入状态
    for (uint8_t i = 0; i < sizeof(gpioDiscreteInputMap) / sizeof(GPIO_DiscreteInput_Map); i++)
    {
        GPIO_DiscreteInput_Map *map = &gpioDiscreteInputMap[i];
        if (HAL_GPIO_ReadPin(map->port, map->pin) == GPIO_PIN_SET)
        {
            // 设置对应位为 1
            DiscreteInputs[map->inputIndex / 8] |= (1 << (map->inputIndex % 8));
        }
        else
        {
            // 清除对应位为 0
            DiscreteInputs[map->inputIndex / 8] &= ~(1 << (map->inputIndex % 8));
        }
    }

    // 调试日志：输出 DiscreteInputs 数组的状态
    for (uint8_t i = 0; i < sizeof(DiscreteInputs); i++)
    {
        log_d("DiscreteInputs[%d]: 0x%02X", i, DiscreteInputs[i]);
    }
}

static void UpdateInputRegisters(void)
{
    // 示例：从传感器读取数据
    AHT20_Read(&modbus_sensorData); // 读取温湿度传感器数据

    // 填充输入寄存器
    InputRegisters[0] = (int16_t)modbus_sensorData.Temperature * 10; // 温度，单位为 0.1°C
    InputRegisters[1] = (int16_t)modbus_sensorData.Humidity * 10;    // 湿度，单位为 0.1%RH

    // 示例：填充其他输入寄存器（如设备状态）
    // InputRegisters[2] = HAL_GPIO_ReadPin(STATUS_GPIO_Port, STATUS_Pin); // 设备状态（0 或 1）

    // 调试日志：输出 InputRegisters 数组的状态
    for (uint16_t i = 0; i < INPUT_REGISTER_COUNT; i++)
    {
        log_d("InputRegisters[%d]: %d", i, InputRegisters[i]);
    }
}

static void UpdateHoldingRegisters(void)
{
    // 示例：从传感器读取数据
    AHT20_Read(&modbus_sensorData); // 读取温湿度传感器数据

    // 填充保持寄存器
    HoldingRegisters[0] = Modbus.MODBUS_SEVER_ID;                      // 将从站地址存入保持寄存器
    HoldingRegisters[1] = (int16_t)modbus_sensorData.Temperature * 10; // 温度，单位为 0.1°C
    HoldingRegisters[2] = (int16_t)modbus_sensorData.Humidity * 10;    // 湿度，单位为 0.1%RH

    // 示例：填充其他保持寄存器（如设备状态）
    // HoldingRegisters[3] = HAL_GPIO_ReadPin(STATUS_GPIO_Port, STATUS_Pin); // 设备状态（0 或 1）
}

// Modbus 数据更新任务
void Modbus_DataUpdate(void *argument)
{
    // Infinite loop
    for (;;)
    {

        // --------------------------更新保持寄存器状态---------------------------//
        UpdateHoldingRegisters(); // 更新保持寄存器状态

        Modbus_ExtractData(); // 提取Modbus数据
        osDelay(100);         // 延时100ms
    }
}
