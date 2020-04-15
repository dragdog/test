/********************************************************
*                                                                                                            
* 文 件 名： ParsePackVobcData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： Vobc数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 



#include "ParsePackTmcData.h"
#include "../HLHTMsgHead/HLHTHeadContexManage.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../HLHTMsgHead/HLHTCommonDataPacketHead.h"
#include "CommonMemory.h"
#include "../QueryFiles/dquCIInterFace.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "TrainIsVT.h"
#include "TrainIsLCT.h"
#include "DataConfigure.h"


/*
* 功能描述： 解析处理TMC-OC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*			 UINT8* saveGalHead,通用报文头是否保留,1:保留，0:不保留
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功
*/
static UINT8 ParseTmcToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, UINT8* saveGalHead);

#define MAX_BAD_ENSURETIME	(10 * 1000)
/*按车站编号映射
  即stationId == 1,则映射到platformToCiDataStru[1]中
*/
extern PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX];

/*屏蔽门在PlatformToCiDataStruct结构中的位置:Psd映射为行，Psd所处车站映射为第一列，所处PsdToCiDataStruct位置映射为第2列
  即屏蔽门1在platformToCiDataStru中的位置为:所属车站为PsdPlatformIndex[1][0] ,在屏蔽门的结构中位置为PsdPlatformIndex[1][1]
*/
extern UINT8 PsdPlatformIndex[PSD_SUM_MAX][2]; 

GALInfoStruct GalHead = { 0 };
UINT16 RecvTmcLCTArray[TRAIN_MAX];/* 从TMC收到的 丢失通信车ID列表 */

/*
* 功能描述： 获取tmc与oc通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： tmc与oc通信中断确认时间单位毫秒
*/
UINT32 GetTmcAndOcTransmitBadEnsureTime(void)
{
	return MAX_BAD_ENSURETIME;
}

/*
* 功能描述： 解析处理TMC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseTmcToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId)
{					
	UINT8 ii = 0;                                /*索引管理*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;						 /*线路标识*/
	UINT8 galHeadSaved = 0;

	retVal = ParseTmcToOcDataFrame(dataBuf, dataLength, systemId, &galHeadSaved);

	if (0 == galHeadSaved)
	{
		ClearHlhtHeadCtx(CI_SYSTEM_TYPE_TMC, systemId);
	}

	return retVal;
}

