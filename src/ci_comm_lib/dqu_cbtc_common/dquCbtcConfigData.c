/************************************************************************
*
* �ļ���   ��  
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  ϵͳ�������ݽṹ�嶨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dquCbtcConfigData.h"
#include "dfsDataRead.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"

static CBTC_CFG_DATA_STRU *qds_dsuCbtcConfigDataStru=NULL;/*CBTC������Ϣ�ò�ѯ����Դ*/
static CBTC_CFG_DATA_STRU *g_pCbtcConfigDataStru=NULL; /*ָ���ʼ�������ݿ��ָ��*/

static dfsDataHead g_DFsDH;			/*FS�ļ�������ͷ�ṹ��*/
static UINT8*  g_pDataAddr;         /*FS��Ԫ���ݵ�ַ*/
static UINT32  g_DataSize;          /*FS��Ԫ���ݴ�С���ֽ�����*/
static UINT16  g_RowCount;			/*����������*/

static UINT16	g_wCurInitStruProg=0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
static UINT16   g_wCurMainIndex=0;  /*��ǰ�������������ֲ�����ʱʹ��*/

/*----------�ڲ�����������ʼ--------------*/

/*�����г�������������1*/
UINT8 dfsInitTrainModelConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*�г����ܲ������ýṹ��2*/
UINT8 dfsInitTrainPerformConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*����ϵͳ�����������ýṹ��3-CPK*/
UINT8 dfsInitSubSystemCommonConfigData_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*����ϵͳ�����������ýṹ��3-FAO*/
UINT8 dfsInitSubSystemCommonConfigData_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*����ϵͳ�����������ýṹ��3-HLT�����ݻ�����ͨ���ݽṹV12.0.0�޸ģ�modify by lmy 20180314*/
UINT8 dfsInitSubSystemCommonConfigData_HLT(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*����ϵͳ�����������ýṹ��3-DG�����ݻ�����ͨ���ݽṹV12.0.0�޸ĺ�DG��HLT����ʹ����ͬ�ĳ�ʼ�������������Ӵ˺�����add by lmy 20180312*/
UINT8 dfsInitSubSystemCommonConfigData_DG(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*����ϵͳ�����������ýṹ��3-CC*/
UINT8 dfsInitSubSystemCommonConfigData_CC(UINT8 *pDataAddr,UINT32 dataSize, UINT16 rowCount);
/*�������������ݽṹ��4-FAO ��������ͨ��˰汾һ��*/
UINT8 dfsInitTrainDepotConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);
/*���ݰ汾�������ݴ�����̺���*/
static UINT8 initConfigCbtcInfoTaskTab(void);



/*----------�ڲ�������������--------------*/

/*��ʼ������ǼǱ�*/
static INIT_TASK_TAB_STRU g_struInitTaskTab[]=
{
	{DQU_QFUN_TRUE, CBTC_TRAIN_TYPE_CONFIG_ID,	dfsInitTrainModelConfigData,		 5,1,0}, 	
	{DQU_QFUN_TRUE, CBTC_TRAIN_INFO_ID,			dfsInitTrainPerformConfigData,		 5,1,0},	
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_CPK,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_FAO,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_HLT,5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_DG, 5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DATA_ID,		dfsInitSubSystemCommonConfigData_CC, 5,1,0},
	{DQU_QFUN_TRUE, CBTC_CONFIG_DEPOT_ID,		dfsInitTrainDepotConfigData,		 5,1,0},	
};
/*��ʼ������ǼǱ���Ŀ����*/
static const UINT16 MAX_INITTASK_COUNT=sizeof(g_struInitTaskTab)/sizeof(INIT_TASK_TAB_STRU);

/*********************************************
*�������ܣ�ϵͳ�������ݰ汾�Ż�ȡ
*����˵����������ϵͳ���ò�ѯ�����汾�ţ���ֹ���ɴ���
*��ڲ�������
*���ڲ�������
*����ֵ��UINT32��ʾ�İ汾��
*********************************************/
UINT32 GetDquCbtcVersion(void)
{
	UINT32 dquCbtcVer=0;
	dquCbtcVer = VerToU32SoftwareModule(DQU_CBTC_VER1,DQU_CBTC_VER2,DQU_CBTC_VER3,DQU_CBTC_VER4);
	return dquCbtcVer;
}

