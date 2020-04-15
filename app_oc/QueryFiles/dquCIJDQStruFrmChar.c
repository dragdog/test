/************************************************************************
*
* �ļ���        ��  dquCIJDQStruFrmChar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  �����ݵ��̵����ṹ�崦����� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIJDQStruFrmChar.h"
#include "dquCIReadCommonChar.h"

/*********************************************
*�������ܣ���ʼ���̵����ӿ����ݽṹ��
*����˵����pDataAddr���ݵ�ַ��  pRelayCfgDtStr�̵����������ݽṹ��ָ�룬Name�̵�������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayConfigDataStruFromChar(UINT8 * pDataAddr,RelayConfigDataStruct* pRelayCfgDtStr,CHAR* Name)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT16 i=0;

    if ((NULL == pDataAddr) || (NULL == pRelayCfgDtStr) || (NULL == Name))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	/*�̵���ID (�����ֶ�)*/
	pRelayCfgDtStr->RelayId = dquCiShortFromChar(pData);
	pData += 2;

	for(i=0;i<JDQ_NAME_LEN;i++)
	{
		Name[i] = *pData;
		pData++;
	}
    /*��������ID (�����ֶ�)*/
	pRelayCfgDtStr->BelongOcId = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*�ɼ�ģʽ(ǰ�ڵ�/��ڵ�/˫�ڵ�) (�����ֶ�)*/
	pRelayCfgDtStr->CollMode = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵǰ�ڵ�ɼ���ַ (�����ֶ�)*/
	pRelayCfgDtStr->ForeNodeCollAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵǰ�ڵ�ɼ�λ�� (�����ֶ�)*/
	pRelayCfgDtStr->ForeNodeCollPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵ��ڵ�ɼ���ַ (�����ֶ�)*/
	pRelayCfgDtStr->BackNodeCollAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵ��ڵ�ɼ�λ�� (�����ֶ�)*/
	pRelayCfgDtStr->BackNodeCollPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Bϵǰ�ڵ�ɼ���ַ (�����ֶ�)*/
	pRelayCfgDtStr->ForeNodeCollAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Bϵǰ�ڵ�ɼ�λ�� (�����ֶ�)*/
	pRelayCfgDtStr->ForeNodeCollPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
	/*Bϵ��ڵ�ɼ���ַ (�����ֶ�)*/
	pRelayCfgDtStr->BackNodeCollAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Bϵ��ڵ�ɼ�λ�� (�����ֶ�)*/
	pRelayCfgDtStr->BackNodeCollPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵ������ַ (�����ֶ�)*/
	pRelayCfgDtStr->DrvAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Aϵ����λ�� (�����ֶ�)*/
	pRelayCfgDtStr->DrvPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Bϵ������ַ (�����ֶ�)*/
	pRelayCfgDtStr->DrvAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*Bϵ����λ�� (�����ֶ�)*/
	pRelayCfgDtStr->DrvPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;

	rtnvalue = 1;
	return rtnvalue;
}



