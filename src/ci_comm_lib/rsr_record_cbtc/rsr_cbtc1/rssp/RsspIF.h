/********************************************************
*                                                                                                            
* 文件名  ： RsspIF.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： Rssp层对外接口定义 
* 使用注意： 
*   
* 修改记录：   
*
********************************************************/ 

#ifndef _RSSP_IF_H_
#define _RSSP_IF_H_

#include "CommonTypes.h"
#include "CommonQueue.h" 
#include "Rsr.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsspReceive
 * 功能描述 : 对红蓝网输入数据进行RSR层的安全处理，将安全数据交付应用。
 * 输入参数 : 
 *	参数名				类型							    输入输出		描述
 *  --------------------------------------------------------------
 *	SigToRs   QueueStruct*		IN		信号层到Rssp层的输入队列
 *	RsToSig   QueueStruct*					OUT		Rssp层到应用层的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspReceive(QueueStruct *SigToRs,QueueStruct *RsToApp, RSR_INFO_STRU *pRsrStru);

/***********************************************************************
 * 方法名   : RsspOutput
 * 功能描述 : 对应用输出数据进行RSR层的安全处理并输出。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	SigToRs   QueueStruct*		IN		信号层到Rssp层的输入队列
 *	RsToSig   QueueStruct*		OUT		Rssp层到应用层的输出队列
 *  varArray	 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspOutput(QueueStruct *AppToRs,QueueStruct *RsToSig,UINT8 *varArray,RSR_INFO_STRU *pRsrStru);

/***********************************************************************
 * 方法名   : RsspInit
 * 功能描述 : 通过该函数，实现对RSR层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR层统一结构体，需进行初始值设置。
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspInit(RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : RsspFreeSpace
 * 功能描述 : 通过该函数，释放RSR层开辟的空间。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspFreeSpace(RSR_INFO_STRU *pRsrStruct);


/***********************************************************************
 * 方法名   : RsspLnkStatus
 * 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
 * 					 存在某个安全接受节点，则返回该安全节点的通信状态
 * 					 不存在某个安全接收节点，则返回错误
 * 输入参数 : 
 *	参数名				类型				输入输出		描述
 *  -------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSP源地址
 *  BsDAdd			INT16			IN				BSD地址
 *  SeDAdd			UINT16			IN				RSSP目标地址（SSE、SSR地址）
 * 返回值   : UINT8  	 	 0：不存在该条链路，返回错误；
 *							0x17：链路在数据传输正常状态DATA
 *	  						0x2b：链路在无法传输数据状态HALT
 ***********************************************************************/
UINT8 RsspLnkStatus(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : RsspLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名				类型		输入输出		描述
 *  --------------------------------------------------------------
 *	SrcAddr  		UINT16			IN  			RSSP源地址
 *  BsdAddr			UINT16			IN				BSD地址
 *  DestAddr		UINT16			IN				RSSP目标地址（SSE、SSR地址）
 *  devName         UINT16*         OUT             设备名称
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspLnkDelete(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,UINT16* devName,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : RsspRefresh
 * 功能描述 : 应用使用该函数来重新对本机RSR层的中间变量重新赋值。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	VarLen  	UINT16			IN			中间数组的大小
 *	VarArray	UINT8*			IN			中间变量数组
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspRefresh(UINT16 VarLen, const UINT8 *VarArray, RSR_INFO_STRU *pRsrStruct);

/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif

