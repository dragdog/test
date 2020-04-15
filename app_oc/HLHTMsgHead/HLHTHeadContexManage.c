/********************************************************
*
* �� �� ���� HLHTHeadContexManage.c
* ��Ȩ˵���� �������ؿƼ����޹�˾
* ����ʱ�䣺 2016-12-17
* ��    �ߣ� ������Ʒ��
* ���������� ������ͨ����ͷ���������ݹ���
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��
*
********************************************************/
#include "HLHTHeadContexManage.h"
#include "HLHTCommonDataPacketHead.h"
#include "CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "CommonMemory.h"

static HLHTHeadContextStru ciHLHTHeadCtxs[CI_SUM_MAX];/*����뱾CIͨ�ŵ�����CI������ͨ����ͷ����������*/
static HLHTHeadContextStru vobcHLHTHeadCtxs[VOBC_SUM_MAX];/*����뱾CIͨ�ŵ�VOBC������ͨ����ͷ����������*/
static HLHTHeadContextStru tmcHLHTHeadCtxs[TMC_SUM_MAX];/*����뱾CIͨ�ŵ�TMC������ͨ����ͷ����������*/

/*
* ��������:���غ��������ĳ���豸���͵����ں�������Ļ�����ͨͷ����������
* ����˵��:
	const UINT8 devType:�豸����
	UINT16 *devMaxSum:���ڲ���,ĳ���豸���͵���������
	HLHTHeadContextStru **hlhtHeadCtxPtr:���ڲ���,ĳ���豸���͵Ļ�����ͨͷ����������ָ��
* ����ֵ:
	CI_ERROR:��ȡʧ��
*	CI_SUCCESS:��ȡ�ɹ�
*/
static UINT8 GetHLHTHeadCtxTmepVars(const UINT8 devType, UINT16 *devMaxSum, HLHTHeadContextStru **hlhtHeadCtxPtr);
/*
* ��������:��ͬϵͳ�Ļ�����ͨͨ��ͷʱЧ�Դ���
* ����˵��:
	const UINT32 timeoutSnCount,������Ч�����������
	const UINT32 timeoutSpan,������Ч�����ʱ��
	HLHTHeadContextStru headCtxs[],�������ͨ��ͷ����
	const UINT8 headCount,�������ͨ��ͷ������
* ����ֵ:
	��
*/
static void ProcHlhtHeadCtxTimeOutDiffSys(const UINT32 timeoutSnCount, const UINT32 timeoutSpan, HLHTHeadContextStru headCtxs[], const UINT8 headCount);

