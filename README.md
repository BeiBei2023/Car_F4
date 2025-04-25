# Car_F4

## 驱动函数介绍

### LCD 的函数

使用的屏幕驱动芯片是 ST7735S，使用 I2C 接口。主要的驱动文件在这两个文件中：

> "Car_F4\Car_F427\BSP\ST7735S\st7735s.c"  
> "Car_F4\Car_F427\BSP\ST7735S\st7735s.h"

有两个文件是页面管理的程序，主要用来实现页面切换并且显示不同的内容的，这两个文件是：

> "Car_F4\Car_F427\BSP\ST7735S\screen_manager.c"  
> "Car_F4\Car_F427\BSP\ST7735S\screen_manager.h"

新的界面和文字主要在这两个文件中添加:

> "Car_F4\Car_F427\BSP\ST7735S\screen_config.c"  
> "Car_F4\Car_F427\BSP\ST7735S\screen_config.h"

具体的用法是这样的：

在 screen_config.h 中添加如下代码，作用分别为:

首先是**添加一个页面的显示的文字**，使用这个代码

```c
TextRegion voltage_regions[] = {
    {31, 43, &Font_16x26, ST7735_BLACK, ST7735_WHITE, ""} // 电压显示区域
};
TextRegion temp_regions[] = {
    {9, 55, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""}, // 温度
    {91, 55, &Font_11x18, ST7735_BLACK, ST7735_WHITE, ""} // 湿度
};
```

TextRegion 是是一个结构体，用来描述文字显示的位置，字体，颜色，以及显示的文字。其`voltage_regions`是一个数组，用来描述显示的文字的区域，字体，字体颜色，背景颜色和具体显示的数值的空的数组。

接下来是**配置显示的界面的信息**，示例代码如下：

```c
/**** 界面配置 ****/
ScreenConfig screens[] = {
    {// 电压界面
     .bg_image = gImage_dianya,
     .bg_width = 160,
     .bg_height = 80,
     .regions = voltage_regions,
     .region_count = 1},
    {// 温湿度界面
     .bg_image = gImage_wenshidu,
     .bg_width = 160,
     .bg_height = 80,
     .regions = temp_regions,
     .region_count = 2}};
```

这是创建了这个数组`screens`，在其中来创建我们现实的界面。填入的参数是`gImage_dianya`、`gImage_wenshidu`，分别对应了 voltage_regions 和 temp_regions 这两个数组。 然后，`bg_image`就是填入经过取模软件处理过的图片，`bg_width`和`bg_height`就是图片的宽高，`regions`就是上面定义的两个数组，`region_count`就是数组的长度（界面中要显示的文本或者数值的数量）。

最重要的是**声明一个全局变量**，可在全局调用屏幕显示

```c
/**** 全局管理器实例 ****/
ScreenManager screen_mgr;
```

不要忘记在头文件中声明刚才创建好的函数。比如：

```c
// 声明为外部可访问的全局配置
extern TextRegion voltage_regions[];    // 电压显示区域
extern TextRegion temp_regions[];       // 温度显示区域
extern ScreenConfig screens[];           // 屏幕配置
extern ScreenManager screen_mgr;        // 屏幕管理器
```

在文件 main.c 中，初始化相关函数，当然是要初始化屏幕，然后初始化屏幕的管理的函数。

```c
// 初始化界面管理器
ScreenManager_Init(&screen_mgr, screens, 2);    // 初始化屏幕管理器,两个界面
ScreenManager_Switch(&screen_mgr, 0);  // 上电时就切换到第一个界面
```

在使用屏幕管理器时，你需要在合适的时机调用`ScreenManager_UpdateRegion(ScreenManager *mgr, uint8_t region_id, const char *new_str)`函数来更新屏幕。这个函数会根据当前的界面状态，选择正确的界面进行更新。但别忘了，这个的前提是要选择某个界面，否则会报错，使用`ScreenManager_Switch(ScreenManager *mgr, uint8_t screen_id)`来选择某个界面。

以下轮播的示例程序：

```c
for (int i = 0; i < 2; i++)
{
    screen_mgr.current_screen = i;
    ;
    // 电压界面更新
    if (screen_mgr.current_screen == 0)
    {
        ScreenManager_Switch(&screen_mgr, 0);
        char buf[20];
        snprintf(buf, sizeof(buf), "%.2f", adc_v.v_in);
        ScreenManager_UpdateRegion(&screen_mgr, 0, buf);
    }
    else if (screen_mgr.current_screen == 1)
    {
        ScreenManager_Switch(&screen_mgr, 1);
        float temp = sensorData.Temperature;
        float humidity = sensorData.Humidity;

        char temp_str[20];
        snprintf(temp_str, sizeof(temp_str), "%.1f", temp);
        ScreenManager_UpdateRegion(&screen_mgr, 0, temp_str);

        char hum_str[20];
        snprintf(hum_str, sizeof(hum_str), "%.1f", humidity);
        ScreenManager_UpdateRegion(&screen_mgr, 1, hum_str);
    }
    vTaskDelay(pdMS_TO_TICKS(2000)); // 200ms检测周期
}
```