/* 功能描述： 解析处理TMCC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*			 UINT8* saveGalHead,通用报文头是否保留,1:保留，0:不保留
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功
*/
static UINT8 ParseTmcToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, UINT8* saveGalHead)
{
	UINT8 ii = 0;                                /*索引管理*/
	UINT8 jj = 0;
	UINT8 retVal = CI_ERROR;
	UINT8 retValue = 0;
	UINT16 appInfoType = 0;						 /*应用信息类型*/
	UINT32 curntCycleNo = 0;					/*当前周期号*/
	UINT16 tmcName = 0;						/*设备名称：类型+ID*/
	UINT16 tmcId = 0;
	UINT16 ocName = 0;							/*OC设备名称：类型+ID*/
	UINT8 localOcId = GetSystemParaLocalOcId();/*联锁ID*/
	UINT32 errCode = 0;
	INT32 tmcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*互联互通检查结果*/
	UINT8 i= 0;
	UINT8 j= 0;
	UINT8 UCTLineNum = DEFAULT_ZERO;
	UINT16 wTSRIndex = DEFAULT_ZERO;
	UINT16 wRegionBlockIndex = DEFAULT_ZERO;
	UINT16 N_COMMAND = DEFAULT_ZERO;        /*命令总数*/
	UINT16 wTMClockIndex = DEFAULT_ZERO;   /*TMC锁命令数组下标*/
	UINT16 wTMCSwitchCmdIndex = DEFAULT_ZERO;		/*TMC道岔命令数组下标*/
	UINT16 wMultSwitchIndex = DEFAULT_ZERO;      /*交叉渡线命令数组下标*/
	UINT16 wSignalCtrlIndex = DEFAULT_ZERO; /*信号机命令数组下标*/
	UINT16 wPSDIndex=DEFAULT_ZERO;          /*PSD命令数组下标*/
	UINT16 temLCTNum = DEFAULT_ZERO;	/* 丢失通信车数量 */
	UINT16 wLCTIndex = DEFAULT_ZERO;	/* 丢失通信车数组下标 */
	UINT16 wUCTIinkIndex = DEFAULT_ZERO;/* 非通信车数组下标 */
	UINT32 ComandPara = DEFAULT_ZERO;/* 命令参数解析 */
	UINT16 ComandParaU16 = DEFAULT_ZERO;/* 命令参数解析 */
	UINT16 SwitchId = DEFAULT_ZERO;/* 对象ID */

	wTSRIndex = g_stOCRevCmdDataStru.CommandNum_TSRCtrl;
	wRegionBlockIndex = g_stOCRevCmdDataStru.CommandNum_RegionBlock;
	wTMClockIndex = g_stOCRevCmdDataStru.CommandNum_TMCLock;
	wTMCSwitchCmdIndex = g_stOCRevCmdDataStru.CommandNum_Switch;
	wMultSwitchIndex = g_stOCRevCmdDataStru.CommandNum_MultSwitch;

	tmcName = (CI_SYSTEM_TYPE_TMC)*256 + systemId;
	ocName = (CI_SYSTEM_TYPE_OC)*256 + localOcId;
	tmcId = (CI_SYSTEM_TYPE_TMC)*256 + systemId;

	/*解析通用消息包头*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);/* 接口信息类型 */
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);/* 发送方标识信息 源ID */
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);/* 接收方标识信息 目的ID */
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);/* 电子地图版本校验信息 */
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);/* 本方消息序列号 */
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);/* 通讯周期 */
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);/* 对方消息序列号 */
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);/* 收到上一条消息时本方序列号 */
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];/* 协议版本号 */

	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);/* 应用层数据长度 */
	ii += 2;

	curntCycleNo = g_dwOCCycle;
	if (FLAG_UNSET == g_stOC.TMCInputFlag)
	{
		g_stOC.TMCInputFlag = FLAG_SET;
	}
	/*（通用消息包头校验通过 && 报文生存时间有效）才继续解析其他信息*/
	/*第一次收到TMC消息时校验不通过，只记录curntOwnMsg以及当前本方软件周期号*/
	headRet = HLHTRecvCommonPacketHeadCheck(tmcName, ocName, OC_TMC_INTERFACE_INFOTYPE, OC_TMC_PRTCL_VER, &GalHead, &errCode, 0);

	if (HLHT_HEAD_SUCCESS == headRet)
	{
		/*计算消息的时效性*/
		tmcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetTmcAndOcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (tmcInfoRemainValidiy > 0)
		{
			/*解析应用数据*/
			debug_infor_printf("\n\nTMC_RECV:");
			/*更新接收到上一条消息时的本方周期号*/
			ii += 2;/*跳过应用报文长度2字节*/
			appInfoType = ShortFromChar(&dataBuf[ii]);/*应用信息类型，控制信息或者心跳帧信息*/
			debug_infor_printf("\nInfoType:%02x", appInfoType);
			ii += 2;
			ii += 2;/*跳过两个预留字节*/

			if (TMC_OC_INFO_TYPE_CONTROL == appInfoType)
			{
				/* 收到的丢失通车数量 */
				temLCTNum = dataBuf[ii++];
				debug_infor_printf("\nLComTrainNum:%d", temLCTNum);

				/* 把丢失通信列车从丢失通信列车列表里删除 */
				for(wLCTIndex = 0; wLCTIndex < temLCTNum; wLCTIndex++)
				{
					RecvTmcLCTArray[wLCTIndex] = ShortFromChar(&dataBuf[ii]);
					ii += 2;
					for (i = 0; i < TRAIN_MAX; i++)
					{
						if ((g_stOC.TrainInfoFlagArray[i] != 0) && (RecvTmcLCTArray[wLCTIndex] == g_stOC.TrainInfoArray[i].ID_CT))		/*列表中存在该车且列车信息有效*/
						{
							if (g_stOC.TrainCommStateArray[i])
							{
								/*该车在TMC处理过程中恢复了通信，则不作处理*/
							}
							else
							{
								g_stOC.TrainInfoFlagArray[i] = 0;		/*从列表中删除该车信息*/
								debug_infor_printf("\nLComTrain_To_UnComTrain_Id:%d.", g_stOC.TrainInfoArray[i].ID_CT);
							}
						}
					}
				}
				
				UCTLineNum = dataBuf[ii++];/* 非通信车数量 */
				g_stOC.UCTLineNum = UCTLineNum;
				debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

				for (i = 0; i < UCTLineNum; i++)
				{
					g_stOC.UCTLineInfoArray[i].ID = ShortFromChar(&dataBuf[ii]);/*路径归属列车ID*/
					ii += 2;
					debug_infor_printf("\nUnComTrainID:%d", g_stOC.UCTLineNum);

					//g_stOC.UCTLineInfoArray[i].DevPos = dataBuf[ii++];/*是否在OC管辖范围 0x55：在OC管辖范围内 0xaa：不在OC管辖范围内*/
					g_stOC.UCTLineInfoArray[i].UCTLineDir = dataBuf[ii++];/*路径方向*/
					g_stOC.UCTLineInfoArray[i].UCTLineHeadLink = ShortFromChar(&dataBuf[ii]);/*起点位置*/
					ii += 2;
					g_stOC.UCTLineInfoArray[i].UCTLineHeadOffset = LongFromChar(&dataBuf[ii]);
					ii += 4;
					g_stOC.UCTLineInfoArray[i].UCTLineTailLink = ShortFromChar(&dataBuf[ii]);/*终点位置*/
					ii += 2;
					g_stOC.UCTLineInfoArray[i].UCTLineTailOffset = LongFromChar(&dataBuf[ii]);
					ii += 4;

					g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum = dataBuf[ii++];/*非通信车锁闭路径包含的link数量*/
					debug_infor_printf("\nUnComTrainLinkListNum:%d", g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum);

					for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum; wUCTIinkIndex++)
					{
						g_stOC.UCTLineInfoArray[i].UCTLineLinkID[wUCTIinkIndex] = ShortFromChar(&dataBuf[ii]);/*非通信车锁闭路径包含的Link序列*/
						ii += 2;
						debug_infor_printf("\nUnComTrainLinkID:%d", g_stOC.UCTLineInfoArray[i].UCTLineLinkID[wUCTIinkIndex]);
					}
				}				
				g_stOCRevCmdDataStru.OCRevTSRCtrlCmdDataArray[wTSRIndex].TSRFlag = dataBuf[ii++];/*临时限速标志位 0x55有效 0xaa无效*/
				
				g_stOC.TMCEmapCheckVer = LongFromChar(&dataBuf[ii]);/*TMC电子地图校验码*/
				ii += 4;

				g_stOC.TSRNum = dataBuf[ii++];/*临时限速数量*/
				debug_infor_printf("\nTSRNum:%d", g_stOC.TSRNum);
				for (i = 0 ; i < g_stOC.TSRNum; i++)
				{					
					g_stOC.TSRDataArray[i].TSRSpeed = dataBuf[ii++];/*本临时限速值*/
					g_stOC.TSRDataArray[i].TSRSectionNum = dataBuf[ii++];/*本临时限速包含区段数*/
					for(j = 0; j < g_stOC.TSRDataArray[i].TSRSectionNum; j++)
					{
						g_stOC.TSRDataArray[i].TSRSectionIdBuf[j] = ShortFromChar(&dataBuf[ii]);/*临时限速包含区段ID*/
						ii += 2;
					}
				}

				/*命令总数*/
				N_COMMAND = dataBuf[ii++];
				debug_infor_printf("\nCmdNum:%d", N_COMMAND);
				for (i = 0; i < N_COMMAND; i++)
				{
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
					debug_infor_printf("\nCmd_%d:", i); 

					switch(dataBuf[ii++])/*命令类型*/
					{
						case TMC_CMD_REGION_BLOCK:/*OC封锁命令*/
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							ii += 2;/*跳过命令对象*/
							ComandPara = LongFromChar(&dataBuf[ii]);/* 0x01:全线封锁 0x02:上行封锁 0x03:下行封锁 */
							debug_infor_printf(" Cmd:%02x", ComandPara); 
							ii += 4;	
							if(TMC_CMD_REGION_BLOCK_ALL == ComandPara)/* 0x01:全线封锁 */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xff;
							}
							else if(TMC_CMD_REGION_BLOCK_UP == ComandPara)/* 0x02:上行封锁 */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0x55;
							}
							else if(TMC_CMD_REGION_BLOCK_DOWN == ComandPara)/* 0x03:下行封锁 */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xaa;
							}
							else 
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_RELIEVE;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xff;
							}
							wRegionBlockIndex++;
							break;

						case TMC_CMD_SIGNAL_CTRL :/*信号机点灯回复命令*/
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							ii += 2;/*跳过命令对象*/
							ii += 4;
							break;

						case TMC_CMD_LOCK :/*锁命令*/  	
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							SwitchId = ShortFromChar(&dataBuf[ii]);/*命令对象*/
							ii += 2;
							debug_infor_printf(" SwitchId:%02x", SwitchId); 

							/* 0x01-删除道岔锁 0x05-申请共享定位锁 0x06-申请共享反位锁 0x07-申请独占定位锁 
								0x08-申请独占反位锁 0x09-申请交叉分区锁 0x0A-删除交叉分区锁 */
							ComandParaU16 = ShortFromChar(&dataBuf[ii]);
							ii += 2;
							debug_infor_printf(" Cmd:%02x", ComandParaU16); 

							switch(ComandParaU16)
							{
							case TMC_CMD_LOCK_DELETE_ALL:/* 删除道岔锁 */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* 控制对象的ID */
								ii += 2; 
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = DEL_LOCK;
								//g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								//g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_SLOCK_MAIN:/* 申请共享定位锁 */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_SLOCK_SIDE:/* 申请共享反位锁 */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_ELOCK_MAIN:/* 申请独占定位锁 */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wTMCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* 控制对象的ID */
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SwitchID = SwitchId;
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
								wTMClockIndex++;
								wTMCSwitchCmdIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_ELOCK_SIDE:/* 申请独占反位锁 */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wTMCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* 控制对象的ID */
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SwitchID = SwitchId;
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
								wTMClockIndex++;
								wTMCSwitchCmdIndex++;
								break;

							case TMC_CMD_APPLY_MULTSWITCHLOCK_1:/* 申请1号位交叉分区锁 */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_DELETE_MULTSWITCHLOCK_1:/* 删除1号位交叉分区锁 */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_APPLY_MULTSWITCHLOCK_2:/* 申请2号位交叉分区锁 */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_DELETE_MULTSWITCHLOCK_2:/* 删除2号位交叉分区锁 */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* 控制对象的ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							}
							debug_infor_printf(" SourceID:%02x", ShortFromChar(&dataBuf[ii-2])); 
							
							break;
													
						default:
							break;
					}		
				}

				g_stOCRevCmdDataStru.CommandNum_TSRCtrl = wTSRIndex;
				g_stOCRevCmdDataStru.CommandNum_RegionBlock = wRegionBlockIndex;
				g_stOCRevCmdDataStru.CommandNum_Switch = wTMCSwitchCmdIndex;
				g_stOCRevCmdDataStru.CommandNum_TMCLock = wTMClockIndex;
				g_stOCRevCmdDataStru.CommandNum_MultSwitch = wMultSwitchIndex;

				retVal = CI_SUCCESS;
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*校验数据长度*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*数据经过校验，需要存储通用报文上下文*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_TMC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, systemId, curntCycleNo);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*判断序号大于0，因为把0当做无效序号*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_TMC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, systemId, curntCycleNo);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}


