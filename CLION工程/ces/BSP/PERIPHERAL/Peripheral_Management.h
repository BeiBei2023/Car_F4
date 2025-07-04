/**
 * ==================================================
 *  @file Peripheral_Management.h
 *  @brief 用于外设的初始化
 *  @author wangb
 *  @date 2025-06-04 下午7:47
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */




#ifndef PERIPHERAL_MANAGEMENT_H
#define PERIPHERAL_MANAGEMENT_H


#define motor_can &hcan1 // 电机使用的can的名字



void Peripheral_Management_Init(void) ;

#endif //PERIPHERAL_MANAGEMENT_H
