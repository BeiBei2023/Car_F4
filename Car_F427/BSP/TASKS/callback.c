#include "callback.h"


/**
 * @brief 处理CAN接收的电机数据回调函数（主要用于接收电机数据）
 * @param Rx_Buffer 指向接收的CAN帧数据缓冲区的指针
 * @return 无返回值
 */
void CAN_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer)
{

  /* 取出CAN数据负载 */
  uint8_t *Rx_Data = Rx_Buffer->Data;

  /* 获取CAN帧标准ID */
  uint16_t can_id = Rx_Buffer->Header.StdId;

  /* 检查CAN ID是否在0x201到0x204范围内 */
  if (can_id >= motor_id[0] && can_id <= motor_id[3] && Rx_Buffer->Header.DLC >= 7)
  {

    /* 根据can_id计算电机编号（电机编号范围0-3） */
    uint8_t motor_num = can_id - motor_id[0];

    /* 保存历史数据 */
    motor_data[motor_num].motor_angle_last = motor_data[motor_num].motor_angle; // 角度
    motor_data[motor_num].motor_diff = motor_data[motor_num].motor_angle;
    motor_data[motor_num].motor_omega_last = motor_data[motor_num].motor_omega;
    motor_data[motor_num].motor_torque_last = motor_data[motor_num].motor_torque;
    motor_data[motor_num].motor_temperture_last = motor_data[motor_num].motor_temperture;

    /* 更新当前接收到的数据 存放在结构体中*/
    motor_data[motor_num].motor_angle = Rx_Data[0] << 8 | Rx_Data[1];
    motor_data[motor_num].motor_omega_original = Rx_Data[2] << 8 | Rx_Data[3];
    motor_data[motor_num].motor_omega =(motor_data[motor_num].motor_omega_original / 19);
    motor_data[motor_num].motor_torque = Rx_Data[4] << 8 | Rx_Data[5];
    motor_data[motor_num].motor_temperture = Rx_Data[6];

    /*-----------------用于计算总角度-------------------------*/
    /*判断是否转了1圈，大疆M3508是8192,转子19.02圈，输出轴1圈*/
    int16_t position_diff = motor_data[motor_num].motor_angle - motor_data[motor_num].motor_angle_last; // 角度差，本次收到的数据-上次收到的数据
    if (position_diff > 4096)
    {
      motor_data[motor_num].round_count--;
    }
    else if (position_diff < -4096)
    {
      motor_data[motor_num].round_count++;
    }
    motor_data[motor_num].motor_poeition = motor_data[motor_num].round_count * 8192 + motor_data[motor_num].motor_angle - motor_data[motor_num].motor_diff;
    /*-----------------------------------------------------*/

    
  }
}

