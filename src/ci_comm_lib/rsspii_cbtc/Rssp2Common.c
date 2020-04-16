/************************************************************************
*
* 文件名   ：  Rssp2Common.c
* 版权说明 ：  
* 版本号   ：  1.0
* 创建时间 ：  
* 作者     ：  车载及协议部
* 功能描述 ：  RSSP2协议层使用的基本函数及检查函数.  
* 使用注意 ： 
* 修改记录 ：
* 时间				修改人		修改理由
*---------------------------------------------
************************************************************************/

#include "Rssp2Common.h"
#include "Convert.h"
 
/***********************************************************************
 * 方法名   : Rssp2MemSet
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型		输入输出	描述
 *  --------------------------------------------------------------
 *	pDestAdd	UINT8 *		INOUT		内存空间开始地址
 *  Len			UINT16		IN			内存空间大小
 *	Val			UINT8		IN			指定值	
 * 
 * 返回值   : 1 正常
 ***********************************************************************/
UINT8 Rssp2MemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
	    pDestAdd[index] = Val;
    }

	return (UINT8)1;
}

/***********************************************************************
 * 方法名   : Rssp2MemCopy
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型			输入输出	    描述
 *  --------------------------------------------------------------
 *  pSrcAdd		const UINT8 *	IN		    源内存空间地址
 *	pDestAdd	UINT8 *			INOUT	    目的内存空间地址
 *  Len			UINT16			IN		    内存空间大小	
 * 
 * 返回值   : 1 正常
 ***********************************************************************/
UINT8 Rssp2MemCopy(const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
        pDestAdd[index] = pSrcAdd[index];
    }
    
    return (UINT8)1;
}

/***********************************************************************
 * 方法名   : Rssp2TypeId2DevName
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
void Rssp2TypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName)
{
	UINT8 buff[2];
	buff[0] = Type;
	buff[1] = Id;	  
	*pDevName = ShortFromChar(buff);
	return;
}
/***********************************************************************
 * 方法名   : DevName2TypeId_WL
 * 功能描述 : 根据类型取得相对应的类型和Id
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	 DevName	UINT8			IN			标识
 *	 pType		UINT8*			OUT			类型
 *	 pID		UINT8*			OUT			Id	 
 * 
 * 返回值   : 无
 ***********************************************************************/
void Rssp2DevName2TypeId(UINT16 DevName, UINT8* pType,UINT8* pId)
{
	UINT8 buff[2] = { 0 };
	ShortToChar(DevName,buff);
	*pType = buff[0];
	*pId = buff[1];
	return;
}


