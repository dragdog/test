/********************************************************
*                                                                                                            
* 文件名  ： FsfbProtcl.h   
* 版权说明： 北京交控科技有限公司 
* 版本号  ： 1.0
* 创建时间： 2013.09.05
* 作者    ： 王佩佩
* 功能描述： Fsfb协议实现  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef	_FSFB_PROTCL_H_
#define	_FSFB_PROTCL_H_

#include "fsfbProtclStruct.h"

#ifdef __cplusplus
extern "C" {
#endif	

UINT8 FsfbProtclInit(UINT8* fileName,FSFB_INFO_STRU* pFsfbStruct);

/***********************************************************************
* 方法名   : FsfbProtclOutPut
* 功能描述 : 。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pAppData   	const UINT8*			IN			应用层到Rssp层的输入队列队列节点
*	DataLen	   	UINT8					IN			数组pAppData的数据长度
*  OutTsdMsg  	UINT8*					OUT			FSFD消息帧 
*  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			更新对应节点的索引，如果不存在返回 -1 

* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
UINT8 FsfbProtclOutPut(FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclReceiveData(FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclDeleteLnk(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclGetLnkStatus(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclRefreshLnk(UINT8* pLnkStatusData,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclFreeSpace(FSFB_INFO_STRU* pFsfbStruct);

#ifdef __cplusplus
}
#endif	


#endif