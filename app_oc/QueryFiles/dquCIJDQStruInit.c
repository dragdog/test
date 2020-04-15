/************************************************************************
*
* �ļ���   ��  dquCIJDQStruInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  �̵������ݽṹ���ʼ��������� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIJDQStruInit.h"



/*********����ȫ�ֵļ̵�����Ϣ���ݱ���*******/
RelayDataStruct_Config RelayData;
RelayDataStruct_ConfigLen RelayDataLen;

/***************************/


/*********************************************
*�������ܣ�����źŻ��̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_SignalStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_SignalStruct* pSgnlStr;   /*��ʱ�źŻ��̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelaySignalStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, SIGNAL_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_SIGNAL_MODULE_CONFIG != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelaySignalStrLen = (UINT16)(DataSize/LEN_SIGNAL_MODULE_CONFIG);

		if(DataSize<LEN_SIGNAL_MODULE_CONFIG)
	   {
		   RelayData.pRelaySignalStr = (Relay_SignalStruct *) MyNew (sizeof(Relay_SignalStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelaySignalStr = (Relay_SignalStruct *) MyNew (sizeof(Relay_SignalStruct) * RelayDataLen.RelaySignalStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelaySignalStr)
		{
            RelayDataLen.RelaySignalStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelaySignalStr, 0x00,sizeof(Relay_SignalStruct) * RelayDataLen.RelaySignalStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelaySignalStrLen;j++)
		{
			pSgnlStr = RelayData.pRelaySignalStr + j;
			
			/*������ݽṹ��*/
			 R_SignalStructFromChar(pDataAddr,pSgnlStr);
			pDataAddr += LEN_SIGNAL_MODULE_CONFIG;

		}

	rtnValue = 1;
	return rtnValue;
}
/*********************************************
*�������ܣ����ϴ�����̵����ṹ������---�ڲ���
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_WashMachineStru(CHAR* CIDataName)
{
	UINT8 funcRtn = 0;
	Relay_WashMachineStruct* pWashMachineStr = NULL;   /*��ʱϴ�����̵�������*/
	UINT8* pDataAddr;                                  /*���ݵ�ַ*/
	UINT32 DataSize;                                   /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayWashMachineStr != NULL)
	{
		rtnValue = 1;
	}
    else
    {
	   /*��ȡϴ�����̵������ݱ�*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_WASH_MACHINE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
	   }
	   else
	   {
			
		   if(DataSize%LEN_RELAY_WASH_MACHINE != 0)
		   { /*��ø�����Ϊ���������ݴ���*/  
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayWashMachineStrLen = (UINT16)(DataSize / LEN_RELAY_WASH_MACHINE);/*����ṹ�����*/

				if(DataSize < LEN_RELAY_WASH_MACHINE)
			   {/*����һ��*/
				   RelayData.pRelayWashMachineStr = (Relay_WashMachineStruct *) MyNew (sizeof(Relay_WashMachineStruct) );
				   rtnValue = 1;
			   }
			   else
			   {

					/*��̬����ռ�*/
					RelayData.pRelayWashMachineStr = (Relay_WashMachineStruct *) MyNew (sizeof(Relay_WashMachineStruct) * RelayDataLen.RelayWashMachineStrLen);
					
					if(NULL == RelayData.pRelayWashMachineStr)
					{/*��̬����ռ�ʧ��*/
			            RelayDataLen.RelayWashMachineStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(RelayData.pRelayWashMachineStr, 0x00,sizeof(Relay_WashMachineStruct) * RelayDataLen.RelayWashMachineStrLen);
						/*�������ݵ� Ĭ���г����� �ṹ������ */  
						for(j = 0;j < RelayDataLen.RelayWashMachineStrLen;j++)
						{
							pWashMachineStr = RelayData.pRelayWashMachineStr + j;
							
							/*������ݽṹ��*/
							 R_WashMachineStructFromChar(pDataAddr,pWashMachineStr);
							pDataAddr += LEN_RELAY_WASH_MACHINE;

						}
						rtnValue = 1; 
					}/*end:if(NULL == RelayData.pRelayWashMachineStr)*/
			   }/*end:if(DataSize < LEN_RELAY_WASH_MACHINE)*/
		   	}/*end:if(DataSize%LEN_RELAY_WASH_MACHINE != 0)*/
	   	}/*end:	if(funcRtn == 0)*/
    }/*end:if(RelayData.pRelayWashMachineStr != NULL)*/
	return rtnValue;

}

