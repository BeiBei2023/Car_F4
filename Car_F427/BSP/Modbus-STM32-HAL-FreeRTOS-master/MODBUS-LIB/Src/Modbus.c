/*
 * Modbus.c
 *  Modbus RTU Master and Slave library for STM32 CUBE with FreeRTOS
 *  Created on: May 5, 2020
 *      Author: Alejandro Mera
 *      Adapted from https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include "Modbus.h"
#include "timers.h"
#include "semphr.h"




#if ENABLE_TCP == 1
#include "api.h"
#include "ip4_addr.h"
#include "netif.h"
#endif

// ... existing code ...

// 启用USART DMA功能（0-禁用，1-启用），需在ModbusConfig.h中修改
#ifndef ENABLE_USART_DMA
#define ENABLE_USART_DMA 0
#endif

// 位操作宏定义
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)     // 读取指定位的值
#define bitSet(value, bit) ((value) |= (1UL << (bit)))      // 设置指定位为1
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))   // 清除指定位（置0）
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit)) // 根据条件设置位值

// 字节分割宏
#define lowByte(w) ((w) & 0xff)      // 取16位整数的低字节
#define highByte(w) ((w) >> 8)       // 取16位数的高字节




modbusHandler_t *mHandlers[MAX_M_HANDLERS];


// ... existing code ...

///Modbus主站用于存放电报的消息队列属性（FreeRTOS消息队列配置）
const osMessageQueueAttr_t QueueTelegram_attributes = {
       .name = "QueueModbusTelegram"  // 消息队列名称（用于调试识别）
};

// ... existing code ...
// ... existing code ...

// 定义 Modbus 从站任务的线程属性（非 TCP 模式）
// 该任务用于处理 Modbus 从站相关操作
const osThreadAttr_t myTaskModbusA_attributes = {
    .name = "TaskModbusSlave",  // 线程名称，方便调试时识别
    .priority = (osPriority_t) osPriorityNormal,  // 线程优先级，设置为正常优先级
    .stack_size = 128 * 4  // 线程栈大小，分配 128 * 4 字节的栈空间
};

// 定义 Modbus 从站任务的线程属性（TCP 模式）
// 由于 TCP 通信可能需要更多资源，栈空间设置更大
const osThreadAttr_t myTaskModbusA_attributesTCP = {
    .name = "TaskModbusSlave",  // 线程名称，方便调试时识别
    .priority = (osPriority_t) osPriorityNormal,  // 线程优先级，设置为正常优先级
    .stack_size = 256 * 6  // 线程栈大小，分配 256 * 6 字节的栈空间
};

// ... existing code ...



// ... existing code ...

// 注释：Modbus 主站任务
// osThreadId_t myTaskModbusAHandle;
// 定义 Modbus 主站任务的线程属性（非 TCP 模式）
const osThreadAttr_t myTaskModbusB_attributes = {
    .name = "TaskModbusMaster",  // 线程名称，用于在调试时识别该线程为 Modbus 主站任务
    .priority = (osPriority_t) osPriorityNormal,  // 线程优先级，设置为正常优先级
    .stack_size = 128 * 4  // 线程栈大小，分配 128 * 4 字节的栈空间
};

// 定义 Modbus 主站任务的线程属性（TCP 模式）
// 由于 TCP 通信可能需要更多资源，栈空间设置更大
const osThreadAttr_t myTaskModbusB_attributesTCP = {
    .name = "TaskModbusMaster",  // 线程名称，用于在调试时识别该线程为 Modbus 主站任务
    .priority = (osPriority_t) osPriorityNormal,  // 线程优先级，设置为正常优先级
    .stack_size = 256 * 4  // 线程栈大小，分配 256 * 4 字节的栈空间
};

// 用于访问 Modbus 数据的信号量属性
const osSemaphoreAttr_t ModBusSphr_attributes = {
    .name = "ModBusSphr"  // 信号量名称，用于在调试时识别该信号量
};

// ... existing code ...
// 记录当前 Modbus 处理器的数量
uint8_t numberHandlers = 0;

// 静态函数声明，用于发送 Modbus 发送缓冲区的数据
static void sendTxBuffer(modbusHandler_t *modH);
// 静态函数声明，用于从 Modbus 接收缓冲区获取数据
static int16_t getRxBuffer(modbusHandler_t *modH);
// 静态函数声明，用于验证 Modbus 应答消息的有效性
static uint8_t validateAnswer(modbusHandler_t *modH);
// 静态函数声明，用于构建 Modbus 异常响应
static void buildException( uint8_t u8exception, modbusHandler_t *modH );
// 静态函数声明，用于验证 Modbus 请求消息的有效性
static uint8_t validateRequest(modbusHandler_t * modH);
// 静态函数声明，用于将两个 8 位无符号整数组合成一个 16 位无符号整数
static uint16_t word(uint8_t H, uint8_t l);
// 静态函数声明，用于处理功能码 1 (读取线圈状态) 的请求
static void get_FC1(modbusHandler_t *modH);
// 静态函数声明，用于处理功能码 3 (读取保持寄存器) 的请求
static void get_FC3(modbusHandler_t *modH);
// 静态函数声明，用于处理功能码 1 (读取线圈状态) 的请求并返回处理结果
static int8_t process_FC1(modbusHandler_t *modH );
// 静态函数声明，用于处理功能码 3 (读取保持寄存器) 的请求并返回处理结果
static int8_t process_FC3(modbusHandler_t *modH );
// 静态函数声明，用于处理功能码 5 (写单个线圈) 的请求并返回处理结果
static int8_t process_FC5( modbusHandler_t *modH);
// 静态函数声明，用于处理功能码 6 (写单个寄存器) 的请求并返回处理结果
static int8_t process_FC6(modbusHandler_t *modH );
// 静态函数声明，用于处理功能码 15 (写多个线圈) 的请求并返回处理结果
static int8_t process_FC15(modbusHandler_t *modH );
// 静态函数声明，用于处理功能码 16 (写多个寄存器) 的请求并返回处理结果
static int8_t process_FC16(modbusHandler_t *modH);
// 静态函数声明，T35 定时器回调函数，用于处理 Modbus 通信中的 T35 超时事件
static void vTimerCallbackT35(TimerHandle_t *pxTimer);
// 静态函数声明，超时定时器回调函数，用于处理 Modbus 通信中的超时事件
static void vTimerCallbackTimeout(TimerHandle_t *pxTimer);
// 静态函数声明，用于发送 Modbus 查询请求并返回发送结果
static int8_t SendQuery(modbusHandler_t *modH ,  modbus_t telegram);

#if ENABLE_TCP ==1
// 静态函数声明，用于等待 TCP 连接数据并验证数据的有效性
static bool TCPwaitConnData(modbusHandler_t *modH);
// 静态函数声明，用于初始化 Modbus TCP 服务器
static void  TCPinitserver(modbusHandler_t *modH);
// 静态函数声明，用于连接 Modbus TCP 服务器并返回连接结果
static mb_errot_t TCPconnectserver(modbusHandler_t * modH, modbus_t *telegram);
// 静态函数声明，用于从 Modbus TCP 连接中获取接收缓冲区的数据并返回结果
static mb_errot_t TCPgetRxBuffer(modbusHandler_t * modH);
#endif



/* 环形缓冲区函数 */
// 此函数必须在禁用 USART 接收中断后调用，或者在接收中断内部调用
void RingAdd(modbusRingBuffer_t *xRingBuffer, uint8_t u8Val)
{
    // 将新值添加到环形缓冲区的当前结束位置
    xRingBuffer->uxBuffer[xRingBuffer->u8end] = u8Val;
    // 更新结束位置，使用取模运算确保索引在缓冲区范围内
    xRingBuffer->u8end = (xRingBuffer->u8end + 1) % MAX_BUFFER;

    // 检查缓冲区是否已满
    if (xRingBuffer->u8available == MAX_BUFFER)
    {
        // 缓冲区已满，设置溢出标志
        xRingBuffer->overflow = true;
        // 移除最早的数据，更新起始位置
        xRingBuffer->u8start = (xRingBuffer->u8start + 1) % MAX_BUFFER;
    }
    else
    {
        // 缓冲区未满，清除溢出标志
        xRingBuffer->overflow = false;
        // 增加可用数据数量
        xRingBuffer->u8available++;
    }
}

// 此函数必须在禁用 USART 接收中断后调用
// 从环形缓冲区中获取所有可用字节
uint8_t RingGetAllBytes(modbusRingBuffer_t *xRingBuffer, uint8_t *buffer)
{
    // 调用 RingGetNBytes 函数获取所有可用字节
    return RingGetNBytes(xRingBuffer, buffer, xRingBuffer->u8available);
}

// 此函数必须在禁用 USART 接收中断后调用
// 从环形缓冲区中获取指定数量的字节
uint8_t RingGetNBytes(modbusRingBuffer_t *xRingBuffer, uint8_t *buffer, uint8_t uNumber)
{
    // 用于计数已获取字节数的变量
    uint8_t uCounter;
    // 检查缓冲区是否为空或者请求的字节数是否为 0
    if(xRingBuffer->u8available == 0  || uNumber == 0 ) return 0;
    // 检查请求的字节数是否超过缓冲区的最大容量
    if(uNumber > MAX_BUFFER) return 0;

    // 循环从缓冲区中获取指定数量的字节
    for(uCounter = 0; uCounter < uNumber && uCounter< xRingBuffer->u8available ; uCounter++)
    {
        // 将缓冲区中的字节复制到目标缓冲区
        buffer[uCounter] = xRingBuffer->uxBuffer[xRingBuffer->u8start];
        // 更新缓冲区的起始位置
        xRingBuffer->u8start = (xRingBuffer->u8start + 1) % MAX_BUFFER;
    }
    // 更新缓冲区中可用字节的数量
    xRingBuffer->u8available = xRingBuffer->u8available - uCounter;
    // 清除溢出标志
    xRingBuffer->overflow = false;
    // 清空环形缓冲区
    RingClear(xRingBuffer);

    // 返回实际获取的字节数
    return uCounter;
}

// 计算环形缓冲区中可用字节的数量
// 参数: xRingBuffer - 指向环形缓冲区结构体的指针
// 返回值: 环形缓冲区中可用字节的数量
uint8_t RingCountBytes(modbusRingBuffer_t *xRingBuffer)
{
    // 直接返回环形缓冲区中记录的可用字节数
    return xRingBuffer->u8available;
}

