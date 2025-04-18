#include "pid.h"

#include <math.h>

pid_t pid_angular_speed; // 电机角度PID控制
pid_t pid_position;      // 电机位置PID控制
pid_t pid_speed[4];      // 电机速度PID控制

#define ABS(x) ((x > 0) ? (x) : (-x))

#define ABS(x) ((x > 0) ? (x) : (-x))

void abs_limit(float *a, float ABS_MAX)
{
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}
/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
    pid_t *pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    float kp,
    float ki,
    float kd)
{

    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->pid_mode = mode;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/*中途更改参数设定(调试)------------------------------------------------------------*/
void pid_reset(pid_t *pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/**
 *@bref. calculate delta PID and position PID
 *@param[in] set： target
 *@param[in] real	measure
 */
/**
 * @brief PID控制器计算函数(位置式/增量式)
 *
 * @param pid   PID控制器结构体指针，包含算法参数和中间状态
 * @param get   测量值，当前被控对象的实际值
 * @param set   设定值，期望达到的目标值
 * @return float 计算得到的控制输出值
 *
 * 函数处理流程：
 * 1. 计算当前误差并进行边界检查
 * 2. 根据PID模式选择对应算法
 * 3. 更新历史数据记录
 */
float pid_calc(pid_t *pid, float get, float set)
{
    // 更新当前时刻的输入数据
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get; // 当前误差 = 设定值 - 测量值

    // 边界保护检查：当误差超过允许范围或处于死区时，立即返回0输出
    if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err)
        return 0;
    if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
        return 0;

    // 位置式PID算法分支
    if (pid->pid_mode == POSITION_PID)
    {
        // 计算比例项
        pid->pout = pid->p * pid->err[NOW];
        // 累加积分项并限制积分饱和
        pid->iout += pid->i * pid->err[NOW];
        // 计算微分项（当前误差与上次误差的差值）
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

        // 对积分项和总输出进行限幅
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->MaxOutput);

        // 保存当前输出状态用于下次计算
        pid->last_pos_out = pid->pos_out;
    }
    // 增量式PID算法分支
    else if (pid->pid_mode == DELTA_PID)
    {
        // 计算比例项（误差变化量）
        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
        // 当前误差的积分量
        pid->iout = pid->i * pid->err[NOW];
        // 计算二阶微分项（误差的变化趋势）
        pid->dout = pid->d * (pid->err[NOW] - 2 * pid->err[LAST] + pid->err[LLAST]);

        // 对积分项进行限幅
        abs_limit(&(pid->iout), pid->IntegralLimit);
        // 计算增量并更新输出
        pid->delta_u = pid->pout + pid->iout + pid->dout;
        pid->delta_out = pid->last_delta_out + pid->delta_u;
        abs_limit(&(pid->delta_out), pid->MaxOutput);

        // 保存增量输出状态
        pid->last_delta_out = pid->delta_out;
    }

    // 更新历史数据队列：将当前数据转为历史数据
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];

    // 根据模式返回对应计算结果
    return pid->pid_mode == POSITION_PID ? pid->pos_out : pid->delta_out;
}

/**
 *@bref. special calculate position PID @attention @use @gyro data!!
 *@param[in] set： target
 *@param[in] real	measure
 */
float pid_sp_calc(pid_t *pid, float get, float set, float gyro)
{
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get; // set - measure

    if (pid->pid_mode == POSITION_PID) // 位置式p
    {
        pid->pout = pid->p * pid->err[NOW];
        if (fabs(pid->i) >= 0.001f)
            pid->iout += pid->i * pid->err[NOW];
        else
            pid->iout = 0;
        pid->dout = -pid->d * gyro / 100.0f;
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; // update last time
    }
    else if (pid->pid_mode == DELTA_PID) // 增量式P
    {
        //        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
        //        pid->iout = pid->i * pid->err[NOW];
        //        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
        //
        //        abs_limit(&(pid->iout), pid->IntegralLimit);
        //        pid->delta_u = pid->pout + pid->iout + pid->dout;
        //        pid->delta_out = pid->last_delta_out + pid->delta_u;
        //        abs_limit(&(pid->delta_out), pid->MaxOutput);
        //        pid->last_delta_out = pid->delta_out;	//update last time
    }

    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pid_mode == POSITION_PID ? pid->pos_out : pid->delta_out;
    //
}
/*pid总体初始化-----------------------------------------------------------------*/
/**
 * @brief 初始化PID控制器结构体并配置参数
 *
 * 该函数用于初始化PID控制器的函数指针和基本参数。通过函数指针将具体初始化工作
 * 委托给pid_param_init函数完成，支持后续的参数重置和多种计算模式(注释部分显示可扩展性)
 *
 * @param pid               指向pid_t结构体的指针，用于存储PID配置
 * @param mode              PID控制模式选择
 * @param maxout            PID输出最大值限制(抗饱和)
 * @param intergral_limit   积分项限幅值(抗积分饱和)
 * @param kp                比例系数
 * @param ki                积分系数
 * @param kd                微分系数
 */
void PID_struct_init(
    pid_t *pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,

    float kp,
    float ki,
    float kd)
{
    /* 初始化函数指针：绑定结构体操作方法 */
    /* 设置参数初始化函数和重置函数，注释部分保留扩展计算方法的接口 */
    pid->f_param_init = pid_param_init;
    pid->f_pid_reset = pid_reset;
    //	pid->f_cal_pid = pid_calc;
    //	pid->f_cal_sp_pid = pid_sp_calc;	//addition

    /* 执行实际参数初始化：通过函数指针调用具体实现 */
    /* 将模式、限制参数和PID系数传递给底层初始化函数 */
    pid->f_param_init(pid, mode, maxout, intergral_limit, kp, ki, kd);

    // EMLOG(LOG_INFO, 
    //     "PID Init: mode=%d, maxout=%d, integral_limit=%d, kp=%.2f, ki=%.2f, kd=%.2f", 
    //     mode, maxout, intergral_limit, kp, ki, kd);
}

// pid_t pid_rol = {0};
// pid_t pid_pit = {0};
// pid_t pid_yaw = {0};
// pid_t pid_yaw_omg = {0};//角速度环
// pid_t pid_pit_omg = {0};//角速度环
// pid_t pid_yaw_alfa = {0};		//angle acce

// pid_t pid_chassis_angle={0};
// pid_t pid_poke = {0};
// pid_t pid_poke_omg = {0};
// pid_t pid_imu_tmp;
// pid_t pid_x;
// pid_t pid_cali_bby;
// pid_t pid_cali_bbp;

/* 进程ID存储变量组 */
/* pid_angular_speed: 主控制进程ID，用于跟踪系统核心调度进程 */
/* pid_position: 位置管理进程ID，负责处理坐标定位相关服务 */
/* pid_speed: 速度控制进程组ID数组，包含4个元素分别对应 */
/*          不同轴向的运动控制进程(如X/Y/Z轴及其备用轴) */

void pid_test_init()
{

    // 为了解决上位机调参的时候第一次赋值的时候清零其他参数， 应该提前把参数表填充一下！
}

