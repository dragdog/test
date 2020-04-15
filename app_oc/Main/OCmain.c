/* ***********************************************************************
*
* �ļ���      ��  ocmain.c
* ��Ȩ˵��    ��  �������ؿƼ��������ι�˾
* �汾��      ��  1.0
* ����ʱ��    ��  2016-6-13
* ����        ��  ��һ��
* ��������    ��  ����Ӧ�ó������ڵ�.
* ʹ��ע��    ��  
* �޸ļ�¼    ��  2016-6-13 			��һ��				  ����
                                                                      
*********************************************************************** */
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

#include "TypesDef.h"
#include "CommonTypes.h"
#include "PointCommandManage.h"
#include "GLOBALVAR.h"
#include "OCmain.h"
#include "DataConfigure.h"
#include "ScadeOCPeriodMain.h"

#pragma comment(lib,"ws2_32.lib")


/**=============================================================================
    ������: OCPeriodMain
      ����: �ϵ��ʼ��      
      ����: ��
      ��������
    ����ֵ: ��
==============================================================================*/
void OCFirstPeriodInitial(void)
{

	UINT32  ret=0;                                                /*������֤SOCKET�������Ƿ�ɹ�*/
	UINT16  PrintIndex=0;                                         /*��дά������ʹ�õľֲ�����*/
	INT32 Ret;
	UINT32 i;
	UINT j = 0;
	
	/*��ʼ��ȫ�ֽṹ��*/
	memset(&g_stOC,DEFAULT_ZERO,sizeof(g_stOC));	
	/*��ʼ��ȫ������*/
	memset(&g_stOCRevCmdDataStru,DEFAULT_ZERO,sizeof(g_stOCRevCmdDataStru));
	
	g_stOC.RegionBlockFlag=REGION_UNBLOCK;
	g_stOC.ElecPowerFlag=ELEC_LOCK;
	g_stOC.TMCInputFlag = FLAG_UNSET;
	g_stOC.bRRBfITS = FLAG_UNSET;
	g_stOC.SysRunMode = MODE_VBTC;

	/*���ü�������������ID*/
	for(i=0;i<g_stOC.AcNum;i++)
	{
		g_stOC.AcStateArray[i].State = 0xff;
	}
	/*���ý���ͣ����ť������ID*/
	for(i=0;i<g_stOC.EmpNum;i++)
	{
		g_stOC.EmpStateArray[i].State = 0xff;
	}

	/*OC��������*/
	DataConfigure();
}
/**=============================================================================
    ������: OCPeriodMain
      ����: �������к������      
      ����: ������ÿ���ڰ���һ��˳������๦��ģ����е��� ����Э����������ڲ�ģ���ϵ������
      ��������
    ����ֵ: ��
==============================================================================*/
void OCPeriodMain(void)
{  

	/*��ȡϵͳʱ��*/
	struct tm * Local = NULL;
	time_t t = 0;
	inC_ScadeOCPeriodMain inData;
	outC_ScadeOCPeriodMain outData;
	
	/*OC�߼�����Scade��*/
	memcpy(&inData.inATSLockDelCountDown_SA, &g_ATSLockDeleteCountDown, sizeof(inData.inATSLockDelCountDown_SA));
	ScadeOCPeriodMain(&inData, &outData);
	/*Scade�������*/
	ScadeOutputManage(&outData);
	/*����ʹ�ã������豸*/
	DeviceCommandManage();

	/*�ۻ����ڼ���*/
	g_dwOCCycle += 1;
}

