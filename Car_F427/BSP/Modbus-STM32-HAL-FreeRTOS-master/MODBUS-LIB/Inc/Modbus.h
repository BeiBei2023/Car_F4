/*
 * Modbus.h
 *
 *  创建时间: 2020年5月5日
 *      作者: Alejandro Mera
 */

#ifndef THIRD_PARTY_MODBUS_INC_MODBUS_H_
#define THIRD_PARTY_MODBUS_INC_MODBUS_H_

// 包含 Modbus 配置文件
#include "ModbusConfig.h"
#include <inttypes.h>
#include <stdbool.h>
// 包含 FreeRTOS 相关头文件
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/**
 * @enum mb_hardware_t
 * @brief Modbus 支持的硬件类型枚举
 */
typedef enum
{
    USART_HW = 1,     // 通用同步异步收发器硬件
    USB_CDC_HW = 2,   // USB 通用类设备硬件
    TCP_HW = 3,       // TCP 网络硬件
    USART_HW_DMA = 4, // 带有 DMA 功能的通用同步异步收发器硬件
} mb_hardware_t;

/**
 * @enum mb_masterslave_t
 * @brief Modbus 主从模式枚举
 */
typedef enum
{
    MB_SLAVE = 3, // Modbus 从站模式
    MB_MASTER = 4 // Modbus 主站模式
} mb_masterslave_t;

/**
 * @enum mb_address_t
 * @brief Modbus 地址模式枚举
 */
typedef enum
{
    ADDRESS_BROADCAST = 0, // 广播模式 -> modH->u8Buffer[ID] == 0
    ADDRESS_NORMAL = 1,    // 正常模式 -> modH->u8Buffer[ID] > 0
} mb_address_t;

/**
 * @enum MB_FC
 * @brief
 * Modbus 功能码总结。
 * 这些是主站或从站实现的功能码。
 *
 * @see also fctsupported
 * @see also modbus_t
 */
typedef enum MB_FC
{
    MB_FC_READ_COILS = 1,               // FCT=1 -> 读取线圈或数字输出
    MB_FC_READ_DISCRETE_INPUT = 2,      // FCT=2 -> 读取数字输入
    MB_FC_READ_REGISTERS = 3,           // FCT=3 -> 读取寄存器或模拟输出
    MB_FC_READ_INPUT_REGISTER = 4,      // FCT=4 -> 读取模拟输入
    MB_FC_WRITE_COIL = 5,               // FCT=5 -> 写入单个线圈或输出
    MB_FC_WRITE_REGISTER = 6,           // FCT=6 -> 写入单个寄存器
    MB_FC_WRITE_MULTIPLE_COILS = 15,    // FCT=15 -> 写入多个线圈或输出
    MB_FC_WRITE_MULTIPLE_REGISTERS = 16 // FCT=16 -> 写入多个寄存器
} mb_functioncode_t;

/**
 * @struct modbusRingBuffer_t
 * @brief Modbus 环形缓冲区结构体
 */
typedef struct
{
    uint8_t uxBuffer[MAX_BUFFER]; // 缓冲区数组
    uint8_t u8start;              // 缓冲区起始索引
    uint8_t u8end;                // 缓冲区结束索引
    uint8_t u8available;          // 可用字节数
    bool overflow;                // 缓冲区溢出标志
} modbusRingBuffer_t;

/**
 * @enum MESSAGE
 * @brief 电报帧位置索引
 */
typedef enum MESSAGE
{
    ID = 0,  // ID 字段
    FUNC,    // 功能码位置
    ADD_HI,  // 地址高字节
    ADD_LO,  // 地址低字节
    NB_HI,   // 线圈或寄存器数量高字节
    NB_LO,   // 线圈或寄存器数量低字节
    BYTE_CNT // 字节计数器
} mb_message_t;

/**
 * @enum COM_STATES
 * @brief Modbus 通信状态枚举
 */
typedef enum COM_STATES
{
    COM_IDLE = 0,    // 通信空闲状态
    COM_WAITING = 1, // 通信等待状态
} mb_com_state_t;

/**
 * @enum ERR_OP_LIST
 * @brief Modbus 错误和操作码枚举
 */
