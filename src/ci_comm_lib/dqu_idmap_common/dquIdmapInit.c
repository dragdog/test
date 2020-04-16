/************************************************************************
*
* �ļ���   ��  dquIdmapInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2016.03.21
* ����     ��  �����
* �������� ��  ��Ŷ��ձ��ʼ���������
* ʹ��ע�� ��
* �޸ļ�¼ ��
*
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include <string.h>
#include "dquIdmapInit.h"
#include "dsuIdmapStruct.h"
#include "dquCbtcType.h"

UINT8	dfsIdmapInitFlag = 0;
UINT8	initIdMapErrNo[INIT_IDMAP_ERR_NUM];      /*�����ݳ�ʼ��ʧ�ܹ��Ϻ�����*/

static dfsDataHead g_DFsDH;			/*FS�ļ�������ͷ�ṹ��*/
static UINT8*  g_pDataAddr;         /*FS��Ԫ���ݵ�ַ*/
static UINT32  g_DataSize;          /*FS��Ԫ���ݴ�С���ֽ�����*/
static UINT16  g_RowCount;			/*����������*/

static UINT16	g_wCurInitStruProg=0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
static UINT16   g_wCurMainIndex=0;  /*��ǰ�������������ֲ�����ʱʹ��*/
static UINT8	g_IdmapInitMode=0x01; /*��Ŷ��ձ��ʼ��ģʽ*/
static UINT8	g_IsIdmapAllocMemory = 0x01;  /*��������ļ����ʽӿ��ϵ��ʼ���������ڴ�*/
static UINT8  IdmapInitFunStepCtrl(UINT16 wStep);/*���ӵ�ͼ��ʼ���ֲ����ƺ�������*/

/*��ʼ������ǼǱ�*/
static INIT_TASK_TAB_STRU g_struInitTaskTab[]=
{
	{DQU_QFUN_TRUE, IDMAP_ZC_ID,		initZCNum,		5,1,0}, 	
	{DQU_QFUN_TRUE, IDMAP_CI_ID,		initCINum,		5,1,0},	
	{DQU_QFUN_TRUE, IDMAP_ATS_ID,		initATSNum,		5,1,0},
	{DQU_QFUN_TRUE, IDMAP_SIGNAL_ID,	initSignalNum,	5,1,0},	
	{DQU_QFUN_TRUE, IDMAP_TRACKSEC_ID,	initTrackSecNum,5,1,0},	
	{DQU_QFUN_TRUE, IDMAP_SWITCH_ID,	initSwitchNum,	5,1,0},
	{DQU_QFUN_TRUE, IDMAP_STATION_ID,	initStationNum,	5,1,0},
	{DQU_QFUN_TRUE, IDMAP_EMERGBTN_ID,	initEmergBtnNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_BALISE_ID,	initBaliseNum,	5,1,0},
	{DQU_QFUN_TRUE, IDMAP_PSD_ID,	    initPSDoorNum,	5,1,0},
	{DQU_QFUN_TRUE, IDMAP_VOBC_ID,		initVobcNum,	5,1,0},	
	{DQU_QFUN_TRUE, IDMAP_FPD_ID,   	initFPDoorNum,	5,1,0},
	{DQU_QFUN_TRUE, IDMAP_DSU_ID,   	initDsuNum,		5,1,0},
	{DQU_QFUN_TRUE, IDMAP_LOGICSEC_ID,  initLogicSecNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_AOMSEC_ID,    initAomSecNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_SPKSSEC_ID,   initSpksSecNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_GODSEC_ID,    initGodSecNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_STASEC_ID,    initStaSecNum,5,1,0},
	{DQU_QFUN_TRUE, IDMAP_PHYSEC_ID,    initPhySecNum,5,1,0 },

};
/*��ʼ������ǼǱ���Ŀ����*/
static const UINT16 MAX_INITTASK_COUNT=sizeof(g_struInitTaskTab)/sizeof(INIT_TASK_TAB_STRU);