/*����CBTC����������Ϣ*/
UINT8 dquSetInitCbtcConfigInfoRef(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru)
{
	UINT8 bRetVal=0;
	if (NULL!=pCbtcConfigDataStru)
	{
		g_pCbtcConfigDataStru=pCbtcConfigDataStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*���ݰ汾�������ݴ�����̺���*/
/*��ע�����ݻ�����ͨ���ݽṹV12.0.0�޸ģ�modify by lmy 20180312*/
static UINT8 initConfigCbtcInfoTaskTab(void)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType = GetCbtcSysType();
	if (DQU_CBTC_CPK == cbtcVersionType)/*�Ͽ⹦������*/
	{
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*�Ͽ�Ĭ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*�Ͽ�Ĭ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*�Ͽ�Ĭ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*�Ͽ�Ĭ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CC*/
		g_struInitTaskTab[7].bIsEffective=DQU_QFUN_FASLE;/*�Ͽ�Ĭ�ϲ�ִ�У�����������*/
		rtnValue = 1;
	}	
	else if (DQU_CBTC_FAO == cbtcVersionType) /*FAO��������*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CPK*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_HLT == cbtcVersionType) /*������ͨ��������*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*������ͨĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*������ͨĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-FAO*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*������ͨĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-DG*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_DG == cbtcVersionType)/*���칦������*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-HLT*/
		g_struInitTaskTab[6].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CC*/
		rtnValue = 1;
	}
	else if(DQU_CBTC_CC == cbtcVersionType)/*������������*/
	{
		g_struInitTaskTab[2].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-CPK*/
		g_struInitTaskTab[3].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-FAO*/
		g_struInitTaskTab[4].bIsEffective=DQU_QFUN_FASLE;/*DGĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-HLT*/
		g_struInitTaskTab[5].bIsEffective=DQU_QFUN_FASLE;/*FAOĬ�ϲ�ִ�У�����ϵͳ�����������ýṹ��3-DG*/
		rtnValue = 1;
	}
	else /*δ���ֵ����ð汾ֵ������δ���ñ�־λ*/
	{
		rtnValue = 0;
	}

	return rtnValue;
}

/*ϵͳ�������ݳ�ʼ��*/
UINT8 dquCbtcConfigDataInit(CHAR* FSName)
{
	UINT16	wIndex=0;
	UINT8   funcRtn = 0;
	UINT8	rtnValue = 0;
	UINT32	DataID=0;/*�������ͱ�ʶ*/
	UINT8	*pDataAddr=NULL;
	UINT32	DataSize=0;
	UINT16	rowCount = 0;

	if(NULL!=g_pCbtcConfigDataStru)
	{

		/*��ȡFS�ļ�,����֤�ļ�*/
		if(1 != dataRead(FSName))
		{
			/*���ݶ�ȡ���ɹ�*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*���ݰ汾�������ݴ�����̺���*/
			funcRtn =initConfigCbtcInfoTaskTab();
			if (0 == funcRtn)
			{
				rtnValue = 0;
				return rtnValue;
			}
		}

		/*������ʼ������*/
		wIndex=0;
		do /*�������ݵ���Ӧ�Ľṹ��*/
		{
			if (DQU_QFUN_TRUE == g_struInitTaskTab[wIndex].bIsEffective)/*�ж������ʼ�������Ƿ�ִ��*/
			{
				/*��ȡ��������*/
				DataID=g_struInitTaskTab[wIndex].DataTypeId;
				funcRtn=dquGetConfigDataNew( FSName, CBTC_CONFIGDATA_TYPE, DataID, &pDataAddr, &DataSize, &rowCount);
				if (funcRtn != 1)
				{
					/*��ȡ����ʧ��*/
					rtnValue = 0;
					return rtnValue;
				}
				funcRtn=g_struInitTaskTab[wIndex].pExeFun(pDataAddr,DataSize,rowCount);
				if (0 == funcRtn)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else/*����ִ��*/
				{
				}
			}

			wIndex++;
		} while (wIndex<MAX_INITTASK_COUNT);

		/*Ĭ�ϵ��ô˽ӿ��Զ�������ѯ����Դ*/
		dquSetCurCbtcCfgInfoQueryDataSource(g_pCbtcConfigDataStru);

		rtnValue = 1;
	}
	else
	{
		rtnValue = 0;
	}

	return rtnValue;

}

