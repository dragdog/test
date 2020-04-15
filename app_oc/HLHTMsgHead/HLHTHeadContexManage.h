/********************************************************
*
* 文 件 名： HLHTHeadContexManage.h
* 版权说明： 北京交控科技有限公司
* 创建时间： 2016-12-17
* 作    者： 联锁产品组
* 功能描述： 互联互通报文头上下文数据管理
* 函数包括：
* 使用注意：
* 修改记录：
*
********************************************************/

#ifndef _HLHT_HEAD_CONTEX_MANAGE_H_
#define _HLHT_HEAD_CONTEX_MANAGE_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*互联互通报文头上下文数据结构体*/
	typedef struct S_HLHTHeadContextStru
	{
		UINT16 sourceComPeriod;			/*向CI发送数据的一方的通信周期，单位：ms*/
		UINT8 sourceId;            /*向CI发送数据的一方的ID*/
		UINT16 logId;			/*向CI发送数据的一方的逻辑ID，目前仅互联互通4IP VOBC使用*/
		UINT32 sourceCrntOwnMsgSN;		/*向CI发送数据的一方发送报文时当前的SN*/
		UINT32 sourceRecvOppMsgSN;		/*向CI发送数据的一方上次收到CI数据时CI的SN*/
		UINT32 sourcePreOwnMsgSN;			/*向CI发送数据的一方上次收到CI数据时的SN*/

		UINT32 ciPreRecvOwnSn;/*CI接收到数据时本身的SN*/
	} HLHTHeadContextStru;
	/*
	* 功能描述:获取某个设备最近一次给CI发送数据时其当前的序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	某个设备最近一次给CI发送数据时其当前的序号,0为无效值
	*/
	UINT32 GetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId);
	/*
	* 功能描述:获取某个设备最近一次给CI发送数据时里面填写的上次收到CI数据其当时的序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	某个设备最近一次给CI发送数据时里面填写的上次收到CI数据其当时的序号,0为无效值
	*/
	UINT32 GetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId);
	/*
	* 功能描述:获取CI最近收到某个设备的数据时CI自身当时的序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	CI最近收到某个设备的数据时自身当时的序号,0为无效值
	*/
	UINT32 GetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId);
	/*
	* 功能描述:设置某个与CI通信的设备的通信周期
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 period:通信周期
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxComPeriod(const UINT8 devType, const UINT8 devId, const UINT16 period);
	/*
	* 功能描述:设置CI最近收到某个设备数据时通用头里面包含的对方当前的周期号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 srcCrntSN:对方当前的周期号
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId, const UINT32 srcCrntSN);
	/*
	* 功能描述:设置某个设备上一次收到CI数据时里面包含的CI当前的序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 srcOppSN:对方上一次收到CI数据时里面包含的CI当前的序号
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxSrcOppSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOppSN);
	/*
	* 功能描述:设置某个设备上一次收到CI数据时其自身的当前序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 srcOwnSN:某个设备上一次收到CI数据时其自身的当前序号
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOwnSN);
	/*
	* 功能描述:设置CI收到数据时CI自身的序号
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 ciRcvSn:CI收到数据时CI自身的序号
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId, const UINT32 ciRcvSn);
	/*
	* 功能描述:删除某个设备的互联互通头
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 ClearHlhtHeadCtx(const UINT8 devType, const UINT8 devId);
	/*
	* 功能描述： 处理互联互联互通通用报文头时效性
	* 参数说明：
	无
	* 返回值  ：
	无
	*/
	void ProcHlhtHeadCtxTimeOut(void);
	/*
	* 功能描述： 根据设备类型，获得具备初始通信互联互通头的所有设备ID
	* 参数说明：
	const UINT8 devType, 设备类型
	UINT8* devIdBuf, 存储满足条件的设备编号的数组
	const UINT8 devIdBufMaxLen, devIdBuf的容量，防止函数内访问越界
	UINT8* devSum,表明满足条件的设备编号的数量
	UINT16* devLogIdBuf 存储满足条件的设备逻辑ID的数组
	* 返回值  ：
	CI_SUCCESS:获得成功
	CI_ERROR:获得失败
	*/
	UINT8 GetHlhtInitHeadDevsByDevType(const UINT8 devType, UINT8* devIdBuf, const UINT8 devIdBufMaxLen, UINT8* devSum, UINT16* devLogIdBuf);

	/*
	* 功能描述:获取某个与CI通信的设备的互联互通报文头上下文信息是否存在并且有效
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	CI_ERROR:不存在
	*	CI_SUCCESS:存在
	*/
	UINT8 GetHLHTHeadCtxValid(const UINT8 devType, const UINT8 devId);
		/*
	* 功能描述:获取某个与CI通信的设备的互联互通报文头上下文信息是否存在
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	* 返回值:
	CI_ERROR:不存在
	*	CI_SUCCESS:存在
	*/
	UINT8 GetHLHTHeadCtxExistance(const UINT8 devType,const UINT8 devId);

	/*
	* 功能描述:设置CI收到数据时对方的LogId
	* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
	const UINT16 ciRcvLogId:CI收到数据时对方的LogId
	* 返回值:
	CI_SUCCESS:成功
	CI_ERROR:失败
	*/
	UINT8 SetHLHTHeadCtxCiRcvLogId(const UINT8 devType, const UINT8 devId, const UINT16 ciRcvLogId);
#ifdef __cplusplus
}
#endif

#endif