typedef enum ERR_OP_LIST
{
    // 错误码
    ERR_NOT_MASTER = 10,    // 非主站错误
    ERR_POLLING = 11,       // 轮询错误
    ERR_BUFF_OVERFLOW = 12, // 缓冲区溢出错误
    ERR_BAD_CRC = 13,       // CRC 校验错误
    ERR_EXCEPTION = 14,     // 异常错误
    ERR_BAD_SIZE = 15,      // 大小错误
    ERR_BAD_ADDRESS = 16,   // 地址错误
    ERR_TIME_OUT = 17,      // 超时错误
    ERR_BAD_SLAVE_ID = 18,  // 从站 ID 错误
    ERR_BAD_TCP_ID = 19,    // TCP ID 错误
                         // 操作码
    OP_OK_QUERY = 20 // 此值不是错误码，用于确认正确操作，
                     // 因为 FreeRTOS 通知在超时情况下返回 0，
                     // 因此我们定义自己的错误和操作码，并将 0 专门用于 FreeRTOS 原语
} mb_err_op_t;

/**
 * @enum 异常码枚举
 */
enum
{
    EXC_FUNC_CODE = 1,  // 不支持的功能码异常
    EXC_ADDR_RANGE = 2, // 地址范围异常
    EXC_REGS_QUANT = 3, // 寄存器数量异常
    EXC_EXECUTE = 4     // 执行异常
};

/**
 * @union bytesFields
 * @brief 字节字段联合体，用于不同数据类型的转换
 */
typedef union
{
    uint8_t u8[4];   // 8 位无符号整数数组
    uint16_t u16[2]; // 16 位无符号整数数组
    uint32_t u32;    // 32 位无符号整数
} bytesFields;

/**
 * @struct modbus_t
 * @brief
 * Modbus 主站查询结构体:
 * 该结构体包含主站生成 Modbus 查询所需的所有字段。
 * 主站可以保存多个这样的结构体，并循环发送或根据程序需要使用。
 */
typedef struct
{
    uint8_t u8id;             // 从站地址，范围 1 到 247，0 表示广播
    mb_functioncode_t u8fct;  // 功能码: 1, 2, 3, 4, 5, 6, 15 或 16
    uint16_t u16RegAdd;       // 要访问的从站第一个寄存器的地址
    uint16_t u16CoilsNo;      // 要访问的线圈或寄存器数量
    uint16_t *u16reg;         // 指向主站内存映像的指针
    uint32_t *u32CurrentTask; // 指向将接收 Modbus 通知的任务的指针
#if ENABLE_TCP == 1
    uint32_t xIpAddress; // IP 地址
    uint16_t u16Port;    // 端口号
    uint8_t u8clientID;  // 客户端 ID
#endif
} modbus_t;

// 如果启用 TCP 功能
#if ENABLE_TCP == 1
/**
 * @struct tcpclients_t
 * @brief TCP 客户端结构体
 */
typedef struct
{
    struct netconn *conn; // 网络连接指针
    uint32_t aging;       // 老化时间
} tcpclients_t;
#endif

/**
 * @struct modbusHandler_t
 * @brief
 * Modbus 处理程序结构体
 * 包含 Modbus 守护进程操作所需的所有变量
 */
typedef struct
{
    mb_masterslave_t uModbusType;            // Modbus 主从模式
    UART_HandleTypeDef *port;                // HAL 串口句柄
    uint8_t u8id;                            // 0=主站，1..247=从站编号
    GPIO_TypeDef *EN_Port;                   // 流控制引脚: 0=USB 或 RS-232 模式，>1=RS-485 模式
    uint16_t EN_Pin;                         // 流控制引脚: 0=USB 或 RS-232 模式，>1=RS-485 模式
    mb_err_op_t i8lastError;                 // 最后一次错误码
    uint8_t u8Buffer[MAX_BUFFER];            // Modbus 通信缓冲区
    uint8_t u8BufferSize;                    // 缓冲区大小
    uint8_t u8lastRec;                       // 最后接收的字节
    uint16_t *u16regs;                       // 寄存器指针
    uint16_t u16InCnt, u16OutCnt, u16errCnt; // 记录 Modbus 通信统计信息
    uint16_t u16timeOut;                     // 超时时间
    uint16_t u16regsize;                     // 寄存器大小
    uint8_t dataRX;                          // 接收的数据
    int8_t i8state;                          // 状态

    mb_address_t u8AddressMode; // 0=广播，1..247=正常

    // FreeRTOS 组件

    // Modbus 电报队列
    osMessageQueueId_t QueueTelegramHandle;

    // Modbus 从站任务
    osThreadId_t myTaskModbusAHandle;
    // Modbus 接收定时器
    xTimerHandle xTimerT35;
    // 主站超时定时器
    xTimerHandle xTimerTimeout;
    // Modbus 数据信号量
    osSemaphoreId_t ModBusSphrHandle;
    // USART 接收环形缓冲区
    modbusRingBuffer_t xBufferRX;
    // 硬件类型 TCP, USB CDC, USART
    mb_hardware_t xTypeHW;

#if ENABLE_TCP == 1
    tcpclients_t newconns[NUMBERTCPCONN]; // 新的 TCP 连接数组
    struct netconn *conn;                 // 网络连接指针
    uint32_t xIpAddress;                  // IP 地址
    uint16_t u16TransactionID;            // 事务 ID
    uint16_t uTcpPort;                    // 仅用于从站（即服务器）
    uint8_t newconnIndex;                 // 新连接索引
#endif
} modbusHandler_t;