// 清空环形缓冲区，将其恢复到初始状态
// 参数: xRingBuffer - 指向环形缓冲区结构体的指针
void RingClear(modbusRingBuffer_t *xRingBuffer)
{
    // 将缓冲区的起始索引置为 0
    xRingBuffer->u8start = 0;
    // 将缓冲区的结束索引置为 0
    xRingBuffer->u8end = 0;
    // 将缓冲区中可用字节数置为 0
    xRingBuffer->u8available = 0;
    // 清除溢出标志
    xRingBuffer->overflow = false;
}
/* End of Ring Buffer functions */



// 定义支持的 Modbus 功能码数组
const unsigned char fctsupported[] =
{
    MB_FC_READ_COILS,             // 功能码 1：读取线圈状态
    MB_FC_READ_DISCRETE_INPUT,    // 功能码 2：读取离散输入状态
    MB_FC_READ_REGISTERS,         // 功能码 3：读取保持寄存器
    MB_FC_READ_INPUT_REGISTER,    // 功能码 4：读取输入寄存器
    MB_FC_WRITE_COIL,             // 功能码 5：写单个线圈
    MB_FC_WRITE_REGISTER,         // 功能码 6：写单个寄存器
    MB_FC_WRITE_MULTIPLE_COILS,   // 功能码 15：写多个线圈
    MB_FC_WRITE_MULTIPLE_REGISTERS// 功能码 16：写多个寄存器
};
/**
 * @brief
 * 主站或从站的初始化函数。
 * 此函数将检查 Modbus 处理器的配置参数。
 *
 * @param modH   Modbus 处理器指针
 */
void ModbusInit(modbusHandler_t * modH)
{
    // 检查是否还有可用的 Modbus 处理器槽位
    if (numberHandlers < MAX_M_HANDLERS)
    {
        // 初始化环形缓冲区
        RingClear(&modH->xBufferRX);

        // 判断 Modbus 类型是否为从站
        if(modH->uModbusType == MB_SLAVE)
        {
            // 创建 Modbus 从站任务
#if ENABLE_TCP == 1
            // 如果硬件类型为 TCP
            if( modH->xTypeHW == TCP_HW)
            {
                // 使用 TCP 模式的线程属性创建 Modbus 从站任务
                modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusSlave, modH, &myTaskModbusA_attributesTCP);
            }
            else
            {
                // 使用非 TCP 模式的线程属性创建 Modbus 从站任务
                modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusSlave, modH, &myTaskModbusA_attributes);
            }
#else
            // 未启用 TCP 时，使用非 TCP 模式的线程属性创建 Modbus 从站任务
            modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusSlave, modH, &myTaskModbusA_attributes);
#endif
        }
        // 判断 Modbus 类型是否为主站
        else if (modH->uModbusType == MB_MASTER)
        {
            // 创建 Modbus 主站任务和电报队列
#if ENABLE_TCP == 1
            // 如果硬件类型为 TCP
            if( modH->xTypeHW == TCP_HW)
            {
                // 使用 TCP 模式的线程属性创建 Modbus 主站任务
                modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusMaster, modH, &myTaskModbusB_attributesTCP);
            }
            else
            {
                // 使用非 TCP 模式的线程属性创建 Modbus 主站任务
                modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusMaster, modH, &myTaskModbusB_attributes);
            }
#else
            // 未启用 TCP 时，使用非 TCP 模式的线程属性创建 Modbus 主站任务
            modH->myTaskModbusAHandle = osThreadNew(StartTaskModbusMaster, modH, &myTaskModbusB_attributes);
#endif

            // 创建超时定时器
            modH->xTimerTimeout=xTimerCreate("xTimerTimeout",  // 定时器名称，仅用于调试，内核不使用
                                             modH->u16timeOut ,  // 定时器周期，单位为节拍
                                             pdFALSE,          // 定时器到期后不自动重载
                                             ( void * )modH->xTimerTimeout,  // 为定时器分配唯一的 ID
                                             (TimerCallbackFunction_t) vTimerCallbackTimeout  // 定时器到期时调用的回调函数
                                             );

            // 检查定时器是否创建成功
            if(modH->xTimerTimeout == NULL)
            {
                while(1); // 定时器创建失败，检查堆和栈的大小
            }

            // 创建电报消息队列
            modH->QueueTelegramHandle = osMessageQueueNew (MAX_TELEGRAMS, sizeof(modbus_t), &QueueTelegram_attributes);

            // 检查消息队列是否创建成功
            if(modH->QueueTelegramHandle == NULL)
            {
                while(1); // 消息队列创建失败，检查堆和栈的大小
            }
        }
        else
        {
            while(1); // 不支持的 Modbus 类型，请选择有效的类型
        }

        // 检查 Modbus 任务是否创建成功
        if  (modH->myTaskModbusAHandle == NULL)
        {
            while(1); // Modbus 任务创建失败，检查堆和栈的大小
        }

        // 创建 T35 定时器
        modH->xTimerT35 = xTimerCreate("TimerT35",         // 定时器名称，仅用于调试，内核不使用
                                       T35 ,     // 定时器周期，单位为节拍
                                       pdFALSE,         // 定时器到期后不自动重载
                                       ( void * )modH->xTimerT35,     // 为定时器分配唯一的 ID
                                       (TimerCallbackFunction_t) vTimerCallbackT35     // 定时器到期时调用的回调函数
                                       );
        // 检查 T35 定时器是否创建成功
        if (modH->xTimerT35 == NULL)
        {
            while(1); // 定时器创建失败，检查堆和栈的大小
        }

        // 创建 Modbus 数据访问信号量
        modH->ModBusSphrHandle = osSemaphoreNew(1, 1, &ModBusSphr_attributes);

        // 检查信号量是否创建成功
        if(modH->ModBusSphrHandle == NULL)
        {
            while(1); // 信号量创建失败，检查堆和栈的大小
        }

        // 将 Modbus 处理器添加到处理器数组中
        mHandlers[numberHandlers] = modH;
        // 可用处理器槽位数量加 1
        numberHandlers++;
    }
    else
    {
        while(1); // 错误：没有更多的 Modbus 处理器槽位可用
    }
}
/**
 * @brief
 * 启动 Modbus 对象。
 *
 * 通常在 setup() 函数中调用串口的 begin() 函数之后调用此函数。
 *
 * （如果调用此函数，则不应调用 ModbusRtu 自身的 begin() 函数。）
 *
 * @ingroup setup
 */
void ModbusStart(modbusHandler_t * modH)
{
    // 检查硬件类型是否为支持的类型
    if(modH->xTypeHW != USART_HW && modH->xTypeHW != TCP_HW && modH->xTypeHW != USB_CDC_HW  && modH->xTypeHW != USART_HW_DMA )
    {
        while(1); // 错误：请选择有效的硬件类型
    }

    // 检查是否选择了 USART_HW_DMA 硬件类型但未启用 DMA 功能
    if (modH->xTypeHW == USART_HW_DMA && ENABLE_USART_DMA == 0  )
    {
        while(1); // 错误：要使用 USART_HW_DMA，需要在 ModbusConfig.h 文件中启用该功能
    }

    // 如果硬件类型为 USART 或 USART_HW_DMA
    if (modH->xTypeHW == USART_HW || modH->xTypeHW ==  USART_HW_DMA )
    {
        // 如果 RS485 收发器使能端口不为空
        if (modH->EN_Port != NULL )
        {
            // 将 RS485 收发器设置为接收模式
            HAL_GPIO_WritePin(modH->EN_Port, modH->EN_Pin, GPIO_PIN_RESET);
        }

        // 如果 Modbus 类型为从站且数据寄存器指针为空
        if (modH->uModbusType == MB_SLAVE &&  modH->u16regs == NULL )
        {
            while(1); // 错误：请定义通过 Modbus 共享的数据指针
        }

        // 检查串口是否已初始化
        while (HAL_UART_GetState(modH->port) != HAL_UART_STATE_READY)
        {
            // 等待串口准备好
        }

#if ENABLE_USART_DMA ==1
        // 如果硬件类型为 USART_HW_DMA
        if( modH->xTypeHW == USART_HW_DMA )
        {
            // 使用 DMA 接收数据直到空闲
            if(HAL_UARTEx_ReceiveToIdle_DMA(modH->port, modH->xBufferRX.uxBuffer, MAX_BUFFER ) != HAL_OK)
            {
                while(1)
                {
                    // 错误：初始化代码存在问题
                }
            }
            // 禁用 DMA 半传输中断
            __HAL_DMA_DISABLE_IT(modH->port->hdmarx, DMA_IT_HT); 
        }
        else
        {
            // 使用中断从串口接收 Modbus 数据
            if(HAL_UART_Receive_IT(modH->port, &modH->dataRX, 1) != HAL_OK)
            {
                while(1)
                {
                    // 错误：初始化代码存在问题
                }
            }
        }
#else
        // 使用中断从串口接收 Modbus 数据
        if(HAL_UART_Receive_IT(modH->port, &modH->dataRX, 1) != HAL_OK)
        {
            while(1)
            {
                // 错误：初始化代码存在问题
            }
        }
#endif

        // 如果 Modbus 类型为主站且 ID 不为 0
        if(modH->u8id !=0 && modH->uModbusType == MB_MASTER )
        {
            while(1)
            {
                // 错误：主站 ID 必须为 0
            }
        }

        // 如果 Modbus 类型为从站且 ID 为 0
        if(modH->u8id ==0 && modH->uModbusType == MB_SLAVE )
        {
            while(1)
            {
                // 错误：从站 ID 不能为 0
            }
        }
    }

#if ENABLE_TCP == 1
    // 此处可添加 TCP 相关初始化代码
#endif

    // 重置最后接收字节索引、缓冲区大小、输入计数、输出计数和错误计数
    modH->u8lastRec = modH->u8BufferSize = 0;
    modH->u16InCnt = modH->u16OutCnt = modH->u16errCnt = 0;
}

#if ENABLE_USB_CDC == 1
// 外部声明 USB 设备初始化函数
extern void MX_USB_DEVICE_Init(void);
/**
 * @brief
 * 启动 USB CDC 模式下的 Modbus 对象。
 *
 * @param modH Modbus 处理器指针
 */
void ModbusStartCDC(modbusHandler_t * modH)
{
    // 如果 Modbus 类型为从站且数据寄存器指针为空
    if (modH->uModbusType == MB_SLAVE &&  modH->u16regs == NULL )
    {
        while(1); // 错误：请定义通过 Modbus 共享的数据指针
    }

    // 重置最后接收字节索引、缓冲区大小、输入计数、输出计数和错误计数
    modH->u8lastRec = modH->u8BufferSize = 0;
    modH->u16InCnt = modH->u16OutCnt = modH->u16errCnt = 0;
}
#endif

/**
 * @brief T35 定时器回调函数，用于通知有新的数据流到达
 * 
 * 该函数会遍历所有的 Modbus 处理器，找到与传入定时器句柄匹配的处理器，
 * 若该处理器为 Modbus 主站，则停止超时定时器，并通知对应的 Modbus 任务。
 * 
 * @param pxTimer 定时器句柄指针
 */
