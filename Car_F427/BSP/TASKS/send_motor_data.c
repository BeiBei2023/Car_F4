#include "send_motor_data.h"
#include "wheel.h"
#include "st7735s.h"

void send_motor_data_task(void *argument)
{

    for (;;)
    {
        send_data(motor_data[0].motor_omega,
                  motor_data[1].motor_omega,
                  motor_data[2].motor_omega,
                  motor_data[3].motor_omega,
                  sbus_ch_data.channels[5]);

        // char str_buffer[20];
        // sprintf(str_buffer, "%d", 4566);
        // LCD_ST7735_DrawString(str_buffer, 0, 0, &Font_20x24, ST7735_YELLOW,ST7735_BLUE);

        osDelay(20);
    }
}
