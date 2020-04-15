/********************************************************
*                                                                                                            
* �� �� ���� HLHTCommonDataPacketHead.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2016-10-21
* ��    �ߣ� ������Ʒ��
* ���������� ������ͨͨ�ñ���ͷУ�鼰��Ϣ��Ч�Լ��� 
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 
#include "HLHTCommonDataPacketHead.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../QueryFiles/dquCIInterFace.h"

/*************************************************************************************************************
* �����������յ���ͨ����Ϣ��ʱ,�������Ϣ��ʣ����Ч��
* ���������  
		   const UINT16 maxMsgValidTime	��ĿԼ���ĸ���Ϣ��Ч�� ;
		   const UINT32 CurrentOwnSN �����ĵ�ǰ���ں�
		   const UINT32 SNPeriod ������ͨ�����ڣ���CIϵͳ�У���Ϊ��Ӧ�õ�����
		   const HLHTGALInfoStruct *pRecvCommonPackage  ͨ����Ϣ����ͷ
		   const UINT32 srcRecvCiMsgSN,�Է��ϴ��յ��ı��������
		   const UINT32 srcCrntOwnMsgSN,�Է������������ʱ�䵱ǰ�����
		   const UINT32 srcPreOwnMsgSN,�Է��ϴ��յ���������ʱ�䵱ʱ�����
		   const UINT32 srcPeriod,�Է���ͨ������
* �����������.
* ����ֵ��RemainValidity  ����Ϣ��ʣ����Ч��
*************************************************************************************************************/
INT32 HLHTCommonMsgRemainingValidity(
	const UINT32 MaxMsgValidTime,
	const UINT32 CurrentOwnSN,
	const UINT32 SNPeriod,
	const UINT32 srcRecvCiMsgSN,
	const UINT32 srcCrntOwnMsgSN,
	const UINT32 srcPreOwnMsgSN,
	const UINT32 srcPeriod)
{
	INT32 RemainValidity = 0;

	if((SNPeriod<= 0) || (srcPeriod <= 0))
	{
		RemainValidity = -1;
	}
	else if( MaxMsgValidTime <= 0 )
	{
		RemainValidity = -1;
	}
	else if(srcRecvCiMsgSN > CurrentOwnSN)
	{
		RemainValidity = -1;
	}
	else if(srcPreOwnMsgSN > srcCrntOwnMsgSN)
	{
		RemainValidity = -1;
	}
	else if (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)(SNPeriod)) -
		((INT32)(srcCrntOwnMsgSN)-(INT32)(srcPreOwnMsgSN))*((INT32)(srcPeriod)) <= 0)
	{
		RemainValidity = (INT32)MaxMsgValidTime - (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)SNPeriod));
	}
	else
	{
		/*ͨ���յ���Ϣ�еĻظ�����"��һ��"��Ϣ��ʱ�������RcvOppSN��,������ܵ��ӳ�,
		  �ټ�ȥ��Ϣ�ڶԷ��豸�ڵ�ʱ��,�Ӷ��õ�˫��Ĵ����ӳ�,������Ϊ������ӳ�ʹ��*/
		RemainValidity = (INT32)MaxMsgValidTime - (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)(SNPeriod)) -
			((INT32)(srcCrntOwnMsgSN)-(INT32)(srcPreOwnMsgSN))*((INT32)(srcPeriod)));
	}
	return RemainValidity;  
}

