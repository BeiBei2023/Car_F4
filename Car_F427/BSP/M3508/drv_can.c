/**
 * @file drv_can.c
 * @author yssickjgd (1345578933@qq.com)
 * @brief 仿照SCUT-Robotlab改写的CAN通信初始化与配置流程
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright USTC-RoboWalker (c) 2022
 */

/* Includes ------------------------------------------------------------------*/
#include "drv_can.h"
#include "main.h" // 包含hcan1定义
#include "elog.h"
extern CAN_HandleTypeDef hcan;

/* 宏定义 -------------------------------------------------------------------*/

#define MAX_CAN_DATA_LENGTH 8

/* 全局变量 -------------------------------------------------------------*/
Struct_CAN_Manage_Object CAN1_Manage_Object = {0};

// uint8_t CAN1_0x1ff_Tx_Data[8];
uint8_t CAN1_0x200_Tx_Data[8];
// uint8_t CAN1_0x2ff_Tx_Data[8];
// uint8_t CAN1_0x220_Tx_Data[8];

/* 函数声明 -------------------------------------------------------------*/

/**
 * @brief 初始化CAN总线
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定要初始化的CAN实例
 * @param Callback_Function 回调函数指针，用于处理接收到的数据
 */
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function)
{
    if (!hcan || HAL_CAN_Start(hcan) != HAL_OK)
    {
        //EMLOG(LOG_ERROR, "CAN Init Failed for Instance: 0x%X", (unsigned int)hcan->Instance);
        return;
    }

   // EMLOG(LOG_INFO, "CAN (Base Address: 0x%X) Init Starting", (unsigned int)hcan->Instance);

    // 启用FIFO0和FIFO1的消息挂起中断
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

    // 绑定管理对象和回调函数
    if (hcan->Instance == CAN1)
    {
        CAN1_Manage_Object.CAN_Handler = hcan;
        CAN1_Manage_Object.Callback_Function = Callback_Function;
    }

   // EMLOG(LOG_INFO, "CAN (Base Address: 0x%X) Init Successfully", (unsigned int)hcan->Instance);
}

/**
 * @brief 配置CAN的过滤器
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定要配置的CAN实例
 * @param Object_Para 编号 | FIFOx | ID类型 | 帧类型的组合参数
 * @param ID 要匹配的CAN标识符
 * @param Mask_ID 屏蔽位，用于指定ID的屏蔽范围
 */
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
    if (!hcan || Object_Para > 0xFF)
    {
        log_e("Invalid parameters in CAN_Filter_Mask_Config");
        return;
    }
    // CAN过滤器初始化结构体
    CAN_FilterTypeDef can_filter_init_structure = {0};

    if (Object_Para & 0x02)
    {
        // 数据帧配置
        can_filter_init_structure.FilterIdHigh = ID << 3 << 16;
        can_filter_init_structure.FilterIdLow = ID << 3 | ((Object_Para & 0x03) << 1);
        can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 3 << 16;
        can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | ((Object_Para & 0x03) << 1);
    }
    else
    {
        // 其他帧配置
        can_filter_init_structure.FilterIdHigh = ID << 5;
        can_filter_init_structure.FilterIdLow = ((Object_Para & 0x03) << 1);
        can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 5;
        can_filter_init_structure.FilterMaskIdLow = ((Object_Para & 0x03) << 1);
    }

    // 设置滤波器属性
    can_filter_init_structure.FilterBank = Object_Para >> 3;
    can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;

    can_filter_init_structure.FilterActivation = ENABLE;    // 滤波器使能
    can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;   //设置滤波器模式为IDMask模式，允许使用掩码进行消息过滤
    can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;  //设置滤波器为32位，适应标准和扩展帧
    can_filter_init_structure.SlaveStartFilterBank = 14;

    // 应用滤波器配置
    HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure);
}

/**
 * @brief 发送数据帧
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定要发送数据的CAN实例
 * @param ID 要发送的CAN标识符
 * @param Data 指向要发送的数据缓冲区的指针
 * @param Length 要发送的数据长度
 * @return uint8_t 返回发送操作的状态，成功返回HAL_OK，失败返回其他值
 */
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length)
{
    if (!hcan || !Data || Length > MAX_CAN_DATA_LENGTH)
    {
        log_e("Invalid parameters in CAN_Send_Data");
        return HAL_ERROR;
    }

    CAN_TxHeaderTypeDef tx_header = {0};
    uint32_t used_mailbox;

    tx_header.StdId = ID;
    tx_header.ExtId = 0;
    tx_header.IDE = 0;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    /*日志输出发送出去的数据内容*/
    // EMLOG(LOG_DEBUG, "CAN_Send_Data: ID=%08X, Length=%d, Data=%02X %02X %02X %02X %02X %02X %02X %02X", ID, Length, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], Data[6], Data[7]);
    
    uint8_t status = HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox);
    if (status != HAL_OK)
    {
        log_e("CAN Send Failed for Instance: 0x%X", (unsigned int)hcan->Instance);
    }

    return status;
}

/**
 * @brief CAN的TIM定时器中断发送回调函数
 *
 * 该函数在定时器中断中被调用，用于触发CAN数据的周期性发送。
 */
// void TIM_CAN_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM2)
//     {
//         // 执行CAN相关任务
//         // 例如：触发CAN数据发送或接收
//     }
// }

/**
 * @brief 通用CAN接收中断回调函数
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定触发中断的CAN实例
 * @param fifo 接收FIFO编号
 */
static void CAN_RxMsgPendingCallback(CAN_HandleTypeDef *hcan, uint8_t fifo)
{
    static Struct_CAN_Rx_Buffer can_rx_buffer = {0};

    if (hcan->Instance == CAN1)
    {
        HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(hcan, fifo, &can_rx_buffer.Header, can_rx_buffer.Data);
        if (status == HAL_OK && CAN1_Manage_Object.Callback_Function)
        {
            CAN1_Manage_Object.Callback_Function(&can_rx_buffer);
        }
        else
        {
            log_e("CAN Receive Failed for Instance: 0x%X", (unsigned int)hcan->Instance);
        }
    }
}

/**
 * @brief HAL库CAN接收FIFO0中断回调函数
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定触发中断的CAN实例
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxMsgPendingCallback(hcan, CAN_FILTER_FIFO0);
}

/**
 * @brief HAL库CAN接收FIFO1中断回调函数
 *
 * @param hcan 指向CAN外设句柄的指针，用于指定触发中断的CAN实例
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxMsgPendingCallback(hcan, CAN_FILTER_FIFO1);
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
