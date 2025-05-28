#include "send_motor_data.h"
#include "button_config.h"
#include "elog.h"

void button_task(void *argument)
{

    for (;;)
    {
        button_ticks();
      
        osDelay(5);
    }
}
