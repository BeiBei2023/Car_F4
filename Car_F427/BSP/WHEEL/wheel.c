#include "wheel.h"
#include "st7735s.h"
#include "aht20.h"
#include "adc_v.h"
#include "button_config.h"
#include "screen_config.h"
#include "motor.h"

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

/*
将底盘速度期望解算成四个电机的转速期望
*/

void task_remote_attr(void *argument)
{
    const uint32_t task_period = 7;                 // 10ms周期（100Hz）
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 记录上次执行时间

    float speed_scale = 1.0f, omega_scale = 1.0f;

    g_robot.motor_rpm[0] = 0;
    g_robot.motor_rpm[1] = 0;
    g_robot.motor_rpm[2] = 0;
    g_robot.motor_rpm[3] = 0;

    for (;;)
    {

        // 归1
        g_robot.vx = ((float)sbus_ch_data.channels[SBUS_CH_VX] - 1500.0f) / 500.0f;
        g_robot.vy = ((float)sbus_ch_data.channels[SBUS_CH_VY] - 1500.0f) / 500.0f;
        g_robot.omega = ((float)sbus_ch_data.channels[SBUS_CH_OMEGA] - 1500.0f) / 500.0f;

        /* 速度档位处理 -------------------------------------------------*/
        uint16_t gear_value = sbus_ch_data.channels[SBUS_CH_GEAR];

        if (gear_value < 1500)
            g_robot.speed_mode = 0; // 慢速
        else if (gear_value > 1500)
            g_robot.speed_mode = 2; // 快速
        else
            g_robot.speed_mode = 1; // 中速
        /*****  --------------------------------------------------------- */

        /* 调用 Mecanum_Calc */
        const float a_plus_b = MECANUM_A + MECANUM_B;
        const float rpm_factor = 60.0f / (2 * 3.1415926f * WHEEL_RADIUS) / cosf(180 / 4);

        switch (g_robot.speed_mode)
        {
        case 0: // 低速档
            speed_scale = 0.3f;
            omega_scale = 0.5f;
            break;
        case 1: // 中速档
            speed_scale = 1.0f;
            omega_scale = 1.0f;
            break;
        case 2: // 高速档
            speed_scale = 1.5f;
            omega_scale = 2.0f;
            break;
        }

        float Vx = g_robot.vx * speed_scale;
        float Vy = g_robot.vy * speed_scale;
        float Omega = g_robot.omega * omega_scale;

        g_robot.motor_rpm[0] = (Vx - Vy + a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[1] = (Vx + Vy + a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[2] = (-Vx + Vy + a_plus_b * Omega) * rpm_factor;
        g_robot.motor_rpm[3] = (-Vx - Vy + a_plus_b * Omega) * rpm_factor;

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
    char buf[20]; // 复用缓冲区

    // 用枚举替代魔数,要根据屏幕顺序来排列
    typedef enum
    {
        SCREEN_DEFAULT = 0,
        SCREEN_VOLTAGE = 1,
        SCREEN_TEMP_HUM = 2,
        SCREEN_GITHUB = 3,
        SCREEN_MOTOR = 4
    } ScreenState;

    for (;;)
    {
        ScreenState current_screen = but_cnt.button_count;

        // 统一处理屏幕切换和标志位清零
        if (but_cnt.button_flage == 1)
        {
            ScreenManager_Switch(&screen_mgr, current_screen);
            but_cnt.button_flage = 0;
        }

        // 根据当前屏幕更新内容
        switch (current_screen)
        {
        case SCREEN_VOLTAGE: // 电压屏
            HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
            snprintf(buf, sizeof(buf), "%.2f", adc_v.v_in);
            ScreenManager_UpdateRegion(&screen_mgr, 0, buf);
            break;

        case SCREEN_TEMP_HUM: // 温湿度屏
            AHT20_Read(&sensorData);
            snprintf(buf, sizeof(buf), "%.1f", sensorData.Temperature);
            ScreenManager_UpdateRegion(&screen_mgr, 0, buf);
            snprintf(buf, sizeof(buf), "%.1f", sensorData.Humidity);
            ScreenManager_UpdateRegion(&screen_mgr, 1, buf);
            break;

        case SCREEN_MOTOR: // 电机屏
            for (int i = 0; i < 4; i++)
            {
                snprintf(buf, sizeof(buf), "%d", motor_data[i].motor_omega);
                ScreenManager_UpdateRegion(&screen_mgr, i, buf);
            }
            break;

        default: // SCREEN_DEFAULT及其他
            // HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // 修正为200ms延时
    }
}