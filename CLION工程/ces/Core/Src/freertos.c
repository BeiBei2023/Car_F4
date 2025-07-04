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
#include "Peripheral_Management.h"  //外设管理
#include "updata_registers_value.h"
#include "system_voltage.h"
#include "qmi8658a.h"
#include "sbus.h"
#include "motor_control.h"
#include "motor_target_speed.h"

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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MODBUS */
osThreadId_t MODBUSHandle;
const osThreadAttr_t MODBUS_attributes = {
  .name = "MODBUS",
  .stack_size = 1280 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for MODBUS_REG_DATA */
osThreadId_t MODBUS_REG_DATAHandle;
const osThreadAttr_t MODBUS_REG_DATA_attributes = {
  .name = "MODBUS_REG_DATA",
  .stack_size = 1280 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for IMU_GET_VALUE */
osThreadId_t IMU_GET_VALUEHandle;
const osThreadAttr_t IMU_GET_VALUE_attributes = {
  .name = "IMU_GET_VALUE",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for SBUS_DATA */
osThreadId_t SBUS_DATAHandle;
const osThreadAttr_t SBUS_DATA_attributes = {
  .name = "SBUS_DATA",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for MOTOR_CONTROL */
osThreadId_t MOTOR_CONTROLHandle;
const osThreadAttr_t MOTOR_CONTROL_attributes = {
  .name = "MOTOR_CONTROL",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for MOTOE_SPEED */
osThreadId_t MOTOE_SPEEDHandle;
const osThreadAttr_t MOTOE_SPEED_attributes = {
  .name = "MOTOE_SPEED",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for modbus_buffer_mutex */
osMutexId_t modbus_buffer_mutexHandle;
const osMutexAttr_t modbus_buffer_mutex_attributes = {
  .name = "modbus_buffer_mutex"
};
/* Definitions for imu_data_mutex */
osMutexId_t imu_data_mutexHandle;
const osMutexAttr_t imu_data_mutex_attributes = {
  .name = "imu_data_mutex"
};
/* Definitions for sbus_buffer_mutex */
osMutexId_t sbus_buffer_mutexHandle;
const osMutexAttr_t sbus_buffer_mutex_attributes = {
  .name = "sbus_buffer_mutex"
};
/* Definitions for sbus_data_semaphore */
osSemaphoreId_t sbus_data_semaphoreHandle;
const osSemaphoreAttr_t sbus_data_semaphore_attributes = {
  .name = "sbus_data_semaphore"
};
/* Definitions for modbus_data_semaphore */
osSemaphoreId_t modbus_data_semaphoreHandle;
const osSemaphoreAttr_t modbus_data_semaphore_attributes = {
  .name = "modbus_data_semaphore"
};
/* Definitions for imu_calibration_done_semaphore */
osSemaphoreId_t imu_calibration_done_semaphoreHandle;
const osSemaphoreAttr_t imu_calibration_done_semaphore_attributes = {
  .name = "imu_calibration_done_semaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void Modbus_Data_Parsing(void *argument);
extern void UpdateRegistersTask(void *argument);
extern void imu_data_task(void *argument);
extern void sbus_data_frame(void *argument);
extern void motor_task(void *argument);
extern void task_remote_attr(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of modbus_buffer_mutex */
  modbus_buffer_mutexHandle = osMutexNew(&modbus_buffer_mutex_attributes);

  /* creation of imu_data_mutex */
  imu_data_mutexHandle = osMutexNew(&imu_data_mutex_attributes);

  /* creation of sbus_buffer_mutex */
  sbus_buffer_mutexHandle = osMutexNew(&sbus_buffer_mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of sbus_data_semaphore */
  sbus_data_semaphoreHandle = osSemaphoreNew(1, 1, &sbus_data_semaphore_attributes);

  /* creation of modbus_data_semaphore */
  modbus_data_semaphoreHandle = osSemaphoreNew(1, 1, &modbus_data_semaphore_attributes);

  /* creation of imu_calibration_done_semaphore */
  imu_calibration_done_semaphoreHandle = osSemaphoreNew(1, 1, &imu_calibration_done_semaphore_attributes);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of MODBUS */
  MODBUSHandle = osThreadNew(Modbus_Data_Parsing, NULL, &MODBUS_attributes);

  /* creation of MODBUS_REG_DATA */
  MODBUS_REG_DATAHandle = osThreadNew(UpdateRegistersTask, NULL, &MODBUS_REG_DATA_attributes);

  /* creation of IMU_GET_VALUE */
  IMU_GET_VALUEHandle = osThreadNew(imu_data_task, NULL, &IMU_GET_VALUE_attributes);

  /* creation of SBUS_DATA */
  SBUS_DATAHandle = osThreadNew(sbus_data_frame, NULL, &SBUS_DATA_attributes);

  /* creation of MOTOR_CONTROL */
  MOTOR_CONTROLHandle = osThreadNew(motor_task, NULL, &MOTOR_CONTROL_attributes);

  /* creation of MOTOE_SPEED */
  MOTOE_SPEEDHandle = osThreadNew(task_remote_attr, NULL, &MOTOE_SPEED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  (void) argument;  //  Suppress unused argument warning
  Peripheral_Management_Init();

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port,  LED_BLUE_Pin);
    elog_info("心跳","Working...");
    elog_info("电压","系统电压: %.3f V",adc_v.v_in);

    osDelay(4676);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