void vTimerCallbackT35(TimerHandle_t *pxTimer)
{
    // 通知有新的数据流刚刚到达
    int i;
    // TimerHandle_t aux;
    // 遍历所有的 Modbus 处理器
    for(i = 0; i < numberHandlers; i++)
    {
        // 检查当前遍历到的 Modbus 处理器的 T35 定时器句柄是否与传入的定时器句柄匹配
        if( (TimerHandle_t *)mHandlers[i]->xTimerT35 ==  pxTimer ){
            // 如果当前 Modbus 处理器类型为主站
            if(mHandlers[i]->uModbusType == MB_MASTER)
            {
                // 停止主站的超时定时器
                xTimerStop(mHandlers[i]->xTimerTimeout,0);
            }
            // 通知对应的 Modbus 任务，使用新值覆盖旧值
            xTaskNotify(mHandlers[i]->myTaskModbusAHandle, 0, eSetValueWithOverwrite);
        }
    }
}
/**
 * @brief 超时定时器回调函数，当定时器超时时调用此函数
 * 
 * 该函数会遍历所有的 Modbus 处理器，找到与传入定时器句柄匹配的处理器，
 * 并向对应的 Modbus 任务发送超时错误通知。
 * 
 * @param pxTimer 定时器句柄指针
 */
void vTimerCallbackTimeout(TimerHandle_t *pxTimer)
{
    // 通知有新的数据流刚刚到达（此处注释可能不太准确，实际是超时通知）
    int i;
    // TimerHandle_t aux;
    // 遍历所有的 Modbus 处理器
    for(i = 0; i < numberHandlers; i++)
    {
        // 检查当前遍历到的 Modbus 处理器的超时定时器句柄是否与传入的定时器句柄匹配
        if( (TimerHandle_t *)mHandlers[i]->xTimerTimeout ==  pxTimer ){
            // 向对应的 Modbus 任务发送超时错误通知，使用新值覆盖旧值
            xTaskNotify(mHandlers[i]->myTaskModbusAHandle, ERR_TIME_OUT, eSetValueWithOverwrite);
        }
    }
}
#if ENABLE_TCP ==1

/**
 * @brief 等待 TCP 连接数据并验证数据的有效性
 * 
 * 该函数使用轮询的方式检查是否有新的客户端连接，并尝试接收数据。
 * 接收到数据后，会验证数据的有效性，如果有效则将数据存储到 Modbus 缓冲区中。
 * 
 * @param modH Modbus 处理器指针
 * @return bool 如果接收到有效的 Modbus 数据，返回 true；否则返回 false
 */
bool TCPwaitConnData(modbusHandler_t *modH)
{
    // 定义网络缓冲区指针，用于存储接收到的数据
    struct netbuf *inbuf;
    // 定义接收错误码和接受连接错误码
    err_t recv_err, accept_err;
    // 定义指向接收到的数据缓冲区的指针
    char* buf;
    // 定义接收到的数据缓冲区长度
    uint16_t buflen;
    // 定义 Modbus 数据长度
    uint16_t uLength;
    // 定义标志位，表示是否接收到有效的 Modbus 数据
    bool xTCPvalid;
    xTCPvalid = false;
    // 定义指向 TCP 客户端连接结构体的指针
    tcpclients_t *clientconn;

    // 使用轮询方式选择下一个要处理的连接槽位
    modH->newconnIndex++;
    if (modH->newconnIndex >= NUMBERTCPCONN)
    {
        // 如果索引超出范围，重置为 0
        modH->newconnIndex = 0;
    }
    // 获取当前要处理的客户端连接结构体
    clientconn = &modH->newconns[modH->newconnIndex];

    // 如果连接槽位为空，说明有空闲槽位，可以接受新的客户端连接
    if (clientconn->conn == NULL){
        // 接受任何传入的连接
        accept_err = netconn_accept(modH->conn, &clientconn->conn);
        if(accept_err != ERR_OK)
        {
            // 当前没有有效的传入连接
            // ModbusCloseConn(clientconn->conn);
            ModbusCloseConnNull(modH);
            return xTCPvalid;
        }
        else
        {
            // 连接成功，重置老化计数器
            clientconn->aging=0;
        }
    }

    // 设置接收超时时间
    netconn_set_recvtimeout(clientconn->conn ,  modH->u16timeOut);
    // 接收数据
    recv_err = netconn_recv(clientconn->conn, &inbuf);

    if (recv_err == ERR_CLSD) // 连接已关闭
    {
        // 关闭并清理连接
        // ModbusCloseConn(clientconn->conn);
        ModbusCloseConnNull(modH);
        // 重置老化计数器
        clientconn->aging = 0;
        return xTCPvalid;
    }

    if (recv_err == ERR_TIMEOUT) // 没有新数据
    {
        // 继续老化过程
        modH->newconns[modH->newconnIndex].aging++;

        // 如果连接已老化且长时间无活动，则关闭并清理连接
        if (modH->newconns[modH->newconnIndex].aging >= TCPAGINGCYCLES)
        {
            // ModbusCloseConn(clientconn->conn);
            ModbusCloseConnNull(modH);
            // 重置老化计数器
            clientconn->aging = 0;
        }
        return xTCPvalid;
    }

    if (recv_err == ERR_OK)
    {
        if (netconn_err(clientconn->conn) == ERR_OK)
        {
            // 从端口读取数据，如果没有数据则阻塞。假设请求（我们关心的部分）在一个网络缓冲区中
            netbuf_data(inbuf, (void**)&buf, &buflen);
            if (buflen>11) // Modbus TCP 最小帧大小
            {
                if(buf[2] == 0 || buf[3] == 0 ) // 验证协议 ID
                {
                    // 计算数据长度
                    uLength = (buf[4]<<8 & 0xff00) | buf[5];
                    if(uLength< (MAX_BUFFER-2)  && (uLength + 6) <= buflen)
                    {
                        // 将 Modbus 数据复制到缓冲区
                        for(int i = 0; i < uLength; i++)
                        {
                            modH->u8Buffer[i] = buf[i+6];
                        }
                        // 记录事务 ID
                        modH->u16TransactionID = (buf[0]<<8 & 0xff00) | buf[1];
                        // 设置缓冲区大小，添加 2 个虚拟字节用于 CRC
                        modH->u8BufferSize = uLength + 2; 
                        // 标记接收到有效的 Modbus 数据
                        xTCPvalid = true; 
                    }
                }
            }
            // 总是删除缓冲区
            netbuf_delete(inbuf); 
            // 重置老化计数器
            clientconn->aging = 0; 
        }
    }

    return xTCPvalid;
}

/**
 * @brief 初始化 Modbus TCP 服务器
 * 
 * 该函数创建一个新的 TCP 连接句柄，并将其绑定到指定的端口（默认为 502），
 * 然后将连接设置为监听状态。
 * 
 * @param modH Modbus 处理器指针
 */
void  TCPinitserver(modbusHandler_t *modH)
{
    // 定义错误码
    err_t err;

    // 如果硬件类型为 TCP
    if(modH-> xTypeHW == TCP_HW)
    {
        // 创建一个新的 TCP 连接句柄
        modH->conn = netconn_new(NETCONN_TCP);
        if (modH->conn!= NULL)
        {
            // 如果端口未定义，使用默认端口 502
            if(modH->uTcpPort == 0) modH->uTcpPort = 502; 
            // 绑定到指定端口（Modbus 默认端口 502）和默认 IP 地址
            err = netconn_bind(modH->conn, NULL, modH->uTcpPort);
            if (err == ERR_OK)
            {
                // 将连接设置为监听状态
                netconn_listen(modH->conn);
                // 设置接收超时时间，使其变为非阻塞模式
                netconn_set_recvtimeout(modH->conn, 1); 
            }
            else
            {
                while(1)
                {
                    // 绑定 TCP Modbus 端口出错，请检查配置
                }
            }
        }
        else
        {
            while(1)
            {
                // 创建新连接出错，请检查配置，此函数必须在调度器启动后调用
            }
        }
    }
}

#endif
/**
 * @brief Modbus 从站任务的入口函数
 * 
 * 该任务负责处理 Modbus 从站的相关操作，包括初始化服务器、接收和处理 Modbus 帧。
 * 
 * @param argument 传递给任务的参数，通常是指向 modbusHandler_t 结构体的指针
 */
