#ifndef __SBUS_H_
#define __SBUS_H_

/**
 * @brief S.BUS是FUTABA提出的舵机控制总线，全称Serial Bus，别名S-BUS或SBUS，也称 Futaba S.BUS。
S.BUS是一个串行通信协议，也是一个数字串行通信接口（单线），适合与飞控连接。
它可以连接很多设备，每个设备通过一个HUB与它相连，得到各自的控制信息。
S.BUS可以传输16个比例通道和2个数字（bool）通道。
其硬件上基于RS232协议，采用TTL电平，但高位取反（负逻辑，低电平为“1”，高电平为“0”），通信波特率为100K（不兼容波特率115200）。
 *

通信接口：USART（TTL）
通信参数：1个起始位+8个数据位+偶校验位+2个停止位，波特率=100000bit/s,电平逻辑反转。
通信速率：每14ms（模拟模式）或7ms（高速模式）发送，即数据帧间隔为 11ms（模拟模式）或4ms（高速模式）。
数据帧格式：[1]

 */

#include <stdint.h> // 新增：包含int16_t类型定义
#include "string.h"
#include "stdio.h"

#define SBUS_CHANNEL_NUM 16 // SBUS通道数，与实际遥控的不同，所以没有用到的通道会补零
#define SBUS_FRAME_LENGTH 25

typedef struct
{
    int16_t start_byte; // 起始位
    // 补充完整的SBUS协议数据结构字段（示例）：
    uint16_t channels[SBUS_CHANNEL_NUM]; // 16个通道数据
    //依据遥控器换算通道数据
    uint8_t channels_percentage[SBUS_CHANNEL_NUM];  // 16个通道数据百分比

    int16_t checksum; // 校验和
    int16_t end_byte; // 结束位

    uint16_t flags; // 标志位（包含数字通道等信息）
} sbus_data;


/**
 * @brief 我的遥控是SBUS值范围是 192 - 1792，中值为 992
 * 
 */
#define SBUS_RANGE_MIN  192     // SBUS最小值
#define SBUS_RANGE_MAX  1792    // SBUS最大值
#define SBUS_RANGE_MID  992     // SBUS中值

#define SBUS_TARGET_RANGE_MIN  1000    // SBUS目标最小值
#define SBUS_TARGET_RANGE_MAX  2000    // SBUS目标最大值





extern sbus_data sbus_ch_data; 


void sbus_decode_frame(uint8_t *buf);

uint16_t sbus_value_convert(uint16_t raw_value) ;


#endif // !__SBUS_H_
