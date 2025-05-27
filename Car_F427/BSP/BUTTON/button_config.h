#ifndef __BUTTON_CONFIG_H_
#define __BUTTON_CONFIG_H_

#include "multi_button.h"
#include "main.h"

// 按键ID
#define button1_id 0
#define button2_id 1

// 结构体，存放按键的状态
typedef struct
{
    uint8_t button1_status; // 按键1状态
    uint8_t button2_status; // 按键2状态
} Button_status;
extern Button_status button_status; // 按键状态结构体

void Callback_SINGLE_CLICK_Handler(void *btn);
void Callback_DOUBLE_Click_Handler(void *btn);
void Button_Init();

#endif // !__BUTTON_CONFIG_H_
