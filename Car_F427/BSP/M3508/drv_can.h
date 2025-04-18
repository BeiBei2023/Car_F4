/**
 * @file drv_can.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的CAN通信初始化与配置流程
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/**
 * @author wangbeihong
 * @brief 原版是大疆的A板，有使用到C++,改成C，现在更改为STM32F103RCT6,CAN的引脚为PB8-CAN_RX,PB9_CAN_TX
 * 绿灯每隔 1 秒闪 N 次 当前电调的 ID 为 N，电调 ID 范围为 1 到 8
 * @date 2025-04-02
 */

#ifndef DRV_CAN_H
#define DRV_CAN_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

// 滤波器编号
#define CAN_FILTER(x) ((x) << 3)

// 接收队列
typedef enum
{
    CAN_FIFO_0 = 0 << 2,
    CAN_FIFO_1 = 1 << 2
} CAN_FIFO_Type;

// 标准帧或扩展帧
typedef enum
{
    CAN_STDID = 0 << 1,
    CAN_EXTID = 1 << 1
} CAN_ID_Type;

// 数据帧或遥控帧
typedef enum
{
    CAN_DATA_TYPE = 0 << 0,
    CAN_REMOTE_TYPE = 1 << 0
} CAN_Frame_Type;

/* Exported types ------------------------------------------------------------*/

/**
 * @brief CAN接收的信息结构体
 * Header: 接收到的消息头部信息
 * Data: 接收到的数据缓冲区
 */
typedef struct
{
    CAN_RxHeaderTypeDef Header; // 消息头部信息
    uint8_t Data[8];            // 数据缓冲区
} Struct_CAN_Rx_Buffer;

/**
 * @brief CAN通信接收回调函数数据类型
 *
 */
typedef void (*CAN_Call_Back)(Struct_CAN_Rx_Buffer *);

/**
 * @brief CAN通信处理结构体,用于接收数据时
 * CAN_Handler: CAN句柄
 * Callback_Function: 回调函数指针
 */
typedef struct
{
    CAN_HandleTypeDef *CAN_Handler;  // CAN句柄
    CAN_Call_Back Callback_Function; // 回调函数指针
} Struct_CAN_Manage_Object;

/* Exported variables ---------------------------------------------------------*/

extern CAN_HandleTypeDef hcan1;
// extern CAN_HandleTypeDef hcan2;

extern Struct_CAN_Manage_Object CAN1_Manage_Object;
// extern Struct_CAN_Manage_Object CAN2_Manage_Object;

// extern uint8_t CAN1_0x1ff_Tx_Data[];
extern uint8_t CAN1_0x200_Tx_Data[];
// extern uint8_t CAN1_0x2ff_Tx_Data[];

// extern uint8_t CAN2_0x1ff_Tx_Data[];
// extern uint8_t CAN2_0x200_Tx_Data[];
// extern uint8_t CAN2_0x2ff_Tx_Data[];

// extern uint8_t CAN1_0x220_Tx_Data[];

/* Exported function declarations ---------------------------------------------*/

/**
 * @brief 初始化CAN通信
 * @param hcan CAN句柄
 * @param Callback_Function 接收数据的回调函数
 */
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function);

/**
 * @brief 配置CAN滤波器和掩码
 * @param hcan CAN句柄
 * @param Object_Para 对象参数，用于配置滤波器
 * @param ID 滤波器ID
 * @param Mask_ID 滤波器掩码ID
 */
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID);

/**
 * @brief 发送CAN数据
 * @param hcan CAN句柄
 * @param ID 发送的CAN消息ID
 * @param Data 发送的数据缓冲区
 * @param Length 发送的数据长度
 * @return 发送状态，0表示成功，其他值表示失败
 */
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length);

/**
 * @brief 定时器周期回调函数，用于处理CAN通信的定时任务
 * @param htim 定时器句柄
 */
//void TIM_CAN_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
