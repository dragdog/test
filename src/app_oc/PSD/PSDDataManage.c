/********************************************************
*                                                                                                            
* �� �� ���� PSDConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ���������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "PSDDataManage.h"
#include "../ParsePack/ParsePackDefine.h"

PsdDataStruct PsdDataStru[PSD_SUM_MAX];   /*�������������ݽṹ��*/

/*
* ���������� ��ȡָ��ID����������
* ����˵���� 
* ����ֵ  �� PsdDataStruct *
*/
PsdDataStruct *GetPsdData(void)
{
    return PsdDataStru;
}

/*
* ���������� ��ȡָ��ID�����Ż������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬      
*/
UINT8 GetPsdMutualLockRemoveState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].MutualLockRemoveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�����Ż������״̬
* ����˵���� const UINT16 doorId, ������ID  
*			 const UINT8 mutualLockState, �������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdMutualLockRemoveState(const UINT16 doorId, const UINT8 mutualLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_MUTUALLOCK_REMOVE_NO == mutualLockState) || (PSD_MUTUALLOCK_REMOVE_YES == mutualLockState))
		{
			PsdDataStru[bufIndex].MutualLockRemoveState = mutualLockState;  /*�����Ż������״̬*/

			retVal = CI_SUCCESS;
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


/*
* ���������� ��ȡָ��ID������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������״̬      
*/
UINT8 GetPsdState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ���������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 doorState, ������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdState(const UINT16 doorId, const UINT8 doorState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_STATE_CLOSE == doorState) || (PSD_STATE_OPEN == doorState))
		{
			PsdDataStru[bufIndex].PsdState = doorState;  /*������״̬*/

			retVal = CI_SUCCESS;
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

/*
* ���������� ��ȡָ��ID�����Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����Ű�ť״̬      
*/
UINT8 GetPsdButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].DoorButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�����Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����Ű�ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_BUTTON_STATE_DOWN == buttonState) || (PSD_BUTTON_STATE_UP == buttonState))
		{
			PsdDataStru[bufIndex].DoorButtonState = buttonState;  /*վ̨��ť״̬*/

			retVal = CI_SUCCESS;
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

/*
* ���������� ��ȡָ��ID����������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������״̬      
*/
UINT8 GetPsdDriveState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdDriveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID����������״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 doorDriveState, ����������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdDriveState(const UINT16 doorId, const UINT8 doorDriveState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_DRIVE_STATE_CLOSE == doorDriveState) || (PSD_DRIVE_STATE_OPEN == doorDriveState) || (PSD_DRIVE_STATE_INVALID == doorDriveState))
		{
			PsdDataStru[bufIndex].PsdDriveState = doorDriveState;  /*����������״̬*/

			retVal = CI_SUCCESS;
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

#if 0
/*
* ���������� ��ȡָ��ID������״̬ת�Ʊ�־
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������״̬ת�Ʊ�־     
*/
UINT8 GetPsdStateTransferFlag(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdStateTransferFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}
#endif

#if 0
/*
* ���������� ����ָ��ID������״̬ת�Ʊ�־
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 PsdStateTransferFlag, ״̬ת�Ʊ�־
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdStateTransferFlag(const UINT16 doorId, const UINT8 PsdStateTransferFlag)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_INITIAL_STATE == PsdStateTransferFlag) || 
			(PSD_OPEN_CMD == PsdStateTransferFlag)      || 
			(PSD_OPEN_STATE == PsdStateTransferFlag)    ||
			(PSD_CLOSE_CMD == PsdStateTransferFlag)     ||
			(PSD_CLOSE_STATE== PsdStateTransferFlag)    
		   )
		{
			PsdDataStru[bufIndex].PsdStateTransferFlag = PsdStateTransferFlag;  /*������״̬ת�Ʊ�־*/

			retVal = CI_SUCCESS;
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
#endif

#if 0
/*
* ���������� ��ȡָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·��Ŀ  
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ſ��ŵ����źŹرյĽ�·��Ŀ   
*/
UINT8 GetBuildedOutStationRouteSum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*�����±�*/
	UINT8 retVal = 0;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].BuildedOutStationRouteSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}

/*
* ���������� ����ָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·��Ŀ
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 routeSum, ��Ӧ�İ���ɹ��Ľ�·��Ŀ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBuildedOutStationRouteSum(const UINT16 doorId,const UINT8 routeSum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/
	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].BuildedOutStationRouteSum = routeSum;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;


}
#endif

#if 0
/*
* ���������� ��ȡָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·Id����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��Ӧ�İ���ɹ��Ľ�·Id����
*/
UINT16 * GetBuildedOutStationRouteIdBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*�����±�*/
	UINT16 * retVal = NULL;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].BuildedOutStationRouteIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;

}