/**=============================================================================
    ������: ScadeOutputManage
      ����: Scade�������      
      ����: ��ģ��������뱾��
      ��������
    ����ֵ: ��
==============================================================================*/
void ScadeOutputManage(outC_ScadeOCPeriodMain* outC)
{
	g_stOC.RegionBlockFlag = outC->outRegionBlockFlag;
	g_stOC.ElecPowerState = outC->outElecPowerState;
	g_stOC.HoldTrainNum = outC->outNumOfHoldTrainCmdData;
	g_stOC.JumpTrainNum = outC->outNumOfJumpTrainCmdData;
	g_stOC.RegionBlockPositionStruct = outC->outRegionBlockPosition_S;
	memcpy(g_stOC.TrainInfoFlagArray,outC->outTrainFlag_A,sizeof(g_stOC.TrainInfoFlagArray));
	memcpy(g_stOC.TrainCommStateArray,outC->outTrainCommState_A,sizeof(g_stOC.TrainCommStateArray));
	memcpy(g_stOC.SignalInfoArray,outC->outSignalInfo_SA,sizeof(g_stOC.SignalInfoArray));
	memcpy(g_stOC.SwitchFaultStateArray,outC->outSwitchFaultState_A,sizeof(g_stOC.SwitchFaultStateArray));
	memcpy(g_stOC.SwitchInfoArray,outC->outSwitchInfo_SA,sizeof(g_stOC.SwitchInfoArray));
	memcpy(g_stOC.SwitchLockTypeArray,outC->outSwitchLockType_A,sizeof(g_stOC.SwitchLockTypeArray));
	memcpy(g_stOC.SwitchELockArray,outC->outSwitchELock_SA,sizeof(g_stOC.SwitchELockArray));
	memcpy(g_stOC.SwitchSLockNumArray,outC->outNumOfSwitchSLock_A,sizeof(g_stOC.SwitchSLockNumArray));
	memcpy(g_stOC.SwitchSLockFlagArray,outC->outSwitchSLockFlag_AA,sizeof(g_stOC.SwitchSLockFlagArray));
	memcpy(g_stOC.SwitchSLockArray,outC->outSwitchSLock_SAA,sizeof(g_stOC.SwitchSLockArray));
	memcpy(g_ATSLockDeleteCountDown,outC->outATSLockDelCountDown_SA,sizeof(ATSLockDelete_CountDown_Struct) * CMD_MAX);
	memcpy(g_stOC.MultSwitchStateArray,outC->outMultSwitch_SA,sizeof(g_stOC.MultSwitchStateArray));
	memcpy(g_stOC.HoldTrainCmdDataFlagArray,outC->outHoldTrainCmdDataFlag_A,sizeof(g_stOC.HoldTrainCmdDataFlagArray));
	memcpy(g_stOC.HoldTrainCmdDataArray,outC->outHoldTrainCmdData_SA,sizeof(g_stOC.HoldTrainCmdDataArray));
	memcpy(g_stOC.JumpTrainCmdDataFlagArray,outC->outJumpTrainCmdDataFlag_A,sizeof(g_stOC.JumpTrainCmdDataFlagArray));
	memcpy(g_stOC.JumpTrainCmdDataArray,outC->outJumpTrainCmdData_SA,sizeof(g_stOC.JumpTrainCmdDataArray));
	/*memcpy(g_stOC.TSRDataArray,outC->outTsrData_SA,sizeof(g_stOC.TSRDataArray));*/ /* �޸���ʱ������Ϣ�����м��㣬ֻ����ת�����������θô��� */
	memcpy(g_stOC.PhySecBlockIdFlagArray,outC->outPhySecBlockIdFlag_A,sizeof(g_stOC.PhySecBlockIdFlagArray));
	memcpy(g_stOC.PhySecBlockIdArray,outC->outPhySecBlockId_A,sizeof(g_stOC.PhySecBlockIdArray));

	if (ELEC_TWICE_OUT == g_stOC.ElecPowerState)
	{
		g_stOC.ElecPowerFlag = ELEC_UNLOCK;
	}
}

UINT16 getOCid()
{
	return g_stOC.Id;
}

/**=============================================================================
    ������: DeviceCommandManage()
      ����: �豸���������      
      ����: 
      ��������
    ����ֵ: ��
==============================================================================*/
void DeviceCommandManage(void)
{
	UINT16 wPointIndex=0;           /*���������±�*/
	UINT16 wPsdIndex=0;           /*�����������±�*/
	UINT16 wCmdIndex=0;           /*���������±�*/
#if 0
	for (wPointIndex=0;wPointIndex<g_stOC.SwitchNum;wPointIndex++)
	{
		for (wCmdIndex=0;wCmdIndex<g_wCommandNum_Switch_Send;wCmdIndex++)
		{
			if (g_stOC.SwitchStateArray[wPointIndex].Id == g_stOCSendSwitchCmdDataStruct[wCmdIndex].SwitchID)
			{
				if (LOCK_MAIN == g_stOCSendSwitchCmdDataStruct[wCmdIndex].LockPositionNormal_Reverse)
				{
					g_stOC.SwitchStateArray[wPointIndex].Position = SWITCH_POSITION_MAIN;
				}
				else if (LOCK_SIDE == g_stOCSendSwitchCmdDataStruct[wCmdIndex].LockPositionNormal_Reverse)
				{
					g_stOC.SwitchStateArray[wPointIndex].Position = SWITCH_POSITION_SIDE;
				}
				else
				{

				}
			}
		}
	}

	for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
	{
		for (wCmdIndex=0;wCmdIndex<g_stOCRevCmdDataStru.CommandNum_PSD;wCmdIndex++)
		{
			if (g_stOC.PsdStateArray[wPsdIndex].Id == g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wCmdIndex].PSDID)
			{
				g_stOC.PsdStateArray[wPsdIndex].DriveState = g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wCmdIndex].Open_Close;
				g_wPsdCommandSendNum++;
			}
		}
	}
#endif
}