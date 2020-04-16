/* ***********************************************************************
*
* �ļ���      ��  PointCommandManage.c
* ��Ȩ˵��    ��  �������ؿƼ��������ι�˾
* �汾��      ��  1.0
* ����ʱ��    ��  2016-6-22
* ����        ��  ��һ��
* ��������    ��  
* ʹ��ע��    ��  
* �޸ļ�¼    ��  2016-6-22 			��һ��				  ����
                                                                      
*********************************************************************** */

#include <stdio.h>
#include "TypesDef.h"
#include "CommonTypes.h"
#include "PointCommandManage.h"
#include "GLOBALVAR.h"
/*===============================externȫ������===============================*/
extern CommandData_Struct         g_stOCRevCmdDataStru;                                      /*OC���յĿ�����������*/
extern ATSLock_CommandData_STRU    stOCRevATSLockCmdDataStru[CMD_MAX];                   /*OC���յ�ATS��Դ��������������*/
extern VOBCLock_CommandData_STRU    stOCRevVOBCLockCmdDataStru[CMD_MAX];                 /*OC���յ�VOBC��Դ��������������*/
extern PSD_CommandData_STRU     stOCRevPSDCmdDataStru[CMD_MAX];                          /*OC���յ������ſ�����������*/


/**=============================================================================
    ������: PointBufferSendManage()
      ����: �������������жϺ���      
      ����: �жϻ����������еĵ��������Ƿ���Է��͡�������ܷ��ͣ������û�û��
	  �õ���Ķ�ռ������ɾ���õ������
      ��������
    ����ֵ: ���Է��ͣ�1
	        �����Է��ͣ�0  by hdh
==============================================================================*/
UINT8 PointBufferSendManage(void)
{
	UINT16 wPointIndex = DEFAULT_ZERO;           /*�������������±�*/
	UINT16 wOCIndex = DEFAULT_ZERO;              /*OC�ṹ������ѭ���±�*/
	UINT16 wSwitchID = DEFAULT_ZERO;             /*���������ĵ���ID*/
	UINT16 wSwitchIDConvert = DEFAULT_ZERO;
	UINT16 wSourceID = DEFAULT_ZERO;             /*�������Դ����ID*/
	UINT16 i = DEFAULT_ZERO;                     /*ɾ�������ѭ���±�*/
	UINT8  IDFlage = DEFAULT_ZERO;               /*��������ID��OC����ĵ���ID�Ƿ�ƥ��ı�־*/

	UINT8  Rtn = RETURN_SUCCESS;
	/*��ʼ����־*/
	IDFlage = DEFAULT_ZERO;
	wSwitchID = g_stOCSendSwitchCmdDataStruct[wPointIndex].SwitchID;
	wSourceID = g_stOCSendSwitchCmdDataStruct[wPointIndex].SourceID;
	/*�ҵ�������������ƥ��ĵ���*/
	for(wOCIndex = 0; wOCIndex < g_stOC.SwitchNum; wOCIndex++)
	{
		if(wSwitchID == g_stOC.SwitchStateArray[wOCIndex].Id)
		{
			/*�жϵ��������Դ�����Ƿ���иõ���Ķ�ռ��,�жϸö����Ƿ�Ϊ��λ����������Ϊ�ϵ�����״̬������ǣ���ת��*/
			if((g_stOC.SwitchELockArray[wOCIndex].ELockId == wSourceID) && (g_stOC.SwitchELockArray[wOCIndex].ELockLevel != 4))
			{
				Rtn = RETURN_SUCCESS;
				/*����������*/	
			}
			else
			{
				Rtn = RETURN_FAILURE;
				/*ɾ��������*/
				for(i = wPointIndex; i < g_wCommandNum_Switch_Send; i++)
				{
					g_stOCSendSwitchCmdDataStruct[i].SourceID = g_stOCSendSwitchCmdDataStruct[i+1].SourceID;
					g_stOCSendSwitchCmdDataStruct[i].SwitchID = g_stOCSendSwitchCmdDataStruct[i+1].SwitchID;
					g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse = g_stOCSendSwitchCmdDataStruct[i+1].LockPositionNormal_Reverse;

				}
				/*���ٵ������������*/
				if(g_wCommandNum_Switch_Send)
				{
					g_wCommandNum_Switch_Send--;
				}	
				else
				{
					debug_infor_printf("\ng_wCommandNum_Switch_Send:%d--, failed", g_wCommandNum_Switch_Send);
				}
			}
			IDFlage = FLAG_SET;
			break;
		}
		else
		{

		}
	}
	/*����ҵ���ID����������*/
	if(IDFlage == FLAG_SET)
	{
		;/*ʲôҲ����*/
	}
	/*���������еĿ��ƶ�����OC���÷�Χ��*/
	else
	{
		/*ɾ��������*/
		Rtn = RETURN_FAILURE;
		for(i = wPointIndex; i < g_wCommandNum_Switch_Send; i++)
		{
			g_stOCSendSwitchCmdDataStruct[i].SourceID = g_stOCSendSwitchCmdDataStruct[i+1].SourceID;
			g_stOCSendSwitchCmdDataStruct[i].SwitchID = g_stOCSendSwitchCmdDataStruct[i+1].SwitchID;
			g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse = g_stOCSendSwitchCmdDataStruct[i+1].LockPositionNormal_Reverse;

			/*���ٵ������������*/
			if(g_wCommandNum_Switch_Send)
			{
				g_wCommandNum_Switch_Send--;
			}	
			else
			{
				debug_infor_printf("\ng_wCommandNum_Switch_Send:%d--, failed", g_wCommandNum_Switch_Send);
			}
		}		
	}

	return Rtn;

}
/**=============================================================================
    ������: PointFaultManage()
      ����: ������Ϲ�����      
      ����: �жϵ����Ƿ���ϣ����Ϻ󽫵���ת�����״̬�������ܽ����κο��ơ�
	  ������ϵļ�������:
	  1����ʱ���ڶ�λ��������������ת����λ��������Ϊ�⶯��
      ��������
	  ����ֵ: �жϳɹ���1
	 �ж�ʧ�� 0
==============================================================================*/
UINT8 PointFaultManage(void)
{
	UINT16 wPointIndex=DEFAULT_ZERO;           /*�������������±�*/
	UINT16 wLockIndex=DEFAULT_ZERO;            /*�������±�*/
	UINT16 wOCIndex=DEFAULT_ZERO;              /*OC�ṹ������ѭ���±�*/
	UINT16 i=DEFAULT_ZERO;                     /*ɾ�������ѭ���±�*/
	UINT8  IDFlage=FLAG_SET;               /*������Ҫ��ĵ���״̬��������״̬�Ƿ�һ��*/
	UINT8  Rtn = RETURN_SUCCESS;
	UINT8 FindSwitch = DEFAULT_ZERO;
	

	/*��������1����ʱ���ڶ�λ��������������ת����λ��������Ϊ�⶯��*/
	for (wOCIndex = 0; wOCIndex < g_stOC.SwitchNum ; wOCIndex++)
	{
		IDFlage = FLAG_SET;
		/*�����й�����*/
		if (DEFAULT_ZERO != g_stOC.SwitchSLockNumArray[wOCIndex])
		{
			for (wLockIndex = 0; wLockIndex < g_stOC.SwitchSLockNumArray[wOCIndex];wLockIndex++)
			{
				if (g_stOC.SwitchStateArray[wOCIndex].Position != LockQSwitchStatus2SwitchStatus(g_stOC.SwitchSLockArray[wOCIndex][wLockIndex].SLockSwitchState))
				{
					IDFlage = FLAG_UNSET;
					if (DEFAULT_ZERO == g_wSwitchFaultNum)
					{
						g_wSwitchFaultID[g_wSwitchFaultNum] = g_stOC.SwitchStateArray[wOCIndex].Id;
						g_wSwitchFaultNum++;
					} 
					else
					{
						for (i = 0 ; i < g_wSwitchFaultNum;i++)
						{
							if (g_wSwitchFaultID[i] = g_stOC.SwitchStateArray[wOCIndex].Id)
							{
								FindSwitch = FLAG_SET;
								break;
							} 
							else
							{

							}
						}
						if (FLAG_SET != FindSwitch)
						{
							g_wSwitchFaultID[g_wSwitchFaultNum] = g_stOC.SwitchStateArray[wOCIndex].Id;
							g_wSwitchFaultNum++;
						} 
						else
						{
						}
					}					
					break;
				}
				else
				{
					/*�����������״̬һֱ��������ʲôҲ����*/
				}
			}			
		}
		else
		{
			/*û�й�����������Ҫ���ǵ���״̬*/
		}

		if (FLAG_UNSET == IDFlage)
		{
			g_stOC.SwitchFaultStateArray[wOCIndex] = SWTICH_FAULT;
			/*DebugPrint(PRINT_PERIOD,"�������������ֹ��ϣ�������Ҫ��״̬�����ʵ��״̬��һ��\r\n");*/
		}
		else
		{
			g_stOC.SwitchFaultStateArray[wOCIndex] = SWTICH_NORMAL;
		}
	}
	if ((DEFAULT_0XFF_UIN16 != g_wSwitchFaultNum)&&(DEFAULT_ZERO != g_wSwitchFaultNum))
	{
		if (NUM_ONE == g_wSwitchFaultNum)
		{
			/*DebugPrint(PRINT_PERIOD,"�������������ֹ��ϣ���һ��������Ҫ��״̬�����ʵ��״̬��һ��\r\n");*/
		}
		else if (NUM_TWO == g_wSwitchFaultNum)
		{
			/*DebugPrint(PRINT_PERIOD,"�������������ֹ��ϣ�������������Ҫ��״̬�����ʵ��״̬��һ��\r\n");*/
		}
		else
		{
			/*DebugPrint(PRINT_PERIOD,"�������������ֹ��ϣ��д�������������Ҫ��״̬�����ʵ��״̬��һ��\r\n");*/
		}
	}
	else
	{
		/*do nothing*/
	}
	return Rtn;

}
/**=============================================================================
    ������: LockQSwitchStatus2SwitchStatus()
      ����: ���ṹ��ĵ���״̬����ת���ɵ�������״̬���ͣ�Ϊ����Ҫ�����״̬���������״̬���Ƚ�    
      ����: ���ṹ��ĵ���״̬����ת���ɵ�������״̬���ͣ�Ϊ����Ҫ�����״̬���������״̬���Ƚ�  
      ���������ṹ��ĵ���״̬
	  ����ֵ: �ɹ��������ڲ�����״̬ ��ʧ�ܣ�����0
==============================================================================*/
UINT8 LockQSwitchStatus2SwitchStatus(UINT8 LockQSwitchStatus)
{
	UINT8 ReturnSwitchStatus = DEFAULT_ZERO;
	if (LOCK_Q_SWITCH_STATUS_MAIN == LockQSwitchStatus)
	{
		ReturnSwitchStatus = SWITCH_POSITION_MAIN;
	}
	else if (LOCK_Q_SWITCH_STATUS_SIDE == LockQSwitchStatus)
	{
		ReturnSwitchStatus = SWITCH_POSITION_SIDE;
	}
	else
	{
		/*����״̬�����Ŀ����߼���*/
	}
	return ReturnSwitchStatus;
}

void SwitchSucces(void)
{
	UINT8 i = DEFAULT_ZERO;

	for (i = 0 ;i < g_stOC.SwitchNum;i++)
	{
		if (FLAG_SET == g_stOC.SwitchInfoArray[i].MoveState)
		{
			if(g_dwOCCycle - g_stOC.SwitchInfoArray[i].SendCycle >= SWITCH_CHANGE_TIME)
			{
				g_stOC.SwitchInfoArray[i].MoveState = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].SendCommondState = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].TimeHigh = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].TimeLow = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].CommandID = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].SendCycle = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].CommandSourceID = DEFAULT_ZERO;
			}
			else
			{
				if (LockQSwitchStatus2SwitchStatus(g_stOC.SwitchInfoArray[i].SendCommondState) == g_stOC.SwitchStateArray[i].Position)
				{
					g_stOC.SwitchInfoArray[i].MoveState = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].SendCommondState = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].TimeHigh = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].TimeLow = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].CommandID = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].SendCycle = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].CommandSourceID = DEFAULT_ZERO;
				}
			}
		} 
		else
		{

		}
	}
}
