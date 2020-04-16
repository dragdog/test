/********************************************************
*                                                                                                            
* 文 件 名： HLHTCommonDataPacketHead.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2016-10-21
* 作    者： 联锁产品组
* 功能描述： 互联互通通用报文头校验头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _HLHTCOMMONDATAPACKETHEAD_H_
#define _HLHTCOMMONDATAPACKETHEAD_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#define HLHT_HEAD_SUCCESS (0x55) /*互联互通头检查正确*/
#define HLHT_HEAD_ERROR (0xaa) /*互联互通头检查错误*/
#define HLHT_HEAD_INIT (0x33) /*表明头是互联互通初始通信帧*/

/*************************************通用报文头消息校验错误原因定义**************************************/
#define GAL_COMPARE_INFO_TYPE_ERR 			  (UINT32)0x00000001 		/*GAL通用信息包中接口信息类型错误*/
#define GAL_COMPARE_SEND_FLAG_ERR 			  (UINT32)0x00000002 		/*GAL通用信息包中发送方标识信息错误*/
#define GAL_COMPARE_RECV_FLAG_ERR 			  (UINT32)0x00000004 		/*GAL通用信息包中接收方标志信息错误*/
#define GAL_COMPARE_EMAP_VER_COMP_ERR 		  (UINT32)0x00000008 		/*GAL通用信息包中电子地图版本号比较失败*/
#define GAL_COMPARE_MSG_BDS_SN_ERR 		      (UINT32)0x00000010 	    /*GAL通用信息包中本方、对方及收到上一条消息时本方序列号错误*/
#define GAL_COMPARE_MSG_SN_ERR 		          (UINT32)0x00000020 	    /*GAL通用信息包中对方消息序列号及收到上一条消息时本方序列号错误*/
#define GAL_COMPARE_PROTOCOL_VER_ERR 		  (UINT32)0x00000040 		/*GAL通用信息包中协议版本号错误*/
#define GAL_COMPARE_APP_LEN_ERR 			  (UINT32)0x00000080 		/*GAL通用信息包中应用层数据长度无效*/
#define GAL_COMPARE_COM_PERIOD (UINT32)0x00000100/*GAL通用信息包中通信周期错误*/

#ifdef __cplusplus
extern "C" {
#endif

	/*通用报文头*/
	typedef struct S_GALInfoStruct
	{
		UINT16 devInterInfoType;    /*设备接口信息类型*/
		UINT16 totalDataLen;		/*应用报文总长度*/
		UINT16 comPeriod;			/*通信周期，单位：ms*/
		UINT8 prtclVer;				/*协议版本号*/
		UINT32 sourceId;            /*源ID，发送方标识信息*/
		UINT32 destId;              /*目的ID，接收方标识信息*/
		UINT32 emapVerCrc;			/*电子地图版本校验信息32位CRC*/
		UINT32 crntOwnMsgSN;		/*本方消息序列号，记录发送本条消息时，本方的周期计数，发送方每周期将本计数加1*/	
		UINT32 recvOppMsgSN;		/*对方消息序列号，记录收到对方上一条消息中的对方消息序列号*/
		UINT32 preOwnMsgSN;			/*收到上一条消息时本方序列号，记录收到对方上一条消息时，本方的周期计数*/		
	} GALInfoStruct;

	/*************************************************************************************************************
	* 功能描述：收到新通用消息包时,计算该消息的剩余有效期
	* 输入参数：
	const UINT16 maxMsgValidTime	项目约定的该消息有效期 ;
	const UINT32 CurrentOwnSN 本方的当前周期号
	const UINT32 SNPeriod 本方的通信周期，在CI系统中，认为是应用的周期
	const HLHTGALInfoStruct *pRecvCommonPackage  通用消息包包头
	const UINT32 srcRecvCiMsgSN,对方上次收到的本方的序号
	const UINT32 srcCrntOwnMsgSN,对方发送最近数据时其当前的序号
	const UINT32 srcPreOwnMsgSN,对方上次收到本方数据时其当时的序号
	const UINT32 srcPeriod,对方的通信周期
	* 输出参数：无.
	* 返回值：RemainValidity  该消息的剩余有效期
	*************************************************************************************************************/
	INT32 HLHTCommonMsgRemainingValidity(const UINT32 MaxMsgValidTime, const UINT32 CurrentOwnSN, const UINT32 SNPeriod, const UINT32 srcRecvCiMsgSN, const UINT32 srcCrntOwnMsgSN, const UINT32 srcPreOwnMsgSN, const UINT32 srcPeriod);

	/*************************************************************************
	* 功能描述:    接收的CI通用信息包包头信息合法性检查
	* 输入参数:   UINT16 LocalDevName  发送方设备名称
	*			  UINT16 RemoteDevName  接收方设备名称
	*			  UINT16 DevInterfaceType  设备与设备接口类型
	*             UINT8 prtrlVer        接口协议版本号
	*			  HLHTGALInfoStruct *pMICommonPackage  CI发送给VOBC的通用信息包包头
	*             UINT16 logId        VOBC 1端或2端标识
	* 输入输出参数:   UINT32 *pErrorCode  CI接口中的故障号.
	* 输出参数:    无.
	* 全局变量:         ..
	* 返回值:
		HLHT_HEAD_SUCCESS (0x55) 互联互通头检查正确
		HLHT_HEAD_ERROR (0xaa) 互联互通头检查错误
		HLHT_HEAD_INIT (0x33) 表明头是互联互通初始通信帧
	*************************************************************************/
	UINT8 HLHTRecvCommonPacketHeadCheck(UINT16 LocalDevName, UINT16 RemoteDevName, UINT16 DevInterfaceType, UINT8 prtrlVer, GALInfoStruct *pMICommonPackage, UINT32 *pErrorCode,UINT16 logId);

#ifdef __cplusplus
}
#endif

#endif
