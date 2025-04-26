#include "screen_manager.h"

/**
 * @brief 多界面动态显示
 * 
 */

 /**
  * 初始化屏幕管理器
  * 
  * @param mgr 屏幕管理器的指针，用于存储屏幕管理相关的数据
  * @param screens 屏幕配置数组的指针，包含所有屏幕的配置信息
  * @param count 屏幕的数量，用于屏幕管理器了解总共有多少屏幕需要管理
  * 
  * 此函数的作用是将屏幕管理器与一组屏幕配置关联起来，并将其置于初始状态
  * 它将屏幕配置数组和屏幕数量存储到屏幕管理器中，并将当前屏幕索引设置为0，
  * 即默认第一个屏幕为当前活动屏幕
  */
 void ScreenManager_Init(ScreenManager *mgr, ScreenConfig *screens, uint8_t count)
 {
     mgr->screens = screens;
     mgr->screen_count = count;
     mgr->current_screen = 0;
 }
 
 /**
  * 切换屏幕管理器当前显示的屏幕
  * 
  * 此函数根据提供的屏幕ID，切换到对应的屏幕配置，并执行必要的绘制操作
  * 如果提供的屏幕ID超出了屏幕配置的范围，则函数不执行任何操作
  * 
  * @param mgr 屏幕管理器的指针，用于访问和修改屏幕管理器的属性和方法
  * @param screen_id 要切换到的屏幕的ID，唯一标识一个屏幕配置
  */
 void ScreenManager_Switch(ScreenManager *mgr, uint8_t screen_id)
 {
     // 检查屏幕ID是否在有效范围内，如果无效则直接返回
     if(screen_id >= mgr->screen_count) return;
     
     // 1. 绘制新背景
     // 根据屏幕ID获取对应的屏幕配置，并使用背景图像数据绘制背景
     ScreenConfig *screen = &mgr->screens[screen_id];
     ST7735_DrawImage(0, 0, screen->bg_width, screen->bg_height, screen->bg_image);
     
     // 2. 重置所有旧文本
     // 遍历当前屏幕的所有文本区域，并清空旧文本内容，为显示新内容做准备
     for(int i=0; i<screen->region_count; i++){
         memset(screen->regions[i].old_str, 0, sizeof(screen->regions[i].old_str));
     }

     
     // 更新屏幕管理器的当前屏幕ID
     mgr->current_screen = screen_id;
 }
 
 /**
  * 更新屏幕区域的文本显示
  * 
  * 此函数负责根据指定的区域ID更新屏幕上的文本内容它首先计算旧文本区域的宽度，
  * 然后用背景色覆盖旧文本，最后在该区域绘制新文本这个过程确保了屏幕显示的更新是高效且准确的
  * 
  * @param mgr 屏幕管理器指针，用于访问屏幕配置和状态
  * @param region_id 区域ID，指定需要更新的屏幕区域
  * @param new_str 新的文本字符串，用于替换旧的文本内容
  */
 void ScreenManager_UpdateRegion(ScreenManager *mgr, uint8_t region_id, const char *new_str)
 {
     // 获取当前屏幕的配置信息
     ScreenConfig *screen = &mgr->screens[mgr->current_screen];
     
     // 检查区域ID是否超出当前屏幕的区域数量，如果超出则直接返回
     if(region_id >= screen->region_count) return;
     
     // 获取指定区域的配置信息
     TextRegion *region = &screen->regions[region_id];
     
     // 1. 计算旧文本区域宽度
    // uint16_t old_width = strlen(region->old_str) * region->font->width;
     
     // 2. 覆盖旧文本
    //  ST7735_FillRectangleFast(
    //      region->x,
    //      region->y,
    //      old_width,
    //      region->font->height,
    //      region->bg_color
    //  );
     
     // 3. 绘制新文本
     ST7735_WriteString(
         region->x,
         region->y,
         new_str,
         region->font,
         region->color,
         region->bg_color
     );
     
     // 4. 更新缓存
   //  strncpy(region->old_str, new_str, sizeof(region->old_str)-1);
 }
 






