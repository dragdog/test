/*
*
* 文件名   ：  ProtclManage.c
* 版权说明 ：  北京北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2013.09.15
* 作者     ：  王佩佩
* 功能描述 ：  协议主处理类,主要实现了周期性调用协议。
* 使用注意 ：
* 修改记录 ：
*    时间			修改人		修改理由
*    2014.03.06     yw.lou      发送数据如果没有发送报文,对应的链路状态没有赋值,则导致最终在刷新链路时候链路不一致;
*                               无法区别开到底是哪个协议,因此修改,如果没有中间状态数据,则赋值0,以便能正常处理;
*    2014.03.20     yw.lou      只为ATS通信开辟了1个空间,而ATS最多可能发送2或3帧,导致数组越界;修改分配空间:
*                               最大可能链路数*各协议通信对象个数;
*    2014.04.13     yw.lou      为协议数据接收结构体分配空间后未初始化,接收各协议数据后调用ReceiveAppData函数,proType字段未初始化
*                               导致其它未收到数据未初始化的协议类型清空了已收到数据的协议类型;
*                               malloc申请空间失败后均返回失败,在初始化阶段完成检查,避免随机故障;
*
*---------------------------------------------
*/
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ProtclManage.h"
#include "sfpPrtclCommon.h"
#include "dsuProtclFun.h"
#include "dsuRsrFunc.h"
#include "RsrCommon.h"
#include "protclMgeCfg.h"
#include "CommonMemory.h"
#include "Rssp2.h"
#include "PrtclParseManage.h"

/***********协议初始化相关函数**************************************************************/
static UINT8 InitProtclInfoStru(ProtclConfigInfoStru* pUnifyInfo, UINT8 maxLnkNum);
static UINT8 InitSfpProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRpProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRsspProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRsrProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRssp2ProtclStru(UINT8 *fileName, UINT8 typeNum);

/* 临时存放协议数据的结构体 */
PROTCL_RCV_DATA_TYPE* ptemProtclRcvDataStru = NULL;

/* 各协议函数指针配置表 */
PROTCL_MGECFG_STRU protclCfgStruBuff[PROTCL_MGECFG_MAXCNT + 1] = {
	{ protcl_nul_Init    ,  protcl_nul_Snd , protcl_nul_Rcv  ,protcl_nul_Lnk,protcl_nul_DelLnk,protcl_nul_ReFreshLnk,protcl_nul_End}
	,{ InitSfpProtclStru  ,  sfpSendData    , SfpRcvData     ,GetSfpLnkStatus,SfpDelLnk,SfpReFreshLnk,SfpFreeSpaceFunc}
	,{ InitRpProtclStru   ,  RpSendData     , RpRcvData      ,protcl_nul_Lnk,RpDelLnk,RpReFreshLnk,RpFreeSpaceFunc}
	,{ InitRsrProtclStru  ,  RsrSendData    , RsrRcvData     ,GetRsrLnkStatus,RsrDelLnk,RsrReFreshLnk,RsrFreeSpaceFunc}
	,{ InitRsspProtclStru ,  RsspSendData   , RsspRcvData    ,GetRsspLnkStatus,RsspDelLnk,RsspReFreshLnk,RsspFreeSpaceFunc}
	,{ InitRssp2ProtclStru ,  Rssp2SendData   , Rssp2RcvData    ,GetRssp2LnkStatus,Rssp2DelLnk,Rssp2ReFreshLnk,Rssp2FreeSpaceFunc}
};

ProtclConfigInfoStru *pProtclCfgInfo;/* 协议统一结构体指针，指向全局的变量，在protcl_SetProtclCfgStruPt函数中赋值 */
PROTCL_APP_TO_STRU   *pProtclAppToStru;

/* 发送相关信息结构体 */
PROTCL_SND_CFG_STRU protclSndCfgStru[PROTCL_MGECFG_MAXCNT + 1];

/* SFP协议统一结构体 */
static struc_Unify_Info *gpsfpUnitInfo = NULL;
/* RSR协议统一结构体 */
static RSR_INFO_STRU *gpRsrUnitInfo = NULL;
/* RSSP协议统一结构体 */
static RSSP_INFO_STRU *gpRsspUnitInfo = NULL;
/* 冗余层协议统一结构体 */
static RP_INFO_STRU *gpRedunUnitInfo = NULL;
/* RSSP2协议统一结构体 */
static RSSP2_INFO_STRU *gpRssp2UnitInfo = NULL;

/* 查询函数相关信息结构体 */
static DSU_PROTCL_TYPE_STRU protclTypeInfoStru;
/* 存放本方通信的相关协议个数 */
UINT8 pProtclNumBuff[PROTCL_MGECFG_MAXCNT + 1] = { 0 };
/* 本设备使用日志长度比率*/
UINT32 curDevRecordLenRatio[PROTCL_MGECFG_MAXCNT] = { 0 };
/* 本设备使用日志总长度*/
UINT32 curDevRecordTotalLen = 0;
/*协议适配层非宕机错误码*/
UINT32 gManageErrInfo = 0;
/* RSSP2协议分帧使用的协议包头 */
PRTCL_BASE_CONF Rssp2PrtclHead;

