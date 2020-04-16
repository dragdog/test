#include "dsuRssp2Func.h"
#include "string.h"
#include "CommonMemory.h"
#include "Convert.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquIdmapInit.h"

DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /*用于保存RSSP2表信息的结构体对象*/
DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* 当前正在操作的数据库 */

/*VOBC内部编号转4字节互联互通编号*/
UINT8 dquVobcName2OutDevId(IN UINT16 VobcName, IN UINT8 EndId, OUT UINT32 *pOutDevId);
/*4字节ETCSID转VOBC内部编号*/
UINT8 dquEtcsId2VobcName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pEndId);

UINT8 dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru)
{
	UINT8 ret = 0;
	if (NULL != pDsuRssp2Stru)
	{
		g_dsuRssp2Stru = pDsuRssp2Stru;
		ret = 1;
	}
	else
	{
		g_dsuRssp2Stru = NULL;
		ret = 0;
	}
	return ret;
}
/************************************************************************
* 功能描述：安全通信协议RSSP2协议查询函数初始化函数，用于检查数据正确性，并将数据结构化。
使用协议查询函数之前调用一次
* 输入参数：
FSName 文件名或者数据地址
* 返 回 值：1，成功
*           0，失败，数据有错误
************************************************************************/
UINT8 dsuRssp2Init(UINT8* FSName)
{
	UINT8 chReturnValue = 1; /*返回值*/
	UINT8 TempRet;
	UINT8* pData = NULL;       /*用于读取数据的指针*/
	UINT32 dataLen = 0;        /*数据长度*/
	UINT32 i = 0;
	UINT32 j = 0;
	UINT32 IbookLen = 0;

	if (NULL != g_dsuRssp2Stru)
	{
		chReturnValue = 1;
	}
	else
	{
#if 1  /* TO BE DONE 仅限上电初始化模式的临时版本插桩，协议初始化的具体方式待定 */
		g_dsuRssp2Stru = &dsuRssp2InfoStru;
		chReturnValue = 1;
#else
		chReturnValue = 0;
#endif
	}

	/******************************************表7*****************************************/
	/*初始化rssp2 协议栈层间共享配置 */
	/*读取数据*/
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen%DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE)))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);  /*计算表格中数据的个数*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = (DSU_RSSP2_WS_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
			{
				chReturnValue = 0;
			}			
		}
		if (1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru+i)->RemoteEtcsType = *(pData + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAITsyn = LongFromChar(pData + 1 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnPeriod = LongFromChar(pData + 5 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAISNDiffMax = ShortFromChar(pData + 9 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECAlarm = LongFromChar(pData + 11 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnValue = LongFromChar(pData + 15 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIMaxErrValue = LongFromChar(pData + 19 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIUpdFailCountMax = LongFromChar(pData + 23 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->MASLTsynTimeOut = LongFromChar(pData + 27 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETsynTimeOut = LongFromChar(pData + 31 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETconTimeOut = LongFromChar(pData + 35 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETSNCheckMode = *(pData + 39 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
			}/* end for i */	
			g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	/******************************************表8*****************************************/
	/*初始化rssp2 点对点安全连接配置表 */
	/*读取数据*/
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen % DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE)))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE);  /*计算表格中数据的个数*/

		if (0<IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = (DSU_RSSP2_LOC_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
			{
				chReturnValue = 0;				
			}
		}
		if (1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->LocalEtcsType = *(pData + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_Tupd = LongFromChar(pData + 1 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_EcPeriod = ShortFromChar(pData + 5 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	/******************************************表9*****************************************/
	/* 初始化rssp2 当前TCP连接通道配置表 */
	/* 读取数据 */
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_DEV_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (dataLen > 0))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE);  /*计算表格中数据的个数*/
		if (0 < IbookLen)
		{			
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = (DSU_RSSP2_DEV_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
			{
				chReturnValue = 0;
			}			
		}
		
		if(1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2DevCfgStru, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->EtcsId = LongFromChar(pData + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->Key, 24, (pData + 4 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 24);
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Ip, 4, (pData + 28 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4);				
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Ip, 4, (pData + 32 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4); 
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Port = ShortFromChar(pData + 36 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Port = ShortFromChar(pData + 38 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2DevCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	return chReturnValue;
}

/************************************************************************
功能：内存释放函数
输入：无
输出：无
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dsuRsspIIFreeSpace(void)
{
	UINT8 rt = 0;

	if (NULL != g_dsuRssp2Stru)
	{
		if (NULL != g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = NULL;
		}

		if (NULL != g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = NULL;
		}

		if (NULL != g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2DevCfgStru);
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = NULL;
		}
		rt = 1;
	}
	return rt;
}

/************************************************************************
功能：2字节内部编号和2字节逻辑编号转4字节ETCSID
输入：
IN UINT16 DevName     2字节内部编号（高位设备类型 低位设备索引号)
IN UINT16 LogId       vobc时是使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
                      其它设备时无意义
输出：
OUT UINT32 *pEtcsId    4字节ETCSID
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquDevName2EtcsId(IN UINT16 DevName, IN UINT16 LogId, OUT UINT32 *pEtcsId)
{
	UINT8 ret = 0;
	UINT32 hlhtId = 0;
	UINT32 etcsId = 0;
	UINT8 devType = 0;
	UINT8 devId = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;

	devType = (UINT8)(DevName / 256);
	devId = (UINT8)(DevName & 0x00FF);

	/*入口检查*/
	if (NULL == pEtcsId)
	{
		ret = 0;
	}
	else
	{
		*pEtcsId = 0;
		ret = 1;
	}
	if (NULL == g_dsuRssp2Stru)
	{
		ret = 0;
	}

	/* 查找互联互通编号 */
	if (1 == ret)
	{		
		switch (devType)
		{
		case DEVTYPE_ZC:
			ret = dquQueryOutDevId(QFUN_IDMAP_ZC_ID, devId, &hlhtId);
			break;
		case DEVTYPE_ATS:
			ret = dquQueryOutDevId(QFUN_IDMAP_ATS_ID, devId, &hlhtId);
			break;
		case DEVTYPE_CI:
			ret = dquQueryOutDevId(QFUN_IDMAP_CI_ID, devId, &hlhtId);
			if((((hlhtId)>>29)&0x00000007) == 0x01) /* CQ HOTFIX TH联锁 */
			{
				if(LogId==1||LogId==2)
				{
					hlhtId += LogId-1;
				}
			}
			break;
		case DEVTYPE_VOBC:
			ret = dquVobcName2OutDevId(DevName, (UINT8)LogId, &hlhtId);
			break;
		case DEVTYPE_NDSU:
			ret = dquQueryOutDevId(QFUN_IDMAP_DSU_ID, devId, &hlhtId);
			break;
		case DEVTYPE_AOM:
			ret = dquVobcName2OutDevId(DevName, (UINT8)LogId, &hlhtId);
			break;
		default:
			ret = 0;
			break;
		}
	}

	/*查找对应的ETCSID*/
	if (1 == ret)
	{
		ret = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((hlhtId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				etcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}

	if (1 == ret)
	{
		*pEtcsId = etcsId;
	}

	return ret;
}

/************************************************************************
功能：4字节ETCSID转2字节内部编号和2字节逻辑编号
输入：
IN UINT32 EtcsId    ETCSID
输出：
OUT UINT16 *pDevId     2字节内部编号（高位设备类型 低位设备索引号)
OUT UINT16 *pLogicId   vobc时是使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
                       其它设备时无意义
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquEtcsId2DevName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pLogId)
{
	UINT8 ret = 0;
	UINT16 devName = 0;
	UINT16 logId = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((NULL != pDevName) && (NULL != pLogId))
	{
		GetEtcsTypeFromEtcsId(EtcsId, &type);
		switch (type)
		{
		case DEVICE_TYPE_ZC:/* 查ZC编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ZC_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ZC * 256;
			break;
		case DEVICE_TYPE_ATP:/* 查VOBC编号表 */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		case DEVICE_TYPE_ATS:/* 查ATS编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ATS_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ATS * 256;
			break;
		case DEVICE_TYPE_DSU:/* 查DSU编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_DSU_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_NDSU * 256;
			break;
		case DEVICE_TYPE_CI:/* 查CI编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId, ETCSID_MASK, &devName);
			if(1 != ret)  /* CQ HOTFIX TH联锁 */
			{
				ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId-1, ETCSID_MASK, &devName);
				if(1 == ret)
				{
					logId = 2;
				}
			}
			devName += DEVTYPE_CI * 256;
			break;
		case DEVICE_TYPE_AOM:/* 查AOM编号表 */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		ret = 0;
	}		
	if (1 == ret)
	{
		*pDevName = devName;
		*pLogId = logId;
	}
	return ret;
}


/************************************************************************
功能：vobc内部编号转4字节互联互通编号
输入：
IN UINT16 VobcName      2字节内部编号（高位设备类型 低位设备索引号)
IN UINT8 EndId        vobc使用端编号编号 1-TC1端，2-TC2端 0-2IP车（首尾冗余车）
输出：
OUT UINT32 *pOutDevId    4字节互联互通编号
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquVobcName2OutDevId(IN UINT16 VobcName, IN UINT8 EndId, OUT UINT32 *pOutDevId)
{
	UINT8 ret = 0;
	UINT32 hlhtId = 0;/*互联互通编号*/
	UINT32 vobcId[2] = { 0,0 };
	UINT8 devType = 0;
	UINT8 devId = 0;

	/* 输入检查 */
	if (NULL != pOutDevId)
	{
		*pOutDevId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	if (NULL == g_dsuRssp2Stru)
	{
		ret = 0;
	}
	if ((VOBC_ENDID_TC1 != EndId) && (VOBC_ENDID_TC2 != EndId) && (VOBC_ENDID_REDUN != EndId))
	{
		ret = 0;
	}
	devType = (UINT8)(VobcName / 256);
	if ((DEVTYPE_VOBC != devType)&&(DEVTYPE_AOM != devType))
	{
		ret = 0;
	}
	
	/* 查询VOBC互联互通编号 */
	if (1 == ret)
	{
		devId = (UINT8)(VobcName & 0x00FF);
		if (DEVTYPE_VOBC == devType)
		{
			ret = dquQueryOutVobcDevId(devId, vobcId);
		}
		else
		{
			ret = dquQueryOutAOMDevId(devId, vobcId);
		}
		switch (EndId)
		{
		case VOBC_ENDID_TC1:
			hlhtId = vobcId[0];
			break;
		case VOBC_ENDID_TC2:
			hlhtId = vobcId[1];
			break;
		case VOBC_ENDID_REDUN:/*2IP车（首尾冗余车）使用"VOBC编号1"*/
			hlhtId = vobcId[0];
			break;
		default:
			/* 不可达分支 */
			ret = 0;
			break;
		}
	}

	if (1 == ret)
	{
		*pOutDevId = hlhtId;
	}

	return ret;
}

/************************************************************************
功能：ETCSID转vobc内部编号
输入：
IN UINT32 EtcsId    ETCSID
输出：
OUT UINT16 *pDevName     2字节内部编号（高位设备类型 低位设备索引号)
OUT UINT16 *pEndId		vobc使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquEtcsId2VobcName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pEndId)
{
	UINT8 ret = 0;
	UINT8 endId = 0;
	UINT16 index = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((NULL != pDevName) && (NULL != pEndId))
	{
		*pDevName = 0;
		*pEndId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		endId = GET_VOBC_END_ID(EtcsId);
		switch (endId)
		{
		case 1:
			*pEndId = VOBC_ENDID_TC1;
			break;
		case 2:
			*pEndId = VOBC_ENDID_TC2;
			break;
		case 3:
			*pEndId = VOBC_ENDID_REDUN;
			break;
		default:
			ret = 0;
			break;
		}
	}

	GetEtcsTypeFromEtcsId(EtcsId, &type);
	if ((DEVICE_TYPE_ATP != type)&&(DEVICE_TYPE_AOM != type))
	{
		ret = 0;
	}

	if (1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			ret = dquQueryVobcDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
		else
		{
			ret = dquQueryAOMDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
	}

	if(1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			*pDevName = DEVTYPE_VOBC * 256 + index;
		}
		else
		{
			*pDevName = DEVTYPE_AOM * 256 + index;
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCSID获取RSSP2协议使用的IP端口
输入：
IN UINT32 EtcsId    ETCSID
输出：
OUT UINT8 *pNum,    TCP冗余通道组数(一对红蓝网IP端口为一组，即4IP系统为2组)
OUT DSU_IP_PORT *pRedIpPort,   红网IP端口数组(端口号65535表示随机端口)
OUT DSU_IP_PORT *pBlueIpPort   篮网IP端口数组(与红网IP端口数组相同角标的元素组成一对冗余通道)
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquGetIpPortByEtcsId(IN UINT32 EtcsId, OUT UINT8 *pNum, OUT DSU_IP_PORT *pRedIpPort, OUT DSU_IP_PORT *pBlueIpPort)
{
	UINT8 ret = 0;
	UINT32 i = 0;
	UINT8 j = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;
	if ((NULL != pNum) && (NULL != pRedIpPort) && (NULL != pBlueIpPort) && (NULL!= g_dsuRssp2Stru))
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				CommonMemCpy(pRedIpPort[j].Ip, 4, pDevCfg->RedIpPort.Ip, 4);
				pRedIpPort[j].Port = pDevCfg->RedIpPort.Port;
				CommonMemCpy(pBlueIpPort[j].Ip, 4, pDevCfg->BlueIpPort.Ip, 4);
				pBlueIpPort[j].Port = pDevCfg->BlueIpPort.Port;
				j++;
				ret = 1;
			}
			pDevCfg++;
		}
		*pNum = j;
	}
	return ret;
}

/************************************************************************
功能：根据RSSP-2协议使用的IP端口获取ETCSID
描述：如果配置数据中配置端口号为65535（表示随机端口）则不校验传入的端口号，
	  只要IP匹配就输出对应的ETCSID
输入：
IN DSU_IP_PORT IpPort,   IP端口
输出：
OUT UINT32 *pEtcsId,    ETCSID
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquGetEtcsIdByIpPort(IN DSU_IP_PORT IpPort, OUT UINT32 *pEtcsId)
{
	UINT8 ret = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;
	INT32 rslt1 = 0;
	INT32 rslt2 = 0;
	if ((NULL != pEtcsId) && (NULL != g_dsuRssp2Stru))
	{
		*pEtcsId = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			rslt1 = memcmp(IpPort.Ip, pDevCfg->RedIpPort.Ip, 4);
			rslt2 = memcmp(IpPort.Ip, pDevCfg->BlueIpPort.Ip, 4);
			if (((0 == rslt1) && ((IpPort.Port == pDevCfg->RedIpPort.Port)||(0xFFFF == pDevCfg->RedIpPort.Port)))
				|| ((0 == rslt2) && ((IpPort.Port == pDevCfg->BlueIpPort.Port)||(0xFFFF == pDevCfg->BlueIpPort.Port))))
			{
				*pEtcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
功能：根据轨旁设备类型(ETCS类型)获取协议栈层间共享配置
输入：
IN UINT8 WsEtcsType 轨旁设备类型(ETCS类型)
输出：
OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg 协议栈共享配置
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquRssp2GetWaysideTypeCfg(IN UINT8 WsEtcsType, OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;

	if ((NULL != pRssp2RmtTypeCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}

	if (1 == tmpRet)
	{
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen; i++)
		{
			if (WsEtcsType == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i].RemoteEtcsType)
			{
				CommonMemCpy((UINT8*)pRssp2RmtTypeCfg, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU), (UINT8*)&g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i], sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCS设备类型获取本地设备类型配置数据
输入：
IN UINT8 LocalEtcsType 互联互通设备编号(4字节外部编号)
输出：
OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg RSSP2设备类型参数
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquRssp2GetLocalTypeCfg(IN UINT8 LocalEtcsType, OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;
	DSU_RSSP2_LOC_TYPE_CFG_STRU *pLocTypeCfg = NULL;
	if ((NULL != pRssp2LocTypeCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pLocTypeCfg = g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen; i++)
		{
			if (LocalEtcsType == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru[i].LocalEtcsType)
			{
				CommonMemCpy((UINT8*)pRssp2LocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU), (UINT8*)pLocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
			pLocTypeCfg++;
		}
	}
	return ret;
}


/************************************************************************
功能：根据ETCSID获取设备配置数据
输入：
IN UINT32 EtcsId ETCSID
输出：
OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg 点对点安全连接配置
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquRssp2GetDevCfg(IN UINT32 EtcsId, OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg=NULL;

	if ((NULL != pRssp2DevCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				CommonMemCpy((UINT8*)pRssp2DevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU), (UINT8*)pDevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU));
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCSID获取ETCS类型
输入：
IN UINT32 EtcsId, ETCSID
输出：
OUT RSSP2_DEVICE_TYPE *pDevType  ETCS类型
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 GetEtcsTypeFromEtcsId(UINT32 EtcsId, RSSP2_DEVICE_TYPE *pDevType)
{
	UINT8 ret = 0;
	UINT8 devType = 0;

	devType = (EtcsId / 256 / 256 / 256) & 0x1F;

	if (NULL != pDevType)
	{
		*pDevType = 0;
		if ((8 > devType) && (0 < devType))
		{
			*pDevType = devType;
			ret = 1;
		}
	}
	return ret;
}

/************************************************************************
功能：根据本方ETCSID获取本方的通信角色
输入：
IN UINT32 LocalEtcsId,  本地设备ETCSID
输出：
OUT UINT8 *pRole  本方通信角色 0发起方(客户端) 1接收方(服务端)
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 dquRssp2GetRole(IN UINT32 LocalEtcsId, OUT UINT8 *pRole)
{
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;
	UINT8 role = 0;
	UINT8 ret = 0;
	GetEtcsTypeFromEtcsId(LocalEtcsId, &type);
	if (NULL == pRole)
	{
		ret = 0;
	}
	else if ((DEVICE_TYPE_ZC == type) || (DEVICE_TYPE_CI == type) || (DEVICE_TYPE_ATS == type) || (DEVICE_TYPE_DSU == type))
	{
		role = 1;
		ret = 1;
	}
	else if ((DEVICE_TYPE_ATP == type)|| (DEVICE_TYPE_AOM == type))
	{
		role = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		*pRole = role;
	}
	return ret;
}