/*********************************************
*�������ܣ���Ŷ��ձ��ʼ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 dfsIdmapInit(CHAR * FSName)
{
	UINT16	wIndex=0;
	UINT8   funcRtn = 0;
	UINT8	rtnValue = 0;
	UINT32	DataID=0;/*�������ͱ�ʶ*/
	UINT8	*pDataAddr=NULL;
	UINT32	DataSize=0;
	UINT16	rowCount = 0;

	/*��ʼ��������Ϣ��¼*/
	memset(initIdMapErrNo, 0, INIT_IDMAP_ERR_NUM);

	if (NULL!=g_dsuIdmapStru)
	{
		/*��ʼ��һ�ξ�����*/
		if (dfsIdmapInitFlag == 1)
		{
			rtnValue = 1;
			return rtnValue;
		}

		g_IdmapInitMode=0x01;/*�ϵ����ģʽ*/

		g_IsIdmapAllocMemory=0x01;/*Ĭ�ϱ��ӿڷ����ڴ�*/

		/*��̬���������ռ�*/
		g_dsuIdmapStru->dsuIdmapLenStru=(DSU_IDMAP_LEN_STRU*)MyNew(sizeof(DSU_IDMAP_LEN_STRU));

		/*��̬����ռ�ʧ��*/
		if (NULL != g_dsuIdmapStru->dsuIdmapLenStru)
		{
			/*��ʼ��Ϊ0xFF*/
			memset(g_dsuIdmapStru->dsuIdmapLenStru, 0x0, sizeof(DSU_IDMAP_LEN_STRU));
		}
		else
		{
			rtnValue = 0;
			
		}
		
		/*��̬���������ռ�*/
		g_dsuIdmapStru->dsuStaticIdmapStru=(DSU_STATIC_IDMAP_STRU*)MyNew(sizeof(DSU_STATIC_IDMAP_STRU));

		/*��̬����ռ�ʧ��*/
		if (NULL != g_dsuIdmapStru->dsuStaticIdmapStru)
		{
			/*��ʼ��Ϊ0xFF*/
			memset(g_dsuIdmapStru->dsuStaticIdmapStru, 0x0, sizeof(DSU_STATIC_IDMAP_STRU));
		}
		else
		{
			rtnValue = 0;

		}
		
		/*��̬���������ռ�*/
		g_dsuIdmapStru->dsuIdmapIndexStru=(DSU_IDMAP_INDEX_STRU*)MyNew(sizeof(DSU_IDMAP_INDEX_STRU));

		/*��̬����ռ�ʧ��*/
		if (NULL != g_dsuIdmapStru->dsuIdmapIndexStru)
		{
			/*��ʼ��Ϊ0xFF*/
			memset(g_dsuIdmapStru->dsuIdmapIndexStru, 0x0, sizeof(DSU_IDMAP_INDEX_STRU));
		}
		else
		{
			rtnValue = 0;

		} 

		/*������ʼ������*/
		wIndex=0;
		do /*�������ݵ���Ӧ�Ľṹ��*/
		{
			if (DQU_QFUN_TRUE == g_struInitTaskTab[wIndex].bIsEffective)/*�ж������ʼ�������Ƿ�ִ��*/
			{
				/*��ȡ��������*/
				DataID=g_struInitTaskTab[wIndex].DataTypeId;
				funcRtn=dquGetConfigDataNew( FSName, IDMAP_DATA_TYPE, DataID, &pDataAddr, &DataSize, &rowCount);
				if (funcRtn != 1)
				{
					/*��������ʧ��*/
					initIdMapErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
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

		dquSetCurIdMapQueryDataSource(g_dsuIdmapStru);
		/*����ִ�б�־��Ϊ1*/
		dfsIdmapInitFlag = 1;
		rtnValue = 1;
	}
	else
	{
		/*����ִ�б�־��Ϊ0*/
		dfsIdmapInitFlag = 0;
		rtnValue = 0;
	}
	return rtnValue;
}

/****************************************
*�������ܣ���ʼ����Ŷ��ձ���չ�ӿ�
*����˵���� pDataBuf,�������ݿ飬
*			dsuIdmapStru,�ⲿʵ���Ŷ��ձ�洢�ṹ��ָ��
*			mode,����ģʽ��1�ϵ�һ�μ��أ�2����ʱ��δ���
*			timeSlice��ִ�е�ʱ��Ƭ����λ�����룩
*����ֵ��  0ʧ�ܣ�1�ɹ���2�ֲ�����δ���
*****************************************/
UINT8 dfsIdmapInitExp(UINT8 * pDataBuf,DSU_IDMAP_STRU *dsuIdmapStru,UINT8 mode,UINT16 timeSlice)
{
	UINT32 DataID;
	UINT16 wPos=0;
	UINT8 rtnValue=0;
	UINT16 wFreqTimes=0;
	UINT8 bExeState=DQU_QFUN_FASLE;
	UINT32 dwOnceTime,dwUsedTime=0;
	UINT32 dwCurTimePoint,dwBeginTimePoint=0,dwLastTimePoint=0;

	UINT8 funcRtn=0;

	if(0==g_wCurMainIndex)/*��������������ִ��̬��ʱ�򣬾Ͳ��ܸı䴦��ģʽ*/
	{
		g_IdmapInitMode=mode;
		g_IsIdmapAllocMemory=0x00;/*Ĭ�ϱ��ӿڲ������ڴ�*/
		/*����VOBC�����ڴ˴��ӿڹ���ָ��*/
		if (NULL!=dsuIdmapStru)
		{
			g_dsuIdmapStru=dsuIdmapStru;
		} 
		else
		{
			rtnValue = 0;
			return rtnValue;
		}
	}


	/*��ȡ���뿪ʼʱ��*/
	dwBeginTimePoint=dquGetCurrentTimeByMS();
	dwLastTimePoint=dwBeginTimePoint;/*��¼��ʷʱ���*/
	do /*�ϵ���س������б��ֿ���*/
	{	
		/*��ȡ���뿪ʼʱ��
		dwBeginTimePoint=dquGetCurrentTimeByMS();*/
		switch(g_wCurMainIndex)/*�������̿���*/
		{
		case 0:
			/*����FS�ļ�����ͷ������������Ϣ*/
			funcRtn=dquAnalyzeFsDataHead(pDataBuf, &g_DFsDH);
			if (funcRtn != 1)
			{
				/*����FSͷ����ʧ��*/
				rtnValue = 0;
			}
			else /*����ɹ�*/
			{
				g_wCurMainIndex=1;
				rtnValue = 2;
			}

			break;
		case 1:		
			/*��ȡ��������*/
			DataID=g_struInitTaskTab[g_wCurInitStruProg].DataTypeId;
			funcRtn=dquGetConfigDataExpNew(pDataBuf, &g_DFsDH,  IDMAP_DATA_TYPE, DataID, &g_pDataAddr, &g_DataSize, &g_RowCount);
			if (funcRtn != 1)
			{
				/*��������ʧ��*/
				initIdMapErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
				rtnValue = 0;
			}
			else /*����ɹ�*/
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
			else if(2 == funcRtn)/*�ֲ�����δ���*/
			{
				g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage++;
				rtnValue = 2;	

			}
			else/*��������쳣*/
			{
				rtnValue = 0;
			}
						
			break;
		case 3:
			rtnValue=1;

			break;
		default:
			rtnValue = 0;
			break;

		}	

		/*�쳣�˳�*/
		if(2 != rtnValue) 
		{
			break;
		}

		/*ʱ��ͳ�Ƽ����㴦��*/
		if (0x02 == mode)
		{
			/*��ȡ��ǰʱ��*/
			dwCurTimePoint=dquGetCurrentTimeByMS();

			/*����ʣ��ʱ�� */
			dwUsedTime=dwCurTimePoint-dwBeginTimePoint; 
			/*���㵥��ִ��ʱ�� */
			dwOnceTime=dwCurTimePoint-dwLastTimePoint; 
			/*������ʷʱ��� */
			dwLastTimePoint=dwCurTimePoint; 

			/*ʹ�õ�ʱ��С��ʱ��Ƭ */
			if (dwUsedTime < timeSlice) 
			{
				/*ʣ�µĴ���ʱ�乻��ִ��һ�δ����*/
				if ( dwOnceTime > 0 )
				{
					wFreqTimes = (timeSlice-dwUsedTime)/dwOnceTime;
				}
				else /*�������ִ��ʱ��С��1ms�������ִ��*/
				{
					wFreqTimes = 1;
				}

				/*ִ�б�־*/
				if (wFreqTimes >= 1)
				{
					bExeState=DQU_QFUN_TRUE;
				}
				else /*ʣ��ʱ�䲻����һ��ִ��*/
				{
					bExeState=DQU_QFUN_FASLE;
				}
			}
			else/*��ʱ�˳�������һ�δ���ʱ��С��1�����Ĭ��Ҳ�˳���*/
			{	/* ��ֹ���ֽ��ж��С��1���봦���ۻ������Ѿ���ʱ�����*/

				bExeState=DQU_QFUN_FASLE;
			}
		}

		/*����ģʽΪ�ϵ����ʱһֱ��ִ����,��ִ��״̬Ϊ���*/	
	} while ((0x01 == mode) ||(DQU_QFUN_TRUE == bExeState) );

	return rtnValue;
}


/*��Ŷ��ձ��ʼ����ϣ�ִ�б�Ŷ��ձ��ڲ�����������������*/
UINT8 dfsIdmapInitFinally()
{
	UINT16 wIndex=0;
	UINT8 bRetVal=0;

	g_wCurInitStruProg=0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
	g_wCurMainIndex=0; /*��ǰ�������������ֲ�����ʱʹ��*/
	g_IdmapInitMode=0x01; /*���ӵ�ͼ��ʼ��ģʽ*/

	/*�����ݳ�ʼ��״̬����*/
	for(wIndex=0;wIndex<MAX_INITTASK_COUNT;wIndex++)
	{
		g_struInitTaskTab[wIndex].wCurProcStage = 0x01;
		g_struInitTaskTab[wIndex].wCurInitPos = 0x0;
	}

	/*��ʼ��������Ϣ��¼*/
	memset(initIdMapErrNo, 0, INIT_IDMAP_ERR_NUM);

	bRetVal=1;
	return bRetVal;
}

/*���ӵ�ͼ��ʼ���ֲ����ƺ���*/
static UINT8  IdmapInitFunStepCtrl(UINT16 wStep)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_IdmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_TRUE;
	}
	else /*�ֲ�����*/
	{
		if (wStep == g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage)
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else if (  (wStep == 0x03) && (g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage > 0x03))
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else
		{

		}
	}

	return bIsEffective;

}