/******************************************
*�������ܣ�ϵͳ�������ݳ�ʼ����չ�ӿ�
*����˵���� pDataBuf,�������ݿ飬
*			cbtcConfigDataStru,�ⲿʵ��ϵͳ�������ݴ洢�ṹ��ָ��
*			mode,����ģʽ��1�ϵ�һ�μ��أ�2����ʱ��δ���
*			timeSlice��ִ�е�ʱ��Ƭ����λ�����룩
*����ֵ��  0ʧ�ܣ�1�ɹ���2�ֲ�����δ���
*****************************************/
UINT8 dquCbtcConfigDataInitExp(UINT8 *pDataBuf,CBTC_CFG_DATA_STRU *cbtcConfigDataStru,UINT8 mode,UINT16 timeSlice)
{
	UINT32 DataID;
	UINT16 wPos=0;
	UINT8 rtnValue=0; 
	UINT16 wFreqTimes=0;
	UINT8 bExeState=DQU_QFUN_FASLE;
	UINT32 dwOnceTime,dwUsedTime=0;
	UINT32 dwCurTimePoint,dwBeginTimePoint=0;

	UINT8 funcRtn;


	if(0==g_wCurMainIndex)/*��������������ִ��̬��ʱ�򣬾Ͳ��ܸı䴦��ģʽ*/
	{
		g_pCbtcConfigDataStru=cbtcConfigDataStru;
		if (NULL==g_pCbtcConfigDataStru)
		{
			/*ȫ�ֽṹ��ָ��ռ�δ����*/
			rtnValue = 0;
			return rtnValue;
		}
	}

	do /*�ϵ���س������б��ֿ���*/
	{
		/*��ȡ���뿪ʼʱ��*/
		dwBeginTimePoint=dquGetCurrentTimeByMS();
		switch(g_wCurMainIndex)/*�������̿���*/
		{
		case 0:
			/*����FS�ļ�����ͷ������������Ϣ*/
			funcRtn=dquAnalyzeFsDataHead(pDataBuf, &g_DFsDH);
			if (funcRtn != 1)
			{
				/*��ȡ����ʧ��*/
				rtnValue = 0;
			}
			else
			{
				/*���ݰ汾�������ݴ�����̺���*/
				funcRtn =initConfigCbtcInfoTaskTab();
				if (0 == funcRtn)
				{
					rtnValue = 0;
				}
				else
				{
					g_wCurMainIndex=1;
					rtnValue = 2;
				}
			}
			break;
		case 1:		
			/*��ȡ��������*/
			DataID=g_struInitTaskTab[g_wCurInitStruProg].DataTypeId;
			funcRtn=dquGetConfigDataExpNew(pDataBuf, &g_DFsDH, CBTC_CONFIGDATA_TYPE, DataID, &g_pDataAddr, &g_DataSize, &g_RowCount);
			if (funcRtn != 1)
			{
				/*��ȡ����ʧ��*/
				rtnValue = 0;
			}
			else/*��ȡ���ݳɹ�*/
			{
				g_wCurMainIndex=2;
				rtnValue = 2;
			}
			break;
		case 2:
			funcRtn=g_struInitTaskTab[g_wCurInitStruProg].pExeFun(g_pDataAddr,g_DataSize,g_RowCount);
			if(1==funcRtn)/*��ɱ��������Ĵ���*/
			{
				do 
				{
					g_wCurInitStruProg++;/*��ʼ���ṹ�������һ���׶�*/
					/*Խ����Ч������*/
				}while((DQU_QFUN_FASLE==g_struInitTaskTab[g_wCurInitStruProg].bIsEffective) && (g_wCurInitStruProg<MAX_INITTASK_COUNT));
				/*���еĳ�ʼ������δ���*/
				if(g_wCurInitStruProg<MAX_INITTASK_COUNT)
				{	
					g_wCurMainIndex=1;
				}
				else /*if (g_wCurInitStruProg >=MAX_INITTASK_COUNT) �������״̬�£���������нṹ���ʼ������*/
				{
					g_wCurMainIndex=3;
				}
				rtnValue = 2;
			}
			else/*��������쳣*/
			{
				rtnValue = 0;
			}
							
			break;
		case 3:
			rtnValue = 1;
			break;
		default:
			rtnValue = 0;
			break;

		}
		/*������ϻ��쳣�˳�*/
		if(2 != rtnValue) 
		{
			break;
		}

		/*ʱ��ͳ�Ƽ����㴦��*/
		if (0x02 == mode)
		{
			/*��ȡ��ǰʱ��*/
			dwCurTimePoint=dquGetCurrentTimeByMS();
			/*�����Ѿ���ʹ�ù���ʱ��*/
			dwOnceTime=dwCurTimePoint-dwBeginTimePoint;
			dwUsedTime+=dwOnceTime;
			/*ʹ�õ�ʱ��С��ʱ��Ƭ*/
			if ((dwUsedTime < timeSlice) && (dwOnceTime > 0) )
			{
				/*ʣ�µĴ���ʱ�乻��ִ��һ�δ����*/
				wFreqTimes=(timeSlice-dwUsedTime)/dwOnceTime;
				if (wFreqTimes >= 1)
				{
					bExeState=DQU_QFUN_TRUE;
				}
				else/*ʣ��ʱ�䲻����һ��ִ��*/
				{
					bExeState=DQU_QFUN_FASLE;
				}
			}
			else/*��ʱ�˳�*/
			{
				bExeState=DQU_QFUN_FASLE;
			}
		}

		/*����ģʽΪ�ϵ����ʱһֱ��ִ����,��ִ��״̬Ϊ���*/	
	} while ((0x01 == mode) || (DQU_QFUN_TRUE == bExeState));

	return rtnValue;
}

