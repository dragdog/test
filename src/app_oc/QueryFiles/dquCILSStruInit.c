/************************************************************************
*
* �ļ���   ��  dquCILSStruInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �����ṹ�嶨�崦�����
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCILSStruInit.h"
#include "dquCILSStru.h"


/*********����ȫ�ֵ�������Ϣ���ݱ���*******/
LSDataStruct LSData;
LSDataStructLen LSDataLen;
/***************************/



/*********************************************
*�������ܣ���䳤��·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_LongRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_LongRouteStru* pLsLongRouteStr;   /*��ʱ����·����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSLongRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,LONG_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LONG_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSLongRouteStrLen = (UINT16)(DataSize/LONG_ROUTE_DATA_LEN);

		if(DataSize<LONG_ROUTE_DATA_LEN)
	   {
		   LSData.pLSLongRouteStr = (LS_LongRouteStru *) MyNew (sizeof(LS_LongRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSLongRouteStr = (LS_LongRouteStru  *) MyNew (sizeof(LS_LongRouteStru) * LSDataLen.LSLongRouteStrLen);
		/*��̬����ռ�ʧ��*/
        if(NULL == LSData.pLSLongRouteStr)
        {
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSLongRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
        }
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSLongRouteStr, 0x00,sizeof(LS_LongRouteStru) * LSDataLen.LSLongRouteStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSLongRouteStrLen;j++)
		{
			pLsLongRouteStr = LSData.pLSLongRouteStr + j;
			
			/*������ݽṹ��*/
			 LS_LongRouteStruFromChar(pDataAddr,pLsLongRouteStr);
			pDataAddr += LONG_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ�����Զ���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AutoRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_AutoRouteStru* pLsAutoRouteStr;   /*��ʱ����·����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSAutoRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,AUTO_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%AUTO_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSAutoRouteStrLen = (UINT16)(DataSize/AUTO_ROUTE_DATA_LEN);

		if(DataSize<AUTO_ROUTE_DATA_LEN)
	   {
		   LSData.pLSAutoRouteStr = (LS_AutoRouteStru *) MyNew (sizeof(LS_AutoRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSAutoRouteStr = (LS_AutoRouteStru  *) MyNew (sizeof(LS_AutoRouteStru) * LSDataLen.LSAutoRouteStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSAutoRouteStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSAutoRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSAutoRouteStr, 0x00,sizeof(LS_AutoRouteStru) * LSDataLen.LSAutoRouteStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSAutoRouteStrLen;j++)
		{
			pLsAutoRouteStr = LSData.pLSAutoRouteStr + j;
			
			/*������ݽṹ��*/
			 LS_AutoRouteStruFromChar(pDataAddr,pLsAutoRouteStr);
			pDataAddr += AUTO_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�����Զ��۷���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AutoRtnRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_AutoRtnRouteStru* pLsAutoRtnRouteStr;   /*��ʱ����·����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSAutoRtnRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,AUTO_RTN_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%AUTO_RTN_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSAutoRtnRouteStrLen = (UINT16)(DataSize/AUTO_RTN_ROUTE_DATA_LEN);

		if(DataSize<AUTO_RTN_ROUTE_DATA_LEN)
	   {
		   LSData.pLSAutoRtnRouteStr = (LS_AutoRtnRouteStru *) MyNew (sizeof(LS_AutoRtnRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSAutoRtnRouteStr = (LS_AutoRtnRouteStru  *) MyNew (sizeof(LS_AutoRtnRouteStru) * LSDataLen.LSAutoRtnRouteStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSAutoRtnRouteStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSAutoRtnRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSAutoRtnRouteStr, 0x00,sizeof(LS_AutoRtnRouteStru) * LSDataLen.LSAutoRtnRouteStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSAutoRtnRouteStrLen;j++)
		{
			pLsAutoRtnRouteStr = LSData.pLSAutoRtnRouteStr + j;
			
			/*������ݽṹ��*/
			 LS_AutoRtnRouteStruFromChar(pDataAddr,pLsAutoRtnRouteStr);
			pDataAddr += AUTO_RTN_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���䷴���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_ReverseRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_ReverseRouteStru* pLsReverseRouteStr = NULL;   /*��ʱ�����·����*/
	UINT8* pDataAddr = NULL;         /*���ݵ�ַ*/
	UINT32 DataSize = 0;             /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 1;

	
	if(LSData.pLSReverseRouteStr != NULL)
	{/*�Ѿ���ʼ���ˣ������ظ����г�ʼ��*/
		rtnValue = 1;
	}
	else
	{
		
		if(1 == rtnValue)
		{	/*��ȡ�����·�����ݵ�ָ���ڴ�*/
			funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,REVERSE_ROUTE_DATA_ID,&pDataAddr,&DataSize);
			if(0 == funcRtn)
			{
				/*��������ʧ��,ֱ�ӷ���*/
				rtnValue = 0;
			} 	
		}
		
		if(1 == rtnValue)
		{	/*��ø���*/
			if(DataSize % REVERSE_ROUTE_DATA_LEN != 0)
			{   /*���ݲ��Ϸ���ֱ�ӷ���*/
				rtnValue = 0;
			}			
		}
		
		if(1 == rtnValue)
		{
			/*��ȡ������*/
			LSDataLen.LSReverseRouteStrLen = (UINT16)(DataSize / REVERSE_ROUTE_DATA_LEN);

			if(DataSize < REVERSE_ROUTE_DATA_LEN)
			{/*�����·���в���һ������:�����飬ֱ�ӷ��سɹ�*/
			   LSData.pLSReverseRouteStr = (LS_ReverseRouteStru *) MyNew(sizeof(LS_ReverseRouteStru));
			}
			else
			{
				/*��̬����ռ�*/
				LSData.pLSReverseRouteStr = (LS_ReverseRouteStru  *) MyNew(sizeof(LS_ReverseRouteStru) * LSDataLen.LSReverseRouteStrLen);
				/*��̬����ռ�ʧ��*/
				if(NULL == LSData.pLSReverseRouteStr)
				{
					/*����ռ�ʧ�ܣ������鳤����0�������������Խ�磬ֱ�ӷ���ʧ��*/
					LSDataLen.LSReverseRouteStrLen = 0;
					
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(LSData.pLSReverseRouteStr, 0x00,sizeof(LS_ReverseRouteStru) * LSDataLen.LSReverseRouteStrLen);
				}
				
				if(1 == rtnValue)
				{
					/*�������ݵ������·�ṹ������ */  
					for(j = 0;j < LSDataLen.LSReverseRouteStrLen;j++)
					{
						pLsReverseRouteStr = LSData.pLSReverseRouteStr + j;
						
						/*������ݽṹ��*/
						LS_ReverseRouteStruFromChar(pDataAddr,pLsReverseRouteStr);
						pDataAddr += REVERSE_ROUTE_DATA_LEN;

					}	
				}
			}
		}
		

	}
	return rtnValue;
}