/*��Ŷ��ձ��ʼ��ѭ�������ֲ��и��*/
static UINT8 IdmapInitLoopStepLenCtrl(UINT32 dwIndex)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_IdmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_FASLE;
	}
	else /*�ֲ�����*/
	{
		if (g_struInitTaskTab[g_wCurInitStruProg].wStepLen==dwIndex-g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos + 1)
		{
			g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos=dwIndex+1;
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}


/**********************************************
�������ܣ���ʼ��[ZC]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initZCNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ZC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wZCNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ZCNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->ZCNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuZCNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->ZCNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuZCNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuZCNumIndex, 0xFF, dsuIdmapIndexStru->ZCNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pZCNumStru = (DSU_ZC_NUM_STRU *)MyNew(sizeof(DSU_ZC_NUM_STRU) * dsuIdmapLenStru->wZCNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pZCNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pZCNumStru, 0xFF, sizeof(DSU_ZC_NUM_STRU) * dsuIdmapLenStru->wZCNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_ZC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wZCNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_ZC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->ZCNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wZCNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pZCNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ZCNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuZCNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wZCNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[CI]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCINum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CI_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CI_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;


	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wCINumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CINumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->CINUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuCINumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->CINUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuCINumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuCINumIndex, 0xFF, dsuIdmapIndexStru->CINUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pCINumStru = (DSU_CI_NUM_STRU *)MyNew(sizeof(DSU_CI_NUM_STRU) * dsuIdmapLenStru->wCINumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pCINumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pCINumStru, 0xFF, sizeof(DSU_CI_NUM_STRU) * dsuIdmapLenStru->wCINumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_CI_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wCINumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_CI_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->CINUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wCINumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pCINumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CINumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuCINumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wCINumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ATS]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initATSNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ATS_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ATS_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wATSNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ATSNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->ATSNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuATSNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->ATSNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuATSNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuATSNumIndex, 0xFF, dsuIdmapIndexStru->ATSNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pATSNumStru = (DSU_ATS_NUM_STRU *)MyNew(sizeof(DSU_ATS_NUM_STRU) * dsuIdmapLenStru->wATSNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pATSNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pATSNumStru, 0xFF, sizeof(DSU_ATS_NUM_STRU) * dsuIdmapLenStru->wATSNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_ATS_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wATSNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_ATS_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->ATSNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wATSNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pATSNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ATSNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuATSNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wATSNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�źŻ�]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSignalNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SIGNAL_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SIGNAL_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wSignalNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			SignalNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->SIGNALNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuSignalNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->SIGNALNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuSignalNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuSignalNumIndex, 0xFF, dsuIdmapIndexStru->SIGNALNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pSignalNumStru = (DSU_SIGNAL_NUM_STRU *)MyNew(sizeof(DSU_SIGNAL_NUM_STRU) * dsuIdmapLenStru->wSignalNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pSignalNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pSignalNumStru, 0xFF, sizeof(DSU_SIGNAL_NUM_STRU) * dsuIdmapLenStru->wSignalNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_SIGNAL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wSignalNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_SIGNAL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->SIGNALNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wSignalNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pSignalNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			SignalNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuSignalNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wSignalNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initTrackSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_TRACKSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_TRACKSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wTrackSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			TrackSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->TRACKSECNUMINDEXSIZE = stru.wLinkId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuTrackSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->TRACKSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuTrackSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuTrackSecNumIndex, 0xFF, dsuIdmapIndexStru->TRACKSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pTrackSecNumStru = (DSU_TRACKSEC_NUM_STRU *)MyNew(sizeof(DSU_TRACKSEC_NUM_STRU) * dsuIdmapLenStru->wTrackSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pTrackSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pTrackSecNumStru, 0xFF, sizeof(DSU_TRACKSEC_NUM_STRU) * dsuIdmapLenStru->wTrackSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_TRACKSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wTrackSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_TRACKSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->TRACKSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wTrackSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pTrackSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			TrackSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuTrackSecNumIndex[pCurrentStru->wLinkId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wTrackSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSwitchNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SWITCH_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SWITCH_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wSwitchNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			SwitchNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->SWITCHNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuSwitchNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->SWITCHNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuSwitchNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuSwitchNumIndex, 0xFF, dsuIdmapIndexStru->SWITCHNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pSwitchNumStru = (DSU_SWITCH_NUM_STRU *)MyNew(sizeof(DSU_SWITCH_NUM_STRU) * dsuIdmapLenStru->wSwitchNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pSwitchNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pSwitchNumStru, 0xFF, sizeof(DSU_SWITCH_NUM_STRU) * dsuIdmapLenStru->wSwitchNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_SWITCH_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wSwitchNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_SWITCH_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->SWITCHNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wSwitchNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pSwitchNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			SwitchNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuSwitchNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wSwitchNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�߼�վ̨]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStationNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STATION_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STATION_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wStationNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			StationNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->STATIONNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuStationNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->STATIONNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuStationNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuStationNumIndex, 0xFF, dsuIdmapIndexStru->STATIONNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pStationNumStru = (DSU_STATION_NUM_STRU *)MyNew(sizeof(DSU_STATION_NUM_STRU) * dsuIdmapLenStru->wStationNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pStationNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pStationNumStru, 0xFF, sizeof(DSU_STATION_NUM_STRU) * dsuIdmapLenStru->wStationNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_STATION_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wStationNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_STATION_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->STATIONNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wStationNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pStationNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			StationNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuStationNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wStationNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�����رհ�ť]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initEmergBtnNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_EMERGBTN_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_EMERGBTN_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wEmergBtnNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			EmergBtnNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->EMERGBTNNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuEmergBtnIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->EMERGBTNNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuEmergBtnIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuEmergBtnIndex, 0xFF, dsuIdmapIndexStru->EMERGBTNNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pEmergBtnNumStru = (DSU_EMERGBTN_NUM_STRU *)MyNew(sizeof(DSU_EMERGBTN_NUM_STRU) * dsuIdmapLenStru->wEmergBtnNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pEmergBtnNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pEmergBtnNumStru, 0xFF, sizeof(DSU_EMERGBTN_NUM_STRU) * dsuIdmapLenStru->wEmergBtnNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_EMERGBTN_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wEmergBtnNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_EMERGBTN_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->EMERGBTNNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wEmergBtnNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pEmergBtnNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			EmergBtnNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuEmergBtnIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wEmergBtnNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[Ӧ����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initBaliseNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BALISE_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_BALISE_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wBaliseNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			BaliseNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->BALISENUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuBaliseNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->BALISENUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuBaliseNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuBaliseNumIndex, 0xFF, dsuIdmapIndexStru->BALISENUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pBaliseNumStru = (DSU_BALISE_NUM_STRU *)MyNew(sizeof(DSU_BALISE_NUM_STRU) * dsuIdmapLenStru->wBaliseNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pBaliseNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pBaliseNumStru, 0xFF, sizeof(DSU_BALISE_NUM_STRU) * dsuIdmapLenStru->wBaliseNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_BALISE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wBaliseNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_BALISE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->BALISENUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wBaliseNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pBaliseNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			BaliseNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuBaliseNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wBaliseNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPSDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PSD_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_PSD_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wPSDoorNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			PSDoorNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->PSDNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuPSDoorNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->PSDNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuPSDoorNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuPSDoorNumIndex, 0xFF, dsuIdmapIndexStru->PSDNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pPSDNumStru = (DSU_PSD_NUM_STRU *)MyNew(sizeof(DSU_PSD_NUM_STRU) * dsuIdmapLenStru->wPSDoorNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pPSDNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pPSDNumStru, 0xFF, sizeof(DSU_PSD_NUM_STRU) * dsuIdmapLenStru->wPSDoorNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_PSD_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wPSDoorNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_PSD_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->PSDNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wPSDoorNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pPSDNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			PSDoorNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuPSDoorNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wPSDoorNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[VOBC]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initVobcNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_VOBC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_VOBC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;


	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wVobcNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			VOBCNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->VOBCNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuVobcNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->VOBCNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuVobcNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuVobcNumIndex, 0xFF, dsuIdmapIndexStru->VOBCNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pVobcNumStru = (DSU_VOBC_NUM_STRU *)MyNew(sizeof(DSU_VOBC_NUM_STRU) * dsuIdmapLenStru->wVobcNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pVobcNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pVobcNumStru, 0xFF, sizeof(DSU_VOBC_NUM_STRU) * dsuIdmapLenStru->wVobcNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_VOBC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wVobcNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_VOBC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->VOBCNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wVobcNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pVobcNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			VOBCNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuVobcNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wVobcNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initFPDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_FPD_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_FPD_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;


	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wFPDoorNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			FPDoorNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->FPDNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuFPDoorNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->FPDNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuFPDoorNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuFPDoorNumIndex, 0xFF, dsuIdmapIndexStru->FPDNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pFPDoorNumStru = (DSU_FPD_NUM_STRU *)MyNew(sizeof(DSU_FPD_NUM_STRU) * dsuIdmapLenStru->wFPDoorNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pFPDoorNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pFPDoorNumStru, 0xFF, sizeof(DSU_FPD_NUM_STRU) * dsuIdmapLenStru->wFPDoorNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_FPD_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wFPDoorNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_FPD_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->FPDNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wFPDoorNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pFPDoorNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			FPDoorNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuFPDoorNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wFPDoorNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[DSU]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initDsuNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_DSU_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_DSU_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wDsuNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DsuNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->DSUNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuDsuNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->DSUNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuDsuNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuDsuNumIndex, 0xFF, dsuIdmapIndexStru->DSUNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pDsuNumStru = (DSU_DSU_NUM_STRU *)MyNew(sizeof(DSU_DSU_NUM_STRU) * dsuIdmapLenStru->wDsuNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pDsuNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pDsuNumStru, 0xFF, sizeof(DSU_DSU_NUM_STRU) * dsuIdmapLenStru->wDsuNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_DSU_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wDsuNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_DSU_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->DSUNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wDsuNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pDsuNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DsuNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuDsuNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wDsuNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�߼�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initLogicSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_LOGICSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_LOGICSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wLogicSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			LogicSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->LOGICSECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuLogicSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->LOGICSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuLogicSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuLogicSecNumIndex, 0xFF, dsuIdmapIndexStru->LOGICSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pLogicSecNumStru = (DSU_LOGICSEC_NUM_STRU *)MyNew(sizeof(DSU_LOGICSEC_NUM_STRU) * dsuIdmapLenStru->wLogicSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pLogicSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pLogicSecNumStru, 0xFF, sizeof(DSU_LOGICSEC_NUM_STRU) * dsuIdmapLenStru->wLogicSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_LOGICSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wLogicSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_LOGICSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->LOGICSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wLogicSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pLogicSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			LogicSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuLogicSecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wLogicSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[AOM]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initAomSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AOMSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_AOMSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wAomSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			AomSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->AOMSECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuAomSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->AOMSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuAomSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuAomSecNumIndex, 0xFF, dsuIdmapIndexStru->AOMSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pAomSecNumStru = (DSU_AOMSEC_NUM_STRU *)MyNew(sizeof(DSU_AOMSEC_NUM_STRU) * dsuIdmapLenStru->wAomSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pAomSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pAomSecNumStru, 0xFF, sizeof(DSU_AOMSEC_NUM_STRU) * dsuIdmapLenStru->wAomSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_AOMSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wAomSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_AOMSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->AOMSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wAomSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pAomSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			AomSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuAomSecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wAomSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[SPKS]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSpksSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SPKSSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SPKSSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wSpksSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			SpksSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->SPKSSECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuSpksSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->SPKSSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuSpksSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuSpksSecNumIndex, 0xFF, dsuIdmapIndexStru->SPKSSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pSpksSecNumStru = (DSU_SPKSSEC_NUM_STRU *)MyNew(sizeof(DSU_SPKSSEC_NUM_STRU) * dsuIdmapLenStru->wSpksSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pSpksSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pSpksSecNumStru, 0xFF, sizeof(DSU_SPKSSEC_NUM_STRU) * dsuIdmapLenStru->wSpksSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_SPKSSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wSpksSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_SPKSSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->SPKSSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wSpksSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pSpksSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			SpksSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuSpksSecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wSpksSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initGodSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_GODSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_GODSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;


	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wGodSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			GodSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->GODSECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuGodSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->GODSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuGodSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuGodSecNumIndex, 0xFF, dsuIdmapIndexStru->GODSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pGodSecNumStru = (DSU_GODSEC_NUM_STRU *)MyNew(sizeof(DSU_GODSEC_NUM_STRU) * dsuIdmapLenStru->wGodSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pGodSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pGodSecNumStru, 0xFF, sizeof(DSU_GODSEC_NUM_STRU) * dsuIdmapLenStru->wGodSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_GODSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wGodSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_GODSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->GODSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wGodSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pGodSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			GodSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuGodSecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wGodSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��վ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStaSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STASEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STASEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;

	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wStaSecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			StaSecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->STASECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuStaSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->STASECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuStaSecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuStaSecNumIndex, 0xFF, dsuIdmapIndexStru->STASECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pStaSecNumStru = (DSU_STASEC_NUM_STRU *)MyNew(sizeof(DSU_STASEC_NUM_STRU) * dsuIdmapLenStru->wStaSecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pStaSecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pStaSecNumStru, 0xFF, sizeof(DSU_STASEC_NUM_STRU) * dsuIdmapLenStru->wStaSecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_STASEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wStaSecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_STASEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->STASECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wStaSecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pStaSecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			StaSecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuStaSecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wStaSecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPhySecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PHYSEC_NUM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_PHYSEC_NUM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen = 0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*��Ŷ��ձ�����*/
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru=NULL;
	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru=NULL;
	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru=NULL;


	if (NULL != g_dsuIdmapStru)
	{
		dsuIdmapIndexStru=g_dsuIdmapStru->dsuIdmapIndexStru;
		dsuStaticIdmapStru=g_dsuIdmapStru->dsuStaticIdmapStru;
		dsuIdmapLenStru=g_dsuIdmapStru->dsuIdmapLenStru;
	}
	else
	{
		/*����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x01))
	{		
		dsuIdmapLenStru->wPhySecNumLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			PhySecNumStru_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuIdmapIndexStru->PHYSECNUMINDEXSIZE = stru.wIndexId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsIdmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuIdmapIndexStru->dsuPhySecNumIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuIdmapIndexStru->PHYSECNUMINDEXSIZE + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuIdmapIndexStru->dsuPhySecNumIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuIdmapIndexStru->dsuPhySecNumIndex, 0xFF, dsuIdmapIndexStru->PHYSECNUMINDEXSIZE * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticIdmapStru->pPhySecNumStru = (DSU_PHYSEC_NUM_STRU *)MyNew(sizeof(DSU_PHYSEC_NUM_STRU) * dsuIdmapLenStru->wPhySecNumLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticIdmapStru->pPhySecNumStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticIdmapStru->pPhySecNumStru, 0xFF, sizeof(DSU_PHYSEC_NUM_STRU) * dsuIdmapLenStru->wPhySecNumLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*����Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetIdmapDataStruArrayLen(IDMAP_PHYSEC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuIdmapLenStru->wPhySecNumLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*����Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetIdmapDataIndexArrayLen(IDMAP_PHYSEC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuIdmapIndexStru->PHYSECNUMINDEXSIZE >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == IdmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ��ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuIdmapLenStru->wPhySecNumLen; j++)
		{
			pCurrentStru = dsuStaticIdmapStru->pPhySecNumStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			PhySecNumStru_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuIdmapIndexStru->dsuPhySecNumIndex[pCurrentStru->wIndexId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==IdmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuIdmapLenStru->wPhySecNumLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}


/*-----------------------------------------------�ṹ���������------------------------------------------------*/

/**************************************
�������ܣ�ZC��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pZCNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void ZCNumStru_init(UINT8 * pDataAdd,DSU_ZC_NUM_STRU* pZCNumStru)
{
	UINT8 * pData = pDataAdd;

	pZCNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pZCNumStru->dwZCDevId = LongFromChar(pData);  /*ZC�豸���*/
}


/**************************************
�������ܣ�CI��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pCINumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void CINumStru_init(UINT8 * pDataAdd,DSU_CI_NUM_STRU* pCINumStru)
{
	UINT8 * pData = pDataAdd;
	 /*�������*/
	pCINumStru->wIndexId = ShortFromChar(pData);
	pData += 2;

	 /*CI�豸���*/
	pCINumStru->dwCIDevId = LongFromChar(pData);
}


