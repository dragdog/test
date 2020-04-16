/***************************************************************************************
*		版本号：V1.0
*
*		版权所有 (C) 2007 北京交通大学 CBTC项目 2乘2取2安全计算机课题组
*
*		作者：陈树泉
*
*		文件名：vxWorks_debug_Assistant.h
*
*       描述：vxWorks辅助调试工具中，vxWorks部分相关头文件。设置有关信息接收端IP，端口以及所支持的数据长度等信息。
*
*       参考文档：
*
*       创建时间：2007-5-9
*
*		修改历史：
*			2007-5-10 陈树泉	增加互斥信号量semM_debug_infor保护用于发送消息的全局数组。
*			2007-5-10 陈树泉	把输出功能改为一个函数，写消息队列部分都移至辅助调试输出函数中。
*			2007-5-12 陈树泉	增加了数组输出函数debug_out_array。建议不要一次输出数据长度超过50*MAX_LEN_DEBUG。
*           2007-5-12 陈树泉    修改与上位机通信是的格式。每次通信时第一个字节作为传送数据存储方式标识符。
*       0x55表示方式一，0xAA表示方式二。存储方式一：第一个字节为存储方式标识符，后边紧接着存两个4字节整型
*       变量，紧接着以十六进制存放一个4字节变量，从第十五个字节开始以字符串形式存储。存储方式二：所有数据都以十六进制形式存储。
*			2007-5-17 陈树泉	修改用于保护全局资源的信号量为二进制信号量semB_debug_infor。
*			2007-5-19 	陈树泉	去掉调试信息输出函数和数组输出函数中对全局变量的保护，因为总是会出现这个信号量错误而导致的
*				记录失败。
*           2007-5-25   陈树泉  将相关全局资源的保护改为条件编译处理。原来使用#if 0条件编译去掉。
*           2007-6-19   陈树泉  将表示任务堆栈大小的宏改为DEBUG_TASK_STACK_SIZE。
*           2007-6-26   张岩    增加了第三方printf显示函数debug_infor_out_printf()。
*           2007-7-13   刘朔    增加ZC应用输出数据函数ZC_debug_out_array()。并增加了宏定义ZC_INFOR表示数据类型，
*               以确定接收方保存方式。
*           2007-7-20   杨旭文  增加任意数目个参数的输出函数debug_infor_printf()及所用到的相关函数。相应的增加宏定义STRING。
*           2007-7-25   杨旭文  增加了用于求幂的函数power，因为发现数学库《math.h》中的求幂函数pow存在漏洞，求出来
*               的数据有时候对有时候不对。
*           2007-7-30   陈树泉  去掉了关于用于全局资源保护的信号量及相关条件编译。因为没有全局的资源需要保护。发送缓冲区都改成了局部的了。
*           2007-8-2    陈树泉  调整消息队列的设置，由原来1000个消息，每个消息最多1000字节改为最多2000消息，每个消息最大500个
*               字节，这样主要是为了提高记录能力，因为大部分记录其实不超过500字节，增大消息孰能减小记录不全的可能性。
*           2007-8-24   陈树泉  修改数组输出函数的参数设置，增加记录方式的参数，这样就可以将多种情况的数组输出函数整合。也便于扩展功能。
*           2007-8-24   陈树泉  将16进制记录的记录格式宏定义由原来的MODE2改为HEX_MODE，值不变。
*           2007-9-10   陈树泉  增加任务优先级宏定义。
            2008-7-22   王悉    将原来的发送任务取消，将sendto函数放到了每一个输出的函数中，取消了消息队列和与任务有关的宏定义
            2010-6-30   王悉    删除了debug_infor_out和debug_infor_out_printf函数

    存在的问题：
        1、偶尔有记录不完全的情况。可以通过设置发送消息队列时时间参数以及设置网络通信重发机制避免。

*************************************************************************************/

#ifndef _VXWORKS_DEBUG_ASSITANT_H_
#define _VXWORKS_DEBUG_ASSITANT_H_