/*********************************************
*�������ܣ�������ͣ����ť�̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_EmgcyStpStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_EmgcyStpStruct* pEmgcyStpStr;   /*��ʱ�źŻ��̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayEmgcyStpStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, EMP_RELAY_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_EMP_RELAY != 0)
	   {   
		    rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayEmgcyStpStrLen = (UINT16)(DataSize/LEN_EMP_RELAY);

		if(DataSize<LEN_EMP_RELAY)
	   {
		   RelayData.pRelayEmgcyStpStr = (Relay_EmgcyStpStruct *) MyNew (sizeof(Relay_EmgcyStpStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayEmgcyStpStr = (Relay_EmgcyStpStruct *) MyNew ((sizeof(Relay_EmgcyStpStruct)) * (RelayDataLen.RelayEmgcyStpStrLen));
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayEmgcyStpStr)
		{
            RelayDataLen.RelayEmgcyStpStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayEmgcyStpStr, 0x00,sizeof(Relay_EmgcyStpStruct) * RelayDataLen.RelayEmgcyStpStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayEmgcyStpStrLen;j++)
		{
			pEmgcyStpStr = RelayData.pRelayEmgcyStpStr + j;
			
			/*������ݽṹ��*/
			R_EmegencyStopStructFromChar(pDataAddr,pEmgcyStpStr);
			pDataAddr += LEN_EMP_RELAY;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���������ż̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_ScreenStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_ScreenStruct* pScreenStr;   /*��ʱ�źŻ��̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayScreenStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, PSD_RELAY_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_PSD_RELAY != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayScreenStrLen = (UINT16)(DataSize/LEN_PSD_RELAY);

		if(DataSize<LEN_PSD_RELAY)
	   {
		   RelayData.pRelayScreenStr = (Relay_ScreenStruct *) MyNew (sizeof(Relay_ScreenStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayScreenStr = (Relay_ScreenStruct *) MyNew (sizeof(Relay_ScreenStruct) * RelayDataLen.RelayScreenStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayScreenStr)
		{
            RelayDataLen.RelayScreenStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayScreenStr, 0x00,sizeof(Relay_ScreenStruct) * RelayDataLen.RelayScreenStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayScreenStrLen;j++)
		{
			pScreenStr = RelayData.pRelayScreenStr + j;
			
			/*������ݽṹ��*/
			 R_ScreenStructFromChar(pDataAddr,pScreenStr);
			pDataAddr += LEN_PSD_RELAY;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���䵥�����̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitMonoRailRelay_PointStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PointStruct* pPointRelayStr;   /*��ʱ����̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayPointStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_MONO_RAIL_POINT,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(DataSize%LEN_RELAY_MONO_RAIL_POINT != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayMonoRailPointStrLen = (UINT16)(DataSize/LEN_RELAY_MONO_RAIL_POINT);

	if(DataSize<LEN_RELAY_MONO_RAIL_POINT)
	{
		RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*����ִ��*/
	}

	/*��̬����ռ�*/
	RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) * (RelayDataLen.RelayMonoRailPointStrLen));
	/*��̬����ռ�ʧ��*/
	if(NULL == RelayData.pRelayPointStr)
	{
		RelayDataLen.RelayMonoRailPointStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*�ڴ�����Ϊ��Ч*/
		memset(RelayData.pRelayPointStr, 0x00,sizeof(Relay_PointStruct) * RelayDataLen.RelayMonoRailPointStrLen);
	}

	/*�������ݵ� Ĭ���г����� �ṹ������ */  
	for(j=0;j<RelayDataLen.RelayMonoRailPointStrLen;j++)
	{
		pPointRelayStr = RelayData.pRelayPointStr + j;

		/*������ݽṹ��*/
		R_PointStructFromChar(pDataAddr,pPointRelayStr);
		pDataAddr += LEN_RELAY_MONO_RAIL_POINT;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�������̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_PointStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PointStruct* pPointRelayStr;   /*��ʱ����̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayPointStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,SWITCH_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(DataSize%LEN_SWITCH_MODULE_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayPointStrLen = (UINT16)(DataSize/LEN_SWITCH_MODULE_CONFIG);

	if(DataSize<LEN_SWITCH_MODULE_CONFIG)
	{
		RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*����ִ��*/
	}

	/*��̬����ռ�*/
	RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) * (RelayDataLen.RelayPointStrLen));
	/*��̬����ռ�ʧ��*/
	if(NULL == RelayData.pRelayPointStr)
	{
        RelayDataLen.RelayPointStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*�ڴ�����Ϊ��Ч*/
		memset(RelayData.pRelayPointStr, 0x00,sizeof(Relay_PointStruct) * RelayDataLen.RelayPointStrLen);
	}
		
	/*�������ݵ� Ĭ���г����� �ṹ������ */  
	for(j=0;j<RelayDataLen.RelayPointStrLen;j++)
	{
		pPointRelayStr = RelayData.pRelayPointStr + j;
			
		/*������ݽṹ��*/
		R_PointStructFromChar(pDataAddr,pPointRelayStr);
		pDataAddr += LEN_SWITCH_MODULE_CONFIG;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�������̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_MultSwitchStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_MultSwitchStruct* pMultSwitchRelayStr;   /*��ʱ����̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayMultSwtichStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,MULTSWITCH_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(DataSize%LEN_MULTSWITCH_MODULE_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayMultSwtichStrLen = (UINT16)(DataSize/LEN_MULTSWITCH_MODULE_CONFIG);

	if(DataSize<LEN_MULTSWITCH_MODULE_CONFIG)
	{
		RelayData.pRelayMultSwtichStr = (Relay_MultSwitchStruct *) MyNew (sizeof(Relay_MultSwitchStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*����ִ��*/
	}

	/*��̬����ռ�*/
	RelayData.pRelayMultSwtichStr = (Relay_MultSwitchStruct *) MyNew (sizeof(Relay_MultSwitchStruct) * (RelayDataLen.RelayMultSwtichStrLen));
	/*��̬����ռ�ʧ��*/
	if(NULL == RelayData.pRelayMultSwtichStr)
	{
		RelayDataLen.RelayMultSwtichStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*�ڴ�����Ϊ��Ч*/
		memset(RelayData.pRelayMultSwtichStr, 0x00,sizeof(Relay_MultSwitchStruct) * RelayDataLen.RelayMultSwtichStrLen);
	}

	/*�������ݵ� Ĭ���г����� �ṹ������ */  
	for(j=0;j<RelayDataLen.RelayMultSwtichStrLen;j++)
	{
		pMultSwitchRelayStr = RelayData.pRelayMultSwtichStr + j;

		/*������ݽṹ��*/
		R_MultSwtichStructFromChar(pDataAddr,pMultSwitchRelayStr);
		pDataAddr += LEN_MULTSWITCH_MODULE_CONFIG;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ�����������μ̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_PhysicalSgmtStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PhysicalSegmentStruct* pPhysicalSegmentStr;   /*��ʱ�������μ̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayPhysicalSgmtStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,PHYSICAL_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_PHYSICAL_MODULE_CONFIG != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayPhysicalSgmtStrLen = (UINT16)(DataSize/LEN_PHYSICAL_MODULE_CONFIG);

		if(DataSize<LEN_PHYSICAL_MODULE_CONFIG)
	   {
		   RelayData.pRelayPhysicalSgmtStr = (Relay_PhysicalSegmentStruct *) MyNew (sizeof(Relay_PhysicalSegmentStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayPhysicalSgmtStr = (Relay_PhysicalSegmentStruct *) MyNew (sizeof(Relay_PhysicalSegmentStruct) * RelayDataLen.RelayPhysicalSgmtStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayPhysicalSgmtStr)
		{
            RelayDataLen.RelayPhysicalSgmtStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayPhysicalSgmtStr, 0x00,sizeof(Relay_PhysicalSegmentStruct) * RelayDataLen.RelayPhysicalSgmtStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayPhysicalSgmtStrLen;j++)
		{
			pPhysicalSegmentStr = RelayData.pRelayPhysicalSgmtStr + j;
			
			/*������ݽṹ��*/
			 R_PhysicalSegmentStructFromChar(pDataAddr,pPhysicalSegmentStr);
			pDataAddr += LEN_PHYSICAL_MODULE_CONFIG;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ����ǽ�·�����̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_NoRouteDcStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_NoRouteDcStruct* pNoRouteStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayNoRouteDcStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_NO_ROUTE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_RELAY_NO_ROUTE != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayNoRouteDcStrLen = (UINT16)(DataSize/LEN_RELAY_NO_ROUTE);

		if(DataSize<LEN_RELAY_NO_ROUTE)
	   {
		   RelayData.pRelayNoRouteDcStr = (Relay_NoRouteDcStruct *) MyNew (sizeof(Relay_NoRouteDcStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayNoRouteDcStr = (Relay_NoRouteDcStruct *) MyNew (sizeof(Relay_NoRouteDcStruct) * RelayDataLen.RelayNoRouteDcStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayNoRouteDcStr)
		{
            RelayDataLen.RelayNoRouteDcStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayNoRouteDcStr, 0x00,sizeof(Relay_NoRouteDcStruct) * RelayDataLen.RelayNoRouteDcStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayNoRouteDcStrLen;j++)
		{
			pNoRouteStr = RelayData.pRelayNoRouteDcStr + j;
			
			/*������ݽṹ��*/
			 R_NoRouteDcStruFromChar(pDataAddr,pNoRouteStr);
			pDataAddr += LEN_RELAY_NO_ROUTE;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���䱨���ƽṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_BaoJingDengStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_BaoJingDengStruct* pBjdStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayBjdStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_BJD_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_RELAY_BJD != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayBjdStrLen = (UINT16)(DataSize/LEN_RELAY_BJD);

		if(DataSize<LEN_RELAY_BJD)
	   {
		   RelayData.pRelayBjdStr = (Relay_BaoJingDengStruct *) MyNew (sizeof(Relay_BaoJingDengStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayBjdStr = (Relay_BaoJingDengStruct *) MyNew (sizeof(Relay_BaoJingDengStruct) * RelayDataLen.RelayBjdStrLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayBjdStr)
		{
            RelayDataLen.RelayBjdStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayBjdStr, 0x00,sizeof(Relay_BaoJingDengStruct) * RelayDataLen.RelayBjdStrLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayBjdStrLen;j++)
		{
			pBjdStr = RelayData.pRelayBjdStr + j;
			
			/*������ݽṹ��*/
			 R_BaoJingDengStruFromChar(pDataAddr,pBjdStr);
			pDataAddr += LEN_RELAY_BJD;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ���������۷��ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_NoReturnStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_NoReturnStruct* pNoRtnStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayNoRtnStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_NO_RETURN_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*��ø���*/
	   if(DataSize%LEN_RELAY_NO_RETURN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayNoRtnStruLen = (UINT16)(DataSize/LEN_RELAY_NO_RETURN);

		if(DataSize<LEN_RELAY_NO_RETURN)
	   {
		   RelayData.pRelayNoRtnStru = (Relay_NoReturnStruct *) MyNew (sizeof(Relay_NoReturnStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*����ִ��*/
	   }


		/*��̬����ռ�*/
		RelayData.pRelayNoRtnStru = (Relay_NoReturnStruct *) MyNew (sizeof(Relay_NoReturnStruct) * RelayDataLen.RelayNoRtnStruLen);
		/*��̬����ռ�ʧ��*/
		if(NULL == RelayData.pRelayNoRtnStru)
		{
            RelayDataLen.RelayNoRtnStruLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*�ڴ�����Ϊ��Ч*/
			memset(RelayData.pRelayNoRtnStru, 0x00,sizeof(Relay_NoReturnStruct) * RelayDataLen.RelayNoRtnStruLen);
		}
		
		/*�������ݵ� Ĭ���г����� �ṹ������ */  
		for(j=0;j<RelayDataLen.RelayNoRtnStruLen;j++)
		{
			pNoRtnStr = RelayData.pRelayNoRtnStru + j;
			
			/*������ݽṹ��*/
			 R_NoReturnStruFromChar(pDataAddr,pNoRtnStr);
			pDataAddr += LEN_RELAY_NO_RETURN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*�������ܣ���䳵���ż̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_GaragedoorStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_GaragedoorStruct* pGaragedoorStr;   /*��ʱ�źŻ��̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayGaragedoorStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
	   /*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_GARAGE_DOOR_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
	   } 
	   else
	   {
			/*��ø���*/
		   if(DataSize%LEN_GARAGE_DOOR != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayGaragedoorStrLen = (UINT16)(DataSize/LEN_GARAGE_DOOR);

				if(DataSize < LEN_GARAGE_DOOR)
			   {
				   RelayData.pRelayGaragedoorStr = (Relay_GaragedoorStruct *) MyNew (sizeof(Relay_GaragedoorStruct) );
				   rtnValue = 1;
			   }
			   else
			   {				
				   /*��̬����ռ�*/
					RelayData.pRelayGaragedoorStr = (Relay_GaragedoorStruct *) MyNew ((sizeof(Relay_GaragedoorStruct)) * (RelayDataLen.RelayGaragedoorStrLen));
					/*��̬����ռ�ʧ��*/
					if(NULL == RelayData.pRelayGaragedoorStr)
					{
						RelayDataLen.RelayGaragedoorStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(RelayData.pRelayGaragedoorStr, 0x00,sizeof(Relay_GaragedoorStruct) * RelayDataLen.RelayGaragedoorStrLen);
				    	/*�������ݵ� Ĭ���г����� �ṹ������ */  
						for(j=0;j<RelayDataLen.RelayGaragedoorStrLen;j++)
						{
							pGaragedoorStr = RelayData.pRelayGaragedoorStr + j;
							
							/*������ݽṹ��*/
							 R_GaragedoorStructFromChar(pDataAddr,pGaragedoorStr);
							pDataAddr += LEN_GARAGE_DOOR;

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
*�������ܣ���ʼ��SPKS�̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelaySpksStru(CHAR* CIDataName)
{
	UINT8 tmpRtn = 0;
	RelaySpksStruct* pRelaySpksStr;   /*��ʱ����*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 dataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 1;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelaySpksStr != NULL)
	{
		rtnValue = 1;		
	}
	else
	{
		/*��ȡ Ĭ���г�����  ����*/
		tmpRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_SPKS_ID,&pDataAddr,&dataSize);
		if(tmpRtn == 0)
		{
			/*��������ʧ��*/
			rtnValue = 0;
		} 
		else 
		{
			/*��ø���*/
			if(dataSize%LEN_RELAY_SPKS != 0)
			{   
				rtnValue = 0;
			}
		}
						
		if (rtnValue == 1)
		{
			RelayDataLen.RelaySpksStrLen = (UINT16)(dataSize/LEN_RELAY_SPKS);
			
			if(RelayDataLen.RelaySpksStrLen > 0)
			{
				/* ��̬����ռ� */
				RelayData.pRelaySpksStr = (RelaySpksStruct *) MyNew (sizeof(RelaySpksStruct) * RelayDataLen.RelaySpksStrLen);
				/* ��̬����ռ�ʧ�� */
				if(NULL == RelayData.pRelaySpksStr)
				{
					RelayDataLen.RelaySpksStrLen = 0;
					rtnValue = 0;
				}
				else
				{
					/* ���� */
					memset(RelayData.pRelaySpksStr, 0x00,sizeof(RelaySpksStruct) * RelayDataLen.RelaySpksStrLen);
					/*�������ݵ� Ĭ���г����� �ṹ������ */  
					for(j=0;j<RelayDataLen.RelaySpksStrLen;j++)
					{
						pRelaySpksStr = RelayData.pRelaySpksStr + j;

						/*������ݽṹ��*/
						R_SpksDataFromChar(pDataAddr,pRelaySpksStr);
						pDataAddr += LEN_RELAY_SPKS;

					}
					rtnValue = 1;
				}				
			}
			else
			{
				/* ����Ϊ�� */
				RelayData.pRelaySpksStr = NULL;
				rtnValue = 1;
			}
		}

	}
	
	return rtnValue;

}

/*********************************************
*�������ܣ��������ż̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_FloodGateStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_FloodGateStruct* pFloodGateStr;   /*��ʱ�����ż̵�������*/
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayFloodGateStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
		/*��ȡ Ĭ���г�����  ����*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_FLOOD_GATE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*��������ʧ��*/
		   rtnValue = 0;
	   } 
	   else
	   {
			/*��ø���*/
		   if(DataSize%LEN_FLOOD_GATE != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayFloodGateStrLen = (UINT16)(DataSize/LEN_FLOOD_GATE);

				if(DataSize<LEN_FLOOD_GATE)
			   {
				   RelayData.pRelayFloodGateStr = (Relay_FloodGateStruct *) MyNew (sizeof(Relay_FloodGateStruct) );
				   rtnValue = 1;
			   }
			   else
			   {
					   /*��̬����ռ�*/
					RelayData.pRelayFloodGateStr = (Relay_FloodGateStruct *) MyNew ((sizeof(Relay_FloodGateStruct)) * (RelayDataLen.RelayFloodGateStrLen));
					/*��̬����ռ�ʧ��*/
					if(NULL == RelayData.pRelayFloodGateStr)
					{
						RelayDataLen.RelayFloodGateStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(RelayData.pRelayFloodGateStr, 0x00,sizeof(Relay_FloodGateStruct) * RelayDataLen.RelayFloodGateStrLen);
						/*�������ݵ� Ĭ���г����� �ṹ������ */  
						for(j=0;j<RelayDataLen.RelayFloodGateStrLen;j++)
						{
							pFloodGateStr = RelayData.pRelayFloodGateStr + j;
							
							/*������ݽṹ��*/
							 R_FloodGateStructFromChar(pDataAddr,pFloodGateStr);
							pDataAddr += LEN_FLOOD_GATE;

						}
						rtnValue = 1;
					}					
			   }
		   }
	   }

	}

	return rtnValue;
}