/**************************************
�������ܣ�ATS��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pATSNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void ATSNumStru_init(UINT8 * pDataAdd,DSU_ATS_NUM_STRU* pATSNumStru)
{
	UINT8 * pData = pDataAdd;

	pATSNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pATSNumStru->dwATSDevId = LongFromChar(pData); /*ATS�豸���*/
}


/**************************************
�������ܣ��źŻ���ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pSignalNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
******************************************/
void SignalNumStru_init(UINT8 * pDataAdd,DSU_SIGNAL_NUM_STRU* pSignalNumStru)
{
	UINT8 * pData = pDataAdd;

	pSignalNumStru->wIndexId = ShortFromChar(pData); /*�������*/
	pData += 2;

	pSignalNumStru->dwSigDevId = LongFromChar(pData); /*�źŻ��豸���*/
}


/**************************************
�������ܣ��������α�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void TrackSecNumStru_init(UINT8 * pDataAdd,DSU_TRACKSEC_NUM_STRU* pAxleSecNumStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 index = 0;

#if 0
	pAxleSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pAxleSecNumStru->dwTrackDevId = LongFromChar(pData);  /*��������豸���*/
	pData += 4;

	pAxleSecNumStru->wIncLinkCount = ShortFromChar(pData); / *����link����* /
	pData += 2;

	/ *����link���1~5* /
	for(index=0;index<5;index++)
	{
		pAxleSecNumStru->wIncLinkId[index] = ShortFromChar(pData);
		pData += 2;
	}

