//
// Created by wangb on 25-6-4.
//

#include "elog.h"

#include <stdio.h>
#include "main.h"
#include "system_run_time.h" // 引入你写的运行时间模块
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */


    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {

    /* add your code here */
     printf("%.*s", size, log); //elog 的输出
}

/**
 * output lock
 */
void elog_port_output_lock(void) {

    /* add your code here */

    __disable_irq();    //关闭全局中断
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    /* add your code here */
    __enable_irq();     //开启全局中断
}
/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    /* add your code here */
    return get_uptime_str_days_and_ms();    //获取运行时间
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {

    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {

    /* add your code here */
    //获取STM32 的线程接口


    return "";

}

void easylogger_init(void)
{
    /* init Easylogger */
	elog_init();

	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));



	/* start EasyLogger */
	elog_start();
}