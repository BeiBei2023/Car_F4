#include "send_motor_data.h"
#include "button_config.h"
#include "Modbus.h"
#include "elog.h"

void send_motor_data_task(void *argument)
{

    for (;;)
    {
        button_ticks();
        osDelay(5);
    }
}
