
/************************************************************************
*
* 文件名   ：  CRC16.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.08.21
* 作者     ：  车载及协议部
* 功能描述 ：  CRC16校验 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _CRC16_H_
#define _CRC16_H_



#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*  功能描述： CRC16函数，注意：计算CRC是从输入数据第一个字节的最高BIT开始的
*  参数说明：
*             pData,    数据指针
*             DataLen,  数据长度（字节）
*  返回值：   返回值为CRC值
*/

UINT16 Crc16(const UINT8 *pData,UINT32 DataLen);

#ifdef __cplusplus
}
#endif

#else
#endif