/**
 * @enum 消息长度常量枚举
 */
enum
{
    RESPONSE_SIZE = 6,  // 响应消息长度
    EXCEPTION_SIZE = 3, // 异常消息长度
    CHECKSUM_SIZE = 2   // 校验和长度
};

// 外部声明 Modbus 处理程序数组
extern modbusHandler_t *mHandlers[MAX_M_HANDLERS];

// 函数原型
// 初始化 Modbus 处理程序
void ModbusInit(modbusHandler_t *modH);
// 启动 Modbus 处理程序
void ModbusStart(modbusHandler_t *modH);

// 如果启用 USB CDC 功能
#if ENABLE_USB_CDC == 1
// 启动 USB CDC 模式下的 Modbus 处理程序
void ModbusStartCDC(modbusHandler_t *modH);
#endif

// 设置通信看门狗定时器
void setTimeOut(uint16_t u16timeOut);
// 获取通信看门狗定时器值
uint16_t getTimeOut();
// 获取通信看门狗定时器状态
bool getTimeOutState();
// 将查询放入队列尾部
void ModbusQuery(modbusHandler_t *modH, modbus_t telegram);
// 将查询放入队列尾部并等待通知
uint32_t ModbusQueryV2(modbusHandler_t *modH, modbus_t telegram);
// 将查询放入队列头部
void ModbusQueryInject(modbusHandler_t *modH, modbus_t telegram);
// 启动 Modbus 从站任务
void StartTaskModbusSlave(void *argument);
// 启动 Modbus 主站任务
void StartTaskModbusMaster(void *argument);
// 计算 CRC 校验值
uint16_t calcCRC(uint8_t *Buffer, uint8_t u8length);

// 如果启用 TCP 功能
#if ENABLE_TCP == 1
// 关闭 TCP 连接
void ModbusCloseConn(struct netconn *conn);
// 关闭 TCP 连接并清理 Modbus 处理程序
void ModbusCloseConnNull(modbusHandler_t *modH);
#endif

// Modbus 环形缓冲区函数原型
// 向环形缓冲区添加一个字节
void RingAdd(modbusRingBuffer_t *xRingBuffer, uint8_t u8Val);
// 从环形缓冲区获取所有可用字节到缓冲区并返回读取的字节数
uint8_t RingGetAllBytes(modbusRingBuffer_t *xRingBuffer, uint8_t *buffer);
// 从环形缓冲区获取指定数量的字节，返回实际读取的字节数
uint8_t RingGetNBytes(modbusRingBuffer_t *xRingBuffer, uint8_t *buffer, uint8_t uNumber);
// 返回环形缓冲区中可用的字节数
uint8_t RingCountBytes(modbusRingBuffer_t *xRingBuffer);
// 清空环形缓冲区
void RingClear(modbusRingBuffer_t *xRingBuffer);

// 外部声明用于维护并发处理程序数量的全局变量
extern uint8_t numberHandlers;

/* 原始库中未实现的原型

// 获取传入消息数量
uint16_t getInCnt();
// 获取传出消息数量
uint16_t getOutCnt();
// 获取错误计数器值
uint16_t getErrCnt();
// 获取从站 ID，范围 1 到 247
uint8_t getID();
// 获取状态
uint8_t getState();
// 获取最后一次错误消息
uint8_t getLastError();
// 为从站写入新的 ID
void setID( uint8_t u8id );
// 设置发送结束引脚超时时间
void setTxendPinOverTime( uint32_t u32overTime );
// 结束所有通信并释放串口通信端口
void ModbusEnd();

*/

#endif /* THIRD_PARTY_MODBUS_INC_MODBUS_H_ */