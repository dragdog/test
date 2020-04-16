/************************************************************************
*
* �ļ���   ��  dquCILSStruFrmChar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �����ݵ���·������ת���������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCILSStruFrmChar.h"
#include "dquCIReadCommonChar.h"
/*********************************************
*�������ܣ������ݵ�����·��ת��
*����˵����pDataAddr���ݵ�ַ��  pLongRouteStr����·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_LongRouteStruFromChar(UINT8 * pDataAddr,LS_LongRouteStru* pLongRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pLongRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pLongRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<LR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pLongRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��ǽ�·���������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pNoRouteDcStr�ǽ�·�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_NoRouteDcStruFromChar(UINT8 * pDataAddr,LS_NoRouteDcStru* pNoRouteDcStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pNoRouteDcStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	/*ID*/
	pNoRouteDcStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	/*�ǽ�·���������źŻ�ID*/
	pNoRouteDcStr->wIncludeSignal =  dquCiShortFromChar(pData);
	pData+=2;

	/*�������κͼ������ID*/
	pNoRouteDcStr->wWuliJianchaType =  dquCiShortFromChar(pData);
	pData+=2;

	/*����������*/
	for(i=0;i<NR_MAX_INCLUDE_POINT_NUM;i++)
	{
		pNoRouteDcStr->wIncludePoint[i] = dquCiShortFromChar(pData);
		pData+=2;
		pNoRouteDcStr->wIncludePointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	 /*��������Id*/
	pNoRouteDcStr->wFHPointId =  dquCiShortFromChar(pData);
	pData+=2;

	/*�ǽ�·���������ж��źŽṹ*/
	for(i=0;i<NR_MAX_INCLUDE_DDXH_NUM;i++)
	{
		pNoRouteDcStr->wDiDuiXinhao[i] = dquCiShortFromChar(pData);
		pData+=2;
		
	}
	
	/*��������ID*/
	for(i=0; i<ROUTE_MAX_QXQD_NUM; i++)
	{
	    pNoRouteDcStr->wIntrudePhySecId[i] = dquCiShortFromChar(pData);
	    pData+=2;
    }
	rtnvalue = 1;
	return rtnvalue;
}


