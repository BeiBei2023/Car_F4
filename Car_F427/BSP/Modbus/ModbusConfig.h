/*
 * ModbusConfig.h
 *
 *  创建日期: 2021年4月28日
 *      作者: Alejandro Mera
 *
 *  这是Modbus库配置的模板文件。
 *  每个项目都需要根据需求复制并重命名为ModbusConfig.h，并添加到包含路径中。
 */

#ifndef THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_
#define THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_



/* 取消注释以下行以启用对基于USB CDC协议的Modbus RTU的支持。仅在BluePill f103板上测试通过。 */
//#define ENABLE_USB_CDC 1

/* 取消注释以下行以启用对Modbus TCP的支持。仅在Nucleo144-F429ZI上测试通过。 */
//#define ENABLE_TCP 1

/* 取消注释以下行以启用Modbus RTU USART DMA模式的支持。仅在Nucleo144-F429ZI上测试通过。  */
//#define ENABLE_USART_DMA 1


#define T35  5              // T35定时器周期（以tick为单位），用于帧结束检测
#define MAX_BUFFER  128	    // 通信缓冲区的最大字节数
#define TIMEOUT_MODBUS 1000 // 主站查询超时时间（以tick为单位）
#define MAX_M_HANDLERS 2    // 可同时工作的最大modbus处理器数量
#define MAX_TELEGRAMS 2     // 主站队列中最大报文数量

#if ENABLE_TCP == 1
#define NUMBERTCPCONN   4   // 最大同时客户端连接数，应小于或等于LWIP配置
#define TCPAGINGCYCLES	1000 // 服务器在关闭因无活动的连接前检查请求的次数
/* 注意：连接的总老化时间约为NUMBERTCPCONN*TCPAGINGCYCLES*u16timeOut tick
*/
#endif




#endif /* THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_ */
