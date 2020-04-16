/* ***********************************************************************
*
* �ļ���      ��  DataConfigure.c
* ��Ȩ˵��    ��  �������ؿƼ��������ι�˾
* �汾��      ��  1.0
* ����ʱ��    ��  2016-6-13
* ����        ��  ��һ��
* ��������    ��  �������ã���Ҫ����OC��������Ρ�����ȡ�
* ʹ��ע��    ��  
* �޸ļ�¼    ��  2016-6-13 			��һ��				  ����
                                                                      
*********************************************************************** */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "CommonTypes.h"
#include "TypesDef.h"
#include "maintain.h"
#include "GLOBALVAR.h"
#include "DataConfigure.h"
#include "SystemParameterManage.h"
#include "SignalConfigDataManage.h"
#include "SwitchConfigDataManage.h"
#include "MultSwitchConfigDataManage.h"
#include "PSDConfigDataManage.h"
#include "ESBConfigDataManage.h"


/*===============================externȫ������===============================*/
extern UINT8  g_bszReadBuffer[SIZE_QUEUE_BUF];                                         /*���Ե����������*/
extern UINT8  g_bszQueueBuffer[SIZE_QUEUE_BUF];                                         /*����������ݻ���*/
extern UINT8  g_bszVOBCIN[SIZE_QUEUE_BUF];                                              /*�г�����������ݻ���*/
extern UINT8  g_bszVOBCIN1[SIZE_QUEUE_BUF];
extern UINT8  g_bszATSIN[SIZE_QUEUE_BUF];                                               /*ATS����������ݻ���*/
extern UINT8  g_bszLSCIN[SIZE_QUEUE_BUF];                                               /*LS����������ݻ���*/
extern UINT16 SwitchCurSum;
extern SwitchConfigDataStruct SwitchConfigDataStru[SWITCH_SUM_MAX];
extern UINT16 MultSwitchCurSum;
extern MultSwitchConfigDataStruct MultSwitchConfigDataStru[MULTSWITCH_SUM_MAX];
extern UINT16 SignalCurSum;
extern SignalConfigDataStruct SignalConfigDataStru[SIGNAL_SUM_MAX];
extern UINT16 PsdCurSum;
extern PsdConfigDataStruct PsdConfigDataStru[PSD_SUM_MAX];  
extern UINT16 PhysicalSectionCurSum;
extern UINT16 PsdCurSum;
extern UINT16 EsbCurSum;
extern EsbConfigDataStruct EsbConfigDataStru[ESB_SUM_MAX];
/*==============================externȫ�ֱ���=================================*/

void  FileRead2(const char * szFileName,UINT8 TempMArray[SIZE_QUEUE_BUF]);
void  FileRead(const char * szFileName,UINT8 TempMArray[SIZE_QUEUE_BUF]);
/**=============================================================================
    ������: FileRead()
      ����: �ļ���ȡ   
      ����: �Ӽ��±���ȡȫ������
      ��������
    ����ֵ: ��
==============================================================================*/
void  FileRead(const char * szFileName,UINT8 TempMArray[SIZE_QUEUE_BUF])
{
	UINT16 i=0;
	UINT16 j=0;

	FILE*Fd;
	errno_t err;
	err = fopen_s(&Fd,szFileName,"r");
	if(Fd==NULL)
	{
		/*DebugPrint(PRINT_PERIOD,"error�޷���\r\n");*/

	}
	else 
	{
		for(i=0;fscanf_s(Fd,"%x",&TempMArray[i])!=EOF;i++)
		{
		}
	}
}

void  FileRead2(const char * szFileName,UINT8 TempMArray[SIZE_QUEUE_BUF])
{
	UINT16 i=0;
	UINT16 j=0;
	char a[100];
	UINT8 b=0;

	
	FILE*Fd;
	errno_t err;
	memset(&a[0],0,100);
	err=fopen_s(&Fd,szFileName,"r");
	if(Fd==NULL)
	{
		/*DebugPrint(PRINT_PERIOD,"error�޷���\r\n");*/

	}
	else 
	{
		fgets(a,10000,Fd);
		for(i=0;i<30;i++)
		{
		b=atoi(&a[i]);
		}
	}
}