#endif

	pAxleSecNumStru->wLinkId = ShortFromChar(pData);  /*LINK�������*/
	pData += 2;

	pAxleSecNumStru->wTrackNum = ShortFromChar(pData); /*�����������*/
	pData += 2;

	for(index=0;index<15;index++)
	{
		pAxleSecNumStru->dwTrackSecId[index] = LongFromChar(pData);  /*������α��*/
		pData += 4;
		
		pAxleSecNumStru->dwTrackSecLen[index] = LongFromChar(pData);	/*������γ���*/
		pData += 4;
	}
}


/**************************************
�������ܣ������ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pSwitchNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void SwitchNumStru_init(UINT8 * pDataAdd,DSU_SWITCH_NUM_STRU* pSwitchNumStru)
{
	UINT8 * pData = pDataAdd;

	pSwitchNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pSwitchNumStru->dwSwiDevId = LongFromChar(pData);   /*�����豸���*/
}


/**************************************
�������ܣ�վ̨��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStationNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void StationNumStru_init(UINT8 * pDataAdd,DSU_STATION_NUM_STRU* pStationNumStru)
{
	UINT8 * pData = pDataAdd;

	pStationNumStru->wIndexId = ShortFromChar(pData);   /*�������*/
	pData += 2;

	pStationNumStru->dwStationId = LongFromChar(pData);  /*վ̨���*/
}