/*��Ŷ��ձ��ʼ����ϣ�ִ�б�Ŷ��ձ��ڲ�����������������*/
UINT8 dfsuCbtcConfigInitFinally(void)
{
	UINT16 wIndex=0;
	UINT8 bRetVal=0;

	g_wCurInitStruProg=0x0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
	g_wCurMainIndex=0; /*��ǰ�������������ֲ�����ʱʹ��*/

	bRetVal=1;
	return bRetVal;
}


/*---------------���ݽ���������-------------------------*/

/**********************************************
�������ܣ���ʼ��[�г��������ñ�]
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitTrainModelConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;
	static UINT32 rowDataLen = 0;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->TrainTypeConfigLen = rowCount;	
		
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;

			g_pCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainName = ShortFromChar(pData);
			pData += 2;

			g_pCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainType = *pData;
			pData += 1;
		}
	}	
		
	rtnValue = 1;
	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�г����ܲ���]
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitTrainPerformConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;	
	UINT32 rowDataLen;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->TrainInfoLen = rowCount;	
		
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;

			/*�г�����*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainType = *pData;
			pData += 1;
			/*�г�����*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainLength = ShortFromChar(pData);
			pData += 2;
			/*�г����ǣ�����ٶ�*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].TrainMaxTracAcc = *pData;
			pData += 1;
			/*��·��������¶�*/
			g_pCbtcConfigDataStru->cbtcTrainInfo[i].WorstGrade = ShortFromChar(pData);
			pData += 2;
		}
	}
		
	rtnValue = 1;
	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ϵͳ������������]-CPK(Ҫ�����ҽ���1������)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*������ݽṹ��*/                      
		/*��·�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*��ʱ�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTPУʱʱ����*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*��ȫ��������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*һ��������ʱ���ٵ�������*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*һ����ʱ���ٱ����а���������߼���������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZCͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCIͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATSͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCIͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*ciciͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*Ϊ���ats��ʾ��������������һ���ֶ�---�Ϲ�ѧ2012-12-17*/
		/*link�߼�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		/*����ATO��׼ͣ��������MA���ȣ���ԺϿ����ݽṹV16.0.0��ϵͳ�������ݸ���ϵͳ�������������ֶΡ�����ATO��׼ͣ��������MA���ȡ���add by my 20170220*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;			

		rtnValue = 1;
	} 
	
	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ϵͳ������������]-FAO(Ҫ�����ҽ���1������)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*������ݽṹ��*/
		/*��·�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*��ʱ�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTPУʱʱ����*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*��ȫ��������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*һ��������ʱ���ٵ�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*һ����ʱ���ٱ����а���������߼���������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCIͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCIͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*ciciͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psdͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*link�߼�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		rtnValue = 1;
	} 
	
	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ϵͳ������������]-HLT(Ҫ�����ҽ���1������)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_HLT(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 i = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*������ݽṹ��*/
		/*��·�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*��ʱ�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTPУʱʱ����*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*��ȫ��������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*һ��������ʱ���ٵ�������*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*һ����ʱ���ٱ����а���������߼���������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZCͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCIͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATSͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCIͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*ciciͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psdͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*Ϊ���ats��ʾ��������������һ���ֶ�---�Ϲ�ѧ2012-12-17*/
		/*link�߼�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		g_pCbtcConfigDataStru->cbtcConfigData.SpanLineNum=ShortFromChar(pDataAddr); /*��Ч������·������������ͨ���������� add by qxt 20160811*/
		pDataAddr += 2;

		for (i=0;i<4;i++ )
		{
			g_pCbtcConfigDataStru->cbtcConfigData.LineID[i]=*pDataAddr; /*��·��ţ�������ͨ���������� add by qxt 20160811*/
			pDataAddr += 1;
			g_pCbtcConfigDataStru->cbtcConfigData.OverlapLineLogDirUpAndDown[i]=*pDataAddr;  /*�ص����ڸ���·��ϵͳ�����߼������������з���Ĺ�ϵ��������ͨ���������� add by qxt 20160811*/
			pDataAddr += 1;
		}

		/*�����ж�����(cm/s)�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180314*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZeroSpeedThreshold = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*����ATO��׼ͣ��������MA���ȣ����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180314*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*Ԥ��Ÿֵ������������� MS=MagneticSteel  NZ=NeutralZone�����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180614*/
		g_pCbtcConfigDataStru->cbtcConfigData.StartDistanceOfMSToNZ = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*������ͨFAO�ӿڼ����ֶΣ����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180614*/
		g_pCbtcConfigDataStru->cbtcConfigData.CompatibleOfHLHTFAO = *pDataAddr;
		pDataAddr += 1;

		/*�����Ƿ�̬�����ֶΣ����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180625*/
	    g_pCbtcConfigDataStru->cbtcConfigData.ValueOfLineDynamicTest = *pDataAddr;
		pDataAddr += 1;

		/*�Ƿ���VOBC����MA�ϰ���,���ݻ�����ͨ���ݽṹV14.0.0��ӣ�add by qxt 20180719*/
		g_pCbtcConfigDataStru->cbtcConfigData.IsVOBCHandleObatacles = *pDataAddr;
		pDataAddr += 1;

		 /*����ͬʱ���ж�̬����(����)���г����������,���ݻ�����ͨ���ݽṹV14.0.0��ӣ�add by qxt 20180719*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTrainNumOfAwake = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*��Ŀ��ʶ(���ֹ��̽ӿ�),���ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
		g_pCbtcConfigDataStru->cbtcConfigData.TypeOfProject = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*ZC-ZC�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToZC = *pDataAddr;
		pDataAddr += 1;

		/*ZC-VOBC�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToVOBC = *pDataAddr;
		pDataAddr += 1;

		/*ZC-DSU�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToDSU = *pDataAddr;
		pDataAddr += 1;

		/*ZC-CI�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToCI = *pDataAddr;
		pDataAddr += 1;

		/*ZC-ATS�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfZCToATS = *pDataAddr;
		pDataAddr += 1;

		/*VOBC-ATS�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfVOBCToATS = *pDataAddr;
		pDataAddr += 1;

		/*VOBC-CI�ӿ�Э��汾��,���ݻ�����ͨ���ݽṹV16.0.0��ӣ�add by sds 2019-1-21*/
		g_pCbtcConfigDataStru->cbtcConfigData.VersionOfVOBCToCI = *pDataAddr;
		pDataAddr += 1;

		rtnValue = 1;
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ϵͳ������������]-DG(Ҫ�����ҽ���1������)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_DG(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 i = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*������ݽṹ��*/
		/*��·�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed=(UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed+0.5);
		pDataAddr += 1;

		/*��ʱ�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTPУʱʱ����*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*��ȫ��������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBCZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*һ��������ʱ���ٵ�������*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*һ����ʱ���ٱ����а���������߼���������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfTsr = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*DSUZCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.DsuZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCZCͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.ZcZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCCIͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*DSUATSͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.DsuAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ZCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCCIͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcCiCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcAtsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBCATSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*ciciͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CICICommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-vobcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-zcͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiZcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ci-psdͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.CiPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*Ϊ���ats��ʾ��������������һ���ֶ�---�Ϲ�ѧ2012-12-17*/
		/*link�߼�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		g_pCbtcConfigDataStru->cbtcConfigData.SpanLineNum=ShortFromChar(pDataAddr); /*��Ч������·������������ͨ���������� add by qxt 20160811*/
		pDataAddr += 2;

		for (i=0;i<4;i++ )
		{
			g_pCbtcConfigDataStru->cbtcConfigData.LineID[i]=*pDataAddr; /*��·��ţ�������ͨ���������� add by qxt 20160811*/
			pDataAddr += 1;
			g_pCbtcConfigDataStru->cbtcConfigData.OverlapLineLogDirUpAndDown[i]=*pDataAddr;  /*�ص����ڸ���·��ϵͳ�����߼������������з���Ĺ�ϵ��������ͨ���������� add by qxt 20160811*/
			pDataAddr += 1;
		}

		rtnValue = 1;
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ϵͳ������������]-CC(Ҫ�����ҽ���1������)��������add by sds 2019-6-13
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitSubSystemCommonConfigData_CC(UINT8 *pDataAddr,UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;

	if ((NULL != pDataAddr) && (1 == rowCount))
	{
		/*������ݽṹ��*/                      
		/*��·�������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = *pDataAddr;
		g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed = (UINT16)(27.77778*g_pCbtcConfigDataStru->cbtcConfigData.MaxLineLimitSpeed + 0.5);
		pDataAddr += 1;

		/*��ʱ�����������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrSpeed = *pDataAddr;
		pDataAddr += 1;

		/*NTPУʱʱ����*/
		g_pCbtcConfigDataStru->cbtcConfigData.TimeZoneDifference = *pDataAddr;
		pDataAddr += 1;

		/*��ȫ��������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MABackDistance = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*һ��������ʱ���ٵ�������*/	
		g_pCbtcConfigDataStru->cbtcConfigData.MaxTsrNum = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*һ����ʱ���ٱ����а��������������������*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxSectionOfSpeed = ShortFromChar(pDataAddr);
		pDataAddr += 2;

		/*VOBC-VOBCͨ�Ź����ж�ʱ�䣨ms��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcVobcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*VOBC-OCͨ�Ź����ж�ʱ��*/	
		g_pCbtcConfigDataStru->cbtcConfigData.VobcOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*VOBC-ITSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.VobcItsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*TMC-ITSͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.TmcItsCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
				
		/*TMC-OCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.TmcOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*ITS-OCͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.ItsOcCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;
		
		/*OC-PSDͨ�Ź����ж�ʱ��*/
		g_pCbtcConfigDataStru->cbtcConfigData.OcPsdCommunicationErrorTime = LongFromChar(pDataAddr);
		pDataAddr += 4;

		/*ϵͳ������󱣻����γ���*/
		g_pCbtcConfigDataStru->cbtcConfigData.MaxOverlapLength = ShortFromChar(pDataAddr);
		pDataAddr += 2;
		
		/*����ϵͳ�����߼������������з���Ĺ�ϵ*/
		g_pCbtcConfigDataStru->cbtcConfigData.LinkLogicDirUpAndDown = *pDataAddr;
		pDataAddr += 1;

		/*�����ж�����(cm/s)*/
		g_pCbtcConfigDataStru->cbtcConfigData.ZeroSpeedThreshold = *pDataAddr;
		pDataAddr += 1;

		/*����ATO��׼ͣ��������MA����*/
		g_pCbtcConfigDataStru->cbtcConfigData.MALengthForATOParking = ShortFromChar(pDataAddr);
		pDataAddr += 2;			

		rtnValue = 1;
	} 
	else
	{
		rtnValue = 0;
	}

	return rtnValue;
}
/**********************************************
�������ܣ���ʼ��[������������Ϣ]
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:1�ɹ�,0ʧ��
**********************************************/
UINT8 dfsInitTrainDepotConfigData(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8  rtnValue = 0;
	UINT32 i =0;
	UINT32 rowDataLen;
	UINT8 *pData = pDataAddr;

	g_pCbtcConfigDataStru->DepotConfigLen = rowCount;
	
	if(rowCount > 0)
	{
		rowDataLen = dataSize/rowCount;

		for(i=0;i<rowCount;i++)
		{
			pData = pDataAddr + rowDataLen*i;
			
			/*�����α��*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotID = *pData;
			pData += 1;
			/*��������������ID*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotCIID = ShortFromChar(pData);
			pData += 2;
			/*�����γ��ⷽ��*/
			g_pCbtcConfigDataStru->cbtcDepotInfo[i].DepotDir = *pData;
			pData += 1;
		}

	}
	
	rtnValue = 1;
	return rtnValue;
}