void StartTaskModbusSlave(void *argument)
{
    // 将传入的参数转换为 modbusHandler_t 结构体指针
    modbusHandler_t *modH =  (modbusHandler_t *)argument;
    // uint32_t notification;

    // 如果启用了 TCP 且硬件类型为 TCP
#if ENABLE_TCP ==1
    if( modH->xTypeHW == TCP_HW )
    {
        // 启动 Modbus TCP 从站服务器
        TCPinitserver(modH); 
    }
#endif

    // 进入无限循环，持续处理 Modbus 从站事务
    for(;;)
    {
        // 重置最后一次错误码
        modH->i8lastError = 0;

        // 如果启用了 USB CDC 且硬件类型为 USB CDC
#if ENABLE_USB_CDC ==1
        if(modH-> xTypeHW == USB_CDC_HW)
        {
            // 无限期阻塞，直到接收到 Modbus 帧
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 
            // 检查缓冲区是否溢出
            if (modH->u8BufferSize == ERR_BUFF_OVERFLOW) 
            {
                // 记录缓冲区溢出错误
                modH->i8lastError = ERR_BUFF_OVERFLOW;
                // 错误计数加 1
                modH->u16errCnt++;
                // 跳过本次循环，继续等待下一个帧
                continue;
            }
        }
#endif

        // 如果启用了 TCP 且硬件类型为 TCP
#if ENABLE_TCP ==1
        if(modH-> xTypeHW == TCP_HW)
        {
            // 等待 TCP 连接并接收数据
            if(TCPwaitConnData(modH) == false) 
            {
                // TCP 数据包未通过验证，跳过本次循环
                continue; 
            }
        }
#endif

        // 如果硬件类型为 USART 或 USART_HW_DMA
        if(modH->xTypeHW == USART_HW || modH->xTypeHW == USART_HW_DMA)
        {
            // 阻塞直到接收到 Modbus 帧
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 
            // 从接收缓冲区获取数据并检查是否溢出
            if (getRxBuffer(modH) == ERR_BUFF_OVERFLOW)
            {
                // 记录缓冲区溢出错误
                modH->i8lastError = ERR_BUFF_OVERFLOW;
                // 错误计数加 1
                modH->u16errCnt++;
                // 跳过本次循环，继续等待下一个帧
                continue;
            }
        }

        // 检查 Modbus 帧的大小是否有效
        if (modH->u8BufferSize < 7)
        {
            // 帧大小无效，记录错误
            modH->i8lastError = ERR_BAD_SIZE;
            // 错误计数加 1
            modH->u16errCnt++;
            // 跳过本次循环，继续等待下一个帧
            continue;
        }

        // 检查广播模式，默认设置为正常地址模式
        modH->u8AddressMode = ADDRESS_NORMAL;
        // 如果帧中的地址为广播地址，则设置为广播模式
        if (modH->u8Buffer[ID] == ADDRESS_BROADCAST)
        {
            modH->u8AddressMode = ADDRESS_BROADCAST;
        }

        // 检查从站 ID 是否匹配
        if ( modH->u8Buffer[ID] !=  modH->u8id && modH->u8AddressMode != ADDRESS_BROADCAST)
        {
            // 如果未启用 TCP，跳过本次循环，该帧不是发给当前从站的
#if ENABLE_TCP == 0
            continue; 
#else
            // 如果硬件类型不是 TCP，跳过本次循环
            if(modH->xTypeHW != TCP_HW)
            {
                continue; 
            }
#endif
        }

        // 验证 Modbus 消息：CRC、功能码、地址和大小
        uint8_t u8exception = validateRequest(modH);
        // 如果验证过程中出现异常
        if (u8exception > 0)
        {
            // 如果异常不是超时异常
            if (u8exception != ERR_TIME_OUT)
            {
                // 构建异常响应
                buildException( u8exception, modH);
                // 发送异常响应
                sendTxBuffer(modH);
            }
            // 记录异常错误码
            modH->i8lastError = u8exception;
            // 跳过本次循环，继续等待下一个帧
            continue;
        }

        // 重置最后一次错误码
        modH->i8lastError = 0;
        // 在处理消息之前获取信号量，确保线程安全
        xSemaphoreTake(modH->ModBusSphrHandle , portMAX_DELAY); 

        // 根据 Modbus 消息的功能码处理消息
        switch(modH->u8Buffer[ FUNC ] )
        {
            case MB_FC_READ_COILS:
            case MB_FC_READ_DISCRETE_INPUT:
                // 广播模式下应忽略读功能
                if (modH->u8AddressMode == ADDRESS_BROADCAST)
                {
                    break;
                }
                // 处理功能码 1 相关请求
                modH->i8state = process_FC1(modH);
                break;
            case MB_FC_READ_INPUT_REGISTER:
            case MB_FC_READ_REGISTERS :
                // 广播模式下应忽略读功能
                if (modH->u8AddressMode == ADDRESS_BROADCAST)
                {
                    break;
                }
                // 处理功能码 3 相关请求
                modH->i8state = process_FC3(modH);
                break;
            case MB_FC_WRITE_COIL:
                // 处理功能码 5 相关请求
                modH->i8state = process_FC5(modH);
                break;
            case MB_FC_WRITE_REGISTER :
                // 处理功能码 6 相关请求
                modH->i8state = process_FC6(modH);
                break;
            case MB_FC_WRITE_MULTIPLE_COILS:
                // 处理功能码 15 相关请求
                modH->i8state = process_FC15(modH);
                break;
            case MB_FC_WRITE_MULTIPLE_REGISTERS :
                // 处理功能码 16 相关请求
                modH->i8state = process_FC16(modH);
                break;
            default:
                break;
        }

        // 处理完消息后释放信号量
        xSemaphoreGive(modH->ModBusSphrHandle); 

        // 继续下一次循环
        continue;
    }
}


/**
 * @brief 将 Modbus 报文添加到发送队列的尾部
 * 
 * 仅 Modbus 主站可以调用此函数，将 Modbus 报文添加到发送队列的尾部。
 * 如果调用者是从站，函数将进入无限循环报错。
 * 
 * @param modH Modbus 处理器句柄指针
 * @param telegram Modbus 报文结构体
 */
void ModbusQuery(modbusHandler_t * modH, modbus_t telegram )
{
    // 将报文添加到 Modbus 的发送队列尾部
    if (modH->uModbusType == MB_MASTER)
    {
        // 记录当前任务的 ID
        telegram.u32CurrentTask = (uint32_t *) osThreadGetId();
        // 将报文发送到队列尾部，不等待
        xQueueSendToBack(modH->QueueTelegramHandle, &telegram, 0);
    }
    else{
        while(1); // 错误：从站不能像主站一样发送查询
    }
}

/**
 * @brief 将 Modbus 报文添加到发送队列的尾部，并等待任务通知
 * 
 * 仅 Modbus 主站可以调用此函数，将 Modbus 报文添加到发送队列的尾部，
 * 然后阻塞当前任务，直到收到任务通知。
 * 
 * @param modH Modbus 处理器句柄指针
 * @param telegram Modbus 报文结构体
 * @return uint32_t 任务通知的值
 */
uint32_t ModbusQueryV2(modbusHandler_t * modH, modbus_t telegram )
{
    // 将报文添加到 Modbus 的发送队列尾部
    if (modH->uModbusType == MB_MASTER)
    {
        // 记录当前任务的 ID
        telegram.u32CurrentTask = (uint32_t *) osThreadGetId();
        // 将报文发送到队列尾部，不等待
        xQueueSendToBack(modH->QueueTelegramHandle, &telegram, 0);

        // 阻塞当前任务，直到收到任务通知
        return ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    }
    else{
        while(1); // 错误：从站不能像主站一样发送查询
    }
}

/**
 * @brief 将 Modbus 报文添加到发送队列的头部
 * 
 * 该函数会先重置 Modbus 报文发送队列，然后将指定的报文添加到队列头部。
 * 
 * @param modH Modbus 处理器句柄指针
 * @param telegram Modbus 报文结构体
 */
void ModbusQueryInject(modbusHandler_t * modH, modbus_t telegram )
{
    // 将报文添加到 Modbus 的发送队列头部
    // 重置 Modbus 报文发送队列
    xQueueReset(modH->QueueTelegramHandle);
    // 记录当前任务的 ID
    telegram.u32CurrentTask = (uint32_t *) osThreadGetId();
    // 将报文发送到队列头部，不等待
    xQueueSendToFront(modH->QueueTelegramHandle, &telegram, 0);
}

#if ENABLE_TCP ==1
/**
 * @brief 关闭并删除指定的 TCP 连接
 * 
 * 如果传入的连接句柄不为空，该函数将关闭并删除该 TCP 连接。
 * 
 * @param conn 要关闭和删除的 TCP 连接句柄
 */
void ModbusCloseConn(struct netconn *conn)
{
    if(conn != NULL)
    {
        // 关闭 TCP 连接
        netconn_close(conn);
        // 删除 TCP 连接
        netconn_delete(conn);
    }
}

/**
 * @brief 关闭并删除当前 Modbus 处理器的 TCP 连接
 * 
 * 如果当前 Modbus 处理器的指定连接槽位中的连接句柄不为空，
 * 该函数将关闭并删除该连接，并将连接句柄置为 NULL。
 * 
 * @param modH Modbus 处理器句柄指针
 */
void ModbusCloseConnNull(modbusHandler_t * modH)
{
    if(modH->newconns[modH->newconnIndex].conn  != NULL)
    {
        // 关闭 TCP 连接
        netconn_close(modH->newconns[modH->newconnIndex].conn);
        // 删除 TCP 连接
        netconn_delete(modH->newconns[modH->newconnIndex].conn);
        // 将连接句柄置为 NULL
        modH->newconns[modH->newconnIndex].conn = NULL;
    }
}
#endif

/**
 * @brief
 * *** 仅适用于 Modbus 主站 ***
 * 使用 modbus_t 报文结构体生成一个对从站的查询
 * 主站必须处于 COM_IDLE 模式。执行后，其状态将变为 COM_WAITING。
 * 此方法必须仅在 loop() 部分调用。
 *
 * @see modbus_t
 * @param modH Modbus 处理器句柄
 * @param telegram Modbus 报文结构体 (id, fct, ...)
 * @ingroup loop
 */
int8_t SendQuery(modbusHandler_t *modH ,  modbus_t telegram )
{
    uint8_t u8regsno, u8bytesno;
    uint8_t  error = 0;
    // 在处理消息之前获取信号量，确保线程安全
    xSemaphoreTake(modH->ModBusSphrHandle , portMAX_DELAY); 

    // 检查是否为主站
    if (modH->u8id!=0) error = ERR_NOT_MASTER;
    // 检查主站是否处于空闲状态
    if (modH->i8state != COM_IDLE) error = ERR_POLLING ;
    // 检查从站 ID 是否合法
    if ((telegram.u8id==0) || (telegram.u8id>247)) error = ERR_BAD_SLAVE_ID;

    if(error)
    {
        // 记录错误码
        modH->i8lastError = error;
        // 释放信号量
        xSemaphoreGive(modH->ModBusSphrHandle);
        return error;
    }

    // 设置 Modbus 寄存器起始地址
    modH->u16regs = telegram.u16reg;

    // 填充报文头
    modH->u8Buffer[ ID ]         = telegram.u8id;
    modH->u8Buffer[ FUNC ]       = telegram.u8fct;
    modH->u8Buffer[ ADD_HI ]     = highByte(telegram.u16RegAdd );
    modH->u8Buffer[ ADD_LO ]     = lowByte( telegram.u16RegAdd );

    switch( telegram.u8fct )
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUT:
    case MB_FC_READ_REGISTERS:
    case MB_FC_READ_INPUT_REGISTER:
        // 填充要读取的数量
        modH->u8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
        modH->u8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
        // 设置报文大小
        modH->u8BufferSize = 6;
        break;
    case MB_FC_WRITE_COIL:
        // 填充要写入的值
        modH->u8Buffer[ NB_HI ]      = (( telegram.u16reg[0]> 0) ? 0xff : 0);
        modH->u8Buffer[ NB_LO ]      = 0;
        // 设置报文大小
        modH->u8BufferSize = 6;
        break;
    case MB_FC_WRITE_REGISTER:
        // 填充要写入的寄存器值
        modH->u8Buffer[ NB_HI ]      = highByte( telegram.u16reg[0]);
        modH->u8Buffer[ NB_LO ]      = lowByte( telegram.u16reg[0]);
        // 设置报文大小
        modH->u8BufferSize = 6;
        break;
    case MB_FC_WRITE_MULTIPLE_COILS: // TODO: 实现 "发送线圈" 功能
        // 计算寄存器数量
        u8regsno = telegram.u16CoilsNo / 16;
        // 计算字节数
        u8bytesno = u8regsno * 2;
        if ((telegram.u16CoilsNo % 16) != 0)
        {
            u8bytesno++;
            u8regsno++;
        }

        // 填充要写入的线圈数量
        modH->u8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
        modH->u8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
        // 填充字节计数
        modH->u8Buffer[ BYTE_CNT ]    = u8bytesno;
        // 设置报文大小
        modH->u8BufferSize = 7;

        for (uint16_t i = 0; i < u8bytesno; i++)
        {
            if(i%2)
            {
                // 填充低字节
                modH->u8Buffer[ modH->u8BufferSize ] = lowByte( telegram.u16reg[ i/2 ] );
            }
            else
            {
                // 填充高字节
                modH->u8Buffer[  modH->u8BufferSize ] = highByte( telegram.u16reg[ i/2 ] );
            }
            // 增加报文大小
            modH->u8BufferSize++;
        }
        break;

    case MB_FC_WRITE_MULTIPLE_REGISTERS:
        // 填充要写入的寄存器数量
        modH->u8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
        modH->u8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
        // 填充字节计数
        modH->u8Buffer[ BYTE_CNT ]    = (uint8_t) ( telegram.u16CoilsNo * 2 );
        // 设置报文大小
        modH->u8BufferSize = 7;

        for (uint16_t i=0; i< telegram.u16CoilsNo; i++)
        {
            // 填充高字节
            modH->u8Buffer[  modH->u8BufferSize ] = highByte(  telegram.u16reg[ i ] );
            modH->u8BufferSize++;
            // 填充低字节
            modH->u8Buffer[  modH->u8BufferSize ] = lowByte( telegram.u16reg[ i ] );
            modH->u8BufferSize++;
        }
        break;
    }

    // 发送 Modbus 报文
    sendTxBuffer(modH);

    // 释放信号量
    xSemaphoreGive(modH->ModBusSphrHandle);

    // 设置主站状态为等待响应
    modH->i8state = COM_WAITING;
    // 清除最后一次错误码
    modH->i8lastError = 0;
    return 0;
}

