#include "wheel.h"
#include "st7735s.h"
#include "aht20.h"

RobotState g_robot;
/*
由于45°的角度关系，我们可以很轻易地通过轮子的转速来计算出辊子的速度，当然反之亦然。
但是要注意的是V轮是线速度，而我们一般所说的电机的转速是rpm，也就是每分钟多少转。
我们通过这个“真正的转速rpm”可以得到的是角速度，角速度到线速度还要进行一步转换。
这里的转化，在代码中也会有所体现。这里我们先以线速度来进行运算，即 V辊 = V轮*cos45°
*/

/*
1.	假设一个底盘的速度Vc，那么每一个轮子上都会存在这样一个速度Vc
2.	假设一个底盘加速度W
3.	通过刚体旋转公式得到对应轮子的旋转速度Vw
4.	轮子的Vc与Vw进行速度的合成得到轮子满足目标运动所需的速度Vx（V0,V1,V2,V3）
5.	将Vx沿辊子轴方向分解得到Vx平行
6.	根据根据辊子沿辊子轴平行方向速度与轮子的转速的关系，得到轮子转速
*/

// 死区过滤函数
/*
将底盘速度期望解算成四个电机的转速期望
*/

void task_remote_attr(void *argument)
{
    const uint32_t task_period = 7;                 // 10ms周期（100Hz）
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 记录上次执行时间
    g_robot.motor_rpm[0] = 0;
    g_robot.motor_rpm[0] = 0;
    g_robot.motor_rpm[0] = 0;
    g_robot.motor_rpm[0] = 0;

    for (;;)
    {

        // 归1
        g_robot.vx = ((float)sbus_ch_data.channels[SBUS_CH_VX] - 1500.0f) / 500.0f;
        g_robot.vy = ((float)sbus_ch_data.channels[SBUS_CH_VY] - 1500.0f) / 500.0f;
        g_robot.omega = ((float)sbus_ch_data.channels[SBUS_CH_OMEGA] - 1500.0f) / 500.0f;

        /* 速度档位处理 */
        uint16_t gear_value = sbus_ch_data.channels[SBUS_CH_GEAR];
        if (gear_value < 1500)
            g_robot.speed_mode = 0; // 慢速
        else if (gear_value > 1500)
            g_robot.speed_mode = 2; // 快速
        else
            g_robot.speed_mode = 1; // 中速

        /* 调用 Mecanum_Calc */
        const float a_plus_b = MECANUM_A + MECANUM_B;
        const float rpm_factor = 60.0f / (2 * 3.1415926f * WHEEL_RADIUS) / cosf(180 / 4) * 19;

        float speed_scale = 1.0f, omega_scale = 1.0f;

        switch (g_robot.speed_mode)
        {
        case 0: // 低速档
            speed_scale = 0.3f;
            omega_scale = 0.5f;
            break;
        case 2: // 高速档
            speed_scale = 1.5f;
            omega_scale = 2.0f;
            break;
        }

        float Vx = g_robot.vx * speed_scale;
        float Vy = g_robot.vy * speed_scale;
        float Omega = g_robot.omega * omega_scale;

        g_robot.motor_rpm[0] = (Vx - Vy - a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[1] = (Vx + Vy - a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[2] = (-Vx + Vy - a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[3] = (-Vx - Vy - a_plus_b * Omega) * rpm_factor;

        for (int i = 0; i < 4; i++)
        {
            g_robot.motor_rpm[i] = fmaxf(fminf(g_robot.motor_rpm[i], 4960.0f), -4960.0f);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(task_period));
    }
}
void task_led_attr(void *argument)
{
    AHT20_Data_t sensorData;
    for (;;)
    {
       
        AHT20_Read(&sensorData);
      
        char tempStr[32];
        char humStr[32];
      
        sprintf(tempStr, "Temp: %.2f C", sensorData.Temperature);
        sprintf(humStr, "Hum: %.2f %%", sensorData.Humidity);
      
        ST7735_FillScreen(ST7735_BLACK); // 清屏
        ST7735_WriteString(0, 0, tempStr, Font_7x10, ST7735_WHITE, ST7735_BLACK);
        ST7735_WriteString(0, 15, humStr, Font_7x10, ST7735_WHITE, ST7735_BLACK);
      
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 200ms检测周期
    }
}