/*********************************************
*�������ܣ���ʼ������ͣ����ť�̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pEmgcyStpStr����ͣ����ť�̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_EmegencyStopStructFromChar(UINT8 * pDataAddr,Relay_EmgcyStpStruct* pEmgcyStpStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pEmgcyStpStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pEmgcyStpStr->wEmgcyStopBtnId = dquCiShortFromChar(pData);          /*����ͣ����ťID */
    pData += 2;   
	pEmgcyStpStr->wBelongOcId = dquCiShortFromChar(pData);              /*����OC ID*/
	pData += 2;
    pEmgcyStpStr->wEmgcyStopBtnRelayId = dquCiShortFromChar(pData);     /*����ͣ����ť�̵���ID */
    pData += 2; 

    rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pScrnStr�����ż̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_ScreenStructFromChar(UINT8 * pDataAddr,Relay_ScreenStruct* pScrnStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pScrnStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pScrnStr->wPsdId = dquCiShortFromChar(pData);                          /*������ID (�����ֶ�)*/
    pData += 2;   
	pScrnStr->wBelongOcId = dquCiShortFromChar(pData);                        /*����OC ID*/
	pData += 2;
    pScrnStr->wRelayRelatedId = dquCiShortFromChar(pData);                    /*��������̵���ID */
    pData += 2; 
	pScrnStr->wRelayDoorClosedId = dquCiShortFromChar(pData);                 /*�Źؼ̵���ID */
    pData += 2; 
	pScrnStr->wRelayCloseDoorId = dquCiShortFromChar(pData);                  /*���ż̵���ID */
    pData += 2;   
    pScrnStr->wRelayFourEditGroupOpenDoorId= dquCiShortFromChar(pData);       /*4���鿪�ż̵���ID */
    pData += 2; 
    pScrnStr->wRelaySixEditGroupOpenDoorId= dquCiShortFromChar(pData);        /*6���鿪�ż̵���ID */
    pData += 2;
	pScrnStr->wRelayEightEditGroupOpenDoorId= dquCiShortFromChar(pData);      /*8���鿪�ż̵���ID */
	pData += 2;
	pScrnStr->wRelayDoorButtonId = dquCiShortFromChar(pData);                 /*PCB��ť�̵���*/
	pData += 2;

    rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��������μ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  �������μ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_PhysicalSegmentStructFromChar(UINT8 * pDataAddr,Relay_PhysicalSegmentStruct* pPhysclSgmtStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pPhysclSgmtStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pPhysclSgmtStr->wPhysicalId = dquCiShortFromChar(pData);               /*��������ID*/
    pData += 2;   
    pPhysclSgmtStr->wBelongOcId = dquCiShortFromChar(pData);               /*����OC ID*/
    pData += 2; 
	pPhysclSgmtStr->wSwitchModuleIdA = dquCiShortFromChar(pData);          /*Aϵ��������ģ��ID*/
    pData += 2;
	pPhysclSgmtStr->wSwitchPosA = dquCiShortFromChar(pData);               /*Aϵ��������λ��*/
	pData += 2;
	pPhysclSgmtStr->wSwitchModuleIdB = dquCiShortFromChar(pData);          /*Bϵ��������ģ��ID*/
	pData += 2;
	pPhysclSgmtStr->wSwitchPosB = dquCiShortFromChar(pData);               /*Bϵ��������λ��*/
	pData += 2;  
    
    rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  ����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_PointStructFromChar(UINT8 * pDataAddr,Relay_PointStruct* pSwitchStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
    if ((NULL == pDataAddr) || (NULL == pSwitchStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }
	pSwitchStr->wSwitchId = dquCiShortFromChar(pData);                /*����ID*/
	pData += 2;   
	pSwitchStr->wBelongOcId = dquCiShortFromChar(pData);              /*����OC ID */
	pData += 2; 
	pSwitchStr->wSwitchModuleIdA = dquCiShortFromChar(pData);         /*Aϵ����ģ��ID */
	pData += 2;
	pSwitchStr->wSwitchPosA = dquCiShortFromChar(pData);              /*Aϵת�޻�λ��*/
	pData += 2;   
	pSwitchStr->wSwitchModuleIdB = dquCiShortFromChar(pData);         /*Bϵ����ģ��ID */
	pData += 2; 
	pSwitchStr->wSwitchPosB = dquCiShortFromChar(pData);              /*Bϵת�޻�λ�� */
	pData += 2;
	
    rtnvalue = 1;
	return rtnvalue;
}


/*********************************************
*�������ܣ������ݵ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  ����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_MultSwtichStructFromChar(UINT8 * pDataAddr,Relay_MultSwitchStruct* pMultSwitchStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	if ((NULL == pDataAddr) || (NULL == pMultSwitchStr))
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	pMultSwitchStr->wMultSwitchId = dquCiShortFromChar(pData);                /*����ID*/
	pData += 2;   
	pMultSwitchStr->wBelongOcId = dquCiShortFromChar(pData);              /*����OC ID */
	pData += 2;  
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ��źŻ��̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pSgnlStr�źŻ��̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_SignalStructFromChar(UINT8 * pDataAddr,Relay_SignalStruct* pSgnlStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pSgnlStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

    pSgnlStr->wSignalId = dquCiShortFromChar(pData);               /*�źŻ�ID*/
	pData += 2;
    pSgnlStr->wBelongOcId = dquCiShortFromChar(pData);             /*����OC ID*/
	pData += 2;
    pSgnlStr->wSignalModuleIdA = dquCiShortFromChar(pData);        /*Aϵ�źŻ�ģ��ID*/
	pData += 2;
    pSgnlStr->wSignalPosA = dquCiShortFromChar(pData);             /*Aϵ�źŻ�λ��*/
	pData += 2;
    pSgnlStr->wSignalModuleIdB = dquCiShortFromChar(pData);        /*Bϵ�źŻ�ģ��ID*/
	pData += 2;
    pSgnlStr->wSignalPosB = dquCiShortFromChar(pData);             /*Bϵ�źŻ�λ��*/
	pData += 2;

	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*�������ܣ������ݵ�ϴ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pWashMachineStr ϴ�����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_WashMachineStructFromChar(UINT8 * pDataAddr,Relay_WashMachineStruct * pWashMachineStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 1;

    if ((NULL == pDataAddr) || (NULL == pWashMachineStr))
    {
        rtnvalue = 0;
        
    }
    if(1 == rtnvalue)
    {
	    pWashMachineStr->wId = dquCiShortFromChar(pData);                              /*ϴ����ID*/
		pData += 2;
	    pWashMachineStr->WashMachineReadyRelayId = dquCiShortFromChar(pData);          /*ϴ���������̵���ID*/
		pData += 2;
		pWashMachineStr->WashMachineRequestVerifyRelayId = dquCiShortFromChar(pData);  /*ϴ������ָ�ȷ�ϼ̵���ID*/
		pData += 2;
	    pWashMachineStr->TrainEmergencyStopRelayId = dquCiShortFromChar(pData);        /*�г���ͣ�̵���ID*/
		pData += 2;
	    pWashMachineStr->HeadWashAllowRelayId = dquCiShortFromChar(pData);             /*����1�̵���ID*/
		pData += 2;
	    pWashMachineStr->TailWashAllowRelayId = dquCiShortFromChar(pData);             /*����2�̵���ID*/
		pData += 2;
	    pWashMachineStr->WashTrainRequestRelayId= dquCiShortFromChar(pData);           /*ϴ������̵���ID*/
		pData += 2;
		
	    pWashMachineStr->TrainHeadStopSteadyRelayId = dquCiShortFromChar(pData);       /*�г�ͣ��1�̵���ID*/
		pData += 2; 
	    pWashMachineStr->TrainTailStopSteadyRelayId = dquCiShortFromChar(pData);       /*�г�ͣ��2�̵���ID*/
		pData += 2;		
	    pWashMachineStr->TrainOutWashTrainPortAllowRelayId = dquCiShortFromChar(pData);/*����̵���ID*/
		pData += 2;
		pWashMachineStr->PauseTrainWashRequestRelayId = dquCiShortFromChar(pData);		/*��ͣϴ���̵���ID*/
		pData += 2;
		
		rtnvalue = 1;
    }
	 
	return rtnvalue;

}
/*********************************************
*�������ܣ������ݵ��ǽ�·���ݼ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pNoRotStr�ǽ�·���ݼ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_NoRouteDcStruFromChar(UINT8 * pDataAddr,Relay_NoRouteDcStruct* pNoRotStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pNoRotStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pNoRotStr->wId = dquCiShortFromChar(pData);               /*�ǽ�·����ID*/
	pData += 2;
	pNoRotStr->wRelayShenQingScId = dquCiShortFromChar(pData);         /* �����βɼ��������Գ��̵���ID */
	pData += 2;
	pNoRotStr->wRelayTongYiScId = dquCiShortFromChar(pData);         /* ������������ͬ���Գ��̵���ID */
	pData += 2;
   
	pNoRotStr->wRelayDrvShiCheShengQingId = dquCiShortFromChar(pData);         /* �Գ��������������Գ��̵���ID */
	pData += 2;

	pNoRotStr->wRelayColTongYiScId = dquCiShortFromChar(pData);	/* �Գ��߲ɼ�ͬ���Գ��̵���ID */
	pData += 2;

	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*�������ܣ������ݵ������Ƽ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ�� pBjdStr �����Ƽ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_BaoJingDengStruFromChar(UINT8 * pDataAddr,Relay_BaoJingDengStruct* pBjdStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT16 i=0;

    if ((NULL == pDataAddr) || (NULL == pBjdStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pBjdStr->wId = dquCiShortFromChar(pData);               /*ID*/
	pData += 2;
	pBjdStr->wBelongCiId = dquCiShortFromChar(pData);               /*����ID*/
	pData += 2;
	pBjdStr->wBjdID = dquCiShortFromChar(pData);               /*������ID*/
	pData += 2;
	pBjdStr->wBjdYouXiao = *pData;         /*�Ƿ���Ч*/
	pData += 1;
	for(i=0;i<BJD_MAX_NUM;i++)
	{
		pBjdStr->wBjdJdqId[i] = dquCiShortFromChar(pData);         /*�Զ˼̵���ID*/
		pData += 2;
	}
	
   
	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*�������ܣ������ݵ������۷��̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pWuRenRtnStr�����۷��̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_NoReturnStruFromChar(UINT8 * pDataAddr,Relay_NoReturnStruct* pWuRenRtnStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pWuRenRtnStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pWuRenRtnStr->wId = dquCiShortFromChar(pData);               /*ID*/
	pData += 2;
	pWuRenRtnStr->wWuRenReturnRelayId = dquCiShortFromChar(pData);     /*�̵�����ID*/
	pData += 2;

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ�������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pGaragedoorStr�����ż̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_GaragedoorStructFromChar(UINT8 * pDataAddr,Relay_GaragedoorStruct* pGaragedoorStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pGaragedoorStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pGaragedoorStr->wGaragedoorId = dquCiShortFromChar(pData);          /* ������ID */
		pData += 2;   
		pGaragedoorStr->wGardoorCloseRelayId = dquCiShortFromChar(pData);          /* �����Ź��ż̵���ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorOpenRelayId = dquCiShortFromChar(pData);          /* �����ſ��ż̵���ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorCloseOrderRelayId = dquCiShortFromChar(pData);          /* �����Ź�������̵���ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorOpenOrderRelayId = dquCiShortFromChar(pData);          /* �����ſ�������̵���ID*/
		pData += 2; 
        pGaragedoorStr->wGardoorModeStateRelayId = dquCiShortFromChar(pData);          /* ������ģʽ״̬�̵���ID*/
        pData += 2; 
        pGaragedoorStr->wGardoorFaultStateRelayId = dquCiShortFromChar(pData);          /* �����Ź��ϼ̵���ID*/
        pData += 2; 
		rtnvalue = 1;
	}
	return rtnvalue;
}


/*********************************************
*�������ܣ������ݵ�SPKS�̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pSpksStr SPKS���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_SpksDataFromChar(UINT8 * pDataAddr,RelaySpksStruct* pSpksStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL == pDataAddr) || (NULL == pSpksStr))
	{
		rtnvalue = 0;		
	}
	else
	{
		pSpksStr->id = dquCiShortFromChar(pData);               /* ID */
		pData += 2;
		pSpksStr->colRelayId = dquCiShortFromChar(pData);     /* �ɼ��̵�����ID */
		pData += 2;
		pSpksStr->disRelayId = dquCiShortFromChar(pData);     /* ��ʾ�̵�����ID */
		/*pData += 2;*/
		pSpksStr->cutRelayId = dquCiShortFromChar(pData);     /* ��·�̵�����ID */
		pData += 2;
		rtnvalue = 1;
	}
	
	return rtnvalue;
}


/*********************************************
*�������ܣ������ݵ������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pFloodGateStr ���������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_FloodGateStructFromChar(UINT8 * pDataAddr,Relay_FloodGateStruct* pFloodGateStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL == pDataAddr) || (NULL == pFloodGateStr))
	{
		rtnvalue = 0;		
	}
	else
	{
		pFloodGateStr->wFloodGateId = dquCiShortFromChar(pData);               /* ������ID */
		pData += 2;
		pFloodGateStr->wCloseApplyRelayId = dquCiShortFromChar(pData);     /* ��������̵���ID */
		pData += 2;
		pFloodGateStr->wOpenCloseRelayId = dquCiShortFromChar(pData);     /* ���������ռ̵���ID */
		pData += 2;
		pFloodGateStr->wAllowRequestRelayId = dquCiShortFromChar(pData);     /* ��������̵���ID */
		pData += 2;

		rtnvalue = 1;
	}
	
	return rtnvalue;
}