#if ENABLE_TCP == 1
/**
 * @brief 连接到 Modbus TCP 服务器
 * 
 * 如果当前连接槽位中的连接句柄为空，该函数将创建一个新的 TCP 连接并尝试连接到指定的服务器。
 * 
 * @param modH Modbus 处理器句柄指针
 * @param telegram Modbus 报文结构体，包含服务器的 IP 地址和端口号
 * @return mb_errot_t 连接结果，ERR_OK 表示成功，其他值表示错误
 */
static  mb_errot_t TCPconnectserver(modbusHandler_t * modH, modbus_t *telegram)
{
    err_t err;
    tcpclients_t *clientconn;

    // 选择当前要使用的连接槽位
    clientconn = &modH->newconns[modH->newconnIndex];

    // 检查客户端 ID 是否合法
    if(telegram->u8clientID >= NUMBERTCPCONN )
    {
        return ERR_BAD_TCP_ID;
    }

    // 如果连接为空，打开一个新连接
    if (clientconn->conn == NULL)
    {
        // 创建一个新的 TCP 连接
        clientconn->conn = netconn_new(NETCONN_TCP);
        if (clientconn->conn  == NULL)
        {
            while(1)
            {
                // 创建新连接出错，请检查配置和堆大小
            }
        }

        // 连接到指定的服务器
        err = netconn_connect(clientconn->conn, (ip_addr_t *)&telegram->xIpAddress, telegram->u16Port);

        if (err  != ERR_OK )
        {
            // 关闭并清理连接
            ModbusCloseConnNull(modH);

            return ERR_TIME_OUT;
        }
    }
    return ERR_OK;
}
#endif
/**
 * @brief 从 Modbus TCP 连接中获取接收缓冲区的数据
 * 
 * 该函数尝试从指定的 TCP 连接中接收数据，并验证数据是否为有效的 Modbus TCP 帧。
 * 如果数据有效，则将其复制到 Modbus 处理器的缓冲区中。
 * 
 * @param modH Modbus 处理器指针
 * @return mb_errot_t 错误码，ERR_OK 表示成功，其他值表示错误
 */
static mb_errot_t TCPgetRxBuffer(modbusHandler_t * modH)
{
    // 定义网络缓冲区指针，用于存储接收到的数据
    struct netbuf *inbuf;
    // 初始化错误码为超时错误
    err_t err = ERR_TIME_OUT;
    // 定义指向接收到的数据缓冲区的指针
    char* buf;
    // 定义接收到的数据缓冲区长度
    uint16_t buflen;
    // 定义 Modbus 数据长度
    uint16_t uLength;

    // 定义指向 TCP 客户端连接结构体的指针
    tcpclients_t *clientconn;
    // 选择当前要处理的连接槽位
    clientconn = &modH->newconns[modH->newconnIndex];

    // 设置接收超时时间
    netconn_set_recvtimeout(clientconn->conn, modH->u16timeOut);
    // 接收数据
    err = netconn_recv(clientconn->conn, &inbuf);

    // 初始化 Modbus 数据长度为 0
    uLength = 0;

    // 如果接收数据成功
    if (err == ERR_OK)
    {
        // 检查连接是否仍然有效
        err = netconn_err(clientconn->conn) ;
        if (err == ERR_OK)
        {
            /* 从端口读取数据，如果没有数据则阻塞。
               假设请求（我们关心的部分）在一个网络缓冲区中 */
            err = netbuf_data(inbuf, (void**)&buf, &buflen);
            if(err == ERR_OK )
            {
                // 检查数据长度是否满足 Modbus TCP 最小帧大小要求
                if( (buflen>11  && (modH->uModbusType == MB_SLAVE  ))  ||
                    (buflen>=10 && (modH->uModbusType == MB_MASTER ))  ) 
                {
                    // 验证协议 ID
                    if(buf[2] == 0 || buf[3] == 0 ) 
                    {
                        // 计算数据长度
                        uLength = (buf[4]<<8 & 0xff00) | buf[5];
                        if(uLength< (MAX_BUFFER-2)  && (uLength + 6) <= buflen)
                        {
                            // 将 Modbus 数据复制到缓冲区
                            for(int i = 0; i < uLength; i++)
                            {
                                modH->u8Buffer[i] = buf[i+6];
                            }
                            // 记录事务 ID
                            modH->u16TransactionID = (buf[0]<<8 & 0xff00) | buf[1];
                            // 设置缓冲区大小，添加 2 个虚拟字节用于 CRC
                            modH->u8BufferSize = uLength + 2; 
                        }
                    }
                }
            } // netbuf_data
            // 总是删除缓冲区
            netbuf_delete(inbuf); 
        }
    }

    // 注释掉的代码：关闭并删除连接
    //netconn_close(modH->newconn);
    //netconn_delete(modH->newconn);

    // 返回错误码
    return err;
}

#endif
/**
 * @brief Modbus 主站任务的入口函数
 * 
 * 该任务负责处理 Modbus 主站的查询请求，包括连接到从站、发送查询、接收响应并验证响应。
 * 
 * @param argument 传递给任务的参数，通常是指向 modbusHandler_t 结构体的指针
 */
