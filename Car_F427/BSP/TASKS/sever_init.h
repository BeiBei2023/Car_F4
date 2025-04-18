#ifndef __SEVER_INIT_H
#define __SEVER_INIT_H



#define motor_can &hcan1 // 电机使用的can的名字



/**************电机通用的固定参数**********************/
#define M3508_MOTOR_POSSITION_PID_MAX_OUT 16000 // 3508电机最大输出值
#define M3508_MOTOR_POSSITION_PID_MAX_IOUT 2000  // 3508电机最大积分输出值

/*************电机1的参数*******************/
#define M3508_MOTOR_1_POSSITION_PID_KP 5.05f    // 3508电机1的PID参数 比例
#define M3508_MOTOR_1_POSSITION_PID_KI 0.03f    // 3508电机1的PID参数 积分
#define M3508_MOTOR_1_POSSITION_PID_KD 0.00f    // 3508电机1的PID参数 微分
/**************电机2的参数*****************/
#define M3508_MOTOR_2_POSSITION_PID_KP 5.05f    // 3508电机2的PID参数 比例
#define M3508_MOTOR_2_POSSITION_PID_KI 0.03f    // 3508电机2的PID参数 积分
#define M3508_MOTOR_2_POSSITION_PID_KD 0.00f    // 3508电机2的PID参数 微分
/**************电机3的参数***************/
#define M3508_MOTOR_3_POSSITION_PID_KP 5.05f    // 3508电机3的PID参数 比例
#define M3508_MOTOR_3_POSSITION_PID_KI 0.03f    // 3508电机3的PID参数 积分
#define M3508_MOTOR_3_POSSITION_PID_KD 0.00f    // 3508电机3的PID参数 微分
/**************电机4的参数**************/
#define M3508_MOTOR_4_POSSITION_PID_KP 5.05f    // 3508电机4的PID参数 比例
#define M3508_MOTOR_4_POSSITION_PID_KI 0.03f    // 3508电机4的PID参数 积分
#define M3508_MOTOR_4_POSSITION_PID_KD 0.00f    // 3508电机4的PID参数 微分






void init(void);



#endif // !__SEVER_INIT_H