#include "vxWorks.h"

/*================= 条件编译 =========================================================*/
#define DEBUG_INFOR						/*输出调试相关信息到上位机，该宏可用于作为是否需要输出的编译开关*/

#define  UN_MUTEX_DEBUG                  /*不使用互斥保护，define使能*/


#define GREEN_NET_MAIN                /* 开通绿网和维修机之间的通信。*/
#define PURPLE_NET_MAIN               /* 开通紫网和维修机之间的饿通信。*/

/*辅助调试输出信信息相关宏定义*/
#define DEBUG_PORT 5000				/*辅助调试输出信息的端口号*/
#define AIM_IP "192.168.20.209"      /*接收端上位机IP地址，应与运行该程序的目标机在同一网段*/
#define AIM_IP_GREEN "192.168.20.110"      /*绿网维修机IP地址。*/
#define AIM_IP_PURPLE "192.168.3.110"      /*紫网维修机IP地址。*/
#define MAX_LEN_DEBUG 24000		/*辅助调试单帧输出信息的消息最大长度*/
#define	MAX_OUT_DEBUG_INFO_SIZE	9000	/*调试信息最大输出长度*/


/*上位机存储方式宏定义*/
#define MODE1   0x55        /*方式一：第一个字节为存储方式标识符，后边紧接着存两个4字节整型
                            变量，紧接着以十六进制存放一个4字节变量，从第十五个字节开始以字符串形式存储。*/
#define HEX_MODE   0xAA     /*方式二：所有数据都以十六进制形式存储。*/
#define MODE3   0x5A        /*方式三：在第三方显示异常信息，其他如方式一*/
#define STRING  0xA5        /*字符串方式，即支持不定数目个参数的数出，格式同printf*/
#define TIME_ANNALYS 0x20   /*时间特性分析所需时间信息的记录*/

#define ZC_INFOR 0x10       /*方式四：用于ZC方面的第三方打印的宏*/
#define SAFE_INFO 0x30
#define MODECONTROL 0x40



#ifdef __cplusplus
extern "C" {
#endif
/*辅助调试输出相关函数声明*/
STATUS debug_infor_printf(
						char* debug_string,                 /*用于实现C语言中的printf函数的功能,用于平台的第三方打印*/
						...);

STATUS debug_initial( void );								/*辅助调试输出功能的初始化函数*/
STATUS debug( void );										/*辅助调试输出任务*/

STATUS debug_out_array(										/*辅助调试数组输出函数。建议待输出数据长度不要超过MAX_LEN_DEBUG*50*/
                        char  mode,                          /*记录方式，目前包括ZC，16进制记录和时间分析信息记录*/
						char* debug_array, 					/*待输出数组首地址*/
						short len);							/*待输出长度*/


STATUS ZC_debug_out_array(                                  /*ZC专用数据输出函数，输出数组有ZC应用约定的特定格式*/
                        char* debug_array,
                        short len);


void String_connect(
                        char Temp_String[MAX_LEN_DEBUG],    /*用于处理需要进行字符串连接的函数*/
                        short wLength,
                        ...);

char Format_decimal_to_char(
                        char Array[MAX_LEN_DEBUG],          /*用于将十进制整数转化为char型的字符串用于输出*/
                        unsigned int Interger);

char Format_hex_to_char(
                        char Array[MAX_LEN_DEBUG],          /*用于将十六进制的数转化为char型的字符串输出*/
                        unsigned int Hex);

char Format_string_to_char(
                        char Array[MAX_LEN_DEBUG],          /*用于将字符串插入到原先的输出字符串中的函数*/
                        char String[MAX_LEN_DEBUG]);
long power(                                                 /*求幂函数，替换出现问题的pow库函数*/
                        int dwData,                         /*底数*/
                        char chEquation);                   /*指数*/

#ifdef __cplusplus
}
#endif

#endif _VXWORKS_DEBUG_ASSITANT_H_

