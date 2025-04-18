#include "log.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1; // 串口1
extern UART_HandleTypeDef huart2; // 串口1

/**
 * 获取日志级别对应的字符串（查表法实现）
 */
char *EM_LOGLevelGet(const int level)
{
    static const char *const log_levels[] = {
        [LOG_DEBUG] = "DEBUG",
        [LOG_INFO] = "INFO",
        [LOG_WARN] = "WARN",
        [LOG_ERROR] = "ERROR"};
    // 通过范围检查确保数组访问安全
    if (level >= LOG_DEBUG && level <= LOG_ERROR)
    {
        return (char *)log_levels[level];
    }
    return "UNKNOWN";
}

void EM_LOG(const int level, const char *fun, const int line, const char *fmt, ...)
{

#ifdef OPEN_LOG_DEBUG
    va_list arg;
    va_start(arg, fmt);

    char buf[256];
    int buf_len = vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);

    if (level >= LOG_LEVEL && buf_len >= 0)
    {
        char log_message[512];
        // 计算实际写入长度并限制在缓冲区范围内
        int msg_len = snprintf(log_message, sizeof(log_message), "[%s] [%-20s:%4d] %s \r\n",
                               EM_LOGLevelGet(level), fun, line, buf);
        if (msg_len < 0)
        {
            return; // 格式化错误直接退出
        }
        size_t send_len = (msg_len >= (int)sizeof(log_message)) ? sizeof(log_message) - 1 : msg_len;
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)log_message, send_len);
    }
#endif
}

/**
 * @brief 匿名上位机代码
 * @note  匿名上位机协议采用小端模式，低字节在前，高字节在后，（协议内所有 1 字节以上的数据类型，比如数据长度 LEN 以
及 DATA 数据内容中的数据等，均是低字节在前，比如 DATA 长度等于 5，那么此时数据长度 LEN=5，其对应的小
端十六进制为 0x0500）。
 *
 */

/**
 * @brief 校验计算
 * @note
和校验 SUM CHECK 计算方法：
从帧头 0xAB 字节开始，一直到 DATA 区结束，对每一字节进行累加操作，只取低 8。

附加校验 ADD CHECK 计算方法：
计算和校验时，每进行一字节的加法运算，同时进行一次 SUM CHECK 的累加操作，只取低 8 位。

校验计算示例：
假设数据帧缓存为 data_buf 数组，0xAB 存放于数组起始位置，那么 data_buf[4]、data_buf[5]存放的是数据长
度，校验程序如下
 *
 */

// 发送数据的函数
void send_data(int16_t data1, int16_t data2, int16_t data3, int16_t data4,int16_t data5) {
    static uint8_t data_buffer[18]; // 静态变量确保DMA发送时内存有效

    // 初始化协议字段
    data_buffer[0] = 0xAB;    // 帧头
    data_buffer[1] = 0xFF;    // 源地址
    data_buffer[2] = 0xFF;    // 目的地址
    data_buffer[3] = 0xF1;    // 功能码

    // 设置DATA区长度（6个uint16_t = 12字节）
    uint16_t flen = 10;
    data_buffer[4] = flen & 0xFF;  // LEN低字节
    data_buffer[5] = flen >> 8;    // LEN高字节

    // 填充DATA区（小端模式）
    data_buffer[6]  = data1 & 0xFF; // data1低字节
    data_buffer[7]  = data1 >> 8;   // data1高字节
    data_buffer[8]  = data2 & 0xFF; // data2低字节
    data_buffer[9]  = data2 >> 8;   // data2高字节
    data_buffer[10] = data3 & 0xFF; // data3低字节
    data_buffer[11] = data3 >> 8;   // data3高字节
    data_buffer[12] = data4 & 0xFF; // data4低字节
    data_buffer[13] = data4 >> 8;   // data4高字节
    data_buffer[14] = data5 & 0xFF;
    data_buffer[15] = data5 >> 8;

    // 计算校验和
    uint8_t sumcheck = 0;
    uint8_t addcheck = 0;
    for (uint16_t i = 0; i < (flen + 6); i++) { // 范围：帧头到DATA区结束（共flen+6字节）
        sumcheck += data_buffer[i];
        addcheck += sumcheck;
    }

    // 写入校验和（flen=8 → 位置14和15）
    data_buffer[flen + 6] = sumcheck; // SUM CHECK
    data_buffer[flen + 7] = addcheck; // ADD CHECK

    // 发送完整帧（flen+8 = 16字节）
    HAL_UART_Transmit_DMA(&huart2, data_buffer, flen + 8);
}