/*-----------------------------���ݲ�ѯ�ӿں���-----------------------*/

/*���õ�ǰ��ѯ������Դ*/
UINT8 dquSetCurCbtcCfgInfoQueryDataSource(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru)
{
	UINT8 bRetVal=0;
	if (NULL != pCbtcConfigDataStru)
	{
		qds_dsuCbtcConfigDataStru=pCbtcConfigDataStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/***********************************************
*�������ܣ������г����Ʋ�ѯ�г�����
*���������TrainName�г���
*����������г�����
*����ֵ��0ʧ��1�ɹ�
**********************************************/
UINT8 dquGetTrainType(UINT16 TrainName,UINT8* pTrainType)
{
	UINT32 i;
	UINT8 rtnValue = 0;

	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		for(i=0;i<qds_dsuCbtcConfigDataStru->TrainTypeConfigLen;i++)
		{
			if(qds_dsuCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainName == TrainName)
			{
				/*�ҵ�����*/
				* pTrainType = qds_dsuCbtcConfigDataStru->cbtcTrainTypeConfig[i].TrainType;
				rtnValue = 1;
				return rtnValue;
			}
		}	
		/*û�ҵ�����ѭ��*/
		rtnValue = 0;
	}
	else
	{

	}

	return rtnValue;
}

/*******************************************
*�������ܣ������г����ͻ���г�������Ϣ
*���������TrainType�г�����
*���������pCbtcTrainInfo�г�������Ϣ�ṹ��
*����ֵ�� 0ʧ�� 1�ɹ�
*******************************************/
UINT8 dquGetCbtcTrainInfo(UINT8 TrainType,CBTC_TRAIN_INFO_STRU *pCbtcTrainInfo)
{
	UINT32 i=0;
	UINT8 rtnValue = 0;

	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		for (i=0;i<qds_dsuCbtcConfigDataStru->TrainInfoLen;i++)
		{
			if(TrainType==qds_dsuCbtcConfigDataStru->cbtcTrainInfo[i].TrainType)
			{
				*pCbtcTrainInfo = qds_dsuCbtcConfigDataStru->cbtcTrainInfo[i];
				rtnValue = 1;
				return rtnValue;
			}  
			/*û�ҵ�����ѭ��*/
			rtnValue = 0;
		}
	}
	else
	{

	}

	return rtnValue;

}

