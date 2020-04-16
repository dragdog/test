/************************************************************************
*
* 文件名   ：  FFFE.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0  
* 创建时间 ：  2009.08.17
* 作者     ：  车载及协议部
* 功能描述 ：  FFFE协议转义及接收文件 
* 使用注意 ：  1.所能处理的长度为65535，因为受长度的类型UINT16限制。
* 修改记录 ：  
*
************************************************************************/
#ifndef _FFFE_H
#define _FFFE_H

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  
{
  UINT8 *pRawDataRcvBuf;/*未解转义数据接收缓冲区*/
  UINT16 RawDataRcvBufferSize;/*未解转义数据接收缓冲区大小*/
  UINT8 RcvState ;/*FFFE数据接收状态*/
  UINT8 GotFrame ;/*FFFE是否收到了完整帧*/
  UINT16 DataCount ;/* 收到完整帧之前，接收到的FFFE数据总长度，每次收到完整帧后清零*/
}FFFE_STATE_STRU;

/*
* 功能描述： 将数据进行FFFE转义处理（不加头FFFE及尾FFFD）。
* 输入参数： const UINT8 *pSrc    数据(无FFFE转义)
*            UINT16 SrcLen        数据(无FFFE转义)的长度
* 输出参数： UINT8 *pTgt          数据(经过FFFE转义)
* 返回值：   转义后的长度   
*/
UINT16 TranslateToFFFE(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen);


/*
* 功能描述：  将经过FFFE转义处理后的数据（已经去掉了头FFFE及尾FFFD）还原为原始数据
* 输入参数：  const UINT8 *pSrc,    数据(FFFE转义)
*             UINT16 SrcLen,        数据(FFFE转义)的长度
* 输出参数：  UINT8 *pTgt,          数据(去掉FFFE转义)
* 返回值：    大于0,  转义成功,返回转义后的数据长度
*             0,   有不能转义的字符，转义失败。或者转义后的数据长度为0   
*/
UINT16 TranslateFromFFFE(const UINT8 *pSrc,UINT8 *pTgt,UINT16 SrcLen);

/*
* 功能描述：  FFFE结构体初始化函数
* 输入参数：  UINT16 FFFEBufferSize, 用于接收一帧数据的缓冲区长度   
*             PORT_STATE_STRU *pFFFEStatStru, FFFE状态结构体
* 返回值：    0,函数执行失败
*             1,函数执行成功
*/
UINT8 FFFEInit(FFFE_STATE_STRU *pFFFEStatStru,UINT16 FFFEBufferSize);

/*
* 功能描述：   按字符接收待解转义的数据，获得整帧FFFE报文（不含FFFE和FFFD）。
* 输入参数：   UINT8 RcvChar,      当前接收的字符
*              PORT_STATE_STRU *pFFFEStatStru,
* 输出参数：   UINT8 *pRawDataRcv,  数据接收缓冲区指针
* 返回值：     大于0,  表示收到完整帧的数据长度，并将收到的完整帧拷贝到接收缓冲区；
*              0,   未收到整帧数据； 
*/

UINT16 FFFERcv(FFFE_STATE_STRU *pFFFEStatStru,UINT8 RcvChar, UINT8 *pRawDataRcv);

/*
* 功能描述：  FFFE状态清理函数
* 输入参数：  PORT_STATE_STRU *pFFFEStatStru, FFFE状态结构体
* 返回值：     
*/
void FFFEFree(FFFE_STATE_STRU *pFFFEStatStru);

/*
* 功能描述： 将数据进行FFFE转义处理,加头FFFE及尾FFFD
* 输入参数： const UINT8 *pSrc    数据(无FFFE转义)
*            UINT16 SrcLen        数据(无FFFE转义)的长度
* 输出参数： UINT8 *pTgt          数据(经过FFFE转义),并添加FFFE头及FFFD尾
* 返回值：   转义后的长度   
*/
UINT16 FFFEPack(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen);

#ifdef __cplusplus
}
#endif

#endif
