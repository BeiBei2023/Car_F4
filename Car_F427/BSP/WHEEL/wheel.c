#include "wheel.h"
#include "st7735s.h"
#include "aht20.h"
#include "adc_v.h"
#include "button_config.h"
#include "screen_config.h"
#include "motor.h"
#include "elog.h"
#include "lvgl.h"

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

void task_remote_attr(void *argument)
{
    const uint32_t task_period = 7;                 // 10ms周期（100Hz）
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 记录上次执行时间

    g_robot.x = 0.0f;
    g_robot.y = 0.0f;
    g_robot.w = 0.0f;

    g_robot.motor_rpm[0] = 0;
    g_robot.motor_rpm[1] = 0;
    g_robot.motor_rpm[2] = 0;
    g_robot.motor_rpm[3] = 0;




    for (;;)
    {

        // 归1
        g_robot.x = ((float)sbus_ch_data.channels[SBUS_CH_VX] - 1500.0f) / 500.0f;
        g_robot.y = ((float)sbus_ch_data.channels[SBUS_CH_VY] - 1500.0f) / 500.0f;
        g_robot.w = ((float)sbus_ch_data.channels[SBUS_CH_OMEGA] - 1500.0f) / 500.0f;
        uint16_t gear_value = sbus_ch_data.channels[SBUS_CH_GEAR];



        target_speed_conversion(g_robot.x, g_robot.y, g_robot.w, gear_value);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(task_period));
    }
}

void target_speed_conversion(float Vx, float Vy, float Omega, uint8_t mSpeedGear)
{

    // log_d("Vx: %f, Vy: %f, Omega: %f, mSpeedGear: %d\n", Vx, Vy, Omega, mSpeedGear);
    /* 速度档位处理 */
    float speed_scale, omega_scale = 0;

    /* 调用 Mecanum_Calc */
    const float a_plus_b = MECANUM_A + MECANUM_B;                                     // 轮子间距
    const float rpm_factor = 60.0f / (2 * 3.1415926f * WHEEL_RADIUS) / cosf(180 / 4); // rpm转转角速度

    switch (mSpeedGear)
    {
    case 1000:
        speed_scale = 0.6f, omega_scale = 1.0f;
        break;
    case 1500:
        speed_scale = 1.0f, omega_scale = 1.0f;
        break;
    case 2000:
        speed_scale = 1.5f, omega_scale = 2.0f;
        break;
    default:
        speed_scale = 1.0f, omega_scale = 1.0f;
        break;
    }
    g_robot.vx = Vx * speed_scale;
    g_robot.vy = Vy * speed_scale;
    g_robot.omega = Omega * omega_scale;

    g_robot.motor_rpm[0] = (g_robot.vx - g_robot.vy + a_plus_b * g_robot.omega) * rpm_factor;
    g_robot.motor_rpm[1] = (g_robot.vx + g_robot.vy + a_plus_b * g_robot.omega) * rpm_factor;
    g_robot.motor_rpm[2] = (-g_robot.vx + g_robot.vy + a_plus_b * g_robot.omega) * rpm_factor;
    g_robot.motor_rpm[3] = (-g_robot.vx - g_robot.vy + a_plus_b * g_robot.omega) * rpm_factor;

    for (int i = 0; i < 4; i++)
    {
        g_robot.motor_rpm[i] = fmaxf(fminf(g_robot.motor_rpm[i], 4960.0f), -4960.0f);
    }
}

void lvgl_tick_attr(void *argument)
{

    for (;;)
    {
        lv_task_handler(); // 处理LVGL任务
        osDelay(5);
    }
}