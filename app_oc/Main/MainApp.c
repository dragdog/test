/********************************************************
*                                                                                                            
* 文 件 名： MainAPP.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-26
* 作    者： 联锁组
* 功能描述： 联锁应用主程序
* 修改记录：   
*
********************************************************/ 

#include "MainApp.h"
#include "OCmain.h"
#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "DataConfigure.h"

UINT8 inputflag = 0;
/*
* 功能描述： 联锁应用初始化函数
* 参数说明： void
* 返回值  ： 0: 初始化失败
*			 1: 初始化成功      
*/
UINT8 CiAppInitial(void)
{
	UINT8 retVal = CI_SUCCESS;

    /*初始化配置数据*/
    if (InitialProcess() != CI_SUCCESS)
    {
        retVal = CI_ERROR;			
	}
	/*初始化OC数据*/
	OCFirstPeriodInitial();
	return retVal;
}


/*
* 功能描述： 主处理函数
* 参数说明： void
* 返回值  ： void      
*/
UINT8 MainProcess(void)
{
	UINT8 retVal = CI_SUCCESS;
	UINT8 localCiType = GetLocalCiType();
#ifdef LOGPRINT

    /*2OO2平台打印版本号*/
#ifdef PLATFORM_2OO2
        
    /*打印运行模式*/
    debug_infor_printf("-----------------OC  V1-----------------\n");
    
    /*打印版本号*/

	/*软件内部版本号:产品编号.模块编号.主版本号.子版本号*/
	debug_infor_printf("SOFTWARE_INSIDE_VERSION:%d.%d.%d.%d\n",VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION);

	/*集成版本号:项目编号.产品编号.可执行编号.集成次数*/
	debug_infor_printf("SOFTWARE_INTEGRATION_VERSION:%d.%d.%d.%d\n",VER_INTE_PROJECT,VER_INTE_PRODUCT,VER_INTE_EXE,VER_INTE_TIME);
   

#endif/*2OO2平台标志结束*/

    /*打印周期号*/
	debug_infor_printf("\n\nSystemCycleNum=%d\n", Get2oo2CurCycleNum());/*2oo2平台打印系统周期号*/
    if (CI_START_LOCK_YES == GetStartLockFlag())
    {/*上电锁闭 打印 上电锁闭状态*/

/*2OO2平台打印*/
#ifdef PLATFORM_2OO2

    debug_infor_printf("\n\n StartLock \n", Get2oo2CurCycleNum());/*2oo2平台打印系统周期号*/

#endif/*2OO2平台标志结束*/
    }

#endif/*打印标志结束*/

	/*OC数据初始化函数*/
	OCInitial();
	/*解析输入数据*/
	ParseInputData();

#ifdef DEBUGINPUT
	Debuginput();
#endif
	
	OCPeriodMain();

	PackOutputData();
	
	return retVal;
}

