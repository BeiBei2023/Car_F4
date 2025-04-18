#include "system_working.h"

void sysyem_working_Task(void *argument)
{
    for (;;)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
 
        osDelay(800);

    }
}
