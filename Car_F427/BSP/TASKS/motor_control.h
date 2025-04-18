#ifndef __MOTOR_CONTROL_H_
#define __MOTOR_CONTROL_H_


#include "main.h"
#include "cmsis_os.h"
#include "motor.h"
#include "pid.h"
#include <stdio.h>
#include "wheel.h"


extern osMutexId_t mutexHandle;


void motor_task(void *argument);




#endif // !__MOTOR_CONTROL_H_