/*********************************************
*�������ܣ����ǽ�·�������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_NoRouteDcStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_NoRouteDcStru* pNoRouteDcStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSNoRouteDcStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,NO_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%NO_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSNoRouteDcStrLen = (UINT16)(DataSize/NO_ROUTE_DATA_LEN);

		if(DataSize<NO_ROUTE_DATA_LEN)
	   {
		   LSData.pLSNoRouteDcStr = (LS_NoRouteDcStru *) MyNew (sizeof(LS_NoRouteDcStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSNoRouteDcStr = (LS_NoRouteDcStru  *) MyNew (sizeof(LS_NoRouteDcStru) * LSDataLen.LSNoRouteDcStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSNoRouteDcStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSNoRouteDcStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSNoRouteDcStr, 0x00,sizeof(LS_NoRouteDcStru) * LSDataLen.LSNoRouteDcStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSNoRouteDcStrLen;j++)
		{
			pNoRouteDcStr = LSData.pLSNoRouteDcStr + j;
			
			/*������ݽṹ��*/
			 LS_NoRouteDcStruFromChar(pDataAddr,pNoRouteDcStr);
			pDataAddr += NO_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ�����������μ�������ͱ����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_RoutePhyAndCheckStateStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_RoutePhyAndCheckStateStruct* pRoutePhyChkStateStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSPhyAndChkStaStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,PHY_SGMT_CHECK_STATE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%PHY_SGMT_CHECK_STATE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSPhyAndChkStaStrLen = (UINT16)(DataSize/PHY_SGMT_CHECK_STATE_DATA_LEN);

		if(DataSize<PHY_SGMT_CHECK_STATE_DATA_LEN)
	   {
		   LSData.pLSPhyAndChkStaStr = (LS_RoutePhyAndCheckStateStruct *) MyNew (sizeof(LS_RoutePhyAndCheckStateStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSPhyAndChkStaStr = (LS_RoutePhyAndCheckStateStruct  *) MyNew (sizeof(LS_RoutePhyAndCheckStateStruct) * LSDataLen.LSPhyAndChkStaStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSPhyAndChkStaStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSPhyAndChkStaStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSPhyAndChkStaStr, 0x00,sizeof(LS_RoutePhyAndCheckStateStruct) * LSDataLen.LSPhyAndChkStaStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSPhyAndChkStaStrLen;j++)
		{
			pRoutePhyChkStateStr = LSData.pLSPhyAndChkStaStr + j;
			
			/*������ݽṹ��*/
			 LS_RoutePhyAndCheckStateStruFromChar(pDataAddr,pRoutePhyChkStateStr);
			pDataAddr += PHY_SGMT_CHECK_STATE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ���������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_FHDaoChaStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_FHDaoChaStruct* pFHDaoChaStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSFHDaoChaStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,FHDC_POINT_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%FHDC_POINT_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSFHDaoChaStrLen = (UINT16)(DataSize/FHDC_POINT_DATA_LEN);

		if(DataSize<FHDC_POINT_DATA_LEN)
	   {
		   LSData.pLSFHDaoChaStr = (LS_FHDaoChaStruct *) MyNew (sizeof(LS_FHDaoChaStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSFHDaoChaStr = (LS_FHDaoChaStruct  *) MyNew (sizeof(LS_FHDaoChaStruct) * LSDataLen.LSFHDaoChaStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSFHDaoChaStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSFHDaoChaStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSFHDaoChaStr, 0x00,sizeof(LS_FHDaoChaStruct) * LSDataLen.LSFHDaoChaStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSFHDaoChaStrLen;j++)
		{
			pFHDaoChaStr = LSData.pLSFHDaoChaStr + j;
			
			/*������ݽṹ��*/
			 LS_FHDaoChaStruFromChar (pDataAddr,pFHDaoChaStr);
			pDataAddr += FHDC_POINT_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�����������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DrivedSwitchGroupStru(CHAR* LSDataName)
{
	UINT8 funcRtn = 0;
	LS_RouteDrivedSwitchGroupStruct* pTempDrivedSwitchGroupStru = NULL;   /*��ʱ����*/
	UINT8* pDataAddr = NULL;		  /*���ݵ�ַ*/
	UINT32 DataSize = 0;		  /*���ݴ�С���ֽ�����*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSAssocSwitchGroupStru != NULL)
	{
		rtnValue = 1;
	}
	if (0 == rtnValue)
	{
		/*��ȡ��·��������������*/
		funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, DRV_SWI_GRP_POINT_DATA_ID, &pDataAddr, &DataSize);

		if(0 == funcRtn)
		{
			/*��������ʧ��*/
			rtnValue = 0;
		}
		else
		{

			/*��ø���*/
			if(DataSize % DRV_SWI_GRP_POINT_DATA_LEN != 0)
			{   
				rtnValue = 0;
			}
			else
			{
				LSDataLen.LSAssocSwitchGroupStruLen = (UINT16)(DataSize/DRV_SWI_GRP_POINT_DATA_LEN);

				if(DataSize < DRV_SWI_GRP_POINT_DATA_LEN)
				{
					LSData.pLSAssocSwitchGroupStru = (LS_RouteDrivedSwitchGroupStruct *) MyNew (sizeof(LS_RouteDrivedSwitchGroupStruct));
					rtnValue = 1;
				}
				else
				{
					/*��̬����ռ�*/
					LSData.pLSAssocSwitchGroupStru = (LS_RouteDrivedSwitchGroupStruct *) MyNew (sizeof(LS_RouteDrivedSwitchGroupStruct) * LSDataLen.LSAssocSwitchGroupStruLen);
					/*��̬����ռ�ʧ��*/
					if(NULL == LSData.pLSAssocSwitchGroupStru)
					{
						/*�����鳤����0�������������Խ��*/
						LSDataLen.LSAssocSwitchGroupStruLen = 0;

						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(LSData.pLSAssocSwitchGroupStru, 0x00, sizeof(LS_RouteDrivedSwitchGroupStruct) * LSDataLen.LSAssocSwitchGroupStruLen);

						/*�������ݵ� Ĭ���г����� �ṹ������ */  
						for(j = 0; j < LSDataLen.LSAssocSwitchGroupStruLen; j++)
						{
							pTempDrivedSwitchGroupStru = LSData.pLSAssocSwitchGroupStru + j;

							/*������ݽṹ��*/
							LS_DrvSwiGrpStruFromChar(pDataAddr, pTempDrivedSwitchGroupStru);
							pDataAddr += DRV_SWI_GRP_POINT_DATA_LEN;

						}

						rtnValue = 1;
					}
				}
			}
		}
	}

	return rtnValue;
}


/*********************************************
*�������ܣ����ж��źű����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DDXHStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_DDXHStruct* pDDXHStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSDDXHStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,DDXH_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%DDXH_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSDDXHStrLen = (UINT16)(DataSize/DDXH_DATA_LEN);

		if(DataSize<DDXH_DATA_LEN)
	   {
		   LSData.pLSDDXHStr = (LS_DDXHStruct *) MyNew (sizeof(LS_DDXHStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		LSData.pLSDDXHStr = (LS_DDXHStruct  *) MyNew (sizeof(LS_DDXHStruct) * LSDataLen.LSDDXHStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSDDXHStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSDDXHStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSDDXHStr, 0x00,sizeof(LS_DDXHStruct) * LSDataLen.LSDDXHStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSDDXHStrLen;j++)
		{
			pDDXHStr = LSData.pLSDDXHStr + j;
			
			/*������ݽṹ��*/
			 LS_DDXHStruFromChar (pDataAddr,pDDXHStr);
			pDataAddr += DDXH_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���������ж��źŻ�������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DDXHZStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_DDXHZStruct* pDDXHZStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;
	if(LSData.pLSDDXHZStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,DDXHZ_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   rtnValue = 0;
			return rtnValue;
	   } 	
	   if(DataSize%DDXHZ_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }

	   LSDataLen.LSDDXHZStrLen = (UINT16)(DataSize/DDXHZ_DATA_LEN);
		if(DataSize<DDXHZ_DATA_LEN)
	   {
		   LSData.pLSDDXHZStru = (LS_DDXHZStruct *) MyNew (sizeof(LS_DDXHZStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
	   }

		LSData.pLSDDXHZStru = (LS_DDXHZStruct  *) MyNew (sizeof(LS_DDXHZStruct) * LSDataLen.LSDDXHZStrLen);
		if(NULL == LSData.pLSDDXHZStru)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSDDXHZStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			memset(LSData.pLSDDXHZStru, 0x00,sizeof(LS_DDXHZStruct) * LSDataLen.LSDDXHZStrLen);
		}
		for(j=0;j<LSDataLen.LSDDXHZStrLen;j++)
		{
			pDDXHZStr = LSData.pLSDDXHZStru + j;
			 LS_DDXHZStruFromChar (pDataAddr,pDDXHZStr);
			pDataAddr += DDXHZ_DATA_LEN;
		}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�����źŻ�������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_SignalStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_SignalStruct* pSignalStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSSignalStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,SIGNAL_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%SIGNAL_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSSignalStrLen = (UINT16)(DataSize/SIGNAL_DATA_LEN);

		if(DataSize<SIGNAL_DATA_LEN)
	   {
		   LSData.pLSSignalStr = (LS_SignalStruct *) MyNew (sizeof(LS_SignalStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }

		/*��̬����ռ�*/
		LSData.pLSSignalStr = (LS_SignalStruct  *) MyNew (sizeof(LS_SignalStruct) * LSDataLen.LSSignalStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSSignalStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSSignalStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSSignalStr, 0x00,sizeof(LS_SignalStruct) * LSDataLen.LSSignalStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSSignalStrLen;j++)
		{
			pSignalStr = LSData.pLSSignalStr + j;
			
			/*������ݽṹ��*/
			 LS_SignalStruFromChar (pDataAddr,pSignalStr);
			pDataAddr += SIGNAL_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ�����������μ�������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_IntrudePhySectionStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_IntrudePhySectionStruct* pInterPhySecStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSIntruPhySecStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,INTRU_PHYSEC_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%INTRU_PHYSEC_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSIntruPhySecStrLen = (UINT16)(DataSize/INTRU_PHYSEC_LEN);

		if(DataSize<INTRU_PHYSEC_LEN)
	   {
		   LSData.pLSIntruPhySecStr = (LS_IntrudePhySectionStruct *) MyNew (sizeof(LS_IntrudePhySectionStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }

		/*��̬����ռ�*/
		LSData.pLSIntruPhySecStr = (LS_IntrudePhySectionStruct  *) MyNew (sizeof(LS_IntrudePhySectionStruct) * LSDataLen.LSIntruPhySecStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSIntruPhySecStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSIntruPhySecStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSIntruPhySecStr, 0x00,sizeof(LS_IntrudePhySectionStruct) * LSDataLen.LSIntruPhySecStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSIntruPhySecStrLen;j++)
		{
			pInterPhySecStr = LSData.pLSIntruPhySecStr + j;
			
			/*������ݽṹ��*/
			 LS_IntrudePhySectionStruFromChar (pDataAddr,pInterPhySecStr);
			pDataAddr += INTRU_PHYSEC_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ�������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_OtherCIStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_OtherCIStruct* pOtherCiStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSOterCiStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,OTHER_CI_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%OTHER_CI_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSOterCiStrLen = (UINT16)(DataSize/OTHER_CI_DATA_LEN);

		if(DataSize<OTHER_CI_DATA_LEN)
	   {
		   LSData.pLSOterCiStr = (LS_OtherCIStruct *) MyNew (sizeof(LS_OtherCIStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }

		/*��̬����ռ�*/
		LSData.pLSOterCiStr = (LS_OtherCIStruct  *) MyNew (sizeof(LS_OtherCIStruct) * LSDataLen.LSOterCiStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSOterCiStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSOterCiStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSOterCiStr, 0x00,sizeof(LS_OtherCIStruct) * LSDataLen.LSOterCiStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSOterCiStrLen;j++)
		{
			pOtherCiStr = LSData.pLSOterCiStr + j;
			
			/*������ݽṹ��*/
			 LS_OtherCIStruFromChar (pDataAddr,pOtherCiStr);
			pDataAddr += OTHER_CI_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ����������-��·���ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_RouteStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_RouteStruct* pRouteStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSRouteStrLen = (UINT16)(DataSize/ROUTE_DATA_LEN);

		if(DataSize<ROUTE_DATA_LEN)
	   {
		   LSData.pLSRouteStr = (LS_RouteStruct *) MyNew (sizeof(LS_RouteStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }

		/*��̬����ռ�*/
		LSData.pLSRouteStr = (LS_RouteStruct  *) MyNew (sizeof(LS_RouteStruct) * LSDataLen.LSRouteStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == LSData.pLSRouteStr)
		{
            /*�����鳤����0�������������Խ��*/
            LSDataLen.LSRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(LSData.pLSRouteStr, 0x00,sizeof(LS_RouteStruct) * LSDataLen.LSRouteStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<LSDataLen.LSRouteStrLen;j++)
		{
			pRouteStr = LSData.pLSRouteStr + j;
			
			/*������ݽṹ��*/
			 LS_RouteStruFromChar (pDataAddr,pRouteStr);
			pDataAddr += ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ����ȫ�Զ��۷���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AllAutoRtnRouteStru(CHAR *LSDataName)
{
	UINT8 funcRtn;
	LS_AllAutoRtnRouteStru* pLsAllAutoRtnRouteStr =NULL;   /*��ʱ����·����*/
	UINT8* pDataAddr = NULL;         /*���ݵ�ַ*/
	UINT32 DataSize = 0;          /*���ݴ�С���ֽ�����*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;
	
	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSAllAutoRtnRouteStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
    	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,ALLAUTO_RTN_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
	   }
	   else
	   {
		   /*��ø���*/
		   if(DataSize%ALLAUTO_RTN_ROUTE_DATA_LEN != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {

			   LSDataLen.LSAllAutoRtnRouteStrLen = (UINT16)(DataSize/ALLAUTO_RTN_ROUTE_DATA_LEN);
			   
			   if(DataSize<ALLAUTO_RTN_ROUTE_DATA_LEN)
			   {
				   LSData.pLSAllAutoRtnRouteStr = (LS_AllAutoRtnRouteStru *) MyNew (sizeof(LS_AllAutoRtnRouteStru));
				   rtnValue = 1;
			   }
			   else
			   {
				   
				   /*��̬����ռ�*/
				   LSData.pLSAllAutoRtnRouteStr = (LS_AllAutoRtnRouteStru  *) MyNew (sizeof(LS_AllAutoRtnRouteStru) * LSDataLen.LSAllAutoRtnRouteStrLen);
				   /*��̬����ռ�ʧ��*/
				   if(NULL == LSData.pLSAllAutoRtnRouteStr)
				   {
					   /*�����鳤����0�������������Խ��*/
					   LSDataLen.LSAllAutoRtnRouteStrLen = 0;
					   
					   rtnValue = 0;
				   }
				   else
				   {
					   /*�ڴ�����Ϊ��Ч*/
					   memset(LSData.pLSAllAutoRtnRouteStr, 0x00,sizeof(LS_AllAutoRtnRouteStru) * LSDataLen.LSAllAutoRtnRouteStrLen);
					   
					   /*�������ݵ� Ĭ���г����� �ṹ������ */  
					   for(j=0;j<LSDataLen.LSAllAutoRtnRouteStrLen;j++)
					   {
						   pLsAllAutoRtnRouteStr = LSData.pLSAllAutoRtnRouteStr + j;
						   
						   /*������ݽṹ��*/
						   LS_AllAutoRtnRouteStruFromChar(pDataAddr,pLsAllAutoRtnRouteStr);
						   pDataAddr += ALLAUTO_RTN_ROUTE_DATA_LEN;
						   
					   }
					   
					   rtnValue = 1;
					}
				}
			}
		}
	}

	return rtnValue;
}

/*********************************************
*�������ܣ���䱣�������������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_OverlapLockCondStru(CHAR *LSDataName)
{
	UINT8 funcRtn = 0;
	LS_OveralpLockCondStru* pTempLSProtSecLockCondStru = NULL;   /*��ʱ����·����*/
	UINT8* pDataAddr = NULL;         /*���ݵ�ַ*/
	UINT32 DataSize = 0;          /*���ݴ�С���ֽ�����*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;
	
	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSOverlapLockCondStru != NULL)
	{
		rtnValue = 1;
	}
	else
	{
    	/*��ȡ��������������������*/
	   funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, OVERLAP_LOCK_COND_DATA_ID, &pDataAddr, &DataSize);

	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
	   }
	   else
	   {
		   /*��ø���*/
		   if((DataSize % OVERLAP_LOCK_COND_DATA_LEN) != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {

				/*��ȡ��������������������Ŀ����*/
				LSDataLen.LSOverlapLockCondStruLen = (UINT16)(DataSize/OVERLAP_LOCK_COND_DATA_LEN);

				if(DataSize < OVERLAP_LOCK_COND_DATA_LEN)
				{
					LSData.pLSOverlapLockCondStru = (LS_OveralpLockCondStru *) MyNew (sizeof(LS_OveralpLockCondStru));
					rtnValue = 1;
				}
				else
				{
					/*��̬����ռ�*/
					LSData.pLSOverlapLockCondStru = (LS_OveralpLockCondStru *) MyNew (sizeof(LS_OveralpLockCondStru) * LSDataLen.LSOverlapLockCondStruLen);
					/*��̬����ռ�ʧ��*/
					if(NULL == LSData.pLSOverlapLockCondStru)
					{
						/*�����鳤����0�������������Խ��*/
						LSDataLen.LSOverlapLockCondStruLen = 0;

						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(LSData.pLSOverlapLockCondStru, 0x00, sizeof(LS_OveralpLockCondStru) * LSDataLen.LSOverlapLockCondStruLen);

						/*�������ݵ� Ĭ���г����� �ṹ������ */  
						for(j=0; j < LSDataLen.LSOverlapLockCondStruLen; j++)
						{
							pTempLSProtSecLockCondStru = LSData.pLSOverlapLockCondStru + j;

							/*������ݽṹ��*/
							LS_OverlapLockCondStruFromChar(pDataAddr, pTempLSProtSecLockCondStru);

							pDataAddr += OVERLAP_LOCK_COND_DATA_LEN;
						}

						rtnValue = 1;
					}
				}
			}
		}
	}

	return rtnValue;
}

/*********************************************
*�������ܣ�������·����������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_MonoRailSwitStru(CHAR* LSDataName)
{
	UINT8 funcRtn = 0;
	LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru = NULL;   /*��ʱ����*/
	UINT8* pDataAddr = NULL;         /*���ݵ�ַ*/
	UINT32 DataSize = 0;          /*���ݴ�С���ֽ�����*/
	UINT16 struInx = 0;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(LSData.pLSRouteMonoRailSwitStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}

	/*��ȡ Ĭ���г�����  ����*/
	funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, ROUTE_MONO_RAIL_SWIT_ID, &pDataAddr, &DataSize);

	if(0 == funcRtn)
	{
	   /*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(0 != (DataSize % ROUTE_MONO_RAIL_SWIT_LEN))
	{
		rtnValue = 0;
		return rtnValue;
	}			

	LSDataLen.LSRouteMonoRailSwitStruLen = (UINT16)(DataSize / ROUTE_MONO_RAIL_SWIT_LEN);

	if(ROUTE_MONO_RAIL_SWIT_LEN > DataSize)
	{
		LSData.pLSRouteMonoRailSwitStru = (LS_RouteMonoRailSwitStru *)MyNew(sizeof(LS_RouteMonoRailSwitStru));
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
	   /*����ִ��*/
	}

	/*��̬����ռ�*/
	LSData.pLSRouteMonoRailSwitStru = (LS_RouteMonoRailSwitStru *)MyNew(sizeof(LS_RouteMonoRailSwitStru) * LSDataLen.LSRouteMonoRailSwitStruLen);
	/*��̬����ռ�ʧ��*/
	if(NULL == LSData.pLSRouteMonoRailSwitStru)
	{
        /*�����鳤����0�������������Խ��*/
        LSDataLen.LSRouteMonoRailSwitStruLen = 0;
        
        rtnValue = 0;

        return rtnValue;
	}
	else
	{
		/*�ڴ�����Ϊ��Ч*/
		memset(LSData.pLSRouteMonoRailSwitStru, 0x00, sizeof(LS_RouteMonoRailSwitStru) * LSDataLen.LSRouteMonoRailSwitStruLen);
	}
	
	/*�������ݵ� Ĭ���г����� �ṹ������ */  
	for(struInx = 0; struInx < LSDataLen.LSRouteMonoRailSwitStruLen; struInx++)
	{
		pRouteMonoRailSwitStru = LSData.pLSRouteMonoRailSwitStru + struInx;

		/*������ݽṹ��*/
		LS_RouteMonoRailSwitStruFromChar(pDataAddr, pRouteMonoRailSwitStru);

		pDataAddr += ROUTE_MONO_RAIL_SWIT_LEN;
	}

	rtnValue = 1;

	return rtnValue;
}


