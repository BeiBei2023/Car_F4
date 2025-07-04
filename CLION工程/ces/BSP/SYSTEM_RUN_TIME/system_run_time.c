/**
 * ==================================================
 *  @file system_run_time.c
 *  @brief 系统运行时间模块，支持显示“天数”
 *  @author wangb
 *  @date 2025-06-08 下午5:04
 *  @version 1.0
 *
 *  @details 本文件包含系统运行时间获取和格式化输出
 *
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *
 *  @license 不开源
 * ==================================================
 */


#include "system_run_time.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

// 获取当前系统运行时间（单位：ms）
uint32_t get_system_uptime_ms(void)
{
    return HAL_GetTick(); // 返回系统启动后经过的毫秒数
}

// 获取带天数 + 毫秒的时间字符串（格式：dd 天 hh:mm:ss.SSS）
const char* get_uptime_str_days_and_ms(void)
{
    static char uptime_str[40]; // 足够长以容纳天数+毫秒
    uint32_t total_ms = HAL_GetTick();
    uint32_t seconds = total_ms / 1000;
    uint32_t ms = total_ms % 1000;

    uint32_t days = seconds / 86400;
    uint32_t h = (seconds % 86400) / 3600;
    uint32_t m = (seconds % 3600) / 60;
    uint32_t s = seconds % 60;

    sprintf(uptime_str, "%lu Days %02lu:%02lu:%02lu.%03lu ms", days, h, m, s, ms);


    return uptime_str;
}