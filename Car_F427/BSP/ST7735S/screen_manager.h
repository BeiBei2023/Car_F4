#ifndef __SCREEN_MANAGER_H_
#define __SCREEN_MANAGER_H_
#include "st7735s.h"

// 单个文字区域配置
typedef struct {
    uint16_t x;         // X坐标
    uint16_t y;         // Y坐标
    FontDef* font;       // 字体
    uint16_t color;     // 文字颜色
    uint16_t bg_color;  // 背景颜色
    char old_str[20];   // 旧文本缓存
} TextRegion;

// 界面配置
typedef struct {
    const uint16_t *bg_image;  // 背景图指针
    uint16_t bg_width;         // 背景图宽度
    uint16_t bg_height;        // 背景图高度
    TextRegion *regions;       // 文字区域数组
    uint8_t region_count;      // 区域数量
} ScreenConfig;


// 界面管理器
typedef struct {
    ScreenConfig *screens;     // 所有界面配置
    uint8_t screen_count;      // 界面总数
    uint8_t current_screen;    // 当前界面ID
} ScreenManager;

/* 初始化函数 */
void ScreenManager_Init(ScreenManager *mgr, ScreenConfig *screens, uint8_t count);

/* 界面切换 */
void ScreenManager_Switch(ScreenManager *mgr, uint8_t screen_id);

/* 更新指定区域 */
void ScreenManager_UpdateRegion(ScreenManager *mgr, uint8_t region_id, const char *new_str);







#endif // !__SCREEN_MANAGER_H_
