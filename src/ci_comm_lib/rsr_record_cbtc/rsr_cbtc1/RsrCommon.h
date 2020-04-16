/********************************************************
*                                                                                                            
* 文件名  ： RsrCommon.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.10.25
* 作者    ： 车载及协议部
* 功能描述： Rsr层共通定义
* 使用注意： 
*   
* 修改记录：   
* 2010.05.10  楼宇伟	RsrConfig.h 改名为 RsrPlatform.h
*
********************************************************/ 

#ifndef _RSR_COMMON_H_
#define _RSR_COMMON_H_

#include "Rsr.h" 
#include "CommonTypes.h"
#include "RsrPlatform.h"
#include "sigInterface.h"


#ifndef NULL
#define NULL 0
#endif

/* 类型 */
#define RSR_TYPE_ZC	0x1E	/* ZC */
#define RSR_TYPE_CI	0x3C	/* CI */
#define RSR_TYPE_CC 0x14	/* CC */


#define MAX_RSSP_MSG	1400				/* RSSP层消息帧最大长度，即信号层消息体的最大大小 */

#define TYPE_ID_2_DEVNAME_USE_COMM			/* TRUE ： Type Id 转化为标识 使用共通转化函数 */

#ifdef __cplusplus
extern "C" {
#endif

/*丢包或非宕机错误码记录信息*/
extern UINT32	gRsrErrorInfo;	

/* 内部全局变量 申明 */
/*extern RSR_INFO_STRU *gpRsrInfoStru;*/

/*--------------------------------------------------------------------
 * RSR层内公用函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsrMemSet
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pDestAdd	UINT8 *			INOUT		内存空间开始地址
 *  Len			UINT16			IN			内存空间大小
 *	Val			UINT8			IN			指定值													
 * 
 * 返回值   : 无
 ***********************************************************************/
UINT8 RsrMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val);

/***********************************************************************
 * 方法名   : RsrMemSet
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *  pSrcAdd		const UINT8 *	IN			源内存空间地址
 *	pDestAdd	UINT8 *			INOUT		目的内存空间地址
 *  Len			UINT16			IN			内存空间大小
 *	Val			UINT8			IN			指定值													
 * 
 * 返回值   : 无
 ***********************************************************************/
UINT8 RsrMemCopy(const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len);

/***********************************************************************
 * 方法名   : TypeId2DevName
 * 功能描述 : 根据Type和ID 取得相对应的设备标识
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	 Type		UINT8			IN				类型
 *	 ID			UINT8			IN				Id
 *	 pDevName	UINT16*			OUT				标识
 * 
 * 返回值   : 无
 ***********************************************************************/
void TypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName);

/***********************************************************************
 * 方法名   : DevName2TypeId
 * 功能描述 : 根据类型取得相对应的类型和Id
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	 DevName	UINT16			IN			标识
 *	 pType		UINT8*			OUT			类型
 *	 pID		UINT8*			OUT			Id	 
 * 
 * 返回值   : 无
 ***********************************************************************/
void DevName2TypeId(UINT16 DevName,UINT8* pType, UINT8* pId);


UINT32 GetRsrErrorInfo(void);
UINT32 ReSetRsrErrorInfo(void);


/*--------------------------------------------------------------------
 * RSR层内部共用函数申明 End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif

