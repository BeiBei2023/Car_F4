/**
 * ==================================================
 *  @file kalman.h
 *  @brief TODO 描述该文件的功能
 *  @author wangb
 *  @date 2025-06-08 下午7:09
 *  @version 1.0
 *  
 *  @details 本文件包含...
 *  
 *  @copyright Copyright (c) 2025 wangb. All Rights Reserved.
 *  
 *  @license 不开源
 * ==================================================
 */



#ifndef KALMAN_H
#define KALMAN_H



typedef struct __KalmanTypeDef {
    float x;     // 当前估计角度
    float A;    // 系统模型系数（通常为 1）
    float H;    // 观测模型系数（通常为 1）
    float q;    // 系统过程噪声协方差
    float r;    // 观测噪声协方差
    float p;    // 估计误差协方差
    float gain; // 卡尔曼增益
} KalmanTypeDef;


#ifdef __cplusplus
extern "C" {
#endif

void KalmanFilter_Init(KalmanTypeDef *);
float KalmanFilter_Update(KalmanTypeDef *, float);

#ifdef __cplusplus
}
#endif

#endif //KALMAN_H