/**************************************
�������ܣ������رհ�ť��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pEmergBtnNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void EmergBtnNumStru_init(UINT8 * pDataAdd,DSU_EMERGBTN_NUM_STRU* pEmergBtnNumStru)
{
	UINT8 * pData = pDataAdd;

	pEmergBtnNumStru->wIndexId = ShortFromChar(pData);    /*�������*/
	pData += 2;

	pEmergBtnNumStru->dwEmergBtnId = LongFromChar(pData);  /*�����رհ�ť���*/
}


/**************************************
�������ܣ�Ӧ������ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pBaliseNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void BaliseNumStru_init(UINT8 * pDataAdd,DSU_BALISE_NUM_STRU* pBaliseNumStru)
{
	UINT8 * pData = pDataAdd;

	pBaliseNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pBaliseNumStru->wBaliseId = ShortFromChar(pData);  /*Ӧ�������*/
	pData += 2;

	pBaliseNumStru->wLindId = ShortFromChar(pData); /*������·���*/
}


/**************************************
�������ܣ������ű�ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pPSDoorNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void PSDoorNumStru_init(UINT8 * pDataAdd,DSU_PSD_NUM_STRU* pPSDoorNumStru)
{
	UINT8 * pData = pDataAdd;

	pPSDoorNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pPSDoorNumStru->dwPSDoorId = LongFromChar(pData); /*�����ű��*/
}


