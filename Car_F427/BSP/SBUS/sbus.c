#include "sbus.h"

sbus_data sbus_ch_data; // 解析后的SBUS数据

/**
 * @brief 解码SBUS协议帧数据到通道值及百分比数组
 * @param buf SBUS帧数据缓冲区指针（128位原始数据）
 * @return 无返回值
 */
void sbus_decode_frame(uint8_t *buf)
{
    // 检查帧头和帧尾（使用原索引current_next）
    if (buf[24] == 0x00 && buf[0] == 0x0F)
    {

        // 从原始字节数据中提取16个通道的11位数值
        sbus_ch_data.channels[0] = sbus_value_convert(((buf[1] >> 0 | (buf[2] << 8)) & 0x07FF));
        sbus_ch_data.channels[1] = sbus_value_convert((((buf[2] >> 3) | (buf[3] << 5)) & 0x07FF));
        sbus_ch_data.channels[2] = sbus_value_convert((((buf[3] >> 6) | (buf[4] << 2)) | (buf[5] << 10)) & 0x07FF);
        sbus_ch_data.channels[3] = sbus_value_convert((((buf[5] >> 1) | (buf[6] << 7)) & 0x07FF));
        sbus_ch_data.channels[4] = sbus_value_convert((((buf[6] >> 4) | (buf[7] << 4)) & 0x07FF));
        sbus_ch_data.channels[5] = sbus_value_convert((((buf[7] >> 7) | (buf[8] << 1)) | (buf[9] << 9)) & 0x07FF);
        sbus_ch_data.channels[6] = sbus_value_convert((((buf[9] >> 2) | (buf[10] << 6)) & 0x07FF));
        sbus_ch_data.channels[7] = sbus_value_convert((((buf[10] >> 5) | (buf[11] << 3)) & 0x07FF));

        sbus_ch_data.channels[8] = sbus_value_convert(((buf[12] >> 0 | (buf[13] << 8)) & 0x07FF));
        sbus_ch_data.channels[9] = sbus_value_convert((((buf[13] >> 3) | (buf[14] << 5)) & 0x07FF));
        sbus_ch_data.channels[10] = sbus_value_convert((((buf[14] >> 6) | (buf[15] << 2)) | (buf[16] << 10)) & 0x07FF);
        sbus_ch_data.channels[11] = sbus_value_convert((((buf[16] >> 1) | (buf[17] << 7)) & 0x07FF));
        sbus_ch_data.channels[12] = sbus_value_convert((((buf[17] >> 4) | (buf[18] << 4)) & 0x07FF));
        sbus_ch_data.channels[13] = sbus_value_convert((((buf[18] >> 7) | (buf[19] << 1)) | (buf[20] << 9)) & 0x07FF);
        sbus_ch_data.channels[14] = sbus_value_convert((((buf[20] >> 2) | (buf[21] << 6)) & 0x07FF));
        sbus_ch_data.channels[15] = sbus_value_convert((((buf[21] >> 5) | (buf[22] << 3)) & 0x07FF));
        
        sbus_ch_data.flags = buf[23];   
    }
}

/**
 * @brief 将SBUS的原始值进行转换
 * @param raw_value 原始值
 * @return 转换后的值
 */

uint16_t sbus_value_convert(uint16_t raw_value)
{
    /*定义输入范围和目标值*/
    const uint16_t input_min = SBUS_RANGE_MIN;         // SBUS原始值最小值
    const uint16_t input_max = SBUS_RANGE_MAX;         // SBUS原始值最大值
    const uint16_t target_min = SBUS_TARGET_RANGE_MIN; // SBUS目标值最小值
    const uint16_t target_max = SBUS_TARGET_RANGE_MAX; // SBUS目标值最大值

    /*边界检查，检查数据的范围是否在允许范围内*/
    if (raw_value < input_min)
    {
        return target_min;
    }
    else if (raw_value > input_max)
    {
        return target_max;
    }

    return (uint16_t)(((raw_value - input_min) * (target_max - target_min)) / (input_max - input_min) + target_min); // 计算转换后的值并返回
}