/*
* 从配置文件读取相对应的协议配置数据
*/
UINT8 Init(UINT8 *fileName, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retVal = 0;
	UINT8 tempProtclCnt = 0;
	UINT8 tmpI = 0;
	UINT8 flag = 0;
	UINT8 rslt = 0;
	F_INIT tmpInit;

	/* VOBC的记录数据大小 */
	if ((NULL == pUnifyInfo))
	{
		return retVal;
	}

	protcl_SetProtclCfgStruPt(pUnifyInfo);
	tempProtclCnt = InitGetCfgInfo(fileName, &pProtclNumBuff[1]);

	if (tempProtclCnt == 0)
	{
		retVal = 0;
		return retVal;
	}

	/* 给发送数据的临时存档数据的结构体分配空间 */
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		curDevRecordLenRatio[0] = SFP_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.sfpProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[0];
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		curDevRecordLenRatio[1] = RP_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.redunProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[1];
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		curDevRecordLenRatio[2] = RSR_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rsrProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[2];
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		curDevRecordLenRatio[3] = RSSPI_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rsspProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[3];
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		curDevRecordLenRatio[4] = RSSPII_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rssp2ProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[4];
	}

	/* 初始化各协议*/
	for (tmpI = 0;tmpI < tempProtclCnt + 1;tmpI++)
	{
		tmpInit = protclCfgStruBuff[tmpI].fInit;
		retVal = tmpInit(fileName, pProtclNumBuff[tmpI]);

		if (0 == retVal)
		{
			/* 若有一个初始化失败，将不再继续。 */
			return retVal;
		}
	}

	/* 给临时存放协议数据的结构体分配空间 */
	ptemProtclRcvDataStru = (PROTCL_RCV_DATA_TYPE*)malloc(sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1));

	/* 给接收数据的临时存档数据的结构体分配空间 */
	if (ptemProtclRcvDataStru != NULL)
	{
		/*初始化协议数据的结构体*/
		CommonMemSet(ptemProtclRcvDataStru, sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1), 0, sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1));

		/*申请并初始化协议数据结构体的接收数据缓冲区*/
		ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum);

		/*规定协议数据结构体第2个空间存放SFP数据类型,第一个为ptemProtclRcvDataStru[0],默认不使用*/
		ptemProtclRcvDataStru[DSU_PROTCL_SFP].proType = DSU_PROTCL_SFP;

		/*申请并初始化协议数据结构体的接收数据缓冲区*/
		ptemProtclRcvDataStru[DSU_PROTCL_RP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum);

		/*规定协议数据结构体第3个空间存放redun数据类型*/
		ptemProtclRcvDataStru[DSU_PROTCL_RP].proType = DSU_PROTCL_RP;

		/*申请并初始化协议数据结构体的接收数据缓冲区*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum);

		/*规定协议数据结构体第4个空间存放redun数据类型*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSR].proType = DSU_PROTCL_RSR;

		/*申请并初始化协议数据结构体的接收数据缓冲区*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum);

		/*规定协议数据结构体第5个空间存放RSSP数据类型*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP].proType = DSU_PROTCL_RSSP;

		/*申请并初始化协议数据结构体的接收数据缓冲区*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum);

		/*规定协议数据结构体第5个空间存放RSSP数据类型*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].proType = DSU_PROTCL_RSSP2 - 4;
	}
	else
	{
		/* 不做处理 */
		retVal = 0;
		return retVal;
	}

	for (tmpI = 0;tmpI < PROTCL_MGECFG_MAXCNT + 1;tmpI++)
	{
		/* 防止野指针 */
		protclSndCfgStru[tmpI].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU) * 0);
		protclSndCfgStru[tmpI].DevDataCnt = 0;
	}

	/* 给发送数据的临时存档数据的结构体分配空间 */
	if (0 < protclTypeInfoStru.sfpProtclNum)
	{
		protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData))
		{
			flag = 1;
		}
		else
		{
			/*为声明的发送数据临时存储区初始化*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RP].pData))
		{
			flag = 1;
		}
		else
		{
			/*为声明的发送数据临时存储区初始化*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData))
		{
			flag = 1;
		}
		else
		{
			/*为声明的发送数据临时存储区初始化*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData))
		{
			flag = 1;
		}
		else
		{
			/*为声明的发送数据临时存储区初始化*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		Prtcl_RSSP2_CheckInit(&Rssp2PrtclHead);
		/* add 20161017 xb */  
		for(tmpI=0;tmpI<PRTCL_RQ_NUM;tmpI++)
		{
			rslt = QUEUE_RING_Init(&Prtcl_RQ[tmpI].RingQue, PRTCL_RQ_SIZE);
			if(CM_TRUE != rslt)
			{
				flag = 1;
				break;
			}
			Prtcl_RQ[tmpI].RingQue.m_bIsInited = CM_TRUE;
			Prtcl_RQ[tmpI].RingQue.m_IfCoverQueueFull = CM_TRUE;
			Prtcl_RQ[tmpI].RingQue.m_QueueState = QUEUE_RING_STATE_Empty;
		}

		protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData))
		{
			flag = 1;
		}
		else
		{
			/*为声明的发送数据临时存储区初始化*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}


	if (1 == flag)
	{
		retVal = 0;
	}
	return retVal;
}

/*
* 功能：协议发送数据
* 参数：
* 输入输出参数：  ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*   1：接收数据成功
*   0：失败
*   2:非宕机错误码
*/
UINT8 SendAppData(ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret = 0;
	UINT8 temLenbuf[2] = { 0 };
	PROTCL_APP_TO_STRU tempProtclAppStru;
	UINT16 locDevName = 0;
	UINT16 destDevName = 0;
	UINT8 protclType = 0;
	UINT32 sendDataLen = 0;
	UINT32 tmpDataLen = 0;
	UINT16 temLnkDataLen = 0;
	UINT16 tempRecordLen = 0;
	UINT16 lnkDataLen = 0;
	UINT16 recordDataLen = 0;

	UINT8 tmpDevDataCnt = 0;
	UINT8 tmpI = 0;
	F_SND tmpSndF;
	UINT8 tmpSign = 0;
	UINT32 sfpErrInfo = 0;
	UINT32 rpErrInfo = 0;
	UINT32 rsrErrInfo = 0;
	UINT32 rsspErrInfo = 0;
	UINT32 manageErrInfo = 0;
	
	if (NULL != pUnifyInfo)
	{
		/* 将应用需要发送的数据写入各协议的队列 */
		TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
		while (2 < QueueStatus(&pUnifyInfo->OutputDataQueue))
		{
			QueueScan(2, temLenbuf, &pUnifyInfo->OutputDataQueue);

			ret = AppSendInputDataAnalysis(&pUnifyInfo->OutputDataQueue, &tempProtclAppStru);
			if (1 == ret)
			{
				/* 根据设备类型，决定协议的使用类型 */
				TypeId2DevName(tempProtclAppStru.devType, tempProtclAppStru.devId, &destDevName);
				protclType = 0;
				dsuProtclGetProtclType(locDevName, destDevName, &protclType);
			}
			else
			{
				break;
			}
			if (DSU_PROTCL_RSSP2 == protclType)
			{
				protclType = PROTCL_MGECFG_RSSP2;
			}
			/* 防越界 */
			if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
			{
				/*按帧数填写*/
				tmpDevDataCnt = protclSndCfgStru[protclType].DevDataCnt;
				memcpy(&protclSndCfgStru[protclType].pProtclAppStru[tmpDevDataCnt], &tempProtclAppStru, sizeof(PROTCL_APP_TO_STRU));
				protclSndCfgStru[protclType].DevDataCnt++;
			}
			else
			{
				/* break;*/
			}
		}

		/* 执行各协议的发送函数 */
		for (tmpI = 1;tmpI < PROTCL_MGECFG_MAXCNT + 1;tmpI++)
		{
			tmpDataLen = 0;
			temLnkDataLen = 0;
			tempRecordLen = 0;
			if (0 < pProtclNumBuff[tmpI])
			{
				/* 调用发送函数 */
				protcl_SetProtclAppToStruPt(protclSndCfgStru[tmpI].pProtclAppStru);
				tmpSndF = protclCfgStruBuff[tmpI].fSnd;
				ret = tmpSndF(protclSndCfgStru[tmpI].DevDataCnt, pUnifyInfo->AppArray + 4 + sendDataLen, &tmpDataLen, pUnifyInfo->VarArray + 2 + lnkDataLen, &temLnkDataLen, pUnifyInfo->RecordArray
					+ 2 + recordDataLen, &tempRecordLen);
				/* 增加赋零*/
				protclSndCfgStru[tmpI].DevDataCnt = 0;

			}
			else
			{
				temLnkDataLen = 2;
				ShortToChar(0, pUnifyInfo->VarArray + 2 + lnkDataLen);
				ret = 1;
				/* 无此通信类型 */
			}
			if (1 == ret)
			{
				sendDataLen += tmpDataLen;
				lnkDataLen += temLnkDataLen;
				recordDataLen += tempRecordLen;
			}
			else
			{
				tmpSign = 1;
			}
		}
		/* 发送数据的长度 */
		LongToChar(sendDataLen, pUnifyInfo->AppArray);
		/* 中间变量的长度 */
		ShortToChar(lnkDataLen, pUnifyInfo->VarArray);
		/* 记录数据的长度 */
		ShortToChar(recordDataLen, pUnifyInfo->RecordArray);
	}
	else
	{
		ret = 0;
	}

	if (1 == tmpSign)
	{
		ret = 0;
	}
	return ret;
}


/*
* 功能：协议接收数组
* 参数：
* 输入输出参数：ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    1：接收数据成功
*    0：失败
*    2:非宕机错误码
*/
UINT8 ReceiveAppData(ProtclConfigInfoStru* pUnifyInfo)
{
	UINT8 ret = 0;
	UINT8 flag = 0;
	UINT8 lnkState = 0;
	UINT8 i = 0;  /* 循环使用 */
	UINT16 locDevName = 0;
	UINT8 DevType = 0;
	UINT8 DevId = 0;
	UINT8 LogId = 0;
	UINT32 sfpErrInfo = 0;
	UINT32 rpErrInfo = 0;
	UINT32 rsrErrInfo = 0;
	UINT32 rsspErrInfo = 0;
	UINT32 manageErrInfo = 0;

	QueueClear(&pUnifyInfo->DataToApp);
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		QueueClear(&gpsfpUnitInfo->OutnetQueueA);
		QueueClear(&gpsfpUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		QueueClear(&gpRedunUnitInfo->OutnetQueueA);
		QueueClear(&gpRedunUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		QueueClear(&gpRsrUnitInfo->OutnetQueueA);
		QueueClear(&gpRsrUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		QueueClear(&gpRsspUnitInfo->OutnetQueueA);
		QueueClear(&gpRsspUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		QueueClear(&gpRssp2UnitInfo->OutnetQueueA);
		QueueClear(&gpRssp2UnitInfo->OutnetQueueB);
	}

	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);

	/* 读取红网队列数据 */
	ret = readQueueDataToApp(&pUnifyInfo->OutnetQueueA, 1, locDevName, ptemProtclRcvDataStru);
	if (0 == ret)
	{
		return ret;
	}

	for (i = 1;i < PROTCL_TYPE_NUM + 1;i++)
	{
		if (ptemProtclRcvDataStru[i].rcvDataLen > 0)
		{
			switch (ptemProtclRcvDataStru[i].proType)
			{
			case DSU_PROTCL_SFP:
				/* 设置SFP协议的红网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpsfpUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RP:
				/* 设置RP协议的红网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRedunUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSR:
				/* 设置RSR协议的红网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsrUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSSP:
				/* 设置RSSP协议的红网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsspUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSSP2 - 4:
				/* 设置RSSP2协议的红网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRssp2UnitInfo->OutnetQueueA);
				break;
			default:
				break;
			}
		}
	}
	/* 读取蓝网队列数据 */
	ret = readQueueDataToApp(&pUnifyInfo->OutnetQueueB, 2, locDevName, ptemProtclRcvDataStru);
	if (0 == ret)
	{
		return ret;
	}
	/* TO BE DONE switch 挪到readQueueDataToApp 里面 */
	for (i = 1;i < PROTCL_TYPE_NUM + 1;i++)
	{
		if (ptemProtclRcvDataStru[i].rcvDataLen > 0)
		{
			switch (ptemProtclRcvDataStru[i].proType)
			{
			case DSU_PROTCL_SFP:
				/* 设置SFP协议的蓝网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpsfpUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RP:
				/* 设置RP协议的蓝网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRedunUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSR:
				/* 设置RSR协议的蓝网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsrUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSSP:
				/* 设置RSSP协议的蓝网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsspUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSSP2 - 4:
				/* 设置RSSP2协议的蓝网队列数据的帧格式 */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRssp2UnitInfo->OutnetQueueB);
				break;
			default:
				break;
			}
		}
	}
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		/* SFP协议接收数据 */
		gpsfpUnitInfo->RadamSN = *gpsfpUnitInfo->CycleNum;
		ret = ProtclReceive(gpsfpUnitInfo);
		if (ret == 1)
		{
			SfpRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		/* RP协议接收数据 */
		ret = RpReceive(gpRedunUnitInfo);
		if (ret == 1)
		{
			RpRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		/* RSR协议接收数据 */
		ret = RsrReceive(gpRsrUnitInfo);
		if (ret == 1)
		{
			ret = RsrRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		/* RSSP协议接收数据 */
		ret = Rssp2Receive(gpRsspUnitInfo);
		if (ret == 1)
		{
			RsspRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		/* RSSP2协议接收数据 */
		ret = RsspIIReceive(gpRssp2UnitInfo);
		if (ret == 1)
		{
			Rssp2RcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	/* 清理环形队列 */
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if (Prtcl_RQ[i].bIsInit == CM_TRUE)
		{
			DevType = Prtcl_RQ[i].SourceID / 256 / 256 / 256;
			DevId = Prtcl_RQ[i].SourceID / 256 / 256 % 256;
			LogId = Prtcl_RQ[i].SourceID % 256;
			lnkState = GetLnkStatus(DevType, DevId, (UINT16)LogId, pUnifyInfo);
			if (lnkState == 0)
			{
				Prtcl_Manage_DelRQ(Prtcl_RQ[i].SourceID);
			}
		}
	}


	if (flag == 1)
	{
		ret = 0;
	}
	return ret;
}

/*******************************************************************************************************
* 功能描述: 	   协议适配层丢包和非宕机错误码复位
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gManageErrInfo
* 全局变量: 	  gManageErrInfo
* 返回值:			gManageErrInfo
*******************************************************************************************************/

UINT32 GetManageErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gManageErrInfo;
	return ret;
}
/*******************************************************************************************************
* 功能描述: 	   协议适配层丢包和非宕机错误码复位
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gManageErrInfo
* 全局变量: 	  gManageErrInfo
* 返回值:			1:成功
*******************************************************************************************************/

UINT32 ReSetManageErrorInfo(void)
{
	UINT32 ret = 1;
	gManageErrInfo = 0;
	return ret;
}
/*
* 功能：查询链路状态
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
*             ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*     0x33：链路在准备建立链接状态
*     0x4e：链路在等待建立链接状态
*     0x01: 表示刷新无用的链路
*/
UINT8 GetLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret = 0;
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8   protclType = 0;
	F_LNK   tmpLnkF;

	TypeId2DevName(destType, destId, &destDevName);
	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
	ret = dsuProtclGetProtclType(locDevName, destDevName, &protclType);

	if (ret != 0)
	{
		if (DSU_PROTCL_RSSP2 == protclType)
		{
			protclType = PROTCL_MGECFG_RSSP2;
		}
		/* 防越界 */
		if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
		{
			/* 调用具体协议的获取链路状态函数 */
			tmpLnkF = protclCfgStruBuff[protclType].fLnk;
			ret = tmpLnkF(destType, destId, logId);
		}
		else
		{
			ret = 0;
		}

	}
	return ret;
}




/*
* 功能：删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
*             ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 DelLnkFunc(UINT8 destType, UINT8 destId, UINT16 logId, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retVal = 0;
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8   protclType = 0;
	F_DELLNK tmpDelLnkF;

	if (NULL != pUnifyInfo)
	{
		TypeId2DevName(destType, destId, &destDevName);
		TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
		retVal = dsuProtclGetProtclType(locDevName, destDevName, &protclType);

		if (DSU_PROTCL_RSSP2 == protclType)
		{
			protclType = PROTCL_MGECFG_RSSP2;
		}

		if (retVal != 0)
		{
			/* 防越界 */
			if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
			{
				/* 调用具体协议的删除链路函数 */
				tmpDelLnkF = protclCfgStruBuff[protclType].fDelLnk;
				retVal = tmpDelLnkF(destType, destId, logId);
			}
			else
			{
				retVal = 0;
			}

		}
	}
	return retVal;
}


/*
* 功能：刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 ReFresh(UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	UINT16 inLnkLen = 0;
	UINT16 evrLnkLen = 0;
	UINT16 index = 0;/* 数组偏移量 */
	UINT8 flag = 0;

	UINT8 i = 0;/* 循环使用 */
	F_REFRESHLNK tmpReFreshLnk;

	inLnkLen = ShortFromChar(pLnkStatusData);
	for (i = 1;i < PROTCL_MGECFG_MAXCNT + 1;i++)
	{
		evrLnkLen = ShortFromChar(pLnkStatusData + 2 + index);
		index += 2;
		if (evrLnkLen > 0)
		{
			if (evrLnkLen + 2 <= inLnkLen)
			{
				/* 调用具体协议的刷新链路函数 */
				tmpReFreshLnk = protclCfgStruBuff[i].fReFreshLnk;
				retVal = tmpReFreshLnk(evrLnkLen, pLnkStatusData + 2 + index);
				index += evrLnkLen;
			}
			else
			{
				flag = 1;
				break;
			}

		}
		else
		{
			retVal = 1;
		}
		inLnkLen = inLnkLen - evrLnkLen - 2;
		if (retVal == 0)
		{
			flag = 1;
		}
	}

	if (flag == 1)
	{
		retVal = 0;
	}
	return retVal;
}





/*
* 协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 FreeSpace(void)
{
	UINT8 retVal = 0;
	UINT8 flag = 0;

	UINT8 i = 0;/* 循环使用 */
	F_END tmpEndF;

	for (i = 0;i < PROTCL_MGECFG_MAXCNT + 1;i++)
	{
		/* 调用具体协议的释放空间函数 */
		tmpEndF = protclCfgStruBuff[i].fEnd;
		retVal = tmpEndF();
		if (retVal == 0)
		{
			flag = 1;
		}
	}
	/* 释放适配层分配的结构体空间 */
	FreeSpaceFunc();
	if (flag == 1)
	{
		retVal = 0;
	}
	return retVal;
}

UINT8 ReceiveTcpState(UINT16 TcpStateDataLen, UINT8* pTcpStateData)
{
	UINT8 ret = 0;
	ret = RsspIIInputTcpState(TcpStateDataLen, pTcpStateData, gpRssp2UnitInfo);
	return ret;
}

UINT8 SendTcpCtrl(UINT16 *pTcpCtrlDataLen, UINT8 *pTcpCtrlData)
{
	UINT8 ret = 0;
	ret = RsspIIOutputTcpCtrl(pTcpCtrlDataLen, pTcpCtrlData, gpRssp2UnitInfo);
	return ret;
}
/***********协议初始化相关函数**************************************************************/
/*
* 初始从配置文件读取相对应的协议配置数据
* 输入参数      :UINT8 *pFileName   协议配置数据文件名
* 输出参数      :UINT8 *pEvenPrtclNumBuff 存放本方通信的相关协议个数
* 全局变量      :
* 返回值        : 成功,返回协议个数;失败返回0
*/
UINT8 InitGetCfgInfo(UINT8 *pFileName, UINT8 *pEvenPrtclNumBuff)
{
	UINT8 ret = 0;
	UINT16 locDevName = 0;
	UINT8 tmpIndex = 0;
	UINT8 maxLnkNum = 0;
	DSU_PROTCL_TYPE_STRU tempProtclTypeNum;
	ProtclConfigInfoStru* pUnifyInfo;

	/* 获取协议配置结构体 */
	protcl_GetProtclCfgStruPt(&pUnifyInfo);

	/* 本方的设备类型 */
	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);

	ret = dsuCommonInfoInit(pFileName);/* 查询函数初始化,获取设备IP配置表中的设备IP地址及设备通信协议表数据 */
	if (ret == 0)
	{
		return ret;
	}

	dsuGetProtclTypeInfo(locDevName, &protclTypeInfoStru);
	memcpy(&tempProtclTypeNum, &protclTypeInfoStru, sizeof(DSU_PROTCL_TYPE_STRU));

	maxLnkNum = tempProtclTypeNum.rsspProtclNum + tempProtclTypeNum.rsrProtclNum + tempProtclTypeNum.redunProtclNum + tempProtclTypeNum.sfpProtclNum + tempProtclTypeNum.rssp2ProtclNum;
	ret = InitProtclInfoStru(pUnifyInfo, maxLnkNum);
	if (ret == 0)
	{
		return ret;
	}

	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.sfpProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.redunProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rsrProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rsspProtclNum;
	tmpIndex++;

	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rssp2ProtclNum;
	tmpIndex++;


	ret = tmpIndex;
	return ret;
}

/******************************************************************************************
* 功能描述      : 初始化统一结构体
1. 动态分配统一结构体空间，并对其进行初始化；
* 输入参数      :

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/
static UINT8 InitProtclInfoStru(ProtclConfigInfoStru* pUnifyInfo, UINT8 maxLnkNum)
{
	UINT8  retnVal = 0;
	UINT32 lTemp = 0;

	/* 协议输入给应用的数据队列长度 */
	lTemp = (pUnifyInfo->InputSize + 7) * maxLnkNum;
	pUnifyInfo->DataToAppSize = lTemp;
	retnVal = QueueInitial(&pUnifyInfo->DataToApp, pUnifyInfo->DataToAppSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/* 应用给协议输出的数据队列长度 */
	lTemp = (pUnifyInfo->OutputSize + 6) * maxLnkNum;
	pUnifyInfo->OutputDataQueueSize = lTemp;
	retnVal = QueueInitial(&pUnifyInfo->OutputDataQueue, pUnifyInfo->OutputDataQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/*初始化红网接收队列*/
	pUnifyInfo->OutnetQueueSize = (pUnifyInfo->InputSize + 38 + 6)*maxLnkNum*MAX_NUM_PER_CYCLE;
	retnVal = QueueInitial(&pUnifyInfo->OutnetQueueA, pUnifyInfo->OutnetQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/* 初始化蓝网接收队列 */
	retnVal = QueueInitial(&pUnifyInfo->OutnetQueueB, pUnifyInfo->OutnetQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}

	/* 输出模块输出数组:指向冗余层的输出数组空间 */

	pUnifyInfo->AppArraySize = (pUnifyInfo->OutputSize + 38 + 7)*maxLnkNum + 4;
	pUnifyInfo->AppArray = NULL;
	pUnifyInfo->AppArray = (UINT8*)malloc(pUnifyInfo->AppArraySize);
	if (NULL != pUnifyInfo->AppArray)
	{
		/*初始化*/
		CommonMemSet(pUnifyInfo->AppArray, pUnifyInfo->AppArraySize, 0, pUnifyInfo->AppArraySize);
	}
	else
	{
		retnVal = 0;
		/* 记录 */
		return retnVal;
	}
	/* 输出模块输出中间变量数组大小 */
	pUnifyInfo->VarArraySize = VAR_NODE_LEN * maxLnkNum + 32;
	pUnifyInfo->VarArray = NULL;
	pUnifyInfo->VarArray = (UINT8*)malloc(pUnifyInfo->VarArraySize);
	if (NULL == pUnifyInfo->VarArray)
	{
		retnVal = 0;
		/* 记录 */
		return retnVal;
	}
	else
	{
		/* 初始化 */
		CommonMemSet(pUnifyInfo->VarArray, pUnifyInfo->VarArraySize, 0, pUnifyInfo->VarArraySize);
	}

	/*  记录数组  */
	pUnifyInfo->RecordArray = NULL;
	pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize);
	if (NULL != pUnifyInfo->RecordArray)
	{
		/* 初始化 */
		CommonMemSet(pUnifyInfo->RecordArray, pUnifyInfo->RecordArraySize, 0, pUnifyInfo->RecordArraySize);
	}
	else
	{
		retnVal = 0;
		/* 记录 */
		return retnVal;
	}

	retnVal = 1;
	return retnVal;
}


/******************************************************************************************
* 功能描述      : 初始化SFP协议结构体
1. 动态分配SFP协议结构体空间，并对其进行初始化；
* 输入参数      :UINT8  typeNum   本设备通信使用SFP协议的个数

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/

static UINT8 InitSfpProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (typeNum != 0)
	{
		if (gpsfpUnitInfo == NULL)
		{
			gpsfpUnitInfo = (struc_Unify_Info*)malloc(sizeof(struc_Unify_Info));
		}
		if (gpsfpUnitInfo != NULL)
		{
			CommonMemSet(gpsfpUnitInfo, sizeof(struc_Unify_Info), 0, sizeof(struc_Unify_Info));
			/* 协议初始化 */
			/* 初始化值设定 */
			gpsfpUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpsfpUnitInfo->TimeCounter = pUnifyInfo->CycleNum;
			gpsfpUnitInfo->LocalID = pUnifyInfo->LocalID;		/* 设备ID*/
			gpsfpUnitInfo->LocalType = pUnifyInfo->LocalType;		/* 设备type*/
			gpsfpUnitInfo->MaxNumLink = typeNum;		/* 通信的对象数*/
			gpsfpUnitInfo->MaxNumPerCycle = 5; /* 一周期允许每个对象的最大有效数据*/
			gpsfpUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* 接受数据中最大一帧的应用数据长度*/
			gpsfpUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* 发送数据中最大一帧的应用数据长度*/			
			gpsfpUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[0] / curDevRecordTotalLen;	/* 记录数组大小*/
			gpsfpUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;
			gpsfpUnitInfo->RedundTimeout = 0xffff;
			gpsfpUnitInfo->OutputTimeout = 0xffff;
			gpsfpUnitInfo->RadamSN = 0x1;

			ret = ProtclInit(fileName, gpsfpUnitInfo);

			if (ret != 1)
			{
				printf("Sfp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* 不做处理 */
	}
	ret = 1;
	return ret;
}


/******************************************************************************************
* 功能描述      : 初始化Rp协议结构体
1. 动态分配RP协议结构体空间，并对其进行初始化；
* 输入参数      :UINT8  typeNum   本设备通信使用RP协议的个数

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/
static UINT8 InitRpProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (typeNum != 0)
	{
		if (gpRedunUnitInfo == NULL)
		{
			gpRedunUnitInfo = (RP_INFO_STRU*)malloc(sizeof(RP_INFO_STRU));

		}
		if (gpRedunUnitInfo != NULL)
		{
			CommonMemSet(gpRedunUnitInfo, sizeof(RP_INFO_STRU), 0, sizeof(RP_INFO_STRU));
			/* 协议初始化 */
			/* 初始化值设定 */
			gpRedunUnitInfo->LocalID = pUnifyInfo->LocalID;		/* 设备ID*/
			gpRedunUnitInfo->LocalType = pUnifyInfo->LocalType;		/* 设备type*/
			gpRedunUnitInfo->MaxNumLink = typeNum;		/* 冗余层通信的对象数*/
			gpRedunUnitInfo->MaxNumPerCycle = 5; /* 冗余层一周期允许每个对象的最大有效数据*/
			gpRedunUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* 冗余层接受数据中最大一帧的应用数据长度*/
			gpRedunUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* 冗余层发送数据中最大一帧的应用数据长度*/
			gpRedunUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[1] / curDevRecordTotalLen; /* 记录数组大小*/
			gpRedunUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RpInit(fileName, gpRedunUnitInfo);

			if (ret != 1)
			{
				printf("Rp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* 不做处理 */
	}
	ret = 1;
	return ret;
}

/******************************************************************************************
* 功能描述      : 初始化RSSP协议结构体
1. 动态分配RSSP协议结构体空间，并对其进行初始化；
* 输入参数      :UINT8  typeNum   本设备通信使用RSSP协议的个数

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/
static UINT8 InitRsspProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;

	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (NULL == gpRsspUnitInfo)
		{
			gpRsspUnitInfo = (RSSP_INFO_STRU*)malloc(sizeof(RSSP_INFO_STRU));
		}
		if (NULL != gpRsspUnitInfo)
		{
			CommonMemSet(gpRsspUnitInfo, sizeof(RSSP_INFO_STRU), 0, sizeof(RSSP_INFO_STRU));
			/* 协议初始化,初始化值设定 */
			gpRsspUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRsspUnitInfo->LocalID = pUnifyInfo->LocalID;		/* 设备ID*/
			gpRsspUnitInfo->LocalType = pUnifyInfo->LocalType;		/* 设备type*/
			gpRsspUnitInfo->MaxNumLink = typeNum;		/* 通信的对象数*/
			gpRsspUnitInfo->MaxNumPerCycle = 5; /* 一周期允许每个对象的最大有效数据*/
			if ((pUnifyInfo->InputSize > RSR_RSSP_MAX_LEN) || (pUnifyInfo->OutputSize > RSR_RSSP_MAX_LEN))
			{
				gpRsspUnitInfo->InputSize = RSR_RSSP_MAX_LEN;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRsspUnitInfo->OutputSize = RSR_RSSP_MAX_LEN;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			else
			{
				gpRsspUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRsspUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			gpRsspUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[3] / curDevRecordTotalLen; /* 记录数组大小*/

			gpRsspUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = Rssp2Init(fileName, gpRsspUnitInfo);

			if (ret != 1)
			{
				printf("Rssp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* 不做处理 */
	}
	ret = 1;
	return ret;
}

/******************************************************************************************
* 功能描述      : 初始化RSSP2协议结构体
1. 动态分配RSSP2协议结构体空间，并对其进行初始化；
* 输入参数      :UINT8  typeNum   本设备通信使用RSSP2协议的个数

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/
static UINT8 InitRssp2ProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;

	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (200 < typeNum)
		{
			typeNum = 200;
		}
		if (NULL == gpRssp2UnitInfo)
		{
			gpRssp2UnitInfo = (RSSP2_INFO_STRU*)malloc(sizeof(RSSP2_INFO_STRU));
		}

		if (NULL != gpRssp2UnitInfo)
		{
			CommonMemSet(gpRssp2UnitInfo, sizeof(RSSP2_INFO_STRU), 0, sizeof(RSSP2_INFO_STRU));
			/* 协议初始化,初始化值设定 */
			gpRssp2UnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRssp2UnitInfo->RandomNum = pUnifyInfo->CycleNum; /* add 20160819 xb 增加随机数功能 */
			gpRssp2UnitInfo->LocalID = pUnifyInfo->LocalID;		/* 设备ID*/
			gpRssp2UnitInfo->LocalType = pUnifyInfo->LocalType;	/* 设备type*/
			gpRssp2UnitInfo->MaxNumLink = typeNum;		        /* 通信的对象数*/
			gpRssp2UnitInfo->MaxNumPerCycle = 5; /* 一周期允许每个对象的最大有效数据*/
			if ((pUnifyInfo->InputSize > RSSP2_MAX_LEN) || (pUnifyInfo->OutputSize > RSSP2_MAX_LEN))
			{
				gpRssp2UnitInfo->InputSize = RSSP2_MAX_LEN;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRssp2UnitInfo->OutputSize = RSSP2_MAX_LEN;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			else
			{
				gpRssp2UnitInfo->InputSize = pUnifyInfo->InputSize;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRssp2UnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			gpRssp2UnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[4] / curDevRecordTotalLen; /* 记录数组大小*/

			gpRssp2UnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RsspIIInit(fileName, gpRssp2UnitInfo);

			if (ret != 1)
			{
				printf("Rssp2 init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* 不做处理 */
	}

	ret = 1;
	return ret;
}


/******************************************************************************************
* 功能描述      : 初始化RSR协议结构体
1. 动态分配RSR协议结构体空间，并对其进行初始化；
* 输入参数      :UINT8  typeNum   本设备通信使用RSR协议的个数

* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0
*******************************************************************************************/
static UINT8 InitRsrProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (NULL == gpRsrUnitInfo)
		{
			gpRsrUnitInfo = (RSR_INFO_STRU*)malloc(sizeof(RSR_INFO_STRU));
		}
		if (NULL != gpRsrUnitInfo)
		{
			CommonMemSet(gpRsrUnitInfo, sizeof(RSR_INFO_STRU), 0, sizeof(RSR_INFO_STRU));
			/* 协议初始化,初始化值设定 */
			gpRsrUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRsrUnitInfo->LocalID = pUnifyInfo->LocalID;		/* 设备ID*/
			gpRsrUnitInfo->LocalType = pUnifyInfo->LocalType;		/* 设备type*/
			gpRsrUnitInfo->MaxNumLink = typeNum;		/* 通信的对象数*/
			gpRsrUnitInfo->MaxNumPerCycle = 5; /* 一周期允许每个对象的最大有效数据*/
			if ((pUnifyInfo->InputSize > RSR_RSSP_MAX_LEN) || (pUnifyInfo->OutputSize > RSR_RSSP_MAX_LEN))
			{
				gpRsrUnitInfo->InputSize = RSR_RSSP_MAX_LEN;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRsrUnitInfo->OutputSize = RSR_RSSP_MAX_LEN;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			else
			{
				gpRsrUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* 接受数据中最大一帧的应用数据长度*/
				gpRsrUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* 发送数据中最大一帧的应用数据长度*/
			}
			gpRsrUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[2] / curDevRecordTotalLen; /* 记录数组大小*/

			gpRsrUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RsrInit(fileName, gpRsrUnitInfo);

			if (ret != 1)
			{
				printf("RSR init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* 不做处理 */
	}
	ret = 1;
	return ret;
}

/***********协议发送相关函数**************************************************************/
/*
* 功能：应用发送的数据解析
*
* 参数：
* 输入参数：  const QueueStruct *pAppQueueData    应用输入到协议的数据队列
* 输出参数：  PROTCL_APP_TO_STRU *protclAppStru   解析后的数据
* 返回值：
*    UINT8 1 收到完整数据帧，0 无数据
*/
UINT8 AppSendInputDataAnalysis(QueueStruct *pAppQueueData, PROTCL_APP_TO_STRU *pProtclAppStru)
{
	UINT8 retVal = 0;
	UINT8 tempBuff[2] = { 0 };
	UINT16 tempDatalen = 0;

	QueueScan(2, tempBuff, pAppQueueData);	/* 肯定存在两个字节 */
	tempDatalen = ShortFromChar(tempBuff);
	if (((UINT32)(tempDatalen + 2) <= QueueStatus(pAppQueueData)) && (4 <= tempDatalen))
	{
		pProtclAppStru->appDataLen = tempDatalen - 4;
		QueueRead(2, tempBuff, pAppQueueData);/* 长度 */
		QueueRead(1, &pProtclAppStru->devType, pAppQueueData);/* 类型 */
		QueueRead(1, &pProtclAppStru->devId, pAppQueueData);/* ID */
		QueueRead(2, tempBuff, pAppQueueData);/* LogicID */
		pProtclAppStru->devLogId = ShortFromChar(tempBuff);
		QueueRead((tempDatalen - 4), pProtclAppStru->pAppData, pAppQueueData);/* 应用数据 */

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/*
* 功能：SFP协议入口组帧
*
* 参数：
* 输入参数：  const PROTCL_APP_TO_STRU *protclAppStru   需要组的数据
* 输出参数：  QueueStruct *pAppQueueData    协议入口数据队列
* 返回值：
*    UINT8 1 正常，0 不正常
*/
UINT8 sfpInputPackFms(const PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[6] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* 长度 */
		ShortToChar(protclAppStru.appDataLen + 4, tmpHeadBuf);
		/* 2个字节的预留 */
		tmpHeadBuf[2] = 0;
		tmpHeadBuf[3] = 0;
		/* 设备ID */
		tmpHeadBuf[4] = protclAppStru.devId;
		/* 设备类型 */
		tmpHeadBuf[5] = protclAppStru.devType;
		QueueWrite(6, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* 功能：SFP协议发送数据
*
* 参数：
* 输入参数：  devDataCount	设备数据个数	UINT8
* 输出参数：
*             sendDataLen	发送数据长度	UINT32*
*             pSndData	    发送数据	    UINT8*
*             pLnkStatusData	协议状态数据	UINT8*
*             lnkDataLen	协议状态数据长度	UINT16*
*             recordArray 	协议记录数据	UINT8*
*             recordDataLen	协议记录数据长度	UINT16*

* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 sfpSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8 * pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* 循环使用 */
	UINT32 len = 0;
	UINT32 dataLen = 0;
	UINT16 lenInput = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	QueueClear(&gpsfpUnitInfo->OutputDataQueue);

	if (pAppDataStru != NULL)
	{
		for (i = 0;i < devDataCount;i++)
		{
			sfpInputPackFms(pAppDataStru[i], &gpsfpUnitInfo->OutputDataQueue);
		}

		ret = ProtclOutput(gpsfpUnitInfo);
		if (ret == 1)
		{
			/* 该长度包含中间变量和报文数据 丢弃该长度 */
			/* 从数据长度0到通信链路管理表的数据长度 4字节 */
			len = LongFromChar(gpsfpUnitInfo->PrtclSndBuff);
			/* 报文的总长度*/
			if (len > 4)
			{
				dataLen = LongFromChar(gpsfpUnitInfo->PrtclSndBuff + 4);

				*sendDataLen = dataLen;
				memcpy(pSndData, gpsfpUnitInfo->PrtclSndBuff + 8, dataLen);
				if (len - dataLen - 4 > 0)
				{

					lenInput = (UINT16)(len - dataLen - 4);
					(*lnkDataLen) = lenInput + 2;
					ShortToChar(lenInput, pLnkStatusData);
					memcpy(pLnkStatusData + 2, gpsfpUnitInfo->PrtclSndBuff + 8 + dataLen, lenInput);
				}
				else
				{
					/* 不做任何处理 */
					*sendDataLen = 0;
					ShortToChar(0, pLnkStatusData);
					*lnkDataLen = 2;
				}
			}
			else
			{
				/* 不做处理 */
				*sendDataLen = 0;
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;

			*sendDataLen = 0;
			*lnkDataLen = 0;
		}

		tempRecordSize = ShortFromChar(gpsfpUnitInfo->RecordArray) + 1;
		if (gpsfpUnitInfo->RecordArraySize > 3)
		{
			if (gpsfpUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpsfpUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpsfpUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpsfpUnitInfo->RecordArray);
				ShortToChar(gpsfpUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpsfpUnitInfo->RecordArray + 2, gpsfpUnitInfo->RecordArraySize - 3);
				recordArray[gpsfpUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpsfpUnitInfo->RecordArraySize;
			}
		}

	}
	else
	{
		ret = 0;
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
	}

	return ret;
}


/*
* 功能：RSSP协议入口组帧
*
* 参数：
* 输入参数：  const PROTCL_APP_TO_STRU *protclAppStru   需要组的数据
* 输出参数：  QueueStruct *pAppQueueData    协议入口数据队列
* 返回值：
*    UINT8 1 正常，0 不正常
*/
UINT8 RsspInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[4] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* 两个字节的长度 */
		ShortToChar(protclAppStru.appDataLen + 2, tmpHeadBuf);
		/*一个字节的设备ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		/*一个字节的设备类型 */
		tmpHeadBuf[2] = protclAppStru.devType;
		QueueWrite(4, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* 功能：RSSP协议发送数据
*
* 参数：
* 输入参数：  devDataCount	设备数据个数	UINT8
* 输出参数：  sendDataLen	发送数据长度	UINT32*
*             pSndData	    发送数据	    UINT8*
*             pLnkStatusData	协议状态数据	UINT8*
*             lnkDataLen	协议状态数据长度	UINT16*
*             recordArray 	协议记录数据	UINT8*
*             recordDataLen	协议记录数据长度	UINT16*
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RsspSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* 循环使用 */
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRsspUnitInfo->OutputDataQueue);

		for (i = 0;i < devDataCount;i++)
		{
			RsspInputPackFms(pAppDataStru[i], &gpRsspUnitInfo->OutputDataQueue);
		}


		ret = Rssp2Output(gpRsspUnitInfo);
		if (ret == 1)
		{
			/* 整个数组长度 2字节 */
			len = ShortFromChar(gpRsspUnitInfo->AppArray);
			*sendDataLen = len;

			/* 中间变量整个数组长度 2字节 */
			varlen = ShortFromChar(gpRsspUnitInfo->VarArray);
			*lnkDataLen = varlen + 2;
			/* 报文的总长度*/
			if (len > 2)
			{
				memcpy(pSndData, gpRsspUnitInfo->AppArray + 2, len);

				/*memcpy(pLnkStatusData,gpRsspUnitInfo->VarArray,varlen+2);*/

			}
			else
			{
				/* 不做处理 */
				*sendDataLen = 0;
				/**lnkDataLen = 0;*/
			}
			if (varlen > 2)
			{
				memcpy(pLnkStatusData, gpRsspUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* 不做处理 */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRsspUnitInfo->RecordArray) + 1;
		if (gpRsspUnitInfo->RecordArraySize > 3)
		{
			if (gpRsspUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRsspUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRsspUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRsspUnitInfo->RecordArray);
				ShortToChar(gpRsspUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRsspUnitInfo->RecordArray + 2, gpRsspUnitInfo->RecordArraySize - 3);
				recordArray[gpRsspUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRsspUnitInfo->RecordArraySize;
			}
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}
	return ret;
}


/*
* 功能：RSSP2协议入口组帧
*
* 参数：
* 输入参数：  const PROTCL_APP_TO_STRU *protclAppStru   需要组的数据
* 输出参数：  QueueStruct *pAppQueueData    协议入口数据队列
* 返回值：
*    UINT8 1 正常，0 不正常
*/
UINT8 Rssp2InputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[6] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* 两个字节的长度 */
		ShortToChar(protclAppStru.appDataLen + 4, tmpHeadBuf);
		/*一个字节的设备ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		/*一个字节的设备类型 */
		tmpHeadBuf[2] = protclAppStru.devType;
		/*2个字节的逻辑ID*/
		ShortToChar(protclAppStru.devLogId, &tmpHeadBuf[4]);
		QueueWrite(6, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/*
* 功能：RSSP2协议发送数据
*
* 参数：
* 输入参数：  devDataCount	设备数据个数	UINT8
* 输出参数：  sendDataLen	发送数据长度	UINT32*
*             pSndData	    发送数据	    UINT8*
*             pLnkStatusData	协议状态数据	UINT8*
*             lnkDataLen	协议状态数据长度	UINT16*
*             recordArray 	协议记录数据	UINT8*
*             recordDataLen	协议记录数据长度	UINT16*
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 Rssp2SendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* 循环使用 */
	UINT32 len = 0;
	UINT16 varlen = 0;
	UINT16 recordLen = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRssp2UnitInfo->OutputDataQueue);

		for (i = 0;i < devDataCount;i++)
		{
			Rssp2InputPackFms(pAppDataStru[i], &gpRssp2UnitInfo->OutputDataQueue);
		}


		ret = RsspIIOutput(gpRssp2UnitInfo);

		if (ret == 1)
		{

			/* 报文的总长度*/
			len = (UINT32)(gpRssp2UnitInfo->AppArraySize);			
			if (len > 2)
			{
				memcpy(pSndData, gpRssp2UnitInfo->AppArray, len);
				*sendDataLen = len;
			}
			else
			{
				/* 不做处理 */
				*sendDataLen = 0;
			}
			/* 中间变量整个数组长度 2字节 */
			varlen = ShortFromChar(gpRssp2UnitInfo->VarArray);
			if (varlen > 2)
			{
				memcpy(pLnkStatusData, gpRssp2UnitInfo->VarArray, varlen + 2);
				*lnkDataLen = varlen + 2;
			}
			else
			{
				/* 不做处理 */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}

			recordLen = ShortFromChar(gpRssp2UnitInfo->RecordArray);
			if (recordLen > 2)
			{
				memcpy(recordArray, gpRssp2UnitInfo->RecordArray, recordLen + 2);
				*recordDataLen = recordLen+2;
			}
			else
			{
				ShortToChar(0, recordArray);
				*recordDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			*recordDataLen = 0;
			ret = 0;
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}


	return ret;
}




/*
* 功能：RSR协议入口组帧
*
* 参数：
* 输入参数：  const PROTCL_APP_TO_STRU *protclAppStru   需要组的数据
* 输出参数：  QueueStruct *pAppQueueData    协议入口数据队列
* 返回值：
*    UINT8 1 正常，0 不正常
*/
UINT8 RsrInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[8] = { 0 };
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8 index = 0;

	UINT16 rsspSrcAdd = 0;
	INT16 bsdAdd = 0;
	UINT16 destRsspAdd = 0;

	if (protclAppStru.appDataLen > 0)
	{
		TypeId2DevName(protclAppStru.devType, protclAppStru.devId, &destDevName);
		TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
		if (protclAppStru.devType == VOBC_TYPE)
		{
			retVal = dsuVOBCvsCIOutInfo(protclAppStru.devLogId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
		{
			retVal = dsuVOBCvsCIOutInfo(protclAppStru.devLogId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		else
		{
			retVal = dsuZCvsCIOutInfo(locDevName, destDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		if (retVal != 0)
		{
			/* 两个字节的长度 */
			ShortToChar(protclAppStru.appDataLen + 6, tmpHeadBuf);
			index += 2;
			/* 两个字节的源地址 */
			ShortToChar(rsspSrcAdd, tmpHeadBuf + index);
			index += 2;
			/* 两个字节的BSD地址 */
			ShortToChar(bsdAdd, tmpHeadBuf + index);
			index += 2;
			/* 两个字节的目标地址 */
			ShortToChar(destRsspAdd, tmpHeadBuf + index);
			QueueWrite(8, tmpHeadBuf, pAppQueueData);
			QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);
		}
		else
		{
			retVal = 0;
		}
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}



/*
* 功能：RSR协议发送数据
*
* 参数：
* 输入参数：  devDataCount	设备数据个数	UINT8
* 输出参数：  sendDataLen	发送数据长度	UINT32*
*             pSndData	    发送数据	    UINT8*
*             pLnkStatusData	协议状态数据	UINT8*
*             lnkDataLen	协议状态数据长度	UINT16*
*             recordArray 	协议记录数据	UINT8*
*             recordDataLen	协议记录数据长度	UINT16*
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RsrSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;

	PROTCL_APP_TO_STRU *pAppDataStru;

	UINT8 protclType = GetCbtcSysType();

	protcl_GetProtclAppToStruPt(&pAppDataStru);
	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRsrUnitInfo->OutputDataQueue);
		for (i = 0;i < devDataCount;i++)
		{
			RsrInputPackFms(pAppDataStru[i], &gpRsrUnitInfo->OutputDataQueue);
		}

		ret = RsrOutput(gpRsrUnitInfo);
		if (ret == 1)
		{
			/* 整个数组长度 2字节 */
			len = ShortFromChar(gpRsrUnitInfo->AppArray);
			if (DQU_CBTC_HLT != protclType)
			{
				*sendDataLen = len;
			}
			/* 中间变量整个数组长度 2字节 */
			varlen = ShortFromChar(gpRsrUnitInfo->VarArray);
			/* yw.lou 和RP一样也需要处理不同的情况*/
			/* 报文的总长度*/
			if (len > 2)
			{
				if (DQU_CBTC_HLT == protclType)
				{
					RsrAndRpAppArrayPage(gpRsrUnitInfo->AppArray + 2, len, pSndData, sendDataLen);
				}
				else
				{
					memcpy(pSndData, gpRsrUnitInfo->AppArray + 2, len);
				}
			}
			else
			{
				/* 不做处理 */
				*sendDataLen = 0;
			}
			if (varlen > 2)
			{
				*lnkDataLen = varlen + 2;
				memcpy(pLnkStatusData, gpRsrUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* 不做处理 */
				ShortToChar(0, pLnkStatusData);

				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*lnkDataLen = 0;
			*sendDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRsrUnitInfo->RecordArray) + 1;
		if (gpRsrUnitInfo->RecordArraySize > 3)
		{
			if (gpRsrUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRsrUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRsrUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRsrUnitInfo->RecordArray);
				ShortToChar(gpRsrUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRsrUnitInfo->RecordArray + 2, gpRsrUnitInfo->RecordArraySize - 3);
				recordArray[gpRsrUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRsrUnitInfo->RecordArraySize;
			}
		}

	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}

	return ret;
}

/*
* 功能：RP协议入口组帧
*
* 参数：
* 输入参数：  const PROTCL_APP_TO_STRU *protclAppStru   需要组的数据
* 输出参数：  QueueStruct *pAppQueueData    协议入口数据队列
* 返回值：
*    UINT8 1 正常，0 不正常
*/
UINT8 RpInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[4] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* 两个字节的长度 */
		ShortToChar(protclAppStru.appDataLen + 2, tmpHeadBuf);
		/* 1个字节的设备类型 */
		tmpHeadBuf[2] = protclAppStru.devType;
		/* 1个字节的设备ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		QueueWrite(4, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* 功能：RP协议发送数据
*
* 参数：
* 输入参数：  devDataCount	设备数据个数	UINT8
* 输出参数：  sendDataLen	发送数据长度	UINT32*
*             pSndData	    发送数据	    UINT8*
*             pLnkStatusData	协议状态数据	UINT8*
*             lnkDataLen	协议状态数据长度	UINT16*
*             recordArray 	协议记录数据	UINT8*
*             recordDataLen	协议记录数据长度	UINT16*
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RpSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;

	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRedunUnitInfo->OutputDataQueue);
		for (i = 0;i < devDataCount;i++)
		{
			RpInputPackFms(pAppDataStru[i], &gpRedunUnitInfo->OutputDataQueue);
		}


		ret = RpOutput(gpRedunUnitInfo);
		if (ret == 1)
		{
			/* 整个数组长度 2字节 */
			len = ShortFromChar(gpRedunUnitInfo->AppArray);
			/* 中间变量整个数组长度 2字节 */
			varlen = ShortFromChar(gpRedunUnitInfo->VarArray);

			/*yw.lou 修改: 由于发送应用数据可能存在为空的情况，而此时有状态数据，则就存在问题*/
			/* 报文的总长度*/
			if ((len > 2))
			{
				memcpy(pSndData, gpRedunUnitInfo->AppArray + 2, len);
				*sendDataLen = len;
			}
			else
			{
				/* 不做处理 */
				*sendDataLen = 0;
			}
			if (varlen > 2)
			{
				*lnkDataLen = varlen + 2;
				memcpy(pLnkStatusData, gpRedunUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* 不做处理 */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRedunUnitInfo->RecordArray) + 1;
		if (gpRedunUnitInfo->RecordArraySize > 3)
		{
			if (gpRedunUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRedunUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRedunUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRedunUnitInfo->RecordArray);
				ShortToChar(gpRedunUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRedunUnitInfo->RecordArray + 2, gpRedunUnitInfo->RecordArraySize - 3);
				recordArray[gpRedunUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRedunUnitInfo->RecordArraySize;
			}
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		recordArray = NULL;
		*sendDataLen = 0;
		*lnkDataLen = 0;
		recordDataLen = 0;
		ret = 0;
	}

	return ret;
}





/*
* 功能：RP和RSR协议发送输出数据组帧
*
* 参数：
* 输入参数：  UINT8* pInAppArray         输入应用数据
*             UINT32 appDtaLen           输入数据长度
* 输出参数：  UINT32* outAppLen          组包后的应用数据长度
*             UINT8* pOutAppArray        组包后的应用数据
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RsrAndRpAppArrayPage(UINT8* pInAppArray, UINT32 appDtaLen, UINT8* pOutAppArray, UINT32* outAppLen)
{
	UINT8 retnVal = 0;
	UINT32 index = 0;
	UINT32 outIndex = 0;
	UINT16 dataLen = 0;

	while (appDtaLen > 0)
	{
		/*两个子节点的长度长度等于源长度+2(目标设备个数和源设备个数)*/
		dataLen = ShortFromChar(pInAppArray + index);
		ShortToChar(dataLen + 2, pOutAppArray + outIndex);
		/*1个字节的目标设备个数*/
		index += 2;
		outIndex += 2;
		pOutAppArray[outIndex++] = 1;
		/*红网IP地址以及端口号和蓝网的IP地址和端口号  总共12个字节*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, 12);

		index += 12;
		outIndex += 12;
		/*1个字节的源设备个数*/
		pOutAppArray[outIndex++] = 1;
		/*红网IP地址以及端口号和蓝网的IP地址和端口号  总共12个字节*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, 12);
		index += 12;
		outIndex += 12;
		/*应用数据*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, dataLen - 24);
		appDtaLen = appDtaLen - dataLen - 2;
		outIndex += (dataLen - 24);
		index += (dataLen - 24);
	}
	*outAppLen = outIndex;

	retnVal = 1;
	return retnVal;
}

/***********协议接收相关函数**************************************************************/
/*
* 功能：SFP协议接收数据
*
* 参数：
* 输入参数：
* 输出参数：  QueueStruct* pOutToApp 输出给应用的数据帧格式
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 SfpRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 sfpRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpsfpUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpsfpUnitInfo->DataToApp);/* 两个字节长度 */

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, sfpRcvDataBuff, &gpsfpUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 5 + PROTCL_OUT_FONT_LEN;
		/*ShortToChar(tempDataLen,tempDatabuff);*/
		RcvDataToAppQueue(sfpRcvDataBuff[3], sfpRcvDataBuff[2], tmpLogid, sfpRcvDataBuff[4], tempDataLen, sfpRcvDataBuff + 5, pOutToApp);
	}

	ret = 1;
	return ret;
}




/*
* 功能：RSR协议接收数据
*
* 参数：
* 输入参数：
* 输出参数：  QueueStruct* pOutToApp 输出给应用的数据帧格式
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RsrRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rsrRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;
	UINT16 srcAddr = 0;
	UINT16 destAddr = 0;
	UINT16 destDevName = 0;
	UINT8 destDevType = 0;
	UINT8 destDevId = 0;

	while (QueueStatus(&gpRsrUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRsrUnitInfo->DataToApp);/* 两个字节长度 */

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, rsrRcvDataBuff, &gpRsrUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 4 + PROTCL_OUT_FONT_LEN;
		srcAddr = ShortFromChar(rsrRcvDataBuff);
		destAddr = ShortFromChar(rsrRcvDataBuff + 2);

		if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
		{

			dsuVOBCvsCIInInfo(srcAddr, destAddr, &tmpLogid);
			destDevType = CI_TYPE;
		}
		else if (gpRsrUnitInfo->LocalType == ZC_TYPE)
		{
			dsuZCvsCIInInfo(srcAddr, destAddr, &destDevName);
			DevName2TypeId(destDevName, &destDevType, &destDevId);
			tmpLogid = 0;
		}
		else if (gpRsrUnitInfo->LocalType == CI_TYPE)
		{
			if (dsuVOBCvsCIInInfo(srcAddr, destAddr, &tmpLogid) == 0)
			{
				dsuZCvsCIInInfo(srcAddr, destAddr, &destDevName);
				DevName2TypeId(destDevName, &destDevType, &destDevId);
				tmpLogid = 0;
			}
			else
			{
				/*dsuVOBCvsCIInInfo(srcAddr,destAddr,&tmpLogid);*/
				destDevType = VOBC_TYPE;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
		RcvDataToAppQueue(destDevType, destDevId, tmpLogid, 0, tempDataLen, rsrRcvDataBuff + 4, pOutToApp);

	}

	ret = 1;
	return ret;
}

/*将接收到的数据写入到输出队列中 */
UINT8 RcvDataToAppQueue(UINT8 devType, UINT8 devId, UINT16 logId, UINT8 lifeTime, UINT16 appDatalen, UINT8* pAppData, QueueStruct* pDataToApp)
{
	UINT8 ret = 0;
	UINT8 tempDataBuff[2] = { 0 };
	ShortToChar(appDatalen, tempDataBuff);
	QueueWrite(2, tempDataBuff, pDataToApp);
	QueueWrite(1, &devType, pDataToApp);
	QueueWrite(1, &devId, pDataToApp);
	ShortToChar(logId, tempDataBuff);
	QueueWrite(2, tempDataBuff, pDataToApp);
	QueueWrite(1, &lifeTime, pDataToApp);
	QueueWrite(appDatalen - PROTCL_OUT_FONT_LEN, pAppData, pDataToApp);
	ret = 1;
	return ret;
}


/*
* 功能：RSSP协议接收数据
*
* 参数：
* 输入参数：
* 输出参数：  QueueStruct* pOutToApp 输出给应用的数据帧格式
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RsspRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rsspRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpRsspUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRsspUnitInfo->DataToApp);
		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, rsspRcvDataBuff, &gpRsspUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 2 + PROTCL_OUT_FONT_LEN;
		RcvDataToAppQueue(rsspRcvDataBuff[0], rsspRcvDataBuff[1], tmpLogid, 0, tempDataLen, rsspRcvDataBuff + 2, pOutToApp);
	}

	ret = 1;
	return ret;
}

/*
* 功能：RSSP2协议接收数据
*
* 参数：
* 输入参数：
* 输出参数：  QueueStruct* pOutToApp 输出给应用的数据帧格式
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 Rssp2RcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rssp2RcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 logId = 0;
	UINT8 devType = 0;
	UINT8 devId = 0;

	while (QueueStatus(&gpRssp2UnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRssp2UnitInfo->DataToApp);
		tempDataLen = ShortFromChar(tempDatabuff);/* 获取长度 */
		tempDataLen -= 4;
		QueueRead(1, &devType, &gpRssp2UnitInfo->DataToApp);
		QueueRead(1, &devId, &gpRssp2UnitInfo->DataToApp);
		QueueRead(2, tempDatabuff, &gpRssp2UnitInfo->DataToApp);
		logId = ShortFromChar(tempDatabuff);		
		QueueRead(tempDataLen, rssp2RcvDataBuff, &gpRssp2UnitInfo->DataToApp);/* 获取组包后的数据 */

		tempDataLen = tempDataLen + PROTCL_OUT_FONT_LEN;/* 应用数据的长度,加上5个 */

		RcvDataToAppQueue(devType, devId, logId, 0, tempDataLen, rssp2RcvDataBuff, pOutToApp);
	}

	ret = 1;
	return ret;
}


/*
* 功能：RP协议接收数据
*
* 参数：
* 输入参数：
* 输出参数：  QueueStruct* pOutToApp 输出给应用的数据帧格式
* 返回值：
*    UINT8 1 正常，其他不正常
*/
UINT8 RpRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rpRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT8 tempType = 0;
	UINT8 tempId = 0;
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpRedunUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRedunUnitInfo->DataToApp);
		QueueRead(1, &tempType, &gpRedunUnitInfo->DataToApp);
		QueueRead(1, &tempId, &gpRedunUnitInfo->DataToApp);

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen - 2, rpRcvDataBuff, &gpRedunUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 2 + PROTCL_OUT_FONT_LEN;
		RcvDataToAppQueue(tempType, tempId, tmpLogid, 0, tempDataLen, rpRcvDataBuff, pOutToApp);
	}

	ret = 1;
	return ret;
}



/* 读取队列中的数据 */
UINT8 readQueueDataToApp(QueueStruct *rcvQueueData, UINT8 OutNetFlag, UINT16 locDevName, PROTCL_RCV_DATA_TYPE* pDataToApp)
{
	UINT8 ret = 0;
	UINT8 retFun = 0;
	UINT8 i=0;                               /*  循环使用 */
	UINT16 destDevName=0;
	UINT8 protclType = 0;
	PROTCL_APP_TO_STRU tempProtclAppStru;
	QueueRingID pRingQue = NULL;
	CM_BOOL mbRet = CM_FALSE;
	UINT32 len = 0;

	for (i = 0;i < PROTCL_TYPE_NUM + 1;i++)
	{
		/*清除长度信息,避免后续拷贝出错;*/
		pDataToApp[i].rcvDataLen = 0;
	}

	while (2 < QueueStatus(rcvQueueData))
	{
		retFun = 0;
		ret = AppReceiveDataAnalysis(rcvQueueData, &tempProtclAppStru);
		if (ret == 1)
		{
			TypeId2DevName(tempProtclAppStru.devType, tempProtclAppStru.devId, &destDevName);
			retFun = dsuProtclGetProtclType(locDevName, destDevName, &protclType);
		}
		else
		{
			/* 获取协议类型失败，放弃接收 */		
			gManageErrInfo |= ERR_PRTCL_MANAGE_DATA_ANALYSIS_ERR;
			break;
		}

		if (1 == retFun)
		{
			if (DSU_PROTCL_RSSP2 == protclType)/* RSSP-II协议中没有目的设备类型与ID */
			{
				protclType = 5;
				pRingQue = NULL;
				Prtcl_Manage_GetRQ(tempProtclAppStru.devType * 256 * 256 * 256 + tempProtclAppStru.devId * 256 * 256 + OutNetFlag * 256 + tempProtclAppStru.devLogId, &pRingQue);
				QUEUE_RING_Write(pRingQue, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
				mbRet = CM_TRUE;
				while (CM_TRUE == mbRet)
				{
					mbRet = Prtcl_Base_Check(pRingQue, &Rssp2PrtclHead, tempProtclAppStru.pAppData, MAX_FRAME_SIZE, &len);
					if (CM_TRUE == mbRet)
					{
						tempProtclAppStru.appDataLen = (UINT16)len;/* 通信数据长度 */
						/*将新一帧数据往缓冲区尾部填写*/
						ShortToChar(tempProtclAppStru.appDataLen + 4, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);/*适配层帧长度=通信数据长度+4字节帧头*/
						/*修改缓冲区总长度*/
						pDataToApp[protclType].rcvDataLen += 2;

						/* 增加发送方的设备类型 */
						*(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen) = tempProtclAppStru.devType;
						pDataToApp[protclType].rcvDataLen++;

						/* 增加发送方的设备ID */
						*(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen) = tempProtclAppStru.devId;
						pDataToApp[protclType].rcvDataLen++;

						/* 增加逻辑ID */
						ShortToChar(tempProtclAppStru.devLogId, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);
						pDataToApp[protclType].rcvDataLen += 2;

						/*拷贝接收数据至缓冲区尾部*/
						memcpy(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
						/*更新缓冲区总长度*/
						pDataToApp[protclType].rcvDataLen += tempProtclAppStru.appDataLen;
					}
				}
			}
			else
			{
				/*协议类型不再填写,使用全局规定的各协议类型*/
				/*pDataToApp[protclType].proType = protclType;*/
				/*将新一帧数据往缓冲区尾部填写*/
				ShortToChar(tempProtclAppStru.appDataLen, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);
				/*修改缓冲区总长度*/
				pDataToApp[protclType].rcvDataLen += 2;
				/*拷贝接收数据至缓冲区尾部*/
				memcpy(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
				/*更新缓冲区总长度*/
				pDataToApp[protclType].rcvDataLen += tempProtclAppStru.appDataLen;
			}
		}
		else
		{
			gManageErrInfo |= ERR_PRTCL_MANAGE_TYPE_ERR;			
			break;
		}
	}

	ret =1;
	
	return ret;
}


/*
* 功能：接收应用层的数据解析
*
* 参数：
* 输入参数：  const QueueStruct *pAppQueueData    应用输入到协议的数据队列
* 输出参数：  PROTCL_APP_TO_STRU *protclAppStru   解析后的数据
* 返回值：
*    UINT8 1 收到完整数据帧，0 无数据
*/
UINT8 AppReceiveDataAnalysis(QueueStruct *pAppQueueData, PROTCL_APP_TO_STRU *pProtclAppStru)
{
	UINT8 retVal = 0;
	UINT8 tempBuff[2] = { 0 };
	UINT16 tempDatalen = 0;
	
	UINT8 protclType = GetCbtcSysType();	
	
	QueueScan(2, tempBuff, pAppQueueData);	/* 肯定存在两个字节 */
	tempDatalen = ShortFromChar(tempBuff);
	
	if ((UINT32)(tempDatalen + 2) <= QueueStatus(pAppQueueData))
	{
	
		QueueRead(2, tempBuff, pAppQueueData);/* 长度 */	
		QueueRead(1, &pProtclAppStru->devType, pAppQueueData);/* 类型 */
		QueueRead(1, &pProtclAppStru->devId, pAppQueueData);/* ID */
						
    	if ((DQU_CBTC_HLT == protclType)||(DQU_CBTC_CC == protclType)) 
		{
			if(4 <= tempDatalen)
			{
				pProtclAppStru->appDataLen = tempDatalen - 4;
				QueueRead(2, tempBuff, pAppQueueData);
				pProtclAppStru->devLogId = ShortFromChar(tempBuff);
				QueueRead((tempDatalen - 4), pProtclAppStru->pAppData, pAppQueueData);/* 应用数据 */
				
				retVal = 1;
			}
			else
			{
				retVal = 0;
			}
		}
		else
		{
			pProtclAppStru->appDataLen = tempDatalen-2;
			QueueRead((tempDatalen-2),pProtclAppStru->pAppData,pAppQueueData);/* 应用数据 */
			retVal = 1;
		}
		
		
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
*	将Buff的格式的数据压入队列
*/
UINT8 SetDataToQue(UINT16 dataLen, UINT8 *buff, QueueStruct* q)
{
	UINT8 ret = 0;
	QueueClear(q);
	ret = QueueWrite(dataLen, buff, q);
	return ret;
}



/************************************链路状态查询******************************************/
/*
* 功能：SFP协议查询链路状态
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*     0x33：链路在准备建立链接状态
*     0x4e：链路在等待建立链接状态
*/
UINT8 GetSfpLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = ProtclCheckLnkStatus(destType, destId, gpsfpUnitInfo);

	return retVal;
}

/*
* 功能：RSR协议查询链路状态
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*/
UINT8 GetRsrLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 locDevName = 0;
	UINT16 DestDevName = 0;
	UINT16 rsspSrcAdd = 0;
	INT16   bsdAdd = 0;
	UINT16	destRsspAdd = 0;
	TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
	TypeId2DevName(destType, destId, &DestDevName);

	if (destType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else
	{
		retVal = dsuZCvsCIOutInfo(locDevName, DestDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	if (1 == retVal)
	{
		retVal = RsrLnkStatus(rsspSrcAdd, bsdAdd, destRsspAdd, gpRsrUnitInfo);
	}

	return retVal;
}

/*
* 功能：RSSP协议查询链路状态
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*/
UINT8 GetRsspLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = Rssp2LnkStatus(destType, destId, gpRsspUnitInfo);

	return retVal;
}

/*
* 功能：RSSP2协议查询链路状态
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*/
UINT8 GetRssp2LnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = RsspIILnkStatus(destType, destId, logId, gpRssp2UnitInfo);

	return retVal;
}

/*******************************************删除链路子函数开始 **********************************************/

/*
* 功能：SFP协议删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 SfpDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = ProtclDeleteLnk(destType, destId, gpsfpUnitInfo);

	return retVal;
}

/*
* 功能：Rp协议删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 RpDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 destDevName = 0;
	TypeId2DevName(destType, destId, &destDevName);
	retVal = RpLnkDelete(destDevName, gpRedunUnitInfo);

	return retVal;
}

/*
* 功能：RSR协议删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 RsrDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 locDevName = 0;
	UINT16 DestDevName = 0;
	UINT16 rsspSrcAdd = 0;
	INT16   bsdAdd = 0;
	UINT16	destRsspAdd = 0;
	TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
	TypeId2DevName(destType, destId, &DestDevName);

	if (destType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else
	{
		retVal = dsuZCvsCIOutInfo(locDevName, DestDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	if (1 == retVal)
	{
		retVal = RsrLnkDelete(rsspSrcAdd, bsdAdd, destRsspAdd, gpRsrUnitInfo);
	}

	return retVal;
}

/*
* 功能：RSSP协议删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 RsspDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = Rssp2LnkDelete(destType, destId, gpRsspUnitInfo);

	return retVal;
}

/*
* 功能：RSSP2协议删除链路
*
* 参数：
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
* 返回值：
*     1：删除链路成功
*     0：删除链路失败
*/
UINT8 Rssp2DelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = RsspIILnkDelete(destType, destId, logId,  gpRssp2UnitInfo);

	return retVal;
}

/********************************************刷新链路相关函数*********************************************/
/*
* 功能：SFP刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 SfpReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpsfpUnitInfo)
	{
		retVal = ProtclFreshLnkMngr(slen_input, pLnkStatusData, gpsfpUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* 功能：RP刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 RpReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpRedunUnitInfo)
	{
		retVal = RpRefresh(slen_input, pLnkStatusData, gpRedunUnitInfo);
	}
	else
	{
		retVal = 1;

	}

	return retVal;
}

/*
* 功能：RSR刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 RsrReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpRsrUnitInfo)
	{
		retVal = RsrRefresh(slen_input, pLnkStatusData, gpRsrUnitInfo);
	}
	else
	{
		retVal = 1;

	}
	return retVal;
}

/*
* 功能：RSSP刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 RsspReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;

	if (NULL != gpRsspUnitInfo)
	{
		retVal = Rssp2Refresh(slen_input, pLnkStatusData, gpRsspUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* 功能：RSSP2刷新链路状态
*
* 参数：
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值：
*     0：刷新链路失败
*     1：刷新链路成功
*/
UINT8 Rssp2ReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;

	if (NULL != gpRssp2UnitInfo)
	{
		retVal = RsspIIRefresh(slen_input, pLnkStatusData, gpRssp2UnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/********************************************销毁空间相关函数*********************************************/
/*
* SFP协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 SfpFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpsfpUnitInfo)
	{
		retVal = ProtclFreeSpace(gpsfpUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* RP协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 RpFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRedunUnitInfo)
	{
		retVal = RpFreeSpace(gpRedunUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* RSR协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 RsrFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRsrUnitInfo)
	{
		retVal = RsrFreeSpace(gpRsrUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/*
* RSSP协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 RsspFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRsspUnitInfo)
	{
		retVal = Rssp2FreeSpace(gpRsspUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}
/*
* RSSP2协议销毁协议模块
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 Rssp2FreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRssp2UnitInfo)
	{
		retVal = RsspIIFreeSpace(gpRssp2UnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/*
* 销毁全局变量的空间
* 参数：
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/

UINT8 FreeSpaceFunc(void)
{
	UINT8 retVal = 1;
	UINT8 i = 0;/* 循环使用 */

	if (NULL != ptemProtclRcvDataStru)
	{
		for (i = 1;i < (PROTCL_MGECFG_MAXCNT + 1);i++)
		{
			if ((NULL != ptemProtclRcvDataStru[i].pData))
			{
				free((void*)ptemProtclRcvDataStru[i].pData);
				ptemProtclRcvDataStru[i].pData = NULL;
			}
		}
		free((void*)ptemProtclRcvDataStru);
		ptemProtclRcvDataStru = NULL;
	}

	for (i = 0;i < (PROTCL_MGECFG_MAXCNT + 1);i++)
	{
		if (NULL != protclSndCfgStru[i].pProtclAppStru)
		{
			free((void*)protclSndCfgStru[i].pProtclAppStru);
			protclSndCfgStru[i].pProtclAppStru = NULL;
		}
	}
	return retVal;
}



/***********Getting**&**Setting****封装******************************************************/
/**
* 功能描述:  获取协议配置信息结构体指针
* 输入参数:  无
* 输出参数:  ProtclConfigInfoStru **pUnifyInfo;
* 返回值:    UINT8
0
******************************************************/
UINT8  protcl_GetProtclCfgStruPt(ProtclConfigInfoStru **pUnifyInfo)
{
	UINT8 retVal = 1;

	*pUnifyInfo = pProtclCfgInfo;

	return retVal;
}
/**
* 功能描述:  设置协议配置信息结构体指针
* 输入参数:  ProtclConfigInfoStru *pUnifyInfo;
* 输出参数:  无
* 返回值:    UINT8
0
******************************************************/
UINT8  protcl_SetProtclCfgStruPt(ProtclConfigInfoStru *pUnifyInfo)
{
	UINT8 retVal = 1;

	pProtclCfgInfo = pUnifyInfo;

	return retVal;
}

/**
* 功能描述:  获取协议应用信息转换结构体指针
* 输入参数:  无
* 输出参数:  PROTCL_APP_TO_STRU **pProtclStru;
* 返回值:    UINT8
0
******************************************************/
UINT8  protcl_GetProtclAppToStruPt(PROTCL_APP_TO_STRU **pProtclStru)
{
	UINT8 retVal = 1;

	*pProtclStru = pProtclAppToStru;

	return retVal;
}
/**
* 功能描述:  设置协议应用信息转换结构体指针
* 输入参数:  PROTCL_APP_TO_STRU *pProtclStru;
* 输出参数:  无
* 返回值:    UINT8
0
******************************************************/
UINT8  protcl_SetProtclAppToStruPt(PROTCL_APP_TO_STRU *pProtclStru)
{
	UINT8 retVal = 1;

	pProtclAppToStru = pProtclStru;

	return retVal;
}

/***********异常防护函数**************************************************************/
/** 零函数
* 主要用于函数指针清零。
* 防止异常情况使用空的函数指针而导致的不良后果。
**************************************************/
UINT8  protcl_nul_Init(UINT8* nul_FileName, UINT8 nul_Num)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_Snd(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8 * pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_Rcv(QueueStruct* PQueueStru)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_Lnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_DelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_ReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_End(void)
{
	UINT8  retval = 1;

	return retval;
}

/*******************************************************************************************************
* 功能描述: 	   返回所有协议及协议适配层丢包或非宕机错误码信息
* 输入参数: 		应用获取错误编码的数组vProtclErrorInfo
* 输入输出参数: 	vProtclErrorInfo
* 输出参数: 		返回vProtclErrorInfo错误信息码
* 全局变量: 	  无
* 返回值:			无
*******************************************************************************************************/

UINT8 GetProtclErrorInfo(UINT32 vProtclErrorInfo[])
{
	UINT8 index = 0,retval =0;
	
	vProtclErrorInfo[index++] = GetSfpErrorInfo();
	vProtclErrorInfo[index++] = GetRpErrorInfo();
	vProtclErrorInfo[index++] = GetRsrErrorInfo();
	vProtclErrorInfo[index++] = GetRsspErrorInfo();
	vProtclErrorInfo[index++] = GetManageErrorInfo();
	retval =1;
	return retval;
}


/*******************************************************************************************************
* 功能描述: 	   所有协议及协议适配层丢包和非宕机错误码复位
* 输入参数: 		应用获取错误编码的数组vProtclErrorInfo
* 输入输出参数: 	vProtclErrorInfo
* 输出参数: 		vProtclErrorInfo
* 全局变量: 	  无
* 返回值:			无
*******************************************************************************************************/

UINT8 ReSetProtclErrorInfo(UINT32 vProtclErrorInfo[])
{
	UINT8 ret = 1;
	ReSetSfpErrorInfo();
	ReSetRpErrorInfo();
	ReSetRsrErrorInfo();
	ReSetRsspErrorInfo();
	ReSetManageErrorInfo();
	memset(vProtclErrorInfo,0,sizeof(UINT32)*PRTCL_ERR_INFO_LEN);
	return ret;
}
/*******************************************************************************************************
* 功能描述: 	   设置RSSP协议无发送应用数据周期数接口函数
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gRsspErrorInfo
* 全局变量: 	  gRsspErrorInfo
* 返回值:			1:成功
*******************************************************************************************************/

UINT8 SetRsspConfigData(UINT8 vNoSendDataNum)
{
	UINT8 ret = 1;
	SetRsspNoSendAppData(vNoSendDataNum);
	return ret;
}
