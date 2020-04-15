/********************************************************
*                                                                                                            
* 文 件 名： TransmitDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-04-01
* 作    者： 联锁组
* 功能描述： 通信管理配置定义头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _TRANSMIT_DEFINE_H_
#define _TRANSMIT_DEFINE_H_


/*包含系统配置头文件*/
#include "../System/SystemConfigDefine.h"




#define TRANSMIT_OBJECT_SUM_MAX              8          /*定义联锁通信对象的最大值*/
#define TRANSMIT_OBJECT_DEVICE_SUM_MAX       18         /*定义联锁同相同设备类型通信的设备最大值*/



/*通信状态*/
#define TRANSMIT_STATE_BAD                   0x55      /*通信中断*/
#define TRANSMIT_STATE_GOOD                  0xaa      /*通信良好*/


/*通信故障判断*/
#define  COM_ERROR_FSIO_MAXCYCLENUM    5      /*IO与联锁通信故障判断最大周期数*/


/*通信周期*/
#define  COM_CYCNUM_CI_TO_ATS      1        /*联锁到ATS每两个周期发送一次*/
#define  COM_CYCNUM_CI_TO_XIANDI   1        /*联锁到现地每两个周期发送一次*/

#endif