/*************************************************************************
* ��������:    ���յ�CIͨ����Ϣ����ͷ��Ϣ�Ϸ��Լ��
* �������:   UINT16 LocalDevName  ���ͷ��豸����
*			  UINT16 RemoteDevName  ���շ��豸����
*			  UINT16 DevInterfaceType  �豸���豸�ӿ�����
*             UINT8 prtrlVer        �ӿ�Э��汾��
*			  HLHTGALInfoStruct *pMICommonPackage  CI���͸�VOBC��ͨ����Ϣ����ͷ
*             UINT16 logId        VOBC 1�˻�2�˱�ʶ
* �����������:   UINT32 *pErrorCode  CI�ӿ��еĹ��Ϻ�.
* �������:    ��.
* ȫ�ֱ���:         ..
* ����ֵ:
HLHT_HEAD_SUCCESS (0x55) ������ͨͷ�����ȷ
HLHT_HEAD_ERROR (0xaa) ������ͨͷ������
HLHT_HEAD_INIT (0x33) ����ͷ�ǻ�����ͨ��ʼͨ��֡
*************************************************************************/
UINT8 HLHTRecvCommonPacketHeadCheck(UINT16 LocalDevName, UINT16 RemoteDevName, UINT16 DevInterfaceType, UINT8 prtrlVer, GALInfoStruct *pMICommonPackage, UINT32 *pErrorCode,UINT16 logId)
{
	UINT8 retVal = CI_ERROR;
	UINT32 LocalEtcsId = 0;					  /*���ͷ���ʶ*/
	UINT32 RemoteEtcsId = 0; 				  /*���շ���ʶ*/
	UINT32 emapVer;/*Ӧ��ʹ�õĵ��ӵ�ͼ�汾*/
	
	/*������ÿ�ζ�����*/
	*pErrorCode = 0;
		
	/*�����жϸ���Ϣ�Ƿ��ںϷ���Χ��,ֻҪ�д���������Ӧ������*/
	if(pMICommonPackage->devInterInfoType != DevInterfaceType)
	{
		*pErrorCode |= GAL_COMPARE_INFO_TYPE_ERR;
	}	

#if 0
	/*���ͷ�У��*/
	dquDevName2HlhtId(LocalDevName,logId,&LocalEtcsId);
	if(pMICommonPackage->sourceId != LocalEtcsId)
	{
		*pErrorCode |= GAL_COMPARE_SEND_FLAG_ERR;
	}

	/*���շ�У��*/
	dquDevName2HlhtId(RemoteDevName,logId,&RemoteEtcsId);	
	if(pMICommonPackage->destId != RemoteEtcsId)
	{
		*pErrorCode |= GAL_COMPARE_RECV_FLAG_ERR;
	}

	if (CI_SUCCESS == GetCiEmapCheckVer((UINT8)RemoteDevName, &emapVer))
	{
		/*���ӵ�ͼ�汾У����Ϣ(4�ֽ�CRC�Ƚ�)*/
		if (emapVer != pMICommonPackage->emapVerCrc)
		{
			*pErrorCode |= GAL_COMPARE_EMAP_VER_COMP_ERR;
		}
	}
	else
	{
		*pErrorCode |= GAL_COMPARE_EMAP_VER_COMP_ERR;
	}
#endif
	

#if 0
    else
    {
		/*�ȽϽ����һ��,У��ʧ��*/
		vCIDBVerCmpRst = CBTC_FALSE;
        /*ATS�ӿ��е��ӵ�ͼ�汾��VOBC�Ĳ�һ��,��¼�˴���ԭ��*/
    	/*�����ݲ�֧��,�����ҽӿ��̶����TO BE DONE  20160817*/
		/**pErrorCode |= CI_COMPACK_EMAP_VER_COMP_ERR;*/
    }
#endif

    /* ���кźϷ��Լ��, (2^31-1)*/
	if((1 > pMICommonPackage->crntOwnMsgSN) || (0x7FFFFFFF < pMICommonPackage->crntOwnMsgSN)
		|| (1 > pMICommonPackage->recvOppMsgSN) || (0x7FFFFFFF < pMICommonPackage->recvOppMsgSN)
		|| (1 > pMICommonPackage->preOwnMsgSN) || (0x7FFFFFFF < pMICommonPackage->preOwnMsgSN))
	{
	    *pErrorCode |= GAL_COMPARE_MSG_BDS_SN_ERR;
	}

#if 0
    /* ���Э��汾 */
	if(prtrlVer != pMICommonPackage->prtclVer)
	{
		*pErrorCode |= GAL_COMPARE_PROTOCOL_VER_ERR;
	}
#endif

	/* ���Ӧ�����ݳ��� */
	if(0 >= pMICommonPackage->totalDataLen)
	{
		*pErrorCode |= GAL_COMPARE_APP_LEN_ERR;
	}	
	/*���ͨ����Ϣ����ͨ������*/
	if (0 == pMICommonPackage->comPeriod)
	{
		*pErrorCode |= GAL_COMPARE_COM_PERIOD;
	}
	
	/*����ӿ���Ϣû�з�������,����������ȷ;���򷵻ش���*/
	if(0 == *pErrorCode)
	{
		/*�ӿ�ȫ����ȷ,����������ȷ*/
		retVal = HLHT_HEAD_SUCCESS;
	}
	else if(GAL_COMPARE_MSG_BDS_SN_ERR == *pErrorCode)
	{
		if ((0 < pMICommonPackage->crntOwnMsgSN) && (UINT32_MAX == pMICommonPackage->recvOppMsgSN) && (UINT32_MAX == pMICommonPackage->preOwnMsgSN))
		{/*˵���Է���û���յ�������֡����ǰ֡�ǳ�ʼ֡*/
			retVal = HLHT_HEAD_INIT;
		}
		else
		{
			retVal = HLHT_HEAD_ERROR;
		}
	}
	else
	{
		retVal = HLHT_HEAD_ERROR;
	}
	
	return retVal;		
}