/**************************************
�������ܣ� VOBC��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pVOBCNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void VOBCNumStru_init(UINT8 * pDataAdd,DSU_VOBC_NUM_STRU* pVOBCNumStru)
{
	UINT8 * pData = pDataAdd;

	pVOBCNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pVOBCNumStru->dwVobcId[0] = LongFromChar(pData);   /*VOBC���1*/
	pData += 4;

	pVOBCNumStru->dwVobcId[1] = LongFromChar(pData);   /*VOBC���2*/
	pData += 4;
}


/**************************************
�������ܣ������ű�ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pFPDoorNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void FPDoorNumStru_init(UINT8 * pDataAdd,DSU_FPD_NUM_STRU* pFPDoorNumStru)
{
	UINT8 * pData = pDataAdd;

	pFPDoorNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pFPDoorNumStru->dwFPDoorId = LongFromChar(pData);  /*�����ű��*/
}

/**************************************
�������ܣ�DSU��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pDSUNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void DsuNumStru_init(UINT8 * pDataAdd, DSU_DSU_NUM_STRU* pDSUNumStru)
{
	UINT8 * pData = pDataAdd;

	pDSUNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pDSUNumStru->dwDsuId = LongFromChar(pData);  /*DSU���*/
}


/**************************************
�������ܣ��߼����α�ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pLogicSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void LogicSecNumStru_init(UINT8 * pDataAdd,DSU_LOGICSEC_NUM_STRU* pLogicSecNumStru)
{
	UINT8 * pData = pDataAdd;

	pLogicSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pLogicSecNumStru->dwLogicSecId = LongFromChar(pData);  /*�߼����α�� */
}

