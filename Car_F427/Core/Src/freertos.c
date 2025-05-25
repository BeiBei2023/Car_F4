/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sever_init.h"
#include "lv_port_disp.h"
#include "lv_demo_widgets.h"
#include "UARTCallback.h"
#include "elog.h"
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
    .name = "MainTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
    .name = "MotorTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for MotorDataTask */
osThreadId_t MotorDataTaskHandle;
const osThreadAttr_t MotorDataTask_attributes = {
    .name = "MotorDataTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for Task_Remote */
osThreadId_t Task_RemoteHandle;
const osThreadAttr_t Task_Remote_attributes = {
    .name = "Task_Remote",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityHigh7,
};
/* Definitions for Task_LED */
osThreadId_t Task_LEDHandle;
const osThreadAttr_t Task_LED_attributes = {
    .name = "Task_LED",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for Modbus_Data */
osThreadId_t Modbus_DataHandle;
const osThreadAttr_t Modbus_Data_attributes = {
    .name = "Modbus_Data",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for mutex */
osMutexId_t mutexHandle;
const osMutexAttr_t mutex_attributes = {
    .name = "mutex"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void motor_task(void *argument);
extern void send_motor_data_task(void *argument);
extern void task_remote_attr(void *argument);
extern void task_led_attr(void *argument);
extern void Modbus_DataUpdate(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutex */
  mutexHandle = osMutexNew(&mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MainTask */
  MainTaskHandle = osThreadNew(StartDefaultTask, NULL, &MainTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(motor_task, NULL, &MotorTask_attributes);

  /* creation of MotorDataTask */
  MotorDataTaskHandle = osThreadNew(send_motor_data_task, NULL, &MotorDataTask_attributes);

  /* creation of Task_Remote */
  Task_RemoteHandle = osThreadNew(task_remote_attr, NULL, &Task_Remote_attributes);

  /* creation of Task_LED */
  Task_LEDHandle = osThreadNew(task_led_attr, NULL, &Task_LED_attributes);

  /* creation of Modbus_Data */
  Modbus_DataHandle = osThreadNew(Modbus_DataUpdate, NULL, &Modbus_Data_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the MainTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  init();

  log_d("StartDefaultTask");
  lv_init(); // 初始化LVGL
  log_d("lv_init");
  osDelay(100);        // 延时100ms，等待LVGL初始化完成
  lv_port_disp_init(); // 显示初始化
  log_d("lv_port_disp_init");
  osDelay(100); // 延时100ms，等待显示初始化完成

  ui_init(); // 初始化UI
  log_d("ui_init");


  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