/**=============================================================================
    ������: DataConfigure()
      ����: OC���ݳ�ʼ��     
      ����: ����7���ߵĵ��ӵ�ͼ�����������ã��涨��ǰ�����OC������豸������ID�ȡ�
      ��������
    ����ֵ: ��
==============================================================================*/
void DataConfigure(void)
{   UINT8 i = 0;
	UINT8 j = 0;
	UINT8 bSwitchIndex = DEFAULT_ZERO;
	/*���õ�ǰOC��ID*/
	g_OCNO = LocalOcId;
	g_stOC.Id = 0x3c00 + LocalOcId;
	g_stOC.AcNum = PhysicalSectionCurSum;
	g_stOC.ManRegionNum = 0x03;

	for (i=0;i<SWITCH_SUM_MAX;i++)
	{
		if (LocalOcId == SwitchConfigDataStru[i].wBelongOcId)
		{
			g_stOC.SwitchStateArray[j].Id = SwitchConfigDataStru[i].SwitchId;
			g_stOC.SwitchLockTypeArray[j] = LOCK_E;
			g_stOC.SwitchELockArray[j].ELockId = ItsId;
			g_stOC.SwitchELockArray[j].ELockLevel = 1;
			g_stOC.SwitchELockArray[j].ELockSwitchState = LOCK_MAIN;	 /*��������*/
#ifdef NOWS
			g_stOC.SwitchStateArray[j].Position = SWTICH_POSITION_MAIN;	 /*��������*/
#endif
			j++;
		}
	}
	g_stOC.SwitchNum = SwitchCurSum;
	g_wCommandNum_Switch_Send = 0;

	j = 0;
	for (i=0;i<SIGNAL_SUM_MAX;i++)
	{
		if (LocalOcId == SignalConfigDataStru[i].wBelongOcId)
		{
			g_stOC.SignalStateArray[j].Id = SignalConfigDataStru[i].SignalId;
#ifdef NOWS
			g_stOC.SignalStateArray[j].State = SIGNAL_RED;
#endif
			j++;
		}
	}
	g_stOC.SignalNum = SignalCurSum;
	g_wSignalCommandSendNum = 0;

	j = 0;
	for (i=0;i<PSD_SUM_MAX;i++)
	{
		if (LocalOcId == PsdConfigDataStru[i].wBelongOcId)
		{
			g_stOC.PsdStateArray[j].Id = PsdConfigDataStru[i].PsdId;
#ifdef NOWS
			g_stOC.PsdStateArray[j].State = PSD_CLOSE;
#endif
			j++;
		}
	}
	g_stOC.PsdNum = PsdCurSum;

	j = 0;
	for (i=0;i<ESB_SUM_MAX;i++)
	{
		if (LocalOcId == EsbConfigDataStru[i].wBelongOcId)
		{
			g_stOC.EmpStateArray[j].Id = EsbConfigDataStru[i].EsbId;
#ifdef NOWS
			g_stOC.EmpStateArray[j].State = EMP_OFF;
#endif
			j++;
		}
	}
	g_stOC.EmpNum = EsbCurSum;

	g_MultSwitchInitSuccess = FLAG_SET;
	/*DebugPrint(PRINT_PERIOD,"00.003 PowerLocking:MultSwitch already Init!\r\n");*/

	/*g_stOC.PhySecBlockIdFlagArray[0] = 1;
	g_stOC.PhySecBlockIdArray[0] = 1;*/			/*����ʹ����������������*/

	j = 0;
	for (i=0;i<MULTSWITCH_SUM_MAX;i++)
	{
		if (LocalOcId == MultSwitchConfigDataStru[i].wBelongOcId)
		{
			g_stOC.MultSwitchStateArray[j].Id = MultSwitchConfigDataStru[i].MultSwitchId;
			g_stOC.MultSwitchStateArray[j].SwitchState = DEFAULT_ZERO;
			g_stOC.MultSwitchStateArray[j].ULockNum = DEFAULT_ZERO; 
			j++;
		}
	}
	g_stOC.MultSwitchNum = MultSwitchCurSum;

	///*���н�����߻�������״̬ȫΪ0*/
	//for (bSwitchIndex = 0; bSwitchIndex < g_stOC.MultSwitchNum; bSwitchIndex++)
	//{
	//	g_stOC.MultSwitchStateArray[bSwitchIndex].Id = bSwitchIndex+1;
	//}

	 g_stOC.RegionBlockPositionStruct.AllLine = REGION_BLOCK;/* �ϵ�ȫ�߷��� */
}

