/************************************************************************
*
* �ļ���     ��     dquCIPZStruFrmChar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �����ݵ�CI���ñ�ת���������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIPZStruFrmChar.h"
#include "dquEmapInit.h"
#include "dquCIReadCommonChar.h"

DquCiTransmitStruct *pDquCiTranmitInfoStr = NULL;         /*������ӿ����ݱ�ӳ��ṹ��*/
UINT16 dquCiTranmitInfoLen = 0;


DquOutStationSigStruct  *pDquOutStationSigStr = NULL;    /*��վ�źŻ������߼��������ݱ�ӳ��ṹ��*/
UINT16 dquOutStationSigStrLen = 0;

DquCiFuncSwitchStruct *pDquCiFuncSwitchStru = NULL;    /*���ܿ����������ݱ�ӳ��ṹ��*/
UINT16 dquCiFuncSwitchStruLen = 0;

DquDiffLineDevStruct *pTempDquDiffLineDevData = NULL;	/*������·�豸���ݱ�ӳ��ṹ��*/
UINT16 tempDquDiffLineDevDataLen = 0;

DquVirtualDeviceStruct *pDquVirtualDeviceStr = NULL;    /*�����豸���ݱ�ӳ��ṹ��*/
UINT16 dquVirtualDeviceStrLen = 0;

/*********************************************
*�������ܣ������ݵ�CI���ñ�ת��
*����˵����pDataAddr���ݵ�ַ��pPZSysParStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZ_SystemParameterStruFromChar(UINT8 * pDataAddr,SystemParameterStruct* pPZSysParStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;


    if ((NULL == pDataAddr) || (NULL == pPZSysParStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pPZSysParStr->CiSum =  (UINT8)dquCiShortFromChar(pData); /*ʵ����������վ����*/
	pData+=2;

	pPZSysParStr->AppCycleTime  = (UINT32) dquCiShortFromChar(pData); /*����Ӧ������(��λ:ms) (�����ֶ�)*/
	pData+=2;		
    /*�ϵ��������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->StartUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;
    /*��·ѡ�ų���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->RouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2;
	    /*��·���ճ���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->RouteLockTime =  dquCiShortFromChar(pData); 
	pData+=2;
	   /*�����������ճ���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->OverlapLockTime =  dquCiShortFromChar(pData); 
	pData+=2;
               /*��·���ų���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->RouteOpenSigTime =  dquCiShortFromChar(pData); 
	pData+=2;  
               /*��ʽ��·��ʱ����ʱ��(��λ:s) (�����ֶ�)*/
     pPZSysParStr->BlocRouteDelayTime =  dquCiShortFromChar(pData); 
	pData+=2;           
	
    /*CBTC��·��ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->CbtcRouteDelayTime =  dquCiShortFromChar(pData); 
	pData+=2;
	/*������·�ڷ����ι����źſ��ų���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->LeadRouteFaultOpenTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	/*������ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SecDelayUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*�źſ��ų���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SignalOpenTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*�źŹرճ���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SignalCloseTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	/*����������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2;           
	/*������������ʱ��(��λ:s) (�����ֶ�)*/
	 pPZSysParStr->SwitchDriveTime =  dquCiShortFromChar(pData); 
	pData+=2;
	/*�������ν���ͣ��ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->ProtectSecUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;   
	/*ռ�ñ���м�ʱʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->OccupyToIdleTime =  dquCiShortFromChar(pData); 
	pData+=2;              
	/*�׹�����ι��Ͻ���ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->FaultUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;             
	/*���Ḵλ��ʱʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SectionResetTime =  dquCiShortFromChar(pData); 
	pData+=2;      
	/*�ǽ�·ѡ����ʱ��*/
	pPZSysParStr->NonRouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2;       
	/*�ǽ�·��ʱ������ʱ��*/
    pPZSysParStr->NonRouteDelayUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;     
    /*��·ѡ�ŵ�����ʱ��*/
	pPZSysParStr->SwitchDriveCuoFengTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*��Ͻ�·ѡ��ʱ��*/
    pPZSysParStr->ZuheRouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	
    /*�ȴ�ZC����ʱ��*/
    pPZSysParStr->WaitZCResponseTime =  dquCiShortFromChar(pData); 
    pData+=2; 

	/*������Ծ������ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SecJumpLockDelayUnlockTime = dquCiShortFromChar(pData);
	pData+=2;

	/*�����Ŷ�������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->GaragedoorOperateTime = dquCiShortFromChar(pData);
	pData+=2;
	
	/*CTռ�ý�·�����һ�����������µķ������ν���ͣ��ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->CtProtectSecUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;

	/*���쵥��������������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->SingleSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*��������������������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->ThreeSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*�����忪������������ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->FiveSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*��϶̽��ʱ��(��λ:s) (�����ֶ�)*/
	pPZSysParStr->gapDetectTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ�CI���ñ�ת��
*����˵����pDataAddr���ݵ�ַ��pPZSysParStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquCiTransmitInfoFromChar(UINT8 * pDataAddr,DquCiTransmitStruct* pCiTransmitStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL != pDataAddr) && (NULL != pCiTransmitStr))
	{
		
		pCiTransmitStr->seq = dquCiShortFromChar(pData);       /* ��� */ 
		pData += 2;

		pCiTransmitStr->devType = *pData;  /* �豸���� */
		pData += 1;
		
		pCiTransmitStr->devId = dquCiShortFromChar(pData);    /*	�豸���	*/ 
		pData += 2;
		
		pCiTransmitStr->srcCiId = dquCiShortFromChar(pData);  /* ����CI���	*/ 
		pData += 2;
		
		pCiTransmitStr->dstCiId= dquCiShortFromChar(pData);   /*����CI��� */		
		pData += 2;

		pCiTransmitStr->phyLockDirOppOrNot = *pData;	/*�����������շ����Ƿ�ȡ����־*/
		pData += 1;

		rtnvalue = 1;
	}
	return rtnvalue;

}

/*
��������: ��ԭʼ����ת��Ϊ������·�豸����
�������:	UINT8 * pDataAddr ���ݵ�ַ
�������:	DquDiffLineDevStruct * pDiffLineDevData ������·�������ݽṹ��ָ��
����ֵ:	0 ʧ��
		1 �ɹ�
*/
UINT8 PZDquDiffLineDevDataFromChar(UINT8 * pDataAddr, DquDiffLineDevStruct * pDiffLineDevData)
{
	UINT8 retVal = 0;

	if ((NULL != pDataAddr) && (NULL != pDiffLineDevData))
	{
		
		pDiffLineDevData->devSn = dquCiShortFromChar(pDataAddr);	/*���*/ 
		pDataAddr += 2;

		pDiffLineDevData->devType = *pDataAddr;  					/*�豸����*/
		pDataAddr += 1;
		
		pDiffLineDevData->devId = dquCiShortFromChar(pDataAddr);	/*�豸���*/ 
		pDataAddr += 2;

		retVal = 1;
	}

	return retVal;
}


/*********************************************
*�������ܣ��ӵ�ʽ��ʱ�����������ݱ���ʽ��ʱ�������ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pDquOutStationSigStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquOutStationSigRelatedLogSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii;
	UINT16 logSecId = 0;


	if ((NULL != pDataAddr) && (NULL != pOutStationSigStr))
	{
		pOutStationSigStr->wRalatedLogSecSum = 0;                       /*�����߼�����������ʼ��Ϊ0*/

		pOutStationSigStr->wSignalId = dquCiShortFromChar(pData);        /* �źŻ�ID*/ 
		pData += 2;

		for(ii = 0;ii < OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX;ii++)
		{
			logSecId = dquCiShortFromChar(pData);                           /*�߼�����ID*/
			pData += 2;

			if((0 != logSecId) && (65535 != logSecId))
			{
				pOutStationSigStr->wRalatedLogSecIdBuf[pOutStationSigStr->wRalatedLogSecSum] = logSecId;
				pOutStationSigStr->wRalatedLogSecSum++;
			}
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}

/*********************************************
*�������ܣ��ӹ��ܿ����������ݱ����ܿ������ýṹ��ת��
*����˵����pDataAddr ���ݵ�ַ��
*          pDquCiFuncSwitchStru CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquCiFunctionSwitchInfoFromChar(UINT8 * pDataAddr, DquCiFuncSwitchStruct * pCiFuncSwitchStru)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii = 0;

	if ((NULL != pDataAddr) && (NULL != pDquCiFuncSwitchStru))
	{
		for(ii = 0; ii < CI_FUNCTION_SWITCH_SUM_MAX;ii++)
		{

			pCiFuncSwitchStru->wCiFuncSwitchBuf[ii] = * pData;    /*���ܿ�������*/
			pData += 1;
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}

/*********************************************
*�������ܣ��������豸�������ݱ������豸���ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pVirtualDeviceStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquVirtualDeviceInfoFromChar(UINT8 * pDataAddr, DquVirtualDeviceStruct *pVirtualDeviceStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL != pData) && (NULL != pVirtualDeviceStr))
	{

		pVirtualDeviceStr->vDeviceId = dquCiShortFromChar(pData);    /*�����豸ID*/ 
		pData += 2;

		pVirtualDeviceStr->vDeviceType = *pData;					 /* �����豸����*/
		pData += 1;

		rtnvalue = 1;
	}

	return rtnvalue;
}

/*********************************************
*�������ܣ��ӵ�ʽ��ʱ�����������ݣ��������Σ�����ʽ��ʱ�������ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pDquOutStationSigStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquOutStationSigRelatedAxisSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii;
	UINT16 axisSecId = 0;


	if ((NULL != pDataAddr) && (NULL != pOutStationSigStr))
	{
		pOutStationSigStr->wRelatedAxisSecSum = 0;                       /*������������������ʼ��Ϊ0*/

		pOutStationSigStr->wSignalId = dquCiShortFromChar(pData);        /* �źŻ�ID*/ 
		pData += 2;

		for(ii = 0;ii < OUTSTATION_SIGNAL_RELATED_AXISSEC_SUM_MAX;ii++)
		{
			axisSecId = dquCiShortFromChar(pData);                           /*��������ID*/
			pData += 2;

			if((0 != axisSecId) && (UINT16_MAX != axisSecId))
			{
				pOutStationSigStr->wRelatedAxisSecIdBuf[pOutStationSigStr->wRelatedAxisSecSum] = axisSecId;
				pOutStationSigStr->wRelatedAxisSecSum++;
			}
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}