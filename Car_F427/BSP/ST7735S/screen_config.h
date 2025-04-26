#ifndef __SCREEN_CONFIG_H
#define __SCREEN_CONFIG_H

#include "screen_manager.h"
#include "image.h"
#include "fonts.h"



// 声明为外部可访问的全局配置
extern TextRegion voltage_regions[];    // 电压显示区域
extern TextRegion temp_regions[];       // 温度显示区域
extern TextRegion motor_speed[];
extern ScreenConfig screens[];           // 屏幕配置
extern ScreenManager screen_mgr;        // 屏幕管理器



#endif // !__SCREEN_CONFIG_H




