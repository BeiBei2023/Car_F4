#include "motor.h"
#include "cmsis_os.h"





MotorRxData_t motor_data[CAN_MOTOR_NUM] = {0}; // 存放实时的电机数据
// 电机的CAN ID
const uint32_t motor_id[CAN_MOTOR_NUM] = {0x201, 0x202, 0x203, 0x204};

static int16_t clamp(int16_t value, int16_t min, int16_t max)
{
    return (value < min) ? min : (value > max) ? max
                                               : value;
}



/**
 * @brief   初始化电机控制相关的CAN模块和PID控制器
 * @param   hcan         指向CAN驱动的句柄结构体
 * @param   Callback_Function  CAN中断回调函数指针
 * @return  void
 */
void related_initialization_of_motors(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function)
{
    /* 初始化CAN模块并注册回调函数 */
    CAN_Init(hcan, Callback_Function); // 初始化CAN

    /* 配置CAN过滤器以允许所有标准ID的CAN消息通过 */
    for (int i = 0; i < CAN_MOTOR_NUM; i++)
    {
        CAN_Filter_Mask_Config(hcan,
                               CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE,
                               0,
                               0);
        // 设置过滤器,设置成0，不过滤所有ID的包
    }

}


/**
 * @brief 设置目标的速度,通过电机的ID顺序传入速度值给PID计算输出值
 *
 * int16_t target_speeds[MOTOR_NUM] = {100, 100, 100, 100};
 *
 * Motor_Set_Target_Speeds(target_speeds);
 *
 */
void set_motor_target_speed(int16_t new_target_speed[CAN_MOTOR_NUM])
{

    // int16_t delta_speed[CAN_MOTOR_NUM]; // 速度增量(速度差)
    int16_t max_speed_change = 2500; // 最大速度增量,加减速用，是0到最大速度的不会异常的最大值

    for (int i = 0; i < CAN_MOTOR_NUM; i++)
    {
        motor_data[i].motor_target_speed = clamp(new_target_speed[i],
                                motor_data[i].motor_omega - max_speed_change,
                                motor_data[i].motor_omega + max_speed_change);

        // EMLOG(LOG_DEBUG, "set_motor_target_speed[%d] = %d", i, motor_data[i].motor_target_speed);
        // EMLOG(LOG_DEBUG, "motor_data[%d].motor_omega = %d", i, motor_data[i].motor_omega);
        // EMLOG(LOG_DEBUG, "target_speed[%d] = %d", i, motor_data[i].motor_target_speed);
    }
}

/**
 * @brief 定期调用PID计算，并且更新电机的输出，返回值是pos_out或者delta_out。但不用返回值，直接访问
 * pid_speed[1].pos_out
 *
 */
void motor_pid_update(void)
{
    for (int i = 0; i < 4; i++)
    {
        // 填入的参数是：PID控制参数，实际速度，目标速度
        pid_calc(&pid_speed[i], motor_data[i].motor_omega, motor_data[i].motor_target_speed);
        /*输出对应的参数*/
        //EMLOG(LOG_DEBUG, "Motor_PID_Update is %d", pid_speed[i].delta_out);
    }
    // 假设需要发送16位整型数据（需确保数据范围在±32767内）
}

/**
 * @brief 设置电机的速度，也就是将经过PID计算的速度通过CAN总线发给电机
 *
 */
uint8_t send_motor_speed_commands(CAN_HandleTypeDef *hcan,
                                  int16_t motor1_speed,
                                  int16_t motor2_speed,
                                  int16_t motor3_speed,
                                  int16_t motor4_speed)
{
    uint16_t speeds[4] = {motor1_speed, motor2_speed, motor3_speed, motor4_speed};
    for (int i = 0; i < 4; i++)
    {
        CAN1_0x200_Tx_Data[2 * i] = speeds[i] >> 8;       // 高位
        CAN1_0x200_Tx_Data[2 * i + 1] = speeds[i] & 0xFF; // 低位
    }
    

    return CAN_Send_Data(hcan, CAN_ID_MOTOR_CONTROL, CAN1_0x200_Tx_Data, 8);
}

/**
 * @brief 这个是提供给外部的一个接口，用于获取电机的数据
 * 示例：读取电机0的实时数据
        MotorRxData_t *motor0_data = get_motor_data(0);
        int16_t current_omega = motor0_data->omega;
        int16_t current_temp = motor0_data->temperature;
 */
MotorRxData_t *get_motor_data(uint8_t motor_id)
{
    if (motor_id >= CAN_MOTOR_NUM) // 判断电机ID是否超出范围
    {
        /* code */
        return NULL; // 如果超出范围，返回NULL
    }
    return &motor_data[motor_id];
}
