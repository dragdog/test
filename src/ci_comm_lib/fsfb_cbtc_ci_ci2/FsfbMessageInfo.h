/************************************************************************
*
* 文件名   ：  FsfbMessageInfo.C
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.05
* 作者     ：  王佩佩
* 功能描述 ：  FSFB/2协议中各种帧格式的组帧以及链路状态检查
* 使用注意 ： 无
* 修改记录 ：
* 时间			修改人		修改理由
************************************************************************/
#ifndef	_FSFB_MESSAGE_INFO_H_
#define	_FSFB_MESSAGE_INFO_H_

#include "FsfbProtclConfigMessage.h"
#include "fsfbProtclStruct.h"


#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* 方法名   :   AccordInfoCreateFrameData
* 功能描述 : 根据应用数据确定创建SBT或TSD报文
* 输入参数 : 
*	参数名		类型	输入输出	描述
*  --------------------------------------------------------------------------
*	pAppData		UINT8*	 IN		应用数据
*   DataLen         UINT816    IN     数据长度
*   OutMsg          UINT8*   OUT     输出的报文
*   outPutDataLen   UINT16*   OUT     输出报文长度
*   pPNode          Fsfb_Lnk_Info_Node_Stru** OUT  链路信息
*   pFsfbStru       FSFB_INFO_STRU *        INOUT  协议统一结构体
* 返回值   :	UINT8* :1:正常返回   1:错误返回
***********************************************************************/
UINT8 AccordInfoCreateFrameData(const UINT8 *pAppData,UINT16 DataLen,
								UINT8 *OutMsg,UINT16* outPutDataLen,Fsfb_Lnk_Info_Node_Stru** pPNode,FSFB_INFO_STRU *pFsfbStru);

/***********************************************************************
* 方法名   : ArrordLnkInfoCreateFrame
* 功能描述 : 根据链路中发送节点的状态组建帧格式。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pFsfbStruct   FSFB_INFO_STRU*			IN			统一结构体
*  outMsg  	UINT8*					OUT			FSFB消息帧 
*  datalen 	UINT16*		OUT			输出消息长度

* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
UINT8 ArrordLnkInfoCreateFrame(FSFB_INFO_STRU* pFsfbStruct,UINT8* outMsg,UINT16* datalen);

/***********************************************************************
* 方法名   : BuildBtcBody
* 功能描述 :组建BTC帧的Body部分。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pOutDataBuff   UINT8*			OUT			输出数组
*   outDataLen  	UINT16*					OUT			数据长度 
* 返回值   : 无
***********************************************************************/
void BuildBtcBody(UINT8* pOutDataBuff,UINT16* outDataLen);

/***********************************************************************
* 方法名   : LnkInfoBuildFrame
* 功能描述 :组建输出给应用数据的帧头。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	devName   UINT16			IN			设备名称
*   msgLen  	UINT16					IN			数据长度 
*   pOutDataBuff UINT8*              OUT         输出数组
* 返回值   : 无
***********************************************************************/
void LnkInfoBuildFrame(UINT16 devName,UINT16 msgLen,UINT8* pOutDataBuff);
/***********************************************************************
 * 方法名   : FsfbAnalysisSseMsg
 * 功能描述 : 本函数对收到的SSE消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  FSFB_INFO_STRU  *pFsfbStruct     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  0:错误返回
 ***********************************************************************/
UINT8 FsfbAnalysisSseMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
/***********************************************************************
 * 方法名   : FsfbAnalysisSsrMsg
 * 功能描述 : 本函数对收到的SSR消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  pFsfbStruct FSFB_INFO_STRU*     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisSsrMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
/***********************************************************************
 * 方法名   : FsfbAnalysisAbtMsg
 * 功能描述 : 本函数对收到的ABT消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  pFsfbStruct FSFB_INFO_STRU*     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisAbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
/***********************************************************************
 * 方法名   : FsfbAnalysisEbtMsg
 * 功能描述 : 本函数对收到的EBT消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      设备名称
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     接收到的数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFB协议统一结构体 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisEbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
/***********************************************************************
 * 方法名   : FsfbAnalysisBtdMsg
 * 功能描述 : 本函数对收到的BTD消息进行解帧处理，取得相关信息,
              以及将数据按一定的格式输出。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      设备名称
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     接收到的数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFB协议统一结构体 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisBtdMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);

#ifdef __cplusplus
}
#endif

#endif