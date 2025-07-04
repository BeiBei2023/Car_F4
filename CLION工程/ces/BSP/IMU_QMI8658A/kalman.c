
/**
 * ==================================================
 *  @file kalman.c
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


#include "kalman.h"
#include <math.h>
#include <stdlib.h>

void KalmanFilter_Init(KalmanTypeDef *cfg) {
    cfg->x = 0;
    cfg->p = 5;
    cfg->A = 1;
    cfg->H = 1;
    cfg->q = 0.25;
    cfg->r = 1;
}

float KalmanFilter_Update(KalmanTypeDef *cfg, float measure) {
    cfg->x = cfg->A * cfg->x;
    cfg->p = cfg->A * cfg->A * cfg->p + cfg->q;

    cfg->gain = cfg->p * cfg->H / (cfg->p * cfg->H * cfg->H + cfg->r);
    cfg->x = cfg->x + cfg->gain * (measure - cfg->H * cfg->x);
    cfg->p = (1 - cfg->gain * cfg->H) * cfg->p;

    return cfg->x;
}