/*
* ��������:��ȡĳ���豸���һ�θ�CI��������ʱ�䵱ǰ�����
* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
* ����ֵ:
	ĳ���豸���һ�θ�CI��������ʱ�䵱ǰ�����,0xffffffffΪ��Чֵ
*/
UINT32 GetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].sourceCrntOwnMsgSN;
			}
			else
			{
				retVal = UINT32_MAX;/* ��TMCЭ���һ���ϵ緢ȫf */
			}
		}
		else
		{
			retVal = 0;
		}
	}
	debug_infor_printf("\nSn_Send: OtherMsgSn: %d. ", retVal);

	return retVal;
}
/*
* ��������:��ȡĳ���豸���һ�θ�CI��������ʱ������д���ϴ��յ�CI�����䵱ʱ�����
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
* ����ֵ:
ĳ���豸���һ�θ�CI��������ʱ������д���ϴ��յ�CI�����䵱ʱ�����,0Ϊ��Чֵ
*/
UINT32 GetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].sourcePreOwnMsgSN;
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
	}

	return retVal;
}
/*
* ��������:��ȡCI����յ�ĳ���豸������ʱCI����ʱ�����
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
* ����ֵ:
CI����յ�ĳ���豸������ʱ����ʱ�����,0Ϊ��Чֵ
*/
UINT32 GetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].ciPreRecvOwnSn;
			}
			else
			{
				retVal = UINT32_MAX;/* ��TMCЭ���һ���ϵ緢ȫf */
			}
		}
		else
		{
			retVal = 0;
		}
	}
	debug_infor_printf(" OcMsgSn: %d. ", retVal);

	return retVal;
}
/*
* ��������:����ĳ����CIͨ�ŵ��豸��ͨ������
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 period:ͨ������
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxComPeriod(const UINT8 devType, const UINT8 devId, const UINT16 period)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourceComPeriod = period;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* ��������:����CI����յ�ĳ���豸����ʱͨ��ͷ��������ĶԷ���ǰ�����ں�
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 srcCrntSN:�Է���ǰ�����ں�
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId, const UINT32 srcCrntSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				tempHeadCtxPtr[devId].sourceId = devId;
				tempHeadCtxPtr[devId].sourceCrntOwnMsgSN = srcCrntSN;

				retVal = CI_SUCCESS;
			}
		}
	}

	return retVal;
}
/*
* ��������:����ĳ���豸��һ���յ�CI����ʱ���������CI��ǰ�����
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 srcOppSN:�Է���һ���յ�CI����ʱ���������CI��ǰ�����
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxSrcOppSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOppSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourceRecvOppMsgSN = srcOppSN;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* ��������:����ĳ���豸��һ���յ�CI����ʱ������ĵ�ǰ���
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 srcOwnSN:ĳ���豸��һ���յ�CI����ʱ������ĵ�ǰ���
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOwnSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourcePreOwnMsgSN = srcOwnSN;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* ��������:����CI�յ�����ʱCI��������
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 ciRcvSn:CI�յ�����ʱCI��������
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId, const UINT32 ciRcvSn)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].ciPreRecvOwnSn = ciRcvSn;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}

/*
* ��������:ɾ��ĳ���豸�Ļ�����ͨͷ
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 ClearHlhtHeadCtx(const UINT8 devType, const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			CommonMemSet(tempHeadCtxPtr, sizeof(HLHTHeadContextStru),0,sizeof(HLHTHeadContextStru));

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}

/*
* ���������� ������������ͨͨ�ñ���ͷʱЧ��
* ����˵����
	��
* ����ֵ  ��
	��
*/
void ProcHlhtHeadCtxTimeOut(void)
{
	UINT32 timeoutSnCount;
	UINT32 timeoutSpan;
	/*����VOBCͨ�ñ���ͷ��ʱЧ��*/
	timeoutSnCount = GetCiAndVobcTransmitBadEnsureCount();
	timeoutSpan = GetCiAndVobcTransmitBadEnsureTime();
	ProcHlhtHeadCtxTimeOutDiffSys(timeoutSnCount, timeoutSpan, vobcHLHTHeadCtxs, VOBC_SUM_MAX);
	/*����CIͨ�ñ���ͷ��ʱЧ��*/
	timeoutSnCount = GetCiAndCiTransmitBadEnsureCount();
	timeoutSpan = GetCiAndCiTransmitBadEnsureTime();
	ProcHlhtHeadCtxTimeOutDiffSys(timeoutSnCount, timeoutSpan, ciHLHTHeadCtxs, CI_SUM_MAX);
}
/*
* ���������� �����豸���ͣ���þ߱���ʼͨ�Ż�����ͨͷ�������豸ID
* ����˵����
const UINT8 devType, �豸����
UINT8* devIdBuf, �洢�����������豸��ŵ�����
const UINT8 devIdBufMaxLen, devIdBuf����������ֹ�����ڷ���Խ��
UINT8* devSum,���������������豸��ŵ�����
UINT16* devLogIdBuf �洢�����������豸�߼�ID������
* ����ֵ  ��
CI_SUCCESS:��óɹ�
CI_ERROR:���ʧ��
*/
UINT8 GetHlhtInitHeadDevsByDevType(const UINT8 devType, UINT8* devIdBuf, const UINT8 devIdBufMaxLen, UINT8* devSum, UINT16* devLogIdBuf)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;
	UINT8 i;
	UINT8 tempDevSum = 0;

	if ((devIdBuf != NULL) && (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr)))
	{
		if ((devIdBufMaxLen >= maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			for (i = 0;i < maxDevSum;i++)
			{
				if ((UINT32_MAX == tempHeadCtxPtr[i].sourceRecvOppMsgSN) && (UINT32_MAX == tempHeadCtxPtr[i].sourcePreOwnMsgSN))
				{/*˵����ͨ��ͷ�ǶԷ���û���յ���ci���κ�����ʱ����ci���͵�����֡�ı���ͷ������ʼͨ�ű���ͷ*/
					devIdBuf[tempDevSum] = tempHeadCtxPtr[i].sourceId;
					devLogIdBuf[tempDevSum] = tempHeadCtxPtr[i].logId;
					tempDevSum++;
				}
			}

			retVal = CI_SUCCESS;
		}
	}

	/*Ϊ���ڲ�����ֵ*/
	if (CI_SUCCESS == retVal)
	{
		*devSum = tempDevSum;
	}
	else
	{
		*devSum = 0;
	}

	return retVal;
}

