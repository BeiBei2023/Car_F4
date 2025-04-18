#ifndef __LOG_H_
#define __LOG_H_

#include "main.h"
#include "usart.h"
#include "stdarg.h"

#define OPEN_LOG_DEBUG 1    // 1:打开调试模式, 0:关闭调试模式
#define LOG_LEVEL LOG_DEBUG // LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL



/**
 * 定义日志级别枚举类型
 *
 * 日志级别用于控制日志输出的详细程度，从调试信息到错误信息共分为四个级别：
 * - LOG_DEBUG: 调试信息，最详细的日志信息，用于开发和调试阶段
 * - LOG_INFO: 一般信息，记录系统的正常运行状态
 * - LOG_WARN: 警告信息，表明系统运行中可能出现的问题，但并不严重
 * - LOG_ERROR: 错误信息，记录系统中发生的错误，需要及时处理
 */
typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} E_LOGLEVEL;

void EM_LOG(const int level, const char *fun, const int line, const char *fmt, ...);
#define EMLOG(level, fmt, ...) EM_LOG(level, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)


/*与匿名助手发数据 4个数据   */
void send_data(int16_t data1, int16_t data2, int16_t data3,int16_t data4,int16_t data5);


#endif // !__LOG_H_
