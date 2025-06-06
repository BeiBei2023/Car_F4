/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RESET_Pin GPIO_PIN_4
#define LCD_RESET_GPIO_Port GPIOA
#define OUT5_Pin GPIO_PIN_6
#define OUT5_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_4
#define LCD_DC_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_5
#define LCD_CS_GPIO_Port GPIOC
#define OUT7_Pin GPIO_PIN_0
#define OUT7_GPIO_Port GPIOB
#define OUT8_Pin GPIO_PIN_1
#define OUT8_GPIO_Port GPIOB
#define LCD_BLK_Pin GPIO_PIN_2
#define LCD_BLK_GPIO_Port GPIOB
#define OUT1_Pin GPIO_PIN_9
#define OUT1_GPIO_Port GPIOE
#define OUT2_Pin GPIO_PIN_11
#define OUT2_GPIO_Port GPIOE
#define OUT3_Pin GPIO_PIN_13
#define OUT3_GPIO_Port GPIOE
#define OUT4_Pin GPIO_PIN_14
#define OUT4_GPIO_Port GPIOE
#define LED_G_Pin GPIO_PIN_15
#define LED_G_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_12
#define KEY1_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_15
#define LED_B_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_3
#define LED_R_GPIO_Port GPIOB
#define OUT6_Pin GPIO_PIN_5
#define OUT6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern uint8_t RcUartBuffer[25]; // 正确声明
extern DMA_HandleTypeDef hdma_usart1_rx;



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
