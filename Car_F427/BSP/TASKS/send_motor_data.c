#include "send_motor_data.h"
#include "wheel.h"

void send_motor_data_task(void *argument)
{
    for (;;)
    {
        send_data(sbus_ch_data.channels[0],sbus_ch_data.channels[1], sbus_ch_data.channels[3],g_robot.motor_rpm[0], sbus_ch_data.flags);

        osDelay(20);
    }
}