/*********************************************
*�������ܣ������ݵ��Զ���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAutoRouteStr�Զ���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AutoRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRouteStru* pAutoRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAutoRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pAutoRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<AR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pAutoRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
    pData+=20;
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ��Զ��۷���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAutoRtnRouteStr�Զ��۷���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRtnRouteStru* pAutoRtnRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAutoRtnRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pAutoRtnRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<ARR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pAutoRtnRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
    pData+=20;
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ������·��ת��
*����˵����pDataAddr���ݵ�ַ��  pReverseRouteStr�����·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_ReverseRouteStruFromChar(UINT8 * pDataAddr,LS_ReverseRouteStru* pReverseRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pReverseRouteStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pReverseRouteStr->wId =  dquCiShortFromChar(pData);
		pData+=2;

		for(i=0;i<RR_MAX_INCLUDE_ROUTE_NUM;i++)
		{
			pReverseRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
			pData+=2;
		}
		rtnvalue = 1;	
	}
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ��Զ���·�������μ��������ת��
*����˵����pDataAddr���ݵ�ַ�� pRoutePhyAndCheckStateStr �Զ���·�������μ�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RoutePhyAndCheckStateStruFromChar(UINT8 * pDataAddr,LS_RoutePhyAndCheckStateStruct* pRoutePhyAndCheckStateStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRoutePhyAndCheckStateStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pRoutePhyAndCheckStateStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<MAX_INCLUDE_PHYSC_SGMT_NUM;i++)
	{
		pRoutePhyAndCheckStateStr->wPhysicalSgmtId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pRoutePhyAndCheckStateStr->wCheckState[i] = *pData;
		pData++;
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ���������ת��
*����˵����pDataAddr���ݵ�ַ��  pFHDaoChaStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_FHDaoChaStruFromChar(UINT8 * pDataAddr,LS_FHDaoChaStruct* pFHDaoChaStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pFHDaoChaStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pFHDaoChaStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<FHDC_MAX_INCLUDE_POINT_NUM;i++)
	{
		pFHDaoChaStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pFHDaoChaStr->wPointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ���������������ת��
*����˵����pDataAddr���ݵ�ַ��  pDrivedSwitchGroupStru�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DrvSwiGrpStruFromChar(UINT8 * pDataAddr, LS_RouteDrivedSwitchGroupStruct * pDrivedSwitchGroupStru)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDrivedSwitchGroupStru))
    {
        rtnvalue = 0;
    }
	else
	{
		pDrivedSwitchGroupStru->wId = dquCiShortFromChar(pData);

		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		for(i = 0; i < DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM; i++)
		{
			pDrivedSwitchGroupStru->wPointId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

			pDrivedSwitchGroupStru->wPointSta[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		rtnvalue = 1;
	}

	return rtnvalue;
}


/*********************************************
*�������ܣ������ݵ��ж��ź�ת��
*����˵����pDataAddr���ݵ�ַ�� pDDXHStr �ж��ź����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DDXHStruFromChar(UINT8 * pDataAddr,LS_DDXHStruct* pDDXHStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDDXHStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pDDXHStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	pDDXHStr->wDiDuiXinHaoId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<DDXH_MAX_INCLUDE_POINT_NUM;i++)
	{
		pDDXHStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pDDXHStr->wPointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��ж��ź���ת��
*����˵����pDataAddr���ݵ�ַ��  pDDXHZStr�ж��ź����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DDXHZStruFromChar(UINT8 * pDataAddr,LS_DDXHZStruct* pDDXHZStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDDXHZStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pDDXHZStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	for(i=0;i<DDXHZ_MAX_DDXH_NUM;i++)
	{
		pDDXHZStr->wDiDuiXinHaoId[i] = dquCiShortFromChar(pData);
	    pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��źŻ���ת��
*����˵����pDataAddr���ݵ�ַ�� pSignalStr �źŻ����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_SignalStruFromChar(UINT8 * pDataAddr,LS_SignalStruct* pSignalStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pSignalStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pSignalStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	

	for(i=0;i<MAX_INCLUDE_SIGNAL_NUM;i++)
	{
		pSignalStr->wSignalId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��������μ�������������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pIntruPhySecStr�������μ������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_IntrudePhySectionStruFromChar(UINT8 * pDataAddr,LS_IntrudePhySectionStruct* pIntruPhySecStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pIntruPhySecStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pIntruPhySecStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	pIntruPhySecStr->wIntrudePhySecId = dquCiShortFromChar(pData);
	pData+=2;
	

	for(i=0;i<QXQD_MAX_INCLUDE_POINT_NUM;i++)
	{
		pIntruPhySecStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pIntruPhySecStr->wPointSta[i] = dquCiShortFromChar(pData);
	    pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ��������������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pOtherCIStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_OtherCIStruFromChar(UINT8 * pDataAddr,LS_OtherCIStruct* pOtherCIStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pOtherCIStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pOtherCIStr->wId = dquCiShortFromChar(pData);
	pData+=2;

#if 0
	for(i=0;i<OTHER_MAX_PSD_NUM;i++)
	{
		pOtherCIStr->wPsdId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	for(i=0;i<OTHER_MAX_EMERG_CLSBTN_NUM;i++)
	{
		pOtherCIStr->wEmgClsBtn[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
#else
	/* һ��������������֧���ܹ�10���豸��Ϣ */
	for(i=0;i<OTHER_CI_MAX_DEV_NUM;i++)
	{
		pOtherCIStr->otherCiDevInfoBuf[i].wDevType = *(pData);
		pData+=1;
		pOtherCIStr->otherCiDevInfoBuf[i].wId = dquCiShortFromChar(pData);
		pData+=2;
		pOtherCIStr->otherCiDevInfoBuf[i].wDevStatus = *(pData);
		pData+=1;
	}
#endif
	rtnvalue = 1;
	return rtnvalue;
}



