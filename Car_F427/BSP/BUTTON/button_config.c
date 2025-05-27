#include "button_config.h"
#include "elog.h"

// 申请一个按键结构
struct Button button1; // 对应 KEY1
struct Button button2; // 对应 KEY2

// 申请一个按键状态结构体
Button_status button_status;

// 按键状态读取接口
uint8_t read_button_GPIO(uint8_t button_id)
{
    // 共享GPIO读取功能给多个按键
    switch (button_id)
    {
    case button1_id:
        return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin); // GPIO_PIN_RESET, 返回0表示按键按下，1表示按键松开
    case button2_id:
        return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin); // GPIO_PIN_RESET, 返回0表示按键按下，1表示按键松开

    default:
        return 0; // 默认返回值，表示未定义的按键ID
    }
}

void Callback_SINGLE_CLICK_Handler(void *btn)
{
    Button *button = (Button *)btn; // 将void指针转换为Button指针

    switch (button->button_id)
    {
    case button1_id:
        // 清空状态
        button_status.button1_status = 0; // 按键1状态清零
        button_status.button2_status = 0; // 按键2状态清零
        /* code */
        log_a("Button1 Single Clicked");

        // 将状态信息存放在结构体中
        button_status.button1_status = 1; // 按键1单击状态设置为1

        // 日志测试

        log_a("button_status.button1_status = %d", button_status.button1_status);
        log_a("button_status.button2_status = %d", button_status.button2_status);
        break;

    case button2_id:
        // 清空状态
        button_status.button1_status = 0; // 按键1状态清零
        button_status.button2_status = 0; // 按键2状态清零
        /* code */
        log_a("Button2 Single Clicked");
        button_status.button2_status = 1; // 按键2单击状态设置为1
                                          // 日志测试
        log_a("button_status.button1_status = %d", button_status.button1_status);
        log_a("button_status.button2_status = %d", button_status.button2_status);

        break;

    default:

        break;
    }
}

void Callback_DOUBLE_Click_Handler(void *btn)
{
}

void Button_Init()
{
    // 初始化按键结构
    multi_button_init(&button1, read_button_GPIO, 0, button1_id);
    multi_button_init(&button2, read_button_GPIO, 0, button2_id);

    // 注册回调函数
    button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
    button_attach(&button2, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    button_attach(&button2, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);

    // 启动按键检测
    button_start(&button1);
    button_start(&button2);
}