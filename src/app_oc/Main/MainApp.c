/********************************************************
*                                                                                                            
* �� �� ���� MainAPP.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-26
* ��    �ߣ� ������
* ���������� ����Ӧ��������
* �޸ļ�¼��   
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
* ���������� ����Ӧ�ó�ʼ������
* ����˵���� void
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1: ��ʼ���ɹ�      
*/
UINT8 CiAppInitial(void)
{
	UINT8 retVal = CI_SUCCESS;

    /*��ʼ����������*/
    if (InitialProcess() != CI_SUCCESS)
    {
        retVal = CI_ERROR;			
	}
	/*��ʼ��OC����*/
	OCFirstPeriodInitial();
	return retVal;
}


/*
* ���������� ��������
* ����˵���� void
* ����ֵ  �� void      
*/
UINT8 MainProcess(void)
{
	UINT8 retVal = CI_SUCCESS;
	UINT8 localCiType = GetLocalCiType();
#ifdef LOGPRINT

    /*2OO2ƽ̨��ӡ�汾��*/
#ifdef PLATFORM_2OO2
        
    /*��ӡ����ģʽ*/
    debug_infor_printf("-----------------OC  V1-----------------\n");
    
    /*��ӡ�汾��*/

	/*����ڲ��汾��:��Ʒ���.ģ����.���汾��.�Ӱ汾��*/
	debug_infor_printf("SOFTWARE_INSIDE_VERSION:%d.%d.%d.%d\n",VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION);

	/*���ɰ汾��:��Ŀ���.��Ʒ���.��ִ�б��.���ɴ���*/
	debug_infor_printf("SOFTWARE_INTEGRATION_VERSION:%d.%d.%d.%d\n",VER_INTE_PROJECT,VER_INTE_PRODUCT,VER_INTE_EXE,VER_INTE_TIME);
   

#endif/*2OO2ƽ̨��־����*/

    /*��ӡ���ں�*/
	debug_infor_printf("\n\nSystemCycleNum=%d\n", Get2oo2CurCycleNum());/*2oo2ƽ̨��ӡϵͳ���ں�*/
    if (CI_START_LOCK_YES == GetStartLockFlag())
    {/*�ϵ����� ��ӡ �ϵ�����״̬*/

/*2OO2ƽ̨��ӡ*/
#ifdef PLATFORM_2OO2

    debug_infor_printf("\n\n StartLock \n", Get2oo2CurCycleNum());/*2oo2ƽ̨��ӡϵͳ���ں�*/

#endif/*2OO2ƽ̨��־����*/
    }

#endif/*��ӡ��־����*/

	/*OC���ݳ�ʼ������*/
	OCInitial();
	/*������������*/
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
	UINT16 wATSLockIndex=DEFAULT_ZERO;     /*ATS�����������±�*/
	UINT16 wRegionBlockIndex=DEFAULT_ZERO;     /*ATS�����������±�*/
	UINT16 wATSDeleteIndex=DEFAULT_ZERO;     /*ATS�����������±�*/
	UINT16 wVOBClockIndex=DEFAULT_ZERO;
	UINT16 wVOBCSwitchCmdIndex=DEFAULT_ZERO;
	UINT16 wPSDIndex = DEFAULT_ZERO;


#if 0
	if (5 == CycleNum)
	{
		/*ATS��ռ������*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*��ȡ�豸���*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*��ȡ�豸���*/
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
		/*ATS��ռ������*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SourceID = 0x0301;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0003;   /*��ȡ�豸���*/
		g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSLockIndex].SwitchID = 0x0003;   /*��ȡ�豸���*/
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
	/*ATS����������*/
	if (0 == inputflag)
	{
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0001;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*��ȡ�豸���*/
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
		g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
		wATSLockIndex++;
		g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
		inputflag = 1;
	}
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = 0x0001;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = 0x0001;   /*��ȡ������*/
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = 0x0005;   /*��ȡ�豸���*/
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
	g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
	wATSLockIndex++;
	g_stOCRevCmdDataStru.CommandNum_ATSLock=wATSLockIndex;
#endif

#if 0
	if (0 == inputflag)
	{
		/*ATS�����������*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = 0x0001;
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = 0x0001;   /*��ȡ������*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0x55;   /*��������0xff-ȫ���� 0x55-���� 0xaa-����*/
		g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
		wRegionBlockIndex++;
		g_stOCRevCmdDataStru.CommandNum_RegionBlock = wRegionBlockIndex;
		inputflag = 1;
	}


	/*�ϵ��������*/
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
		/*��������*/
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
    ������: vOCInitial()
      ����: OC���ݳ�ʼ��     
      ����: ��ʼ��
      ��������
    ����ֵ: ��
==============================================================================*/
 void OCInitial(void)
{ 
	UINT16 wInitialIndex=DEFAULT_ZERO;          /*��ʼ��ѭ���±�*/
	UINT16 i =DEFAULT_ZERO;
	UINT16 j =DEFAULT_ZERO;
	g_OutputIVOCOnce = FLAG_UNSET;
	memset(&g_stOCRevCmdDataStru,DEFAULT_ZERO,sizeof(g_stOCRevCmdDataStru));

	/*��ʼ����������ӡ*/
	g_wSignalCommandSendNum = DEFAULT_ZERO;
	/*g_wCommandNum_Switch_Send = DEFAULT_ZERO;*/
	g_stOC.ElecPowerState  = ELEC_NONE_OUT;
	memset(&g_stOCRevCmdResultStru, DEFAULT_ZERO, sizeof(g_stOCRevCmdResultStru));
	
#if 0
	/*��ʼ���г�������Ϣ*/
	for(wInitialIndex=0;wInitialIndex<TRAIN_MAX;wInitialIndex++)
	{
		if (g_stOC.TrainInfoFlagArray[wInitialIndex] != 0 && g_stOC.TrainCommStateArray[wInitialIndex])
			g_stOC.TrainInfoArray[wInitialIndex].Contact_CT = DEFAULT_ZERO;
	}

	/*3������δ�յ���Ϣ*/
	if(g_stOC.WSLoseInputFlag>LOSE_WS_INPUT_MAX_CYCLE) 
	{
		DebugPrint(PRINT_PERIOD,"00.002:Lost WSInfo 3 Cycle\r\n!");
		for(i=0;i<g_stOC.SwitchNum;i++)
		{	
			g_stOC.SwitchStateArray[i].Position=DEFAULT_0XFF;
		}
	
		/*���ü�������������ID*/
		for(i=0;i<g_stOC.AcNum;i++)
		{
			g_stOC.AcStateArray[i].State=DEFAULT_0XFF;
		}
	
		/*���ý���ͣ����ť������ID*/
		for(i=0;i<g_stOC.EmpNum;i++)
		{
			g_stOC.EmpStateArray[i].State=DEFAULT_0XFF;
		}
	
		/*����������������ID*/
		for(i=0;i<g_stOC.PsdNum;i++)
		{
			g_stOC.PsdStateArray[i].State=DEFAULT_0XFF;
		}

		/*���������۷���ť������ID*/
		for(i=0;i<g_stOC.DRBNum;i++)
		{
			g_stOC.DRBStateArray[i].State=DEFAULT_0XFF;
		}
	
	}
	else
	{
		/*ʲôҲ����*/
	}
#endif
}