/*
* 功能描述： 组TMC数据
* 参数说明： QueueStruct* OcToTmcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackOcToTmcData(QueueStruct* ocToTmcDataQueue)
{
	UINT8 OcToTmcDataBuffer[TMC_TO_CI_DATA_LEN_MAX] = {0};/*TMC到OC组帧数组*/
	UINT16 ii = 0;
	UINT8 kk = 0;
	UINT8  k = 0;                                            /*计数*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT8 retVal = CI_SUCCESS;
	UINT8 localOcId = GetSystemParaLocalOcId();  /*本地联锁ID*/
	INT32 tmcInfoRemainValidiy = 0;
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT16 temAppDatalen = 0;
	UINT32 TmcMsgSn = 0;
	UINT32 OcMsgSn = 0;
	UINT8 wSwitchIndex = 0;			/*道岔下标*/
	UINT8 wSignalIndex = 0;			/*信号机下标*/
	UINT16 wSLIndex = 0;            /*共享锁信息下标*/
	UINT16 wULIndex = 0;            /*交叉渡线锁信息下标*/
	UINT8 wPsdIndex = 0;            /*屏蔽门下标*/
	UINT8 wEmpIndex = 0;            /*紧急停车按钮下标*/
	UINT8 wLsbIndex = 0;            /*离站按钮下标*/
	UINT8 wAsbIndex = 0;            /*到站按钮下标*/
	UINT8 wMultSwitchIndex = 0;     /*交叉渡线下标*/
	UINT16 wCTIndex = 0;            /*通信列车信息赋值下标*/
	UINT16 wUCTIndex = 0;           /*非通信列车信息赋值下标*/
	UINT16 wUCTIinkIndex = 0;		/*非通信车数组下标*/
	UINT8 wTSRNumIndex = 0;			/*临时限速总数临时下标*/
	UINT8 wTSRSectionIndex = 0;		/*临时限速区段Id临时下标*/
	UINT8 wVTNum, wLCTNum;
	UINT16 tmpVTNumOutIndex, tmpLCTNumOutIndex, tmpUCTNumOutIndex;	/*VT车数量、LCT车数量、UCT车数量及使用的临时索引*/
	kcg_bool bIsUCT;
	kcg_uint8 bUCTType;
	UINT32 curntCycleNo = 0;		/*当前周期号*/

	curntCycleNo = g_dwOCCycle;
	/*调试写死校验信息*/
	emapCheckVer = 0;

	if (ocToTmcDataQueue != NULL)
	{
#ifdef PLATFORM_2OO2
		/*通用协议帧头*/
		ii += 2;
		OcToTmcDataBuffer[ii++] = CI_SYSTEM_TYPE_TMC;	/*目的逻辑类型*/
		OcToTmcDataBuffer[ii++] = 1;					/*目的逻辑ID*/
		ShortToChar(0, &OcToTmcDataBuffer[ii]);		/*互联互通4IP VOBC1端或2端标识*/
		ii += 2;
#endif
	
		debug_infor_printf("\n\nTMC_SEND:");
		/*计算消息的时效性*/
		tmcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetTmcAndOcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (tmcInfoRemainValidiy == 0)
		{
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, TMC_ID, UINT32_MAX);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, TMC_ID, UINT32_MAX);
			debug_infor_printf("\n\nTMC Lost Communication !!!\n\n");
		}

		/*开始组应用数据帧*/
		ShortToChar(OC_TMC_INTERFACE_INFOTYPE, &OcToTmcDataBuffer[ii]);/*接口信息类型*/
		ii += 2;
		ShortToChar(0, &OcToTmcDataBuffer[ii]);/*发送方标识信息 高2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_OC)*256 + localOcId, &OcToTmcDataBuffer[ii]);/*发送方标识信息 低2*/
		ii += 2;
		ShortToChar(0, &OcToTmcDataBuffer[ii]);/*接收方标识信息 高2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_TMC)*256 + 1, &OcToTmcDataBuffer[ii]);/*接收方标识信息 低2*/
		ii += 2;
		LongToChar(emapCheckVer, &OcToTmcDataBuffer[ii]);/*电子地图版本校验信息*/
		ii += 4;
		LongToChar(curCycleNum, &OcToTmcDataBuffer[ii]);/*本方消息序列号*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &OcToTmcDataBuffer[ii]);/*设备通讯周期*/
		ii += 2;
		TmcMsgSn = GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, TMC_ID);
		LongToChar(TmcMsgSn, &OcToTmcDataBuffer[ii]);/*对方消息序列号：recvOppMsgSN记录收到对方上一条消息中的对方消息序列号*/
		ii += 4;
		OcMsgSn = GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, TMC_ID);
		LongToChar(OcMsgSn, &OcToTmcDataBuffer[ii]);/*收到上一条消息时本方序列号*/
		ii += 4;
		OcToTmcDataBuffer[ii++] = OC_TMC_PRTCL_VER;/*OC_TMC接口协议版本号*/

		temAppDatalen = ii;
		ii += 2;/*通用报文头中的应用层报文总长度*/

		if (CI_SUCCESS == retVal)
		{
			/*以下为应用层数据*/
			ii += 2;/*报文长度*/

			ShortToChar(OC_TMC_INFO_TYPE_CONTROL, &OcToTmcDataBuffer[ii]);/* 应用报文类型 TMC—>OC */
			ii += 2;
			ii += 2;/*预留两个字节*/
			
			/*只有当TMC发送的信息类型是控制信息时，才组包如下内容*/
			/*道岔信息*/
			OcToTmcDataBuffer[ii++] = (UINT8)g_stOC.SwitchNum;/* 道岔数量 0~255 */
			debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);

			for(wSwitchIndex=0; wSwitchIndex<g_stOC.SwitchNum; wSwitchIndex++)
			{
				ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &OcToTmcDataBuffer[ii]); /*道岔ID*/
				ii += 2;
				debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

				/*道岔位置*/
				switch(g_stOC.SwitchStateArray[wSwitchIndex].Position)
				{
				case SWTICH_POSITION_MAIN:
					OcToTmcDataBuffer[ii++] = 0x55;
					break;
				case SWTICH_POSITION_SIDE:
					OcToTmcDataBuffer[ii++] = 0xaa;
					break;
				case SWITCH_POSITION_SIKAI:
					OcToTmcDataBuffer[ii++] = 0x00;
					break;
				case SWITCH_POSITION_MOVE:
					OcToTmcDataBuffer[ii++] = 0x00;
					break;
				case SWITCH_POSITION_JICHA:
					OcToTmcDataBuffer[ii++] = 0xff;
					break;
				default :
					OcToTmcDataBuffer[ii++] = 0xff;
					break;
				}
				debug_infor_printf("  Pos:%x", OcToTmcDataBuffer[ii-1]);

				/*道岔故障状态*/
				OcToTmcDataBuffer[ii++] = 0xaa;/*0x55-故障 0xaa-正常*/
				debug_infor_printf("  FaultSta:%x", OcToTmcDataBuffer[ii-1]);

				/*锁类型*/
				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if(LOCK_MAIN == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x01;/*共享定*/
					}
					else if(LOCK_SIDE == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x02;/*共享反*/
					}
					else
					{
						OcToTmcDataBuffer[ii++] = 0x05;
					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if (LOCK_MAIN == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x03;/*独占定*/
					}
					else if (LOCK_SIDE == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x04;/*独占反*/
					}
					else
					{
						OcToTmcDataBuffer[ii++] = 0x05;
					}
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0x05;/*没有锁*/
				}
				debug_infor_printf("  LockType:%x", OcToTmcDataBuffer[ii-1]);

				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])/* 有共享锁 */
				{
					/*共享锁数量*/
					OcToTmcDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
					debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
					/*共享锁信息*/
					k = 0;
					for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)/* 遍历一遍所有的可能存在的共享锁 */
					{
						if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
						{
							k++;    /*共享锁计数*/
							/*共享锁归属ID，包含类型信息*/
							ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId, &OcToTmcDataBuffer[ii]);
							ii += 2;
							debug_infor_printf("  SLockId%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						}
					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					/*独占锁数量*/
					OcToTmcDataBuffer[ii++] = 1;
					debug_infor_printf("  ELockNum:%d", 1);
					/*独占锁归属ID,此ID包含类型*/
					ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ELockId:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0;
				}
			}

			/*OC管理区域封锁信息*/
			if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
			{
				OcToTmcDataBuffer[ii++] = 0xcc;/*0x55-上行 0xaa-下行 0xcc-全线 0xff-未封锁*/
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
			{
				OcToTmcDataBuffer[ii++] = 0x55;
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
			{
				OcToTmcDataBuffer[ii++] = 0xaa;
			}
			else
			{
				OcToTmcDataBuffer[ii++] = 0xff;
			}
			debug_infor_printf("\nRegionBlockFlag:%x", OcToTmcDataBuffer[ii-1]);

			/* OC倒切状态 */
			if(MODE_VBTC == g_stOC.SysRunMode)
			{
				OcToTmcDataBuffer[ii++] = MODE_VBTC;/* VBTC模式 */
			}
			else if(MODE_SHADOW == g_stOC.SysRunMode)
			{
				OcToTmcDataBuffer[ii++] = MODE_SHADOW;/* 影子模式 */
			}
			else
			{
				OcToTmcDataBuffer[ii++] = 0xff;/* 无效 */
			}
			debug_infor_printf("\nSysRunMode:%x", g_stOC.SysRunMode);

			/*通信列车数量*/
			tmpVTNumOutIndex = ii;/* 记录下该变量在数据包中所占的位置 */
			ii += 1;
			wVTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				/*车信息有效并且与OC处于通信状态*/
				if (TrainIsVT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* 列车ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nComTrainID:%02x%02x", OcToTmcDataBuffer[ii-2],OcToTmcDataBuffer[ii-1]);

					/* 列车是否在本OC管辖范围内 */
					if(IN_MANAGE_AREA == g_stOC.TrainInfoArray[wCTIndex].DevPos)
					{
						OcToTmcDataBuffer[ii++] = IN_MANAGE_AREA;/* 0x55：在OC管辖范围内 */
					}
					else
					{
						OcToTmcDataBuffer[ii++] = NOT_IN_MANAGE_AREA;/* 0xaa：不在OC管辖范围内 */
					}
					debug_infor_printf("  DevPos:%x", g_stOC.TrainInfoArray[wCTIndex].DevPos);

					/* 列车运行级别 01：UR 02:VBTC, 其他无效*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].TrainLevel;
					debug_infor_printf("  TrainLevel:%x", g_stOC.TrainInfoArray[wCTIndex].TrainLevel);

					/*列车驾驶模式：AM模式:0x01 CM模式:0x02 RM模式:0x03 EUM模式:0x04 RD模式:0x05 FAM模式:0x06 （预留） SHD模式:0x07 （影子模式，仅迪士尼使用）默认值：0xFF；其它：非法*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].TrainMode;
					debug_infor_printf("  TrainMode:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMode);

					/* IVOC应用周期 */ 
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SN, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  IVOC_SN:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SN);

					/* 列车最大安全前端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* 列车最大安全前端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* 列车最小安全后端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* 列车最小安全后端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* 最小安全后端指向最大安全前端的方向，以最小安全后端处的上下行方向确定 */
					/* 0x55:列车运行方向为上行; 0xaa:列车运行方向为下行; 0xff:无效 */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* 列车速度方向	0x55:向前 0xAA:向后 */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* 列车车速（cm/s） */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);

					/* MA起点Link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK);

					/* MA起点Link偏移量 (单位cm) */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET);

					/* MA终点Link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK);

					/* MA终点Link偏移量 (单位cm) */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET);

					/* MA方向 */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR;

					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR);

					wVTNum++;
				}
			}
			OcToTmcDataBuffer[tmpVTNumOutIndex] = wVTNum;/* 赋值通信列车数量 */
			debug_infor_printf("\nComTrainNum:%d", wVTNum);

			/*丢失通信列车数量*/
			tmpLCTNumOutIndex = ii;/* 记录下该变量在数据包中所占的位置 */
			ii += 1;
			wLCTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				if (TrainIsLCT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* 丢失通信列车的ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 + (UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nLComTrainID:%02x%02x", OcToTmcDataBuffer[ii-2], OcToTmcDataBuffer[ii-1]);

					/*是否在OC管辖范围*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].DevPos;
					debug_infor_printf("  DevPos:%x", g_stOC.TrainInfoArray[wCTIndex].DevPos);

					/* 列车最大安全前端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* 列车最大安全前端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* 列车最小安全后端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* 列车最小安全后端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* 最小安全后端指向最大安全前端的方向，以最小安全后端处的上下行方向确定 */
					/* 0x55:列车运行方向为上行 */
					/* 0xaa:列车运行方向为下行 */
					/* 0xff:无效 */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					wLCTNum++;
				}
			}
			OcToTmcDataBuffer[tmpLCTNumOutIndex] = wLCTNum;
			debug_infor_printf("\nLComTrainNum:%d", wLCTNum);

			/*非通信列车数量*/
			OcToTmcDataBuffer[ii] = g_stOC.UCTLineNum;
			ii += 1;
			debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

			for (wUCTIndex = 0; wUCTIndex < g_stOC.UCTLineNum; wUCTIndex++)
			{
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].ID, &OcToTmcDataBuffer[ii]);/*列车ID*/
				ii += 2;
				debug_infor_printf("\nUnComTrain ID:%02x%02x", OcToTmcDataBuffer[ii-2],OcToTmcDataBuffer[ii-1]);

				//OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].DevPos;/*是否在OC管辖范围 0x55：在OC管辖范围内	0xaa：不在OC管辖范围内*/
				OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineDir;/*路径方向*/
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineHeadLink, &OcToTmcDataBuffer[ii]);/*起点位置*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineHeadOffset, &OcToTmcDataBuffer[ii]);
				ii += 4;
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineTailLink, &OcToTmcDataBuffer[ii]);/*终点位置*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineTailOffset, &OcToTmcDataBuffer[ii]);
				ii += 4;

				OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum;/*非通信车锁闭路径包含的link数量*/
				debug_infor_printf(" LinkListNum:%d", g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum);

				for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum; wUCTIinkIndex++)
				{
					ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkID[wUCTIinkIndex], &OcToTmcDataBuffer[ii]);/*非通信车锁闭路径包含的Link序列*/
					ii += 2;
					debug_infor_printf(" LinkID:%d", g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkID[wUCTIinkIndex]);
				}
			}

			/*交叉渡线数量*/
			OcToTmcDataBuffer[ii++] = (UINT8)g_stOC.MultSwitchNum;
			debug_infor_printf("\nMultSwitchNum:%d", g_stOC.MultSwitchNum);
			for (wMultSwitchIndex=0;wMultSwitchIndex<g_stOC.MultSwitchNum;wMultSwitchIndex++)
			{
				/*交叉渡线ID*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id;
				debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*交叉渡线锁状态*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;
				debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*交叉渡线资源锁数量*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
				debug_infor_printf("  ULockNum:%d", OcToTmcDataBuffer[ii-1]);
				
				/*交叉渡线资源锁归属ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[0])
				{
					for(wULIndex = 0; wULIndex < g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum; wULIndex++)
					{
						ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &OcToTmcDataBuffer[ii]);
						debug_infor_printf("  ULockId:%02x", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
						ii += 2;
					}
				}
			}

			/*信号机数量*/
			OcToTmcDataBuffer[ii++] = g_stOC.SignalNum;
			debug_infor_printf("\nSignalNum:%d", g_stOC.SignalNum);
			for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
			{
				/*信号机ID*/
				ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id, &OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nSignal Id:%d", g_stOC.SignalStateArray[wSignalIndex].Id);

				/*信号机颜色*/
				if (SIGNAL_RED == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToTmcDataBuffer[ii++] = 0xaa;/* 信号机亮红灯 */
				} 
				else if (SIGNAL_GREEN == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToTmcDataBuffer[ii++] = 0x55;/* 信号机亮绿灯 */
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0xaa;/* 信号机亮红灯 */
				}
			}

			/*屏蔽门数量*/
			OcToTmcDataBuffer[ii++] = g_stOC.PsdNum;
			debug_infor_printf("\nPsdNum:%d", g_stOC.PsdNum);

			for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
			{
				/*屏蔽门ID*/
				ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nPsd Id:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
				/*屏蔽门开关状态*/
				OcToTmcDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-打开 0xaa-关闭 0xff-未知*/
				debug_infor_printf("  State:%x", OcToTmcDataBuffer[ii-1]);
			}

			/*紧急停车按钮数量*/
			OcToTmcDataBuffer[ii++] = g_stOC.EmpNum;
			debug_infor_printf("\nEmpNum:%d", g_stOC.EmpNum);

			for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
			{
				/*紧急停车按钮ID*/
				ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id, &OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nEmp Id:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
				/*紧急停车按钮状态*/
				if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToTmcDataBuffer[ii++] = EMP_OFF;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				else if (EMP_ON == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToTmcDataBuffer[ii++] = EMP_ON;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0xff;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				debug_infor_printf(" State:%x", OcToTmcDataBuffer[ii-1]);
			}

			/*物理区段数量*/
			OcToTmcDataBuffer[ii++] = 0;

			/*入口检测点灯请求数量*/
			OcToTmcDataBuffer[ii++] = 0;

			//for(wSignalIndex = 0; wSignalIndex < g_stOC.SignalNum; g_stOC.SignalNum++)
			//{
			//	/* 申请列车ID */
			//	ShortToChar(g_stOC.SignalInfoArray[wSignalIndex].PreSourceId,&OcToTmcDataBuffer[ii]);
			//	ii += 2;

			//	/* 对应信号机ID */ 
			//	ShortToChar(g_stOC.SignalInfoArray[wSignalIndex].SourceId,&OcToTmcDataBuffer[ii]);
			//	ii += 2;

			//	/* 点灯状态 */
			//	OcToTmcDataBuffer[ii++] = g_stOC.SignalInfoArray[wSignalIndex].SignalExpState;

			//}

			if(g_stOC.TSRNum > 0)
			{
				/*TSR临时限速标志位 有效*/
				OcToTmcDataBuffer[ii++] = TSR_INFO_VALID;/*0xaa:临时限速信息无效 0x55:临时限速信息有效*/
			}
			else
			{
				/*TSR临时限速标志位 无效*/
				OcToTmcDataBuffer[ii++] = TSR_INFO_UNVALID;/*0xaa:临时限速信息无效 0x55:临时限速信息有效*/
			}
			debug_infor_printf("\nTSR Flag:%x", OcToTmcDataBuffer[ii-1]);

			/* TMC电子地图校验码 */
			LongToChar(g_stOC.TMCEmapCheckVer,&OcToTmcDataBuffer[ii]);
			ii += 4;

			/* TSR临时限速数量 */
			OcToTmcDataBuffer[ii++] = g_stOC.TSRNum;
			debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

			for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
			{
				/*TSR临时限速数值*/
				OcToTmcDataBuffer[ii++] = g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed;
				debug_infor_printf("\nTSR Speed:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed);

				/*TSR1临时限速区段数量*/
				OcToTmcDataBuffer[ii++] = g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum;
				debug_infor_printf(" SectionNum:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum);

				for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum; wTSRSectionIndex++)
				{
					/*TSR1限速区段Id*/
					ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex], &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf(" SectionIdBuf:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex]);
				}
			}		

			/*通用报文头中应用层报文总长度*/
			ShortToChar(ii - temAppDatalen - 2, &OcToTmcDataBuffer[temAppDatalen]);

			/*应用层数据的报文长度 */
			OcToTmcDataBuffer[37] = (UINT8) (((ii - 39) >> 8) & 0xff);
			OcToTmcDataBuffer[38] = (UINT8) ((ii - 39) & 0xff);

			/*数据长度 */
			OcToTmcDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
			OcToTmcDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);


			if (QueueWrite(ii, OcToTmcDataBuffer, ocToTmcDataQueue))
			{ /*写队列成功*/
	#ifdef LOGPRINT/*日志打印宏*/                 
	#ifdef PLATFORM_2OO2/*2oo2平台打印*/
				debug_infor_printf("\nTMC_SEND:%d", ii);
				//debug_out_array(0xAA, OcToTmcDataBuffer, ii);
				debug_out_array(0xAA, OcToTmcDataBuffer, 1);
	#endif/*平台打印结束*/
	#endif
			}
			else
			{
				/*写队列失败*/
				retVal = CI_ERROR;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
