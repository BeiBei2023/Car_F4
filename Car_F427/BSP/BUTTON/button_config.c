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
        
        void Callback_SINGLE_CLICK_Handler(void *btn)
        {
            // 单击事件处理
            HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
            if (but_cnt.button_count== 0 || but_cnt.button_count== 4)
            {
                but_cnt.button_count = 1;
            }else if (but_cnt.button_count== 1)
            {
                but_cnt.button_count = 2;
            }else if (but_cnt.button_count== 2)
            {
                but_cnt.button_count = 3;
            } else  if (but_cnt.button_count == 3)
            {
                but_cnt.button_count = 5;
            }else if (but_cnt.button_count == 5)
            {
                but_cnt.button_count = 0;
            }
        }
        
        void Callback_DOUBLE_Click_Handler(void *btn)
        {
            // 双击事件处理
            HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
            if (but_cnt.button_count != 4)
            {
                but_cnt.button_count = 4;
            }else if (but_cnt.button_count == 4)
            {
                but_cnt.button_count = 0;
            }
            
            
        }
        
        void Button_Init()
        {
            // 初始化按键结构
            button_init(&button1, read_button_GPIO, 0, button1_id);
        
            // 注册回调函数
            button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
            button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
        
            but_cnt.button_count = 0;   // 初始化按键计数器
            // 启动按键检测
            button_start(&button1);
        }