void Debuginput(void)
{
	UINT16 wATSLockIndex=DEFAULT_ZERO;     /*ATS锁命令数组下标*/
	UINT16 wRegionBlockIndex=DEFAULT_ZERO;     /*ATS锁命令数组下标*/
	UINT16 wATSDeleteIndex=DEFAULT_ZERO;     /*ATS锁命令数组下标*/
	UINT16 wVOBClockIndex=DEFAULT_ZERO;
	UINT16 wVOBCSwitchCmdIndex=DEFAULT_ZERO;
	UINT16 wPSDIndex = DEFAULT_ZERO;


#if 0
	if (5 == CycleNum)
	{
		/*ATS独占锁调试*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*获取设备编号*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*获取设备编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;

		wATSLockIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
		g_stOCRevCmdDataStru.CommandNum_Switch=wATSLockIndex;
	

		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID = 0x0005;
		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].ZeroLable = FLAG_SET;
		wATSDeleteIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLockDelete=wATSDeleteIndex;
	}
#endif

#if 0
	if (10 == CycleNum)
	{
		/*ATS独占锁调试*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0003;   /*获取设备编号*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SwitchID = 0x0003;   /*获取设备编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;

		wATSLockIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
		g_stOCRevCmdDataStru.CommandNum_Switch=wATSLockIndex;


		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID = 0x0003;
		g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].ZeroLable = FLAG_SET;
		wATSDeleteIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLockDelete=wATSDeleteIndex;
	}
#endif

#if 0
	/*ATS共享锁调试*/
	if (0 == inputflag)
	{
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0001;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*获取设备编号*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
		wATSLockIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
		inputflag = 1;
	}
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0001;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*获取命令编号*/
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*获取设备编号*/
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
	wATSLockIndex++;
	g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
#endif

#if 0
	if (0 == inputflag)
	{
		/*ATS区域封锁调试*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = 0x0001;
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = 0x0001;   /*获取命令编号*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0x55;   /*封锁区域：0xff-全区域 0x55-上行 0xaa-下行*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
		wRegionBlockIndex++;
		g_stOCRevCmdDataStru.CommandNum_RegionBlock = wRegionBlockIndex;
		inputflag = 1;
	}


	/*上电解锁调试*/
	g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OnceOrTwice = ELEC_ONECE;
	g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OC_ID = LocalOcId;
	
#endif

#if 0
	if (30 == CycleNum)
	{
		g_stOC.TrainInfoFlagArray[0]=1;
		g_stOC.TrainInfoArray[0].ID_CT = 0x1401;
		g_stOC.TrainInfoArray[0].Contact_CT = LINK_APPLY;
		g_stOC.TrainInfoArray[0].CycNum_LoseContact = 0x0;
		g_stOC.TrainInfoArray[0].Cycle = g_dwOCCycle;

		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wVOBCSwitchCmdIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = 0x1401;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SourceID = 0x1401;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = 0x0001;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SwitchID = 0x0001;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
		wVOBClockIndex++;
		wVOBCSwitchCmdIndex++;
		g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
		g_stOCRevCmdDataStru.CommandNum_Switch=wVOBCSwitchCmdIndex;
	}
#endif

#if 0
	if (45 == CycleNum)
	{
	g_stOC.TrainInfoFlagArray[0]=1;
	g_stOC.TrainInfoArray[0].ID_CT = 0x1401;
	g_stOC.TrainInfoArray[0].Contact_CT = LINK_APPLY;
	g_stOC.TrainInfoArray[0].CycNum_LoseContact = 0x0;
	g_stOC.TrainInfoArray[0].Cycle = g_dwOCCycle;

	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = 0x1401;
	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = 0x0001;
	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
	g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
	wVOBClockIndex++;
	g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
	}
#endif

#if 0
	if (3 == CycleNum)
	{
		/*打开屏蔽门*/
		g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = 0x1401;
		g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = 0x0001;
		g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_OPEN;
		wPSDIndex++;
		g_stOCRevCmdDataStru.CommandNum_PSD=wPSDIndex;
	}
#endif

#if 0
	 if (3 == CycleNum)
	 {
	 g_stOC.TrainInfoFlagArray[1]=1;
	 g_stOC.TrainInfoArray[1].ID_CT = 0x1403;
	 g_stOC.TrainInfoArray[1].Contact_CT = LINK_APPLY;
	 g_stOC.TrainInfoArray[1].CycNum_LoseContact = 0x0;
	 g_stOC.TrainInfoArray[1].Cycle = g_dwOCCycle;

	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = 0x1403;
	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = 0x0005;
	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
	 g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
	 wVOBClockIndex++;
	 g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
	 }
#endif

#if 0
	if (4 == CycleNum)
	{
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = 0x1401;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = 0x0005;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
		g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
		wVOBClockIndex++;
		g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
	}

#endif

}

/**=============================================================================
    函数名: vOCInitial()
      描述: OC数据初始化     
      功能: 初始化
      参数：无
    返回值: 无
==============================================================================*/
 void OCInitial(void)
{ 
	UINT16 wInitialIndex=DEFAULT_ZERO;          /*初始化循环下标*/
	UINT16 i =DEFAULT_ZERO;
	UINT16 j =DEFAULT_ZERO;
	g_OutputIVOCOnce = FLAG_UNSET;
	memset(&g_stOCRevCmdDataStru,DEFAULT_ZERO,sizeof(g_stOCRevCmdDataStru));

	/*初始化命令结果打印*/
	g_wSignalCommandSendNum = DEFAULT_ZERO;
	/*g_wCommandNum_Switch_Send = DEFAULT_ZERO;*/
	g_stOC.ElecPowerState  = ELEC_NONE_OUT;
	memset(&g_stOCRevCmdResultStru, DEFAULT_ZERO, sizeof(g_stOCRevCmdResultStru));
	
#if 0
	/*初始化列车连接信息*/
	for(wInitialIndex=0;wInitialIndex<TRAIN_MAX;wInitialIndex++)
	{
		if (g_stOC.TrainInfoFlagArray[wInitialIndex] != 0 && g_stOC.TrainCommStateArray[wInitialIndex])
			g_stOC.TrainInfoArray[wInitialIndex].Contact_CT = DEFAULT_ZERO;
	}

	/*3个周期未收到信息*/
	if(g_stOC.WSLoseInputFlag>LOSE_WS_INPUT_MAX_CYCLE) 
	{
		DebugPrint(PRINT_PERIOD,"00.002:Lost WSInfo 3 Cycle\r\n!");
		for(i=0;i<g_stOC.SwitchNum;i++)
		{	
			g_stOC.SwitchStateArray[i].Position=DEFAULT_0XFF;
		}
	
		/*配置计轴区段数量和ID*/
		for(i=0;i<g_stOC.AcNum;i++)
		{
			g_stOC.AcStateArray[i].State=DEFAULT_0XFF;
		}
	
		/*配置紧急停车按钮数量和ID*/
		for(i=0;i<g_stOC.EmpNum;i++)
		{
			g_stOC.EmpStateArray[i].State=DEFAULT_0XFF;
		}
	
		/*配置屏蔽门数量和ID*/
		for(i=0;i<g_stOC.PsdNum;i++)
		{
			g_stOC.PsdStateArray[i].State=DEFAULT_0XFF;
		}

		/*配置无人折返按钮数量和ID*/
		for(i=0;i<g_stOC.DRBNum;i++)
		{
			g_stOC.DRBStateArray[i].State=DEFAULT_0XFF;
		}
	
	}
	else
	{
		/*什么也不做*/
	}
#endif
}
