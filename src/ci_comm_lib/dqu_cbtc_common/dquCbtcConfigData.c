/************************************************************************
*
* 文件名   ：  
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  系统配置数据结构体定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dquCbtcConfigData.h"
#include "dfsDataRead.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"

static CBTC_CFG_DATA_STRU *qds_dsuCbtcConfigDataStru=NULL;/*CBTC配置信息用查询数据源*/
static CBTC_CFG_DATA_STRU *g_pCbtcConfigDataStru=NULL; /*指向初始化用数据库的指针*/

static dfsDataHead g_DFsDH;			/*FS文件的数据头结构体*/
static UINT8*  g_pDataAddr;         /*FS单元数据地址*/
static UINT32  g_DataSize;          /*FS单元数据大小（字节数）*/
static UINT16  g_RowCount;			/*表数据行数*/

static UINT16	g_wCurInitStruProg=0;/*当前结构体初始化进程，分步加载时使用*/
static UINT16   g_wCurMainIndex=0;  /*当前主处理索引，分步加载时使用*/

/*----------内部函数声明开始--------------*/

/*解析列车车型配置数据1*/
UINT8 dfsInitTrainModelConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*列车性能参数配置结构体2*/
UINT8 dfsInitTrainPerformConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*各子系统公用数据配置结构体3-CPK*/
UINT8 dfsInitSubSystemCommonConfigData_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*各子系统公用数据配置结构体3-FAO*/
UINT8 dfsInitSubSystemCommonConfigData_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*各子系统公用数据配置结构体3-HLT，依据互联互通数据结构V12.0.0修改，modify by lmy 20180314*/
UINT8 dfsInitSubSystemCommonConfigData_HLT(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*各子系统公用数据配置结构体3-DG，依据互联互通数据结构V12.0.0修改后，DG和HLT不再使用相同的初始化函数，故增加此函数，add by lmy 20180312*/
UINT8 dfsInitSubSystemCommonConfigData_DG(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*各子系统公用数据配置结构体3-CC*/
UINT8 dfsInitSubSystemCommonConfigData_CC(UINT8 *pDataAddr,UINT32 dataSize, UINT16 rowCount);
/*车辆段配置数据结构体4-FAO ，互联互通与此版本一至*/
UINT8 dfsInitTrainDepotConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*根据版本配置数据处理过程函数*/
static UINT8 initConfigCbtcInfoTaskTab(void);



/*----------内部函数声明结束--------------*/

/*初始化任务登记表*/
static INIT_TASK_TAB_STRU g_struInitTaskTab[]=
{
	{DQU_QFUN_TRUE, CBTC_TRAIN_TYPE_CONFIG_ID,	dfsInitTrainModelConfigData,		 5,1,0}, 	
	{DQU_QFUN_TRUE, CBTC_TRAIN_INFO_ID,			dfsInitTrainPerformConfigData,		 5,1,0},	
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_CPK,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_FAO,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_HLT,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_DG, 5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_CC, 5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DEPOT_ID,		dfsInitTrainDepotConfigData,		 5,1,0},	
};
/*初始化任务登记表项目数量*/
static const UINT16 MAX_INITTASK_COUNT=sizeof(g_struInitTaskTab)/sizeof(INIT_TASK_TAB_STRU);

/*********************************************
*函数功能：系统配置数据版本号获取
*参数说明：供其它系统调用查询函数版本号，防止集成错误。
*入口参数：无
*出口参数：无
*返回值：UINT32表示的版本号
*********************************************/
UINT32 GetDquCbtcVersion(void)
{
	UINT32 dquCbtcVer=0;
	dquCbtcVer = VerToU32SoftwareModule(DQU_CBTC_VER1,DQU_CBTC_VER2,DQU_CBTC_VER3,DQU_CBTC_VER4);
	return dquCbtcVer;
}

/*设置CBTC配置数据信息*/
UINT8 dquSetInitCbtcConfigInfoRef(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru)
{
	UINT8 bRetVal=0;
	if (NULL!=pCbtcConfigDataStru)
	{
		g_pCbtcConfigDataStru=pCbtcConfigDataStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*根据版本配置数据处理过程函数*/
/*备注：依据互联互通数据结构V12.0.0修改，modify by lmy 20180312*/
static UINT8 initConfigCbtcInfoTaskTab(void)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType = GetCbtcSysType();
	if (DQU_CBTC_CPK == cbtcVersionType)/*合库功能配置*/
	{
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*合库默认不执行，各子系统公用数据配置结构体3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*合库默认不执行，各子系统公用数据配置结构体3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*合库默认不执行，各子系统公用数据配置结构体3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*合库默认不执行，各子系统公用数据配置结构体3-CC*/
		g_struInitTaskTab[7].bIsEffective=DQU_QFUN_FASLE;/*合库默认不执行，车辆段配置*/
		rtnValue = 1;
	}	
	else if (DQU_CBTC_FAO == cbtcVersionType) /*FAO功能配置*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-CPK*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_HLT == cbtcVersionType) /*互联互通功能配置*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*互联互通默认不执行，各子系统公用数据配置结构体3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*互联互通默认不执行，各子系统公用数据配置结构体3-FAO*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*互联互通默认不执行，各子系统公用数据配置结构体3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_DG == cbtcVersionType)/*单轨功能配置*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-HLT*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_CC == cbtcVersionType)/*车车功能配置*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*DG默认不执行，各子系统公用数据配置结构体3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*FAO默认不执行，各子系统公用数据配置结构体3-DG*/
		rtnValue = 1;
	}
	else /*未出现的配置版本值，可能未配置标志位*/
	{
		rtnValue = 0;
	}

	return rtnValue;
}

/*系统配置数据初始化*/
UINT8 dquCbtcConfigDataInit(CHAR* FSName)
{
	UINT16	wIndex=0;
	UINT8   funcRtn = 0;
	UINT8	rtnValue = 0;
	UINT32	DataID=0;/*数据类型标识*/
	UINT8	*pDataAddr=NULL;
	UINT32	DataSize=0;
	UINT16	rowCount = 0;

	if(NULL!=g_pCbtcConfigDataStru)
	{

		/*读取FS文件,并验证文件*/
		if(1 != dataRead(FSName))
		{
			/*数据读取不成功*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*根据版本配置数据处理过程函数*/
			funcRtn =initConfigCbtcInfoTaskTab();
			if (0 == funcRtn)
			{
				rtnValue = 0;
				return rtnValue;
			}
		}

		/*索引初始化归零*/
		wIndex=0;
		do /*解析数据到相应的结构体*/
		{
			if (DQU_QFUN_TRUE == g_struInitTaskTab[wIndex].bIsEffective)/*判断这个初始化函数是否执行*/
			{
				/*获取配置数据*/
				DataID=g_struInitTaskTab[wIndex].DataTypeId;
				funcRtn=dquGetConfigDataNew( FSName, CBTC_CONFIGDATA_TYPE, DataID, &pDataAddr, &DataSize, &rowCount);
				if (funcRtn != 1)
				{
					/*读取数据失败*/
					rtnValue = 0;
					return rtnValue;
				}
				funcRtn=g_struInitTaskTab[wIndex].pExeFun(pDataAddr,DataSize,rowCount);
				if (0 == funcRtn)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else/*继续执行*/
				{
				}
			}

			wIndex++;
		} while (wIndex<MAX_INITTASK_COUNT);

		/*默认调用此接口自动关联查询数据源*/
		dquSetCurCbtcCfgInfoQueryDataSource(g_pCbtcConfigDataStru);

		rtnValue = 1;
	}
	else
	{
		rtnValue = 0;
	}

	return rtnValue;

}

/******************************************
*函数功能：系统配置数据初始化扩展接口
*参数说明： pDataBuf,传入数据块，
*			cbtcConfigDataStru,外部实体系统配置数据存储结构体指针
*			mode,处理模式，1上电一次加载，2运行时多次处理
*			timeSlice，执行的时间片（单位：毫秒）
*返回值：  0失败，1成功，2分步处理未完成
*****************************************/
UINT8 dquCbtcConfigDataInitExp(UINT8 *pDataBuf,CBTC_CFG_DATA_STRU *cbtcConfigDataStru,UINT8 mode,UINT16 timeSlice)
{
	UINT32 DataID;
	UINT16 wPos=0;
	UINT8 rtnValue=0; 
	UINT16 wFreqTimes=0;
	UINT8 bExeState=DQU_QFUN_FASLE;
	UINT32 dwOnceTime,dwUsedTime=0;
	UINT32 dwCurTimePoint,dwBeginTimePoint=0;

	UINT8 funcRtn;


	if(0==g_wCurMainIndex)/*主索引变量进入执行态的时候，就不能改变处理模式*/
	{
		g_pCbtcConfigDataStru=cbtcConfigDataStru;
		if (NULL==g_pCbtcConfigDataStru)
		{
			/*全局结构体指针空间未分配*/
			rtnValue = 0;
			return rtnValue;
		}
	}

	do /*上电加载持续运行保持控制*/
	{
		/*获取进入开始时间*/
		dwBeginTimePoint=dquGetCurrentTimeByMS();
		switch(g_wCurMainIndex)/*处理流程控制*/
		{
		case 0:
			/*解析FS文件数据头、数据索引信息*/
			funcRtn=dquAnalyzeFsDataHead(pDataBuf, &g_DFsDH);
			if (funcRtn != 1)
			{
				/*读取数据失败*/
				rtnValue = 0;
			}
			else
			{
				/*根据版本配置数据处理过程函数*/
				funcRtn =initConfigCbtcInfoTaskTab();
				if (0 == funcRtn)
				{
					rtnValue = 0;
				}
				else
				{
					g_wCurMainIndex=1;
					rtnValue = 2;
				}
			}
			break;
		case 1:		
			/*获取配置数据*/
			DataID=g_struInitTaskTab[g_wCurInitStruProg].DataTypeId;
			funcRtn=dquGetConfigDataExpNew(pDataBuf, &g_DFsDH, CBTC_CONFIGDATA_TYPE, DataID, &g_pDataAddr, &g_DataSize, &g_RowCount);
			if (funcRtn != 1)
			{
				/*读取数据失败*/
				rtnValue = 0;
			}
			else/*获取数据成功*/
			{
				g_wCurMainIndex=2;
				rtnValue = 2;
			}
			break;
		case 2:
			funcRtn=g_struInitTaskTab[g_wCurInitStruProg].pExeFun(g_pDataAddr,g_DataSize,g_RowCount);
			if(1==funcRtn)/*完成本次完整的处理*/
			{
				do 
				{
					g_wCurInitStruProg++;/*初始化结构体进行下一个阶段*/
					/*越过无效处理函数*/
				}while((DQU_QFUN_FASLE==g_struInitTaskTab[g_wCurInitStruProg].bIsEffective) && (g_wCurInitStruProg<MAX_INITTASK_COUNT));
				/*还有的初始化处理未完成*/
				if(g_wCurInitStruProg<MAX_INITTASK_COUNT)
				{	
					g_wCurMainIndex=1;
				}
				else /*if (g_wCurInitStruProg >=MAX_INITTASK_COUNT) 这个条件状态下，完成了所有结构体初始化处理*/
				{
					g_wCurMainIndex=3;
				}
				rtnValue = 2;
			}
			else/*处理出现异常*/
			{
				rtnValue = 0;
			}
							
			break;
		case 3:
			rtnValue = 1;
			break;
		default:
			rtnValue = 0;
			break;

		}
		/*处理完毕或异常退出*/
		if(2 != rtnValue) 
		{
			break;
		}

		/*时间统计及计算处理*/
		if (0x02 == mode)
		{
			/*获取当前时间*/
			dwCurTimePoint=dquGetCurrentTimeByMS();
			/*计算已经是使用过的时间*/
			dwOnceTime=dwCurTimePoint-dwBeginTimePoint;
			dwUsedTime+=dwOnceTime;
			/*使用的时间小于时间片*/
			if ((dwUsedTime < timeSlice) && (dwOnceTime > 0) )
			{
				/*剩下的处理时间够再执行一次处理的*/
				wFreqTimes=(timeSlice-dwUsedTime)/dwOnceTime;
				if (wFreqTimes >= 1)
				{
					bExeState=DQU_QFUN_TRUE;
				}
				else/*剩余时间不够下一次执行*/
				{
					bExeState=DQU_QFUN_FASLE;
				}
			}
			else/*超时退出*/
			{
				bExeState=DQU_QFUN_FASLE;
			}
		}

		/*处理模式为上电加载时一直到执行完,或执行状态为完毕*/	
	} while ((0x01 == mode) || (DQU_QFUN_TRUE == bExeState));

	return rtnValue;
}

/*编号对照表初始化完毕，执行编号对照表内部公共变量的清理工作*/
UINT8 dfsuCbtcConfigInitFinally(void)
{
	UINT16 wIndex=0;
	UINT8 bRetVal=0;

	g_wCurInitStruProg=0x0;/*当前结构体初始化进程，分步加载时使用*/
	g_wCurMainIndex=0; /*当前主处理索引，分步加载时使用*/

	bRetVal=1;
	return bRetVal;
}


/*---------------数据解析处理函数-------------------------*/

/**********************************************
函数功能：初始化[列车车型配置表]
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitTrainModelConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;
	static UINT32 rowDataLen = 0;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->TrainTypeConfigLen = rowCount;	
		
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;

			g_pCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainName = ShortFromChar(pData);
			pData += 2;

			g_pCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainType = *pData;
			pData += 1;
		}
	}	
		
	rtnValue = 1;
	return rtnValue;
}

/**********************************************
函数功能：初始化[列车性能参数]
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitTrainPerformConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;	
	UINT32 rowDataLen;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->TrainInfoLen = rowCount;	
		
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;

			/*列车车型*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainType = *pData;
			pData += 1;
			/*列车长度*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainLength = ShortFromChar(pData);
			pData += 2;
			/*列车最大牵引加速度*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainMaxTracAcc = *pData;
			pData += 1;
			/*线路最大下坡坡度*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].WorstGrade = ShortFromChar(pData);
			pData += 2;
		}
	}
		
	rtnValue = 1;
	return rtnValue;
}

/**********************************************
函数功能：初始化[各子系统公用数据配置]-CPK(要求有且仅有1行数据)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*填充数据结构体*/                      
		/*线路最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*临时限速最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTP校时时区差*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*安全防护距离*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*一次设置临时限速的最大个数*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*一个临时限速报文中包含的最大逻辑区段数量*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZC通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCI通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATS通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCI通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*cici通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*为解决ats显示上下行问题新增一个字段---严广学2012-12-17*/
		/*link逻辑方向上下行*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		/*满足ATO精准停车条件的MA长度，针对合库数据结构V16.0.0，系统配置数据各子系统公用数据增加字段“满足ATO精准停车条件的MA长度”，add by my 20170220*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;			

		rtnValue = 1;
	} 
	
	return rtnValue;
}

/**********************************************
函数功能：初始化[各子系统公用数据配置]-FAO(要求有且仅有1行数据)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*填充数据结构体*/
		/*线路最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*临时限速最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTP校时时区差*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*安全防护距离*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*一次设置临时限速的最大个数*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*一个临时限速报文中包含的最大逻辑区段数量*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCI通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCI通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*cici通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psd通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*link逻辑方向上下行*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		rtnValue = 1;
	} 
	
	return rtnValue;
}

/**********************************************
函数功能：初始化[各子系统公用数据配置]-HLT(要求有且仅有1行数据)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_HLT(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 i = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*填充数据结构体*/
		/*线路最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*临时限速最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTP校时时区差*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*安全防护距离*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*一次设置临时限速的最大个数*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*一个临时限速报文中包含的最大逻辑区段数量*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZC通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCI通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATS通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCI通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*cici通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psd通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*为解决ats显示上下行问题新增一个字段---严广学2012-12-17*/
		/*link逻辑方向上下行*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		g_pCbtcConfigDataStru->cbtcConfigData.SpanLineNum=ShortFromChar(pDataAddr); /*有效跨线线路数量，互联互通需求新增， add by qxt 20160811*/
		pDataAddr += 2;

		for (i=0;i<4;i++ )
		{
			g_pCbtcConfigDataStru->cbtcConfigData.LineID[i]=*pDataAddr; /*线路编号，互联互通需求新增， add by qxt 20160811*/
			pDataAddr += 1;
			g_pCbtcConfigDataStru->cbtcConfigData.OverlapLineLogDirUpAndDown[i]=*pDataAddr;  /*重叠区内各线路的系统定义逻辑方向与上下行方向的关系，互联互通需求新增， add by qxt 20160811*/
			pDataAddr += 1;
		}

		/*零速判断门限(cm/s)，依据互联互通数据结构V12.0.0添加，add by lmy 20180314*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZeroSpeedThreshold = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*满足ATO精准停车条件的MA长度，依据互联互通数据结构V12.0.0添加，add by lmy 20180314*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*预告磁钢到中性区起点距离 MS=MagneticSteel  NZ=NeutralZone，依据互联互通数据结构V13.0.0添加，add by sds 20180614*/
		g_pCbtcConfigDataStru->cbtcConfigData.StartDistanceOfMSToNZ = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*互联互通FAO接口兼容字段，依据互联互通数据结构V13.0.0添加，add by sds 20180614*/
		g_pCbtcConfigDataStru->cbtcConfigData.CompatibleOfHLHTFAO = *pDataAddr;
		pDataAddr += 1;

		/*正线是否动态测试字段，依据互联互通数据结构V13.0.0添加，add by sds 20180625*/
	    g_pCbtcConfigDataStru->cbtcConfigData.ValueOfLineDynamicTest = *pDataAddr;
		pDataAddr += 1;

		/*是否由VOBC处理MA障碍物,依据互联互通数据结构V14.0.0添加，add by qxt 20180719*/
		g_pCbtcConfigDataStru->cbtcConfigData.IsVOBCHandleObatacles = *pDataAddr;
		pDataAddr += 1;

		 /*允许同时进行动态测试(唤醒)的列车最大数量。,依据互联互通数据结构V14.0.0添加，add by qxt 20180719*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTrainNumOfAwake = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*项目标识(区分工程接口),依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
		g_pCbtcConfigDataStru->cbtcConfigData.TypeOfProject = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*ZC-ZC接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToZC = *pDataAddr;
		pDataAddr += 1;

		/*ZC-VOBC接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToVOBC = *pDataAddr;
		pDataAddr += 1;

		/*ZC-DSU接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToDSU = *pDataAddr;
		pDataAddr += 1;

		/*ZC-CI接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToCI = *pDataAddr;
		pDataAddr += 1;

		/*ZC-ATS接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToATS = *pDataAddr;
		pDataAddr += 1;

		/*VOBC-ATS接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfVOBCToATS = *pDataAddr;
		pDataAddr += 1;

		/*VOBC-CI接口协议版本号,依据互联互通数据结构V16.0.0添加，add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfVOBCToCI = *pDataAddr;
		pDataAddr += 1;

		rtnValue = 1;
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[各子系统公用数据配置]-DG(要求有且仅有1行数据)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_DG(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 i = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*填充数据结构体*/
		/*线路最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*临时限速最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTP校时时区差*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*安全防护距离*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*一次设置临时限速的最大个数*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*一个临时限速报文中包含的最大逻辑区段数量*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZC通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCI通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATS通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCI通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*cici通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zc通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psd通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*为解决ats显示上下行问题新增一个字段---严广学2012-12-17*/
		/*link逻辑方向上下行*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		g_pCbtcConfigDataStru->cbtcConfigData.SpanLineNum=ShortFromChar(pDataAddr); /*有效跨线线路数量，互联互通需求新增， add by qxt 20160811*/
		pDataAddr += 2;

		for (i=0;i<4;i++ )
		{
			g_pCbtcConfigDataStru->cbtcConfigData.LineID[i]=*pDataAddr; /*线路编号，互联互通需求新增， add by qxt 20160811*/
			pDataAddr += 1;
			g_pCbtcConfigDataStru->cbtcConfigData.OverlapLineLogDirUpAndDown[i]=*pDataAddr;  /*重叠区内各线路的系统定义逻辑方向与上下行方向的关系，互联互通需求新增， add by qxt 20160811*/
			pDataAddr += 1;
		}

		rtnValue = 1;
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[各子系统公用数据配置]-CC(要求有且仅有1行数据)，新增，add by sds 2019-6-13
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_CC(UINT8 *pDataAddr,UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*填充数据结构体*/                      
		/*线路最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = (UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed + 0.5);
		pDataAddr += 1;

		/*临时限速最高限速*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTP校时时区差*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*安全防护距离*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*一次设置临时限速的最大个数*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*一个临时限速报文中包含的最大限速区段数量*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfSpeed = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBC-VOBC通信故障判断时间（ms）*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBC-OC通信故障判断时间*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*VOBC-ITS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcItsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*TMC-ITS通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.TmcItsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
				
		/*TMC-OC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.TmcOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*ITS-OC通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.ItsOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*OC-PSD通信故障判断时间*/
		g_pCbtcConfigDataStru->cbtcConfigData.OcPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*系统定义最大保护区段长度*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*本线系统定义逻辑方向与上下行方向的关系*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		/*零速判断门限(cm/s)*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZeroSpeedThreshold = *pDataAddr;
		pDataAddr += 1;

		/*满足ATO精准停车条件的MA长度*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;			

		rtnValue = 1;
	} 
	else
	{
		rtnValue = 0;
	}

	return rtnValue;
}
/**********************************************
函数功能：初始化[车辆段配置信息]
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:1成功,0失败
**********************************************/
UINT8 dfsInitTrainDepotConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;
	UINT32 rowDataLen;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->DepotConfigLen = rowCount;
	
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;
			
			/*车辆段编号*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotID = *pData;
			pData += 1;
			/*车辆段所属联锁ID*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotCIID = ShortFromChar(pData);
			pData += 2;
			/*车辆段出库方向*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotDir = *pData;
			pData += 1;
		}

	}
	
	rtnValue = 1;
	return rtnValue;
}

/*-----------------------------数据查询接口函数-----------------------*/

/*设置当前查询函数据源*/
UINT8 dquSetCurCbtcCfgInfoQueryDataSource(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru)
{
	UINT8 bRetVal=0;
	if (NULL != pCbtcConfigDataStru)
	{
		qds_dsuCbtcConfigDataStru=pCbtcConfigDataStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/***********************************************
*函数功能：根据列车名称查询列出车型
*输入参数：TrainName列车名
*输出参数：列车车型
*返回值：0失败1成功
**********************************************/
UINT8 dquGetTrainType(UINT16 TrainName,UINT8* pTrainType)
{
	UINT32 i;
	UINT8 rtnValue = 0;

	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		for(i=0;i<qds_dsuCbtcConfigDataStru->TrainTypeConfigLen;i++)
		{
			if(qds_dsuCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainName == TrainName)
			{
				/*找到车型*/
				* pTrainType = qds_dsuCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainType;
				rtnValue = 1;
				return rtnValue;
			}
		}	
		/*没找到跳出循环*/
		rtnValue = 0;
	}
	else
	{

	}

	return rtnValue;
}

/*******************************************
*函数功能：跟据列车车型获得列车性能信息
*输入参数：TrainType列车车型
*输出参数：pCbtcTrainInfo列车性能信息结构体
*返回值： 0失败 1成功
*******************************************/
UINT8 dquGetCbtcTrainInfo(UINT8 TrainType,CBTC_TRAIN_INFO_STRU *pCbtcTrainInfo)
{
	UINT32 i=0;
	UINT8 rtnValue = 0;

	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		for (i=0;i<qds_dsuCbtcConfigDataStru->TrainInfoLen;i++)
		{
			if(TrainType==qds_dsuCbtcConfigDataStru->cbtcTrainInfo[i].TrainType)
			{
				*pCbtcTrainInfo = qds_dsuCbtcConfigDataStru->cbtcTrainInfo[i];
				rtnValue = 1;
				return rtnValue;
			}  
			/*没找到跳出循环*/
			rtnValue = 0;
		}
	}
	else
	{

	}

	return rtnValue;

}

/*获取各子系统公用数据配置表*/
UINT8 dquGetCbtcConfigData (CBTC_CONFIG_DATA_STRU* pCbtcConfigDataStru)
{
	UINT8 rtnValue = 0;
	
	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		*pCbtcConfigDataStru = qds_dsuCbtcConfigDataStru->cbtcConfigData;
		rtnValue = 1;
	}
	else
	{

	}
	return rtnValue;
}

/*********************************************
*函数功能：获得车辆段配置信息
*输入参数：无
*输出参数：pCbtcDepotInfo车辆段配置信息
*			Length配置信息数量
*返回值： 0失败 1成功
***********************************************/
UINT8 dquGetCbtcDepotInfo(CBTC_DEPOT_INFO_STRU *pCbtcDepotInfo, UINT16 *Length)
{
	UINT32 i=0;
	UINT8 rtnValue = 0;
	
	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		if((0 == qds_dsuCbtcConfigDataStru->DepotConfigLen) || (qds_dsuCbtcConfigDataStru->DepotConfigLen > 10))
		{
			/*没找到跳出循环*/
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			for (i=0;i<qds_dsuCbtcConfigDataStru->DepotConfigLen;i++)
			{
				*(pCbtcDepotInfo+i) = qds_dsuCbtcConfigDataStru->cbtcDepotInfo[i];
				(*Length)++;
			}
		}
		rtnValue = 1;
	}
	else
	{

	}

	return rtnValue;
}
