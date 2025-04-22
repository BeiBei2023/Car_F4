#include "send_motor_data.h"
#include "wheel.h"
#include "st7735s.h"

void send_motor_data_task(void *argument)
{

    for (;;)
    {
        send_data(sbus_ch_data.channels[0],
                  sbus_ch_data.channels[1],
                  sbus_ch_data.channels[3],
                  motor_data[0].motor_omega,
                  sbus_ch_data.flags);
  


        char str_buffer[20];
        sprintf(str_buffer, "%d", 4566);
       // LCD_ST7735_DrawString(str_buffer, 0, 0, &Font_20x24, ST7735_YELLOW,ST7735_BLUE);

        osDelay(20);
    }
}