/*��ȡ����ϵͳ�����������ñ�*/
UINT8 dquGetCbtcConfigData (CBTC_CONFIG_DATA_STRU* pCbtcConfigDataStru)
{
	UINT8 rtnValue = 0;
	
	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		*pCbtcConfigDataStru = qds_dsuCbtcConfigDataStru->cbtcConfigData;
		rtnValue = 1;
	}
	else
	{

	}
	return rtnValue;
}

/*********************************************
*�������ܣ���ó�����������Ϣ
*�����������
*���������pCbtcDepotInfo������������Ϣ
*			Length������Ϣ����
*����ֵ�� 0ʧ�� 1�ɹ�
***********************************************/
UINT8 dquGetCbtcDepotInfo(CBTC_DEPOT_INFO_STRU *pCbtcDepotInfo, UINT16 *Length)
{
	UINT32 i=0;
	UINT8 rtnValue = 0;
	
	if (NULL != qds_dsuCbtcConfigDataStru)
	{
		if((0 == qds_dsuCbtcConfigDataStru->DepotConfigLen) || (qds_dsuCbtcConfigDataStru->DepotConfigLen > 10))
		{
			/*û�ҵ�����ѭ��*/
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			for (i=0;i<qds_dsuCbtcConfigDataStru->DepotConfigLen;i++)
			{
				*(pCbtcDepotInfo+i) = qds_dsuCbtcConfigDataStru->cbtcDepotInfo[i];
				(*Length)++;
			}
		}
		rtnValue = 1;
	}
	else
	{

	}

	return rtnValue;
}