/*
* ���������� ��ȡ��վ��·�źŹر��Ƿ���ָ��ID�����Ŵ�֮ǰ�ı�־����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��վ��·�źŹر��Ƿ���ָ��ID�����Ŵ�֮ǰ�ı�־����
*/
UINT8 * GetPsdOpenBeforeOutStationRouteIsBuildedFlagBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*�����±�*/
	UINT8 * retVal = NULL;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdOpenBeforeOutStationRouteIsBuildedFlagBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;

}
#endif 
/*
* ���������� ����PSD��VOBC��������
* ����˵���� const UINT16 doorId,������ID  
*			 const UINT16 vobcId,���������ŵ�VOBC ID
			 const UINT8 operCmd,����VOBC�Ŀ���������
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdVobcCmd(const UINT16 doorId, const UINT16 vobcId,const UINT8 operCmd)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if (PsdDataStru[bufIndex].SrcVobcId == vobcId)
		{
			PsdDataStru[bufIndex].PsdCmd=operCmd;
		}
		else
		{
			/*��һ��VOBC��ʼ�����������ţ���֮ǰVOBC�����������Ų����������Ľ��и�λ*/
			PsdDataStru[bufIndex].SrcVobcId = vobcId;
			PsdDataStru[bufIndex].PsdCmd=operCmd;
			/*PsdDataStru[bufIndex].ClosedRouteId = 0;*/
			/*PsdDataStru[bufIndex].LastDriState=PSD_DRIVE_STATE_INVALID;*/
		}
		PsdDataStru[bufIndex].CmdSetCycle = Get2oo2CurCycleNum();

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

#if 0
/*
* ���������� �������PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����˵���� const UINT16 doorId,������ID  
*			 const UINT16 routeId,���PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdClosedRouteId(const UINT16 doorId,const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].ClosedRouteId = routeId;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#endif

/*
* ���������� �ö�ӦPSD״̬ΪĬ��̬
* ����˵���� const UINT16 psdId,PSD ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 ResetPsdData(const UINT16 psdId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(psdId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		/*PsdDataStru[bufIndex].SrcVobcId = 0;*/
		PsdDataStru[bufIndex].PsdCmd=PSD_DEFAULT_CONTROL_CMD;
		/*վ̨���޳�ʱ�������PSD����������򿪡��رպͲ�ѯ����ʼ����*/
		/*PsdDataStru[bufIndex].CmdSetCycle = 0;*/

		PsdDataStru[bufIndex].PsdDriveState=PSD_DRIVE_STATE_INVALID;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#if 0
/*
* ���������� ��ȡ���PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ���PSD�򿪶�����ʼ���źŹرյĽ�·ID 
*/
UINT16 GetPsdClosedRouteId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 routeId=0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		routeId=PsdDataStru[bufIndex].ClosedRouteId;
	}
	else
	{
		routeId=0;
	}
	return routeId;
}
#endif

/*
* ���������� ���PSD��VOBC��������
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� PSD��VOBC��������
*/
UINT8 GetPsdVobcCmd(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 ctrlCmd;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		ctrlCmd=PsdDataStru[bufIndex].PsdCmd;
	}
	else
	{
		ctrlCmd=PSD_DEFAULT_CONTROL_CMD;
	}
	return ctrlCmd;
}
/*
* ���������� ��õ�ǰ���Ƹ�PSD��VOBC��ID
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ���Ƹ�PSD��VOBC��ID;���û�п��Ƹ�PSD��VOBC,����0
*/
UINT16 GetPsdVobcId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 vobcId = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		vobcId=PsdDataStru[bufIndex].SrcVobcId;
	}
	else
	{
		vobcId=0;
	}
	return vobcId;
}

#if 0
/*
* ���������� ����PSD����һ�ε�����״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 driSta,��PSD������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdLastDriState(const UINT16 doorId,const UINT8 driSta)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal= CI_ERROR;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_DRIVE_STATE_CLOSE == driSta) || (PSD_DRIVE_STATE_OPEN == driSta) || (PSD_DRIVE_STATE_INVALID == driSta))
		{
			PsdDataStru[bufIndex].LastDriState=driSta;
			retVal=CI_SUCCESS;
		}
		else
		{
			retVal=CI_ERROR;
		}
	}
	else
	{
		retVal=CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ø�PSD����һ�ε�����״̬
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ��PSD����һ�ε�����״̬
*/
UINT8 GetPsdLastDriState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 driSta=PSD_DRIVE_STATE_INVALID;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		driSta=PsdDataStru[bufIndex].LastDriState;
	}
	else
	{
		driSta=PSD_DRIVE_STATE_INVALID;
	}
	return driSta;
}
#endif

