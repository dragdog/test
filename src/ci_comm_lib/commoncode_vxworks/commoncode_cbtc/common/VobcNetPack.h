
/************************************************************************

* 文件名   ：  VobcNetPack.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.08.21
* 作者     ：  车载及协议部
* 功能描述 ：  vobc内网信息层协议函数
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _VOBCNETPACK_H_
#define _VOBCNETPACK_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif




/*
*  功能描述： 内网信息层打包函数
*  参数说明： IN UINT8 DataType，   报文类型
*             IN UINT32 Seq,        控制周期序列号
*             IN const UINT8 *pData,应用数据
*             IN UINT16 DataLen,    应用数据长度
*             OUT UINT8 *pPackData,  打包数据指针
*  返回值：   打包信息的数据长度
*/
UINT16 VobcNetPack(IN UINT8 DataType,IN UINT32 Seq,IN const UINT8 *pData, IN UINT16 DataLen,OUT UINT8 *pPackData);

/*
*  功能描述： 内网信息层解包函数
*  参数说明： IN const UINT8 *pPackData,     打包数据
*             IN const UINT16 PackDataLen,   打包数据长度
*             OUT UINT8 *pDataType,          数据类型
*             OUT UINT32 *pSeq,              控制周期序列号
*             OUT UINT8 *pData,              解包后的应用数据
*  返回值：   >0,  解包后的数据长度
*             0,   表示解包错误,或者解包后的数据长度为0
*/
UINT16 VobcNetUnpack(IN const UINT8 *pPackData,IN const UINT16 PackDataLen,OUT UINT8 *pDataType,OUT UINT32 *pSeq,OUT UINT8 *pData);

#ifdef __cplusplus
}
#endif

#endif