void StartTaskModbusMaster(void *argument)
{
    // 将传入的参数转换为 modbusHandler_t 结构体指针
    modbusHandler_t *modH =  (modbusHandler_t *)argument;
    // 用于存储任务通知值的变量
    uint32_t ulNotificationValue;
    // 定义 Modbus 报文结构体
    modbus_t telegram;

    // 进入无限循环，持续处理 Modbus 主站事务
    for(;;)
    {
        /* 无限期等待待发送的 Modbus 报文 */
        xQueueReceive(modH->QueueTelegramHandle, &telegram, portMAX_DELAY);

#if ENABLE_TCP ==1
        // 如果硬件类型为 TCP
        if(modH->xTypeHW == TCP_HW)
        {
            // 设置当前连接槽位索引
            modH->newconnIndex = telegram.u8clientID;
            // 连接到 Modbus TCP 服务器
            ulNotificationValue = TCPconnectserver( modH, &telegram);
            // 如果连接成功
            if(ulNotificationValue == ERR_OK)
            {
                // 发送 Modbus 查询请求
                SendQuery(modH, telegram);
                /* 阻塞直到接收到 Modbus 响应帧或查询超时 */
                // TCP 是同步接收数据和通知
                ulNotificationValue = TCPgetRxBuffer(modH); 

                // 如果接收数据时出现错误，关闭 TCP 连接
                if (ulNotificationValue != ERR_OK) 
                {
                    // 关闭并清理连接
                    ModbusCloseConnNull(modH);
                }
            }
            else
            {
                // 连接失败，关闭并清理连接
                ModbusCloseConnNull(modH);
            }
        }
        else // 为 USART 和 USB_CDC 发送查询
        {
            // 发送 Modbus 查询请求
            SendQuery(modH, telegram);
            /* 阻塞直到接收到 Modbus 响应帧或查询超时 */
            ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
#else
        /* 等待 Modbus 帧之间的静默期 */
        if(modH->port->Init.BaudRate <= 19200)
            osDelay((int)(35000/modH->port->Init.BaudRate) + 2);
        else
            osDelay(3);

        // 仅支持 USART 的实现情况
        // 发送 Modbus 查询请求
        SendQuery(modH, telegram);
        /* 无限期阻塞直到接收到 Modbus 响应帧或查询超时 */
        ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#endif

        // 重置最后一次错误码
        modH->i8lastError = 0;
        // 如果收到通知值，说明请求超时
        if(ulNotificationValue)
        {
            // 设置主站状态为空闲
            modH->i8state = COM_IDLE;
            // 记录超时错误
            modH->i8lastError = ERR_TIME_OUT;
            // 错误计数加 1
            modH->u16errCnt++;
            // 通知发起查询的任务请求超时
            xTaskNotify((TaskHandle_t)telegram.u32CurrentTask, modH->i8lastError, eSetValueWithOverwrite);
            // 跳过本次循环，继续等待下一个报文
            continue;
        }

#if ENABLE_USB_CDC ==1 || ENABLE_TCP ==1
        // 如果硬件类型为 USART，需要调用 getRxBuffer 获取数据
        if(modH->xTypeHW == USART_HW) 
        {
            getRxBuffer(modH);
        }
#else
        // 未启用 USB_CDC 和 TCP 时，调用 getRxBuffer 获取数据
        getRxBuffer(modH);
#endif

        // 检查接收到的 Modbus 响应帧大小是否有效
        if ( modH->u8BufferSize < 6)
        {
            // 设置主站状态为空闲
            modH->i8state = COM_IDLE;
            // 记录帧大小错误
            modH->i8lastError = ERR_BAD_SIZE;
            // 错误计数加 1
            modH->u16errCnt++;
            // 通知发起查询的任务帧大小错误
            xTaskNotify((TaskHandle_t)telegram.u32CurrentTask, modH->i8lastError, eSetValueWithOverwrite);
            // 跳过本次循环，继续等待下一个报文
            continue;
        }

        // 停止超时定时器
        xTimerStop(modH->xTimerTimeout,0); 

        // 验证 Modbus 响应消息：ID、CRC、功能码、异常码
        int8_t u8exception = validateAnswer(modH);
        // 如果验证过程中出现异常
        if (u8exception != 0)
        {
            // 设置主站状态为空闲
            modH->i8state = COM_IDLE;
            // 记录异常错误码
            modH->i8lastError = u8exception;
            // 通知发起查询的任务出现异常
            xTaskNotify((TaskHandle_t)telegram.u32CurrentTask, modH->i8lastError, eSetValueWithOverwrite);
            // 跳过本次循环，继续等待下一个报文
            continue;
        }

        // 记录最后一次错误码
        modH->i8lastError = u8exception;

        // 在处理消息之前获取信号量，确保线程安全
        xSemaphoreTake(modH->ModBusSphrHandle , portMAX_DELAY); 
        // 根据 Modbus 响应消息的功能码处理消息
        switch( modH->u8Buffer[ FUNC ] )
        {
            case MB_FC_READ_COILS:
            case MB_FC_READ_DISCRETE_INPUT:
                // 调用 get_FC1 函数将接收到的消息传输到 u16regs 缓冲区
                get_FC1(modH);
                break;
            case MB_FC_READ_INPUT_REGISTER:
            case MB_FC_READ_REGISTERS :
                // 调用 get_FC3 函数将接收到的消息传输到 u16regs 缓冲区
                get_FC3(modH);
                break;
            case MB_FC_WRITE_COIL:
            case MB_FC_WRITE_REGISTER :
            case MB_FC_WRITE_MULTIPLE_COILS:
            case MB_FC_WRITE_MULTIPLE_REGISTERS :
                // 无需额外处理
                break;
            default:
                break;
        }
        // 设置主站状态为空闲
        modH->i8state = COM_IDLE;

        // 如果没有错误，使用 OP_OK_QUERY 通知发起查询的任务
        if (modH->i8lastError ==0) 
        {
            // 释放信号量
            xSemaphoreGive(modH->ModBusSphrHandle); 
            // 通知发起查询的任务操作成功
            xTaskNotify((TaskHandle_t)telegram.u32CurrentTask, OP_OK_QUERY, eSetValueWithOverwrite);
        }

        // 继续下一次循环
        continue;
    }
}
/**
 * @brief 处理 Modbus 主站的功能码 1 和 2
 * 
 * 该方法将从站的响应数据放入主站的数据缓冲区中。
 * 
 * @ingroup register
 * @param modH Modbus 处理器指针
 */
void get_FC1(modbusHandler_t *modH)
{
    // 定义字节偏移量和循环计数器
    uint8_t u8byte, i;
    // 初始化字节偏移量为 3
    u8byte = 3;
    // 遍历从站响应数据中的每个字节
    for (i = 0; i < modH->u8Buffer[2]; i++) {
        // 根据索引的奇偶性处理数据
        if(i % 2)
        {
            // 奇数索引：将当前字节与主站寄存器的低字节组合
            modH->u16regs[i/2] = word(modH->u8Buffer[i + u8byte], lowByte(modH->u16regs[i/2]));
        }
        else
        {
            // 偶数索引：将主站寄存器的高字节与当前字节组合
            modH->u16regs[i/2] = word(highByte(modH->u16regs[i/2]), modH->u8Buffer[i + u8byte]);
        }
    }
}
/**
 * @brief 处理 Modbus 主站的功能码 3 和 4
 * 
 * 该方法将从站的响应数据放入主站的数据缓冲区中。
 * 
 * @ingroup register
 * @param modH Modbus 处理器指针
 */
void get_FC3(modbusHandler_t *modH)
{
    // 定义字节偏移量和循环计数器
    uint8_t u8byte, i;
    // 初始化字节偏移量为 3
    u8byte = 3;

    // 遍历从站响应数据中的每个 16 位寄存器
    for (i = 0; i < modH->u8Buffer[2] / 2; i++)
    {
        // 将两个 8 位字节组合成一个 16 位寄存器值，并存储到主站的数据缓冲区中
        modH->u16regs[i] = word(modH->u8Buffer[u8byte], modH->u8Buffer[u8byte + 1]);
        // 字节偏移量增加 2，指向下一个 16 位寄存器
        u8byte += 2;
    }
}

/**
 * @brief
 * 此方法用于验证 Modbus 主站接收到的消息
 *
 * @return 如果验证通过返回 0，若出现错误则返回异常码
 * @ingroup buffer
 */
uint8_t validateAnswer(modbusHandler_t *modH)
{
    // 检查消息的 CRC 校验值与计算得到的 CRC 校验值是否一致

#if ENABLE_TCP ==1
    // 如果硬件类型不是 TCP
    if(modH->xTypeHW != TCP_HW)
    {
#endif
    // 组合消息中的 CRC 校验值（高字节和低字节）
    uint16_t u16MsgCRC =
        ((modH->u8Buffer[modH->u8BufferSize - 2] << 8)
         | modH->u8Buffer[modH->u8BufferSize - 1]); 
    // 计算消息的 CRC 校验值并与消息中的 CRC 校验值比较
    if ( calcCRC(modH->u8Buffer,  modH->u8BufferSize-2) != u16MsgCRC )
    {
        // 错误计数加 1
        modH->u16errCnt ++;
        // 返回 CRC 校验错误码
        return ERR_BAD_CRC;
    }
#if ENABLE_TCP ==1
    }
#endif

    // 检查是否存在异常码
    if ((modH->u8Buffer[ FUNC ] & 0x80) != 0)
    {
        // 错误计数加 1
        modH->u16errCnt ++;
        // 返回异常错误码
        return ERR_EXCEPTION;
    }

    // 检查功能码是否支持
    bool isSupported = false;
    // 遍历支持的功能码数组
    for (uint8_t i = 0; i< sizeof( fctsupported ); i++)
    {
        // 如果当前功能码在支持的功能码数组中
        if (fctsupported[i] == modH->u8Buffer[FUNC])
        {
            // 标记功能码支持
            isSupported = 1;
            break;
        }
    }
    // 如果功能码不支持
    if (!isSupported)
    {
        // 错误计数加 1
        modH->u16errCnt ++;
        // 返回不支持的功能码错误码
        return EXC_FUNC_CODE;
    }

    // 验证通过，没有异常码抛出
    return 0; 
}
/**
 * @brief
 * 此方法将串口缓冲区的数据移动到 Modbus 的 u8Buffer 中。
 *
 * @return 如果操作成功，返回缓冲区大小；如果 u8BufferSize >= MAX_BUFFER，返回 ERR_BUFF_OVERFLOW
 * @ingroup buffer
 */
int16_t getRxBuffer(modbusHandler_t *modH)
{
    // 定义结果变量，用于存储操作结果
    int16_t i16result;

    // 如果硬件类型为 USART
    if(modH->xTypeHW == USART_HW)
    {
        // 中止串口接收中断，避免串口出现竞态条件
        HAL_UART_AbortReceive_IT(modH->port); 
    }

    // 如果环形缓冲区溢出
    if (modH->xBufferRX.overflow)
    {
        // 清空溢出的环形缓冲区
        RingClear(&modH->xBufferRX); 
        // 将结果设置为缓冲区溢出错误码
        i16result =  ERR_BUFF_OVERFLOW;
    }
    else
    {
        // 从环形缓冲区中获取所有可用字节到 Modbus 的 u8Buffer 中，并更新缓冲区大小
        modH->u8BufferSize = RingGetAllBytes(&modH->xBufferRX, modH->u8Buffer);
        // 输入计数加 1
        modH->u16InCnt++;
        // 将结果设置为实际获取的缓冲区大小
        i16result = modH->u8BufferSize;
    }

    // 如果硬件类型为 USART
    if(modH->xTypeHW == USART_HW)
    {
        // 重新启用串口接收中断
        HAL_UART_Receive_IT(modH->port, &modH->dataRX, 1);
    }

    // 返回操作结果
    return i16result;
}

/**
 * @brief
 * 此方法用于验证从站接收到的消息
 *
 * @return 如果验证通过返回 0，若出现错误则返回异常码
 * @ingroup modH Modbus 处理器句柄
 */
uint8_t validateRequest(modbusHandler_t *modH)
{
    // 检查消息的 CRC 校验值与计算得到的 CRC 校验值是否一致

#if ENABLE_TCP ==1
    uint16_t u16MsgCRC;
    // 组合消息中的 CRC 校验值（高字节和低字节）
    u16MsgCRC= ((modH->u8Buffer[modH->u8BufferSize - 2] << 8)
                | modH->u8Buffer[modH->u8BufferSize - 1]); 

    // 如果硬件类型不是 TCP
    if (modH->xTypeHW != TCP_HW)
    {
        // 计算消息的 CRC 校验值并与消息中的 CRC 校验值比较
        if ( calcCRC( modH->u8Buffer,  modH->u8BufferSize-2 ) != u16MsgCRC )
        {
            // 错误计数加 1
            modH->u16errCnt ++;
            // 返回 CRC 校验错误码
            return ERR_BAD_CRC;
        }
    }
#else
    uint16_t u16MsgCRC;
    // 组合消息中的 CRC 校验值（高字节和低字节）
    u16MsgCRC= ((modH->u8Buffer[modH->u8BufferSize - 2] << 8)
                | modH->u8Buffer[modH->u8BufferSize - 1]); 

    // 计算消息的 CRC 校验值并与消息中的 CRC 校验值比较
    if ( calcCRC( modH->u8Buffer,  modH->u8BufferSize-2 ) != u16MsgCRC )
    {
        // 错误计数加 1
        modH->u16errCnt ++;
        // 返回 CRC 校验错误码
        return ERR_BAD_CRC;
    }
#endif

    // 检查功能码是否支持
    bool isSupported = false;
    // 遍历支持的功能码数组
    for (uint8_t i = 0; i< sizeof( fctsupported ); i++)
    {
        // 如果当前功能码在支持的功能码数组中
        if (fctsupported[i] == modH->u8Buffer[FUNC])
        {
            // 标记功能码支持
            isSupported = 1;
            break;
        }
    }
    // 如果功能码不支持
    if (!isSupported)
    {
        // 错误计数加 1
        modH->u16errCnt ++;
        // 返回不支持的功能码错误码
        return EXC_FUNC_CODE;
    }

    // 检查起始地址和寄存器数量范围
    uint16_t u16AdRegs = 0;
    uint16_t u16NRegs = 0;

    // 根据不同的功能码进行不同的地址和数量检查
    switch ( modH->u8Buffer[ FUNC ] )
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUT:
    case MB_FC_WRITE_MULTIPLE_COILS:
        // 计算寄存器起始地址（以 16 位为单位）
        u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]) / 16;
        // 计算寄存器数量（以 16 位为单位）
        u16NRegs = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]) /16;
        // 检查是否有未完整的 16 位寄存器
        if(word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]) % 16) u16NRegs++; 
        // 验证地址范围
        if((u16AdRegs + u16NRegs) > modH->u16regsize) return EXC_ADDR_RANGE;

        // 验证响应帧的字节大小
        u16NRegs = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]) / 8;
        if(word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]) % 8) u16NRegs++;
        // 加上头部和 CRC 字节数
        u16NRegs = u16NRegs + 5; 
        if(u16NRegs > 256) return EXC_REGS_QUANT;

        break;
    case MB_FC_WRITE_COIL:
        // 计算寄存器起始地址（以 16 位为单位）
        u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]) / 16;
        // 检查是否有未完整的 16 位寄存器
        if(word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]) % 16) u16AdRegs++;	
        if (u16AdRegs >= modH->u16regsize) return EXC_ADDR_RANGE;
        break;
    case MB_FC_WRITE_REGISTER :
        // 计算寄存器起始地址
        u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]);
        if (u16AdRegs >= modH-> u16regsize) return EXC_ADDR_RANGE;
        break;
    case MB_FC_READ_REGISTERS :
    case MB_FC_READ_INPUT_REGISTER :
    case MB_FC_WRITE_MULTIPLE_REGISTERS :
        // 计算寄存器起始地址
        u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]);
        // 计算寄存器数量
        u16NRegs = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]);
        if (( u16AdRegs + u16NRegs ) > modH->u16regsize) return EXC_ADDR_RANGE;

        // 验证响应帧的字节大小
        u16NRegs = u16NRegs*2 + 5; // 加上头部和 CRC 字节数
        if ( u16NRegs > 256 ) return EXC_REGS_QUANT;
        break;
    }
    return 0; // 验证通过，没有异常码抛出
}