/*
* ���������� ��ø�PSD�Ŀ����������������
* ����˵���� const UINT16 doorId,������ID 
* ����ֵ  �� ��PSD�Ŀ����������������
*/
UINT32 GetPsdCmdSetCycle(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 setCycle=0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		setCycle=PsdDataStru[bufIndex].CmdSetCycle;
	}
	else
	{
		setCycle=0;
	}
	return setCycle;
}
/*
* ���������� ��ȡPSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 
* ����ֵ  �� 0: ��ȡʧ��
*			 >0: ��ȡ�ɹ� 
*/
UINT8 GetPsdRemoraState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdRemoraState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ����PSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 driSta,��PSD���ϰ���״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdRemoraState(const UINT16 doorId, const UINT8 remoraState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_REMORA_YES == remoraState) || (PSD_REMORA_NO == remoraState) )
		{
			PsdDataStru[bufIndex].PsdRemoraState = remoraState;  /*����������״̬*/

			retVal = CI_SUCCESS;
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

/*
* ���������� ��ȡPSD�ļ�϶̽�⻥�����״̬
* ����˵���� const UINT16 doorId,������ID
			 
* ����ֵ  �� 0: ��ȡʧ��
*			 >0: ��ȡ�ɹ� 
*/
UINT8 GetJXTCMutualLockState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].JXTCMutualLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ����PSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 mutualLockState,�ü�϶̽�⻥�����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetJXTCMutualLockState(const UINT16 doorId, const UINT8 mutualLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_REMORA_YES == mutualLockState) || (PSD_REMORA_NO == mutualLockState) )
		{
			PsdDataStru[bufIndex].JXTCMutualLockState = mutualLockState;  /*����������״̬*/

			retVal = CI_SUCCESS;
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

/*
* ��������������ָ��PSD����̽�����ں�
* ����˵���� const UINT16 doorId, ��·ID
*            const UINT32 startCycNum, ��ʼ���ں�
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStartDetectionCycNum(const UINT16 doorId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].StartDetectionCycNum = startCycNum;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����̽����ʼ���ں�
* ����˵���� const UINT16 doorId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ��ʼ���ں�
*/
UINT32 GetStartDetectionCycNum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StartDetectionCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* �������������ÿ�ʼ����̽��״̬
* ����˵���� const UINT16 doorId, PSD ID
*            const UINT32 detectionState, ����״̬
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStartDetectionState(const UINT16 doorId, const UINT32 detectionState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((JXTC_START_DETECTION_NO == detectionState) || (JXTC_START_DETECTION_YES == detectionState) )
		{
			PsdDataStru[bufIndex].StartDetectionState = detectionState;
			retVal = CI_SUCCESS;
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
/*
* ���������� ��ȡ��ʼ����̽��״̬
* ����˵���� const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ѡ����ʼ���ں�
*/
UINT8 GetStartDetectionState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StartDetectionState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ��������������ֹͣ̽��״̬
* ����˵���� const UINT16 doorId, PSD ID
*            const UINT32 detectionState, ֹͣ̽��״̬
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStopDetectionState(const UINT16 doorId, const UINT32 detectionState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((JXTC_STOP_DETECTION_NO == detectionState) || (JXTC_STOP_DETECTION_YES == detectionState) )
		{
			PsdDataStru[bufIndex].StopDetectionState = detectionState;
			retVal = CI_SUCCESS;
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
/*
* ���������� ��ȡֹͣ̽��״̬
* ����˵���� const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ѡ����ʼ���ں�
*/
UINT8 GetStopDetectionState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ��·�����±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StopDetectionState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#if 0
/*
* ���������� ������VOBC��PSD����Ҫ���ؿ��Ľ�·
* ����˵���� const UINT16 doorId,������ID
*			 const UINT16 routeId,��·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�
*/
UINT8 SetPsdToBeOpenedRoute(const UINT16 doorId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].ToBeOpenedRouteId = routeId;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ��VOBC��PSD����Ҫ���ؿ��Ľ�·
* ����˵���� const UINT16 doorId,������ID
* ����ֵ  �� 0:  û����VOBC��PSD����Ҫ���ؿ��Ľ�·���ǲ���ʧ��
*			 >0: ��VOBC��PSD����Ҫ���ؿ��Ľ�·
*/
UINT16 GetPsdToBeOpenedRoute(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].ToBeOpenedRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#endif

/*
* ���������� ��ȡָ��ID�����ſ��Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ſ��Ű�ť״̬      
*/
UINT8 GetPsdOpenButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].doorOpenButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�����ſ��Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����ſ��Ű�ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdOpenButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_OPEN_BUTTON_STATE_VALID == buttonState) || (PSD_OPEN_BUTTON_STATE_INVALID == buttonState))
		{
			PsdDataStru[bufIndex].doorOpenButtonState = buttonState;  /*վ̨��ť״̬*/

			retVal = CI_SUCCESS;
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

/*
* ���������� ��ȡָ��ID�����ŷ�����ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŷ�����ť״̬      
*/
UINT8 GetPsdDepartButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].doorDepartButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�����ŷ�����ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����ŷ�����ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdDepartButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PLATFORM_DEPAR_BUTTON_STATE_VALID == buttonState) || (PLATFORM_DEPAR_BUTTON_STATE_INVALID == buttonState))
		{
			PsdDataStru[bufIndex].doorDepartButtonState = buttonState;  /*վ̨��ť״̬*/

			retVal = CI_SUCCESS;
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