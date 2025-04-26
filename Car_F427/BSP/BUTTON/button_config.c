#include "button_config.h"

// 申请一个按键结构
struct Button button1;
button_get but_cnt;

// 按键状态读取接口
uint8_t read_button_GPIO(uint8_t button_id)
{
    // 共享GPIO读取功能给多个按键
    switch (button_id)
    {
    case button1_id:
        return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin); // GPIO_PIN_RESET, 返回0表示按键按下，1表示按键松开

    default:
        return 0; // 默认返回值，表示未定义的按键ID
    }
}

typedef enum
{
    BTN_STATE_0 = 0,
    BTN_STATE_1,
    BTN_STATE_2,
    BTN_STATE_3,
    BTN_STATE_4,
    BTN_STATE_5,
    BTN_STATE_MAX
} ButtonState;

void Callback_SINGLE_CLICK_Handler(void *btn)
{
    // 单击事件：状态机循环推进
    HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
    but_cnt.button_flage = 1;

    switch (but_cnt.button_count)
    {
    case BTN_STATE_0:
    case BTN_STATE_5: // 合并相同处理的分支
        but_cnt.button_count = BTN_STATE_1;
        break;
    case BTN_STATE_1:
        but_cnt.button_count = BTN_STATE_2;
        break;
    case BTN_STATE_2:
        but_cnt.button_count = BTN_STATE_3;
        break;
    case BTN_STATE_3:
        but_cnt.button_count = BTN_STATE_4;
        break;
    case BTN_STATE_4:
        but_cnt.button_count = BTN_STATE_0;
        break;
    default: // 异常状态保护
        but_cnt.button_count = BTN_STATE_0;
        break;
    }
}

void Callback_DOUBLE_Click_Handler(void *btn)
{
    // 双击事件：状态切换器
    HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    but_cnt.button_flage = 1;

    // 使用条件表达式简化逻辑
    but_cnt.button_count = (but_cnt.button_count != BTN_STATE_5)
                               ? BTN_STATE_5
                               : BTN_STATE_0;
}

void Button_Init()
{
    // 初始化按键结构
    button_init(&button1, read_button_GPIO, 0, button1_id);

    // 注册回调函数
    button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);

    but_cnt.button_count = 0; // 初始化按键计数器
    // 启动按键检测
    button_start(&button1);
}