/*********************************************
*�������ܣ������ݵ���·���ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��pRouteStr  ���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RouteStruFromChar(UINT8 * pDataAddr,LS_RouteStruct* pRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	 /*��·ID */
	pRouteStr->wRouteId = dquCiShortFromChar(pData);
	pData+=2;

	 /*��·���� */
	pRouteStr->wDir = *pData;
	pData++;

	 

	/*��������*/
	pRouteStr->wYinDao = *pData;
	pData++;

	/*ʼ�˰�ť��Ӧ�źŻ�ID*/
	pRouteStr->wStarBtnSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*��ͨ��ť��Ӧ�źŻ�ID*/
	pRouteStr->wFlexibleSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*�ն˰�ť��Ӧ�źŻ�ID*/
	pRouteStr->wEndBtnSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*ʼ���źŻ���ʾ��ɫ*/
	pRouteStr->wStarSignalClr = dquCiShortFromChar(pData);
	pData+=2;

	/*ʼ���źŻ�����������ɫ*/
	pRouteStr->wStarSigYinDaoClr = dquCiShortFromChar(pData);
	pData+=2;

    /*�������μ��������ID*/ 
	pRouteStr->wPhySecCheckStaId= dquCiShortFromChar(pData);
	pData+=2;

    /*��·����������ID*/ 
	pRouteStr->wFHDCId= dquCiShortFromChar(pData);
	pData+=2;

	/*�ж��źż���������ID*/
	
    pRouteStr->wDDXHId = dquCiShortFromChar(pData);
	   pData+=2;
	
	 /*��������ID*/
	for(i=0;i<ROUTE_MAX_QXQD_NUM;i++)
	{
		pRouteStr->wIntrudePhySecId[i] = dquCiShortFromChar(pData);
	   pData+=2;
	}

     /*��·�۷��źŻ���ID*/
	pRouteStr->wReturnSignalId = dquCiShortFromChar(pData);
	pData+=2;

	/*�������ν�����Ӧͣ����������ID*/
	pRouteStr->wFHQD_TW_WLQDId = dquCiShortFromChar(pData);
	pData+=2;

	/*��·��ؿɷ����źŻ���ID*/
	pRouteStr->wFS_SignalId = dquCiShortFromChar(pData);
	pData+=2;

	/*��·������ر��źŻ���ID*/
	pRouteStr->wSignalClsId = dquCiShortFromChar(pData);
	pData+=2;

	 /*��·�����迪���źŻ���ID*/
	pRouteStr->wSignalOpenId = dquCiShortFromChar(pData);
	pData+=2;

	/*��ͬʱ�����źŻ���*/
	pRouteStr->wBTSOpenId = dquCiShortFromChar(pData);
	pData+=2;

	/*����������ID*/
	pRouteStr->wDrvSwiGrpId = dquCiShortFromChar(pData);
	pData+=2;
	
	/*CBTC������������ID*/
	pRouteStr->wCBTCOtherCIID = dquCiShortFromChar(pData);
	pData+=2;
	  /*��ʽ������������ID*/
    pRouteStr->wBLCKOtherCIID = dquCiShortFromChar(pData);
	pData+=2;

	/* ������ ID */
	for(i=0;i<ROUTE_MAX_FLOOD_GATE_NUM;i++)
	{
		pRouteStr->wFloodGateIdBuf[i] = dquCiShortFromChar(pData);
		pData+=2;
	}

	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*�������ܣ������ݵ�ȫ�Զ��۷���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAllAutoRtnRouteStrȫ�Զ��۷���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AllAutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AllAutoRtnRouteStru* pAllAutoRtnRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAllAutoRtnRouteStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pAllAutoRtnRouteStr->wId =  dquCiShortFromChar(pData);
		pData+=2;

		for(i=0;i<AARR_MAX_INCLUDE_ROUTE_NUM;i++)
		{
			pAllAutoRtnRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
			pData+=2;
		}
        pData+=20;
		rtnvalue = 1;
	}
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ�������������������ת��
*����˵����
				pDataAddr���ݵ�ַ��
				pOverlapLockCondStru �������������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_OverlapLockCondStruFromChar(UINT8 * pDataAddr, LS_OveralpLockCondStru * pOverlapLockCondStru)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pOverlapLockCondStru))
    {
        rtnvalue = 0;
    }
	else
	{
		/*��ȡ��������ID*/
		pOverlapLockCondStru->wOverlapId = dquCiShortFromChar(pData);
		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		/*��ȡ�������ΰ����۷���·ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeReturnRouteId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*��ȡ�������ΰ����۷�����������ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeRetTrkPhySecId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*��ȡ�������ΰ����������μ���������ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeInvSecAndConSwiId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*��ȡ�������ΰ����������޵жԱ�������ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeInvOppOverlapId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*��ȡ��������������������������ID*/
		pOverlapLockCondStru->overlapLocProSwitchGroupID = dquCiShortFromChar(pData);
		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		rtnvalue = 1;
	}
	return rtnvalue;
}

/*********************************************
*�������ܣ������ݵ��������������ϵ��ת��
*����˵����
				pDataAddr���ݵ�ַ��
				pRouteMonoRailSwitStru �������������ϵ���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RouteMonoRailSwitStruFromChar(UINT8 * pDataAddr, LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru)
{
	UINT8 * pData = pDataAddr;

	UINT16 i = 0;

	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRouteMonoRailSwitStru))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pRouteMonoRailSwitStru->routeId = dquCiShortFromChar(pData);

	pData += 2;

	for(i = 0; i < ROUTE_MAX_MONO_RAIL_SWITCH_NUM ; i++)
	{
		pRouteMonoRailSwitStru->monoRailSwitIdt[i] = dquCiShortFromChar(pData);
		pData += 2;

		pRouteMonoRailSwitStru->monoRailSwitSta[i] = dquCiShortFromChar(pData);
		pData += 2;
	}

	rtnvalue = 1;

	return rtnvalue;
}


