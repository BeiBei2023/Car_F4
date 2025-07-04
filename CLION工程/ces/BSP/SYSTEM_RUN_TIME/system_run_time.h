/**
 * ==================================================
 *  @file system_run_time.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-08 下午5:04
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef SYSTEM_RUN_TIME_H
#define SYSTEM_RUN_TIME_H

#include <stdint.h>

extern uint32_t system_uptime_seconds;

uint32_t get_system_uptime_ms(void);
const char* get_uptime_str_days_and_ms(void);

#endif //SYSTEM_RUN_TIME_H