/**************************************
�������ܣ�AOM��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAomSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void AomSecNumStru_init(UINT8 * pDataAdd,DSU_AOMSEC_NUM_STRU* pAomSecNumStru)
{
	UINT8 * pData = pDataAdd;

	pAomSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pAomSecNumStru->dwAomSecId[0] = LongFromChar(pData);  /*AOM���1 */
	pData += 4;

	pAomSecNumStru->dwAomSecId[1] = LongFromChar(pData);  /*AOM���2 */
	pData += 4;
}

/**************************************
�������ܣ�SPKS��ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pSpksSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void SpksSecNumStru_init(UINT8 * pDataAdd,DSU_SPKSSEC_NUM_STRU* pSpksSecNumStru)
{
	UINT8 * pData = pDataAdd;

	pSpksSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pSpksSecNumStru->dwSpksSecId = LongFromChar(pData);  /*SPKS��� */
}

/**************************************
�������ܣ������ű�ű�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pGodSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void GodSecNumStru_init(UINT8 * pDataAdd,DSU_GODSEC_NUM_STRU* pGodSecNumStru)
{
	UINT8 * pData = pDataAdd;

	pGodSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pGodSecNumStru->dwGodSecId = LongFromChar(pData);  /*�����ű�� */
}

/**************************************
�������ܣ���վ��ű�ṹ���ʼ������  by slm 2019-1-15
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pStaSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void StaSecNumStru_init(UINT8 * pDataAdd, DSU_STASEC_NUM_STRU* pStaSecNumStru)
{
	UINT8 * pData = pDataAdd;

	pStaSecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pStaSecNumStru->dwStaSecId = LongFromChar(pData);  /*��վ��� */
}

/**************************************
�������ܣ��������α�ű�ṹ���ʼ������  by slm 2019-1-16
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pStaSecNumStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
void PhySecNumStru_init(UINT8 * pDataAdd, DSU_PHYSEC_NUM_STRU* pPhySecNumStru)
{
	UINT8 * pData = pDataAdd;

	pPhySecNumStru->wIndexId = ShortFromChar(pData);  /*�������*/
	pData += 2;

	pPhySecNumStru->dwPhySecId = LongFromChar(pData);  /*�������α�� */
}

/************************************************
�������ܣ���ȡ������
�����������
���������errRecord���������飬errRecordLen���鳤��
����ֵ��1�ɹ�
************************************************/
UINT8 dquGetIdMapErrNo(UINT8* errRecord, UINT32 * errRecordLen)
{
	UINT32 Ofst = 0;
	UINT32 i = 0;
	UINT8 rtnValue = 0;

	ShortToChar(fileErrNo, &errRecord[Ofst]);
	Ofst += 2;

	ShortToChar(dataTypeErrNo, &errRecord[Ofst]);
	Ofst += 2;

	for (i = 0; i < INIT_IDMAP_ERR_NUM; i++)
	{
		errRecord[Ofst] = initIdMapErrNo[i];
		Ofst++;
	}

	*errRecordLen = Ofst;
	rtnValue = 1;
	return rtnValue;
}