/************************************************************************
*
* 文件名   ：  RsspCommonWl.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.10.02
* 作者     ：  车载及协议部
* 功能描述 ：  RSSP协议层使用的FLSR基本函数及检查函数.  
* 使用注意 ： 使用之前必须调用inittab_WL函数来完成gTableFast_WL的值设定。
* 修改记录 ：
* 时间			修改人		修改理由
*---------------------------------------------
* 2009/11/01	楼宇伟	初版做成
* 2009/12/02	楼宇伟	MisraC对应
* 2013/03/01    王佩佩  修改CI与VOBC的定义，删除ZC的定义
************************************************************************/

#ifndef	_RSSP2_COMMON_H_
#define	_RSSP2_COMMON_H_

#include "CommonTypes.h"

#ifndef NULL
#define NULL 0
#endif

#define MAX_RSSP2_MSG	1400				/* RSSP层消息帧最大长度 */


#ifdef __cplusplus
extern "C" {
#endif

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
UINT8 Rssp2MemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val);

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
UINT8 Rssp2MemCopy(const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len);

/***********************************************************************
 * 方法名   : TypeId2DevName_WL
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
void Rssp2TypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName);

/***********************************************************************
 * 方法名   : DevName2TypeId_WL
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
void Rssp2DevName2TypeId(UINT16 DevName,UINT8* pType, UINT8* pId);


#ifdef __cplusplus
}
#endif

#endif		/* endif */