/**
 * @brief
 * 此方法将两个 8 位字节组合成一个 16 位字
 *
 * @return uint16_t 组合后的 16 位字
 * @ingroup H 高字节
 * @ingroup L 低字节
 */
uint16_t word(uint8_t H, uint8_t L)
{
    bytesFields W;
    W.u8[0] = L;
    W.u8[1] = H;

    return W.u16[0];
}

/**
 * @brief
 * 此方法计算消息的 CRC 校验值
 *
 * @return uint16_t 计算得到的消息 CRC 校验值
 * @ingroup Buffer 消息缓冲区
 * @ingroup u8length 消息长度
 */
uint16_t calcCRC(uint8_t *Buffer, uint8_t u8length)
{
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    // 遍历消息缓冲区中的每个字节
    for (unsigned char i = 0; i < u8length; i++)
    {
        temp = temp ^ Buffer[i];
        // 对每个字节的每一位进行处理
        for (unsigned char j = 1; j <= 8; j++)
        {
            flag = temp & 0x0001;
            temp >>=1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    // 反转字节顺序
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    // 返回的 CRC 值已经交换了字节顺序，低字节在前，高字节在后
    return temp;
}

/**
 * @brief
 * 此方法用于构建异常消息
 *
 * @param u8exception 异常编号
 * @param modH Modbus 处理器句柄
 */
void buildException( uint8_t u8exception, modbusHandler_t *modH )
{
    // 获取原始的功能码
    uint8_t u8func = modH->u8Buffer[ FUNC ];  

    // 设置 Modbus 消息的 ID 字段为当前 Modbus 处理器的 ID
    modH->u8Buffer[ ID ]      = modH->u8id;
    // 设置功能码为原始功能码加上 0x80，表示异常响应
    modH->u8Buffer[ FUNC ]    = u8func + 0x80;
    // 设置异常码字段
    modH->u8Buffer[ 2 ]       = u8exception;
    // 设置缓冲区大小为异常消息的固定大小
    modH->u8BufferSize         = EXCEPTION_SIZE;
}

// 如果启用了 USB CDC 功能
#if ENABLE_USB_CDC == 1
// 外部声明 USB CDC 传输函数
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
#endif

/**
 * @brief
 * 此方法将 u8Buffer 中的数据发送到串口。
 * 仅当 u8txenpin 不为 0 时，才会进行流控制，以确保在消息发送期间 RS485 收发器处于输出状态。
 * 这通过检查 TC 位来实现。
 * 在开始发送之前，会将 CRC 校验值追加到缓冲区中。
 *
 * @param modH Modbus 处理器句柄
 */
static void sendTxBuffer(modbusHandler_t *modH)
{
    // 当处于从站模式且地址模式为广播地址时，不发送任何数据
    if (modH->uModbusType == MB_SLAVE && modH->u8AddressMode == ADDRESS_BROADCAST)
    {
        // 清空缓冲区大小
        modH->u8BufferSize = 0;
        // 增加消息计数器
        modH->u16OutCnt++;
        return;
    }

    // 将 CRC 校验值追加到消息中

#if  ENABLE_TCP == 1
    // 如果硬件类型不是 TCP
    if(modH->xTypeHW != TCP_HW)
    {
#endif
        // 计算消息的 CRC 校验值
        uint16_t u16crc = calcCRC(modH->u8Buffer, modH->u8BufferSize);
        // 将 CRC 校验值的高字节添加到缓冲区
        modH->u8Buffer[ modH->u8BufferSize ] = u16crc >> 8;
        modH->u8BufferSize++;
        // 将 CRC 校验值的低字节添加到缓冲区
        modH->u8Buffer[ modH->u8BufferSize ] = u16crc & 0x00ff;
        modH->u8BufferSize++;
#if ENABLE_TCP == 1
    }
#endif

#if ENABLE_USB_CDC == 1 || ENABLE_TCP == 1
    // 如果硬件类型为 USART 或 USART_HW_DMA
    if(modH->xTypeHW == USART_HW || modH->xTypeHW == USART_HW_DMA )
    {
#endif
        // 如果 RS485 收发器使能端口不为空
        if (modH->EN_Port != NULL)
        {
            // 启用发送器，禁用接收器，以避免 RS485 收发器产生回波
            HAL_HalfDuplex_EnableTransmitter(modH->port);
            // 设置 RS485 收发器使能引脚为高电平
            HAL_GPIO_WritePin(modH->EN_Port, modH->EN_Pin, GPIO_PIN_SET);
        }

#if ENABLE_USART_DMA ==1
        // 如果硬件类型为 USART
        if(modH->xTypeHW == USART_HW)
        {
#endif
            // 使用中断方式将缓冲区数据发送到串口
            HAL_UART_Transmit_IT(modH->port, modH->u8Buffer,  modH->u8BufferSize);
#if ENABLE_USART_DMA ==1
        }
        else
        {
            // 使用 DMA 方式将缓冲区数据发送到串口
            HAL_UART_Transmit_DMA(modH->port, modH->u8Buffer, modH->u8BufferSize);
        }
#endif

        // 等待发送完成中断通知，超时时间为 250 个节拍
        ulTaskNotifyTake(pdTRUE, 250); 
        /*
        * 如果你要将库移植到不同的 MCU，请检查 USART 数据手册，
        * 并在以下预处理器条件中添加相应的系列
        */
#if defined(STM32H7)  || defined(STM32F3) || defined(STM32L4) || defined(STM32L082xx) || defined(STM32F7) || defined(STM32WB) || defined(STM32G070xx) || defined(STM32F0) || defined(STM32G431xx) || defined(STM32H5)
        // 等待发送完成标志位（ISR 寄存器中的 TC 位）
        while((modH->port->Instance->ISR & USART_ISR_TC) ==0 )
#else
        // 对于 F429, F103, L152 等系列
        while((modH->port->Instance->SR & USART_SR_TC) ==0 )
#endif
        {
            // 阻塞任务，直到最后一个字节从 USART 的移位缓冲区发送出去
        }

        // 如果 RS485 收发器使能端口不为空
        if (modH->EN_Port != NULL)
        {
            // 将 RS485 收发器设置为接收模式
            HAL_GPIO_WritePin(modH->EN_Port, modH->EN_Pin, GPIO_PIN_RESET);
            // 启用接收器，禁用发送器
            HAL_HalfDuplex_EnableReceiver(modH->port);
        }

        // 如果是 Modbus 主站模式，重置超时定时器
        if(modH->uModbusType == MB_MASTER )
        {
            xTimerReset(modH->xTimerTimeout,0);
        }
#if ENABLE_USB_CDC == 1 || ENABLE_TCP == 1
    }

#if ENABLE_USB_CDC == 1
    // 如果硬件类型为 USB CDC
    else if(modH->xTypeHW == USB_CDC_HW)
    {
        // 使用 USB CDC 方式发送数据
        CDC_Transmit_FS(modH->u8Buffer,  modH->u8BufferSize);
        // 如果是 Modbus 主站模式，重置超时定时器
        if(modH->uModbusType == MB_MASTER )
        {
            xTimerReset(modH->xTimerTimeout,0);
        }
    }
#endif

#if ENABLE_TCP == 1
    // 如果硬件类型为 TCP
    else if(modH->xTypeHW == TCP_HW)
    {
        // 定义网络向量数组
        struct netvector  xNetVectors[2];
        // 定义 Modbus TCP 应用协议头
        uint8_t u8MBAPheader[6];
        // 定义已写入的字节数
        size_t uBytesWritten;

        // 设置事务 ID 的高字节
        u8MBAPheader[0] = highByte(modH->u16TransactionID); // 此处可能需要改进，事务 ID 可以进行验证
        // 设置事务 ID 的低字节
        u8MBAPheader[1] = lowByte(modH->u16TransactionID);
        // 设置协议 ID 高字节为 0
        u8MBAPheader[2] = 0; 
        // 设置协议 ID 低字节为 0
        u8MBAPheader[3] = 0; 
        // 数据长度高字节始终为 0
        u8MBAPheader[4] = 0; 
        // 数据长度低字节为缓冲区大小
        u8MBAPheader[5] = modH->u8BufferSize; 

        // 设置第一个网络向量的长度和指针
        xNetVectors[0].len = 6;
        xNetVectors[0].ptr = (void *) u8MBAPheader;

        // 设置第二个网络向量的长度和指针
        xNetVectors[1].len = modH->u8BufferSize;
        xNetVectors[1].ptr = (void *) modH->u8Buffer;

        // 设置发送超时时间
        netconn_set_sendtimeout(modH->newconns[modH->newconnIndex].conn, modH->u16timeOut);
        // 定义错误枚举类型变量
        err_enum_t err;

        // 部分写入数据到网络连接
        err = netconn_write_vectors_partly(modH->newconns[modH->newconnIndex].conn, xNetVectors, 2, NETCONN_COPY, &uBytesWritten);
        if (err != ERR_OK )
        {
            // 关闭连接
            // ModbusCloseConn(modH->newconns[modH->newconnIndex].conn);
            ModbusCloseConnNull(modH);
        }

        // 如果是 Modbus 主站模式，重置超时定时器
        if(modH->uModbusType == MB_MASTER )
        {
            xTimerReset(modH->xTimerTimeout,0);
        }
    }
#endif

#endif

    // 清空缓冲区大小
    modH->u8BufferSize = 0;
    // 增加消息计数器
    modH->u16OutCnt++;
}

/**
 * @brief
 * 此方法处理功能码 1 和 2
 * 此方法读取一个位数组并将其传输给主站
 *
 * @param modH Modbus 处理器句柄
 * @return 响应主站的消息长度
 */
int8_t process_FC1(modbusHandler_t *modH )
{
    // 当前寄存器地址
    uint16_t u16currentRegister;
    // 当前位位置
    uint8_t u8currentBit, u8bytesno, u8bitsno;
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 当前线圈地址和线圈编号
    uint16_t u16currentCoil, u16coil;

    // 从消息中获取起始线圈地址
    uint16_t u16StartCoil = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ] );
    // 从消息中获取线圈数量
    uint16_t u16Coilno = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ] );

    // 计算输出消息中的字节数
    u8bytesno = (uint8_t) (u16Coilno / 8);
    if (u16Coilno % 8 != 0) u8bytesno ++;
    // 将字节数放入输出消息
    modH->u8Buffer[ ADD_HI ]  = u8bytesno;
    // 设置缓冲区大小
    modH->u8BufferSize         = ADD_LO;
    // 将缓冲区最后一个字节置 0
    modH->u8Buffer[modH->u8BufferSize + u8bytesno - 1 ] = 0;

    // 初始化位计数器
    u8bitsno = 0;

    // 遍历每个线圈
    for (u16currentCoil = 0; u16currentCoil < u16Coilno; u16currentCoil++)
    {
        // 计算当前线圈地址
        u16coil = u16StartCoil + u16currentCoil;
        // 计算当前寄存器地址
        u16currentRegister =  (u16coil / 16);
        // 计算当前位位置
        u8currentBit = (uint8_t) (u16coil % 16);

        // 将寄存器中的位值写入输出缓冲区
        bitWrite(
            modH->u8Buffer[ modH->u8BufferSize ],
            u8bitsno,
            bitRead( modH->u16regs[ u16currentRegister ], u8currentBit ) );
        // 位计数器加 1
        u8bitsno ++;

        // 如果位计数器超过 7，重置位计数器并增加缓冲区大小
        if (u8bitsno > 7)
        {
            u8bitsno = 0;
            modH->u8BufferSize++;
        }
    }

    // 如果线圈数量不是 8 的倍数，增加缓冲区大小
    if (u16Coilno % 8 != 0) modH->u8BufferSize ++;
    // 复制缓冲区大小
    u8CopyBufferSize = modH->u8BufferSize +2;
    // 发送输出消息
    sendTxBuffer(modH);
    return u8CopyBufferSize;
}
/**
 * @brief
 * 此方法处理功能码 3 和 4
 * 此方法读取一个字数组并将其传输给主站
 *
 * @return u8BufferSize 响应主站的消息长度
 * @ingroup register
 */
int8_t process_FC3(modbusHandler_t *modH)
{
    // 从消息中获取起始寄存器地址
    uint16_t u16StartAdd = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ] );
    // 从消息中获取寄存器数量
    uint8_t u8regsno = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ] );
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 循环计数器
    uint16_t i;

    // 设置响应消息中数据字节数
    modH->u8Buffer[ 2 ]       = u8regsno * 2;
    // 初始化缓冲区大小
    modH->u8BufferSize         = 3;

    // 遍历指定范围内的寄存器
    for (i = u16StartAdd; i < u16StartAdd + u8regsno; i++)
    {
        // 将寄存器的高字节添加到响应缓冲区
        modH->u8Buffer[ modH->u8BufferSize ] = highByte(modH->u16regs[i]);
        modH->u8BufferSize++;
        // 将寄存器的低字节添加到响应缓冲区
        modH->u8Buffer[ modH->u8BufferSize ] = lowByte(modH->u16regs[i]);
        modH->u8BufferSize++;
    }
    // 计算包含 CRC 校验的缓冲区大小
    u8CopyBufferSize = modH->u8BufferSize +2;
    // 发送响应缓冲区数据
    sendTxBuffer(modH);

    return u8CopyBufferSize;
}

/**
 * @brief
 * 此方法处理功能码 5
 * 此方法将主站分配的值写入单个位
 *
 * @return u8BufferSize 响应主站的消息长度
 * @ingroup discrete
 */
int8_t process_FC5( modbusHandler_t *modH )
{
    // 当前位的位置
    uint8_t u8currentBit;
    // 当前寄存器的索引
    uint16_t u16currentRegister;
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 从消息中获取要操作的线圈地址
    uint16_t u16coil = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ] );

    // 计算当前寄存器的索引
    u16currentRegister = (u16coil / 16);
    // 计算当前位在寄存器中的位置
    u8currentBit = (uint8_t) (u16coil % 16);

    // 将主站指定的值写入线圈
    bitWrite(
        modH->u16regs[ u16currentRegister ],
        u8currentBit,
        modH->u8Buffer[ NB_HI ] == 0xff );

    // 设置响应消息的缓冲区大小
    modH->u8BufferSize = 6;
    // 计算包含 CRC 校验的缓冲区大小
    u8CopyBufferSize =  modH->u8BufferSize +2;
    // 发送响应缓冲区数据
    sendTxBuffer(modH);

    return u8CopyBufferSize;
}

/**
 * @brief
 * 此方法处理功能码 6
 * 此方法将主站分配的值写入单个字
 *
 * @return u8BufferSize 响应主站的消息长度
 * @ingroup register
 */
int8_t process_FC6(modbusHandler_t *modH )
{
    // 从消息中获取要操作的寄存器地址
    uint16_t u16add = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ] );
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 从消息中获取要写入的值
    uint16_t u16val = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ] );

    // 将值写入指定寄存器
    modH->u16regs[ u16add ] = u16val;

    // 保持响应消息的头部不变
    modH->u8BufferSize = RESPONSE_SIZE;

    // 计算包含 CRC 校验的缓冲区大小
    u8CopyBufferSize = modH->u8BufferSize + 2;
    // 发送响应缓冲区数据
    sendTxBuffer(modH);

    return u8CopyBufferSize;
}

/**
 * @brief
 * 此方法处理功能码 15
 * 此方法将主站分配的位数组写入寄存器
 *
 * @return u8BufferSize 响应主站的消息长度
 * @ingroup discrete
 */
int8_t process_FC15( modbusHandler_t *modH )
{
    // 当前位的位置
    uint8_t u8currentBit, u8frameByte, u8bitsno;
    // 当前寄存器的索引
    uint16_t u16currentRegister;
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 当前线圈的索引和地址
    uint16_t u16currentCoil, u16coil;
    // 临时布尔变量
    bool bTemp;

    // 从消息中获取起始线圈地址
    uint16_t u16StartCoil = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ] );
    // 从消息中获取线圈数量
    uint16_t u16Coilno = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ] );

    // 位计数器初始化为 0
    u8bitsno = 0;
    // 帧字节索引初始化为 7
    u8frameByte = 7;
    // 遍历所有线圈
    for (u16currentCoil = 0; u16currentCoil < u16Coilno; u16currentCoil++)
    {
        // 计算当前线圈地址
        u16coil = u16StartCoil + u16currentCoil;
        // 计算当前寄存器的索引
        u16currentRegister = (u16coil / 16);
        // 计算当前位在寄存器中的位置
        u8currentBit = (uint8_t) (u16coil % 16);

        // 从消息中读取当前位的值
        bTemp = bitRead(
            modH->u8Buffer[ u8frameByte ],
            u8bitsno );

        // 将位值写入寄存器
        bitWrite(
            modH->u16regs[ u16currentRegister ],
            u8currentBit,
            bTemp );

        // 位计数器加 1
        u8bitsno ++;

        // 如果位计数器超过 7，重置位计数器并增加帧字节索引
        if (u8bitsno > 7)
        {
            u8bitsno = 0;
            u8frameByte++;
        }
    }

    // 发送响应消息，响应消息是输入帧的前 6 个字节的副本
    modH->u8BufferSize         = 6;
    // 计算包含 CRC 校验的缓冲区大小
    u8CopyBufferSize = modH->u8BufferSize +2;
    // 发送响应缓冲区数据
    sendTxBuffer(modH);
    return u8CopyBufferSize;
}

/**
 * @brief
 * 此方法处理功能码 16
 * 此方法将主站分配的字数组写入寄存器
 *
 * @return u8BufferSize 响应主站的消息长度
 * @ingroup register
 */
int8_t process_FC16(modbusHandler_t *modH )
{
    // 从消息中获取起始寄存器地址
    uint16_t u16StartAdd = modH->u8Buffer[ ADD_HI ] << 8 | modH->u8Buffer[ ADD_LO ];
    // 从消息中获取寄存器数量
    uint16_t u16regsno = modH->u8Buffer[ NB_HI ] << 8 | modH->u8Buffer[ NB_LO ];
    // 复制的缓冲区大小
    uint8_t u8CopyBufferSize;
    // 循环计数器
    uint16_t i;
    // 临时变量，用于存储读取的值
    uint16_t temp;

    // 构建响应消息的头部
    modH->u8Buffer[ NB_HI ]   = 0;
    // 响应消息的寄存器数量
    modH->u8Buffer[ NB_LO ]   = (uint8_t) u16regsno; // 响应总是 256 字节或更少
    // 初始化响应消息的缓冲区大小
    modH->u8BufferSize         = RESPONSE_SIZE;

    // 写入寄存器
    for (i = 0; i < u16regsno; i++)
    {
        // 从消息中读取要写入的值
        temp = word(
            modH->u8Buffer[ (BYTE_CNT + 1) + i * 2 ],
            modH->u8Buffer[ (BYTE_CNT + 2) + i * 2 ]);

        // 将值写入指定寄存器
        modH->u16regs[ u16StartAdd + i ] = temp;
    }
    // 计算包含 CRC 校验的缓冲区大小
    u8CopyBufferSize = modH->u8BufferSize +2;
    // 发送响应缓冲区数据
    sendTxBuffer(modH);

    return u8CopyBufferSize;
}