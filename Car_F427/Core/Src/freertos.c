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
#include "lv_port_indev.h"
#include "lv_demo_widgets.h"
#include "UARTCallback.h"
#include "elog.h"

#include "events_init.h"
#include "gui_guider.h"
#include "custom.h"     // 自定义函数头文件
lv_ui guider_ui;
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
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for ButtonScanTask */
osThreadId_t ButtonScanTaskHandle;
const osThreadAttr_t ButtonScanTask_attributes = {
  .name = "ButtonScanTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_Remote */
osThreadId_t Task_RemoteHandle;
const osThreadAttr_t Task_Remote_attributes = {
  .name = "Task_Remote",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh7,
};
/* Definitions for Task_LVGL */
osThreadId_t Task_LVGLHandle;
const osThreadAttr_t Task_LVGL_attributes = {
  .name = "Task_LVGL",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Modbus_Data */
osThreadId_t Modbus_DataHandle;
const osThreadAttr_t Modbus_Data_attributes = {
  .name = "Modbus_Data",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UI_DATA_TASK */
osThreadId_t UI_DATA_TASKHandle;
const osThreadAttr_t UI_DATA_TASK_attributes = {
  .name = "UI_DATA_TASK",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorDataTask */
osThreadId_t SensorDataTaskHandle;
const osThreadAttr_t SensorDataTask_attributes = {
  .name = "SensorDataTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mutex */
osMutexId_t mutexHandle;
const osMutexAttr_t mutex_attributes = {
  .name = "mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void motor_task(void *argument);
extern void button_task(void *argument);
extern void task_remote_attr(void *argument);
extern void lvgl_tick_attr(void *argument);
extern void Modbus_DataUpdate(void *argument);
extern void ui_data_update(void *argument);
extern void sensor_data_update(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
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

  /* creation of ButtonScanTask */
  ButtonScanTaskHandle = osThreadNew(button_task, NULL, &ButtonScanTask_attributes);

  /* creation of Task_Remote */
  Task_RemoteHandle = osThreadNew(task_remote_attr, NULL, &Task_Remote_attributes);

  /* creation of Task_LVGL */
  Task_LVGLHandle = osThreadNew(lvgl_tick_attr, NULL, &Task_LVGL_attributes);

  /* creation of Modbus_Data */
  Modbus_DataHandle = osThreadNew(Modbus_DataUpdate, NULL, &Modbus_Data_attributes);

  /* creation of UI_DATA_TASK */
  UI_DATA_TASKHandle = osThreadNew(ui_data_update, NULL, &UI_DATA_TASK_attributes);

  /* creation of SensorDataTask */
  SensorDataTaskHandle = osThreadNew(sensor_data_update, NULL, &SensorDataTask_attributes);

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
    log_i("StartDefaultTask");
    init();

    
    lv_init(); // 初始化LVGL
    log_i("lv_init");
    osDelay(100);        // 延时100ms，等待LVGL初始化完成
    lv_port_disp_init(); // 显示初始化
    log_i("lv_port_disp_init");
    lv_port_indev_init(); // 输入设备初始化
    log_i("lv_port_indev_init");
    osDelay(100); // 延时100ms，等待显示初始化完成

    log_i("ui_init");
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui); // 初始化自定义函数

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

