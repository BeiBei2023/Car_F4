#ifndef __BUTTON_CONFIG_H_
#define __BUTTON_CONFIG_H_

#include "multi_button.h"
#include "main.h"

//按键ID
#define button1_id 0

typedef struct
{
    uint8_t button_count;
    uint8_t button_flage;
} button_get;

extern button_get but_cnt ;


uint8_t read_button1_GPIO();
void Callback_SINGLE_CLICK_Handler(void *btn);
void Callback_DOUBLE_Click_Handler(void *btn);
void Button_Init();

#endif // !__BUTTON_CONFIG_H_