/*
* ��������:���غ��������ĳ���豸���͵����ں�������Ļ�����ͨͷ����������
* ����˵��:
const UINT8 devType:�豸����
UINT16 *devMaxSum:���ڲ���,ĳ���豸���͵���������
HLHTHeadContextStru **hlhtHeadCtxPtr:���ڲ���,ĳ���豸���͵Ļ�����ͨͷ����������ָ��
* ����ֵ:
CI_ERROR:��ȡʧ��
*	CI_SUCCESS:��ȡ�ɹ�
*/
static UINT8 GetHLHTHeadCtxTmepVars(const UINT8 devType, UINT16 *devMaxSum, HLHTHeadContextStru **hlhtHeadCtxPtr)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if ((CI_SYSTEM_TYPE_OC == devType))
	{
		maxDevSum = CI_SUM_MAX;
		tempHeadCtxPtr = ciHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else if (CI_SYSTEM_TYPE_IVOC == devType)
	{
		maxDevSum = VOBC_SUM_MAX;
		tempHeadCtxPtr = vobcHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else if (CI_SYSTEM_TYPE_TMC == devType)
	{
		maxDevSum = TMC_SUM_MAX;
		tempHeadCtxPtr = tmcHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}

	*devMaxSum = maxDevSum;
	*hlhtHeadCtxPtr = tempHeadCtxPtr;

	return retVal;
}
/*
* ��������:��ͬϵͳ�Ļ�����ͨͨ��ͷʱЧ�Դ���
* ����˵��:
const UINT32 timeoutSnCount,������Ч�����������
const UINT32 timeoutSpan,������Ч�����ʱ��
HLHTHeadContextStru headCtxs[],�������ͨ��ͷ����
const UINT8 headCount,�������ͨ��ͷ������
* ����ֵ:
��
*/
static void ProcHlhtHeadCtxTimeOutDiffSys(const UINT32 timeoutSnCount, const UINT32 timeoutSpan, HLHTHeadContextStru headCtxs[], const UINT8 headCount)
{
	UINT32 curSn = Get2oo2CurCycleNum();
	UINT8 i;
	HLHTHeadContextStru tempHeadCtx = { 0 };
	UINT8 headValid = 1;
	INT32 remainValidiy;/*ʣ���ʱ��*/

	if (NULL != headCtxs)
	{
		for (i = 0;i < headCount;i++)
		{
			tempHeadCtx = headCtxs[i];

			if (0 != tempHeadCtx.sourceId)
			{/*˵���Ǹ���Ч�ı���ͷ*/
				if ((UINT32_MAX == tempHeadCtx.sourceRecvOppMsgSN) && (UINT32_MAX == tempHeadCtx.sourcePreOwnMsgSN))
				{/*˵���Է���û���յ���ci���κ�����ʱ����ci���͵�����֡*/
					if ((curSn - tempHeadCtx.ciPreRecvOwnSn) > timeoutSnCount)
					{/*����ͨ�ų�ʱʱ�䣬Ҳ��Ҫ���*/
						headValid = 0;
					}
				}
				else
				{
					/*�������㱨��ͷ��ʱЧ��*/
					remainValidiy = HLHTCommonMsgRemainingValidity(timeoutSpan, curSn, GetSystemParaAppCycleTime(), tempHeadCtx.sourceRecvOppMsgSN, tempHeadCtx.sourceCrntOwnMsgSN, tempHeadCtx.sourcePreOwnMsgSN, tempHeadCtx.sourceComPeriod);

					if (remainValidiy <= 0)
					{
						headValid = 0;
					}
				}

				if (0 == headValid)
				{
					memset(&(headCtxs[i]), 0, sizeof(HLHTHeadContextStru));
				}
			}
		}
	}
}

/*
* ��������:��ȡĳ����CIͨ�ŵ��豸�Ļ�����ͨ����ͷ��������Ϣ�Ƿ���ڲ�����Ч
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
* ����ֵ:
CI_ERROR:������
*	CI_SUCCESS:����
*/
UINT8 GetHLHTHeadCtxValid(const UINT8 devType, const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if ((tempHeadCtxPtr[devId].sourceId == devId) && (tempHeadCtxPtr[devId].sourceRecvOppMsgSN != UINT32_MAX) && (tempHeadCtxPtr[devId].sourcePreOwnMsgSN != UINT32_MAX))
			{
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
	}
	else
	{
		retVal = CI_ERROR;		
	}

	return retVal;
}

/*
* ��������:��ȡĳ����CIͨ�ŵ��豸�Ļ�����ͨ����ͷ��������Ϣ�Ƿ����
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
* ����ֵ:
CI_ERROR:������
*	CI_SUCCESS:����
*/
UINT8 GetHLHTHeadCtxExistance(const UINT8 devType,const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if ((tempHeadCtxPtr[devId].sourceId == devId))
			{
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
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* ��������:����CI�յ�����ʱ�Է���LogId
* ����˵��:
const UINT8 devType:�豸����
const UINT8 devId:�豸ID
const UINT16 ciRcvLogId:CI�յ�����ʱ�Է���LogId
* ����ֵ:
CI_SUCCESS:�ɹ�
CI_ERROR:ʧ��
*/
UINT8 SetHLHTHeadCtxCiRcvLogId(const UINT8 devType, const UINT8 devId, const UINT16 ciRcvLogId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].logId = ciRcvLogId;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}