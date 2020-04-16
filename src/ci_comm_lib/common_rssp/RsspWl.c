/** ******************************************************
*                                                                                                            
* �ļ���  �� RsspWl.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2013.2.5
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp��Э��ʵ��  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*  2009.12.07	¥��ΰ	��������
* 2013.02.07  ������  ���Ӻ��������Լ����Ӧ��RSSP�ṹ��
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsspIFWl.h"
#include "RedunPlatform.h"
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "Convert.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"
#include "RsspLnkNodeMgrWl.h"
#include "dsuRsspFunc.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP���ڲ�ʹ�ú������� Start
 *--------------------------------------------------------------------*/

static UINT8 CheckRsspStruct_WL(const RSSP_INFO_STRU *pRsspStruct);
static UINT8 InitRedunStruct_WL(const RSSP_INFO_STRU *pRsspStruct, REDUN_INFO_STRU *pRedunInfoStru);		

/*--------------------------------------------------------------------
 * RSSP���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : Rssp2Receive
 * �������� : �Ժ������������ݽ���RSSP��İ�ȫ��������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP��ͳһ�ṹ��
 *											������У�pRsspStruct->OutnetQueueA				
 *													 pRsspStruct->OutnetQueueB
 *											������У�pRsspStruct->DataToApp
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 Rssp2Receive(RSSP_INFO_STRU *pRsspStruct)
{
	UINT16 LocalName;
	UINT8 ret = 0;

	if(pRsspStruct == NULL)	/* �ǿռ�� */
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ����������� */
	}

	
	TypeId2DevName_WL(pRsspStruct->LocalType,pRsspStruct->LocalID,&LocalName);		/* ��ʶ = Type + ID */

	/* ��¼���� */	
	ShortToChar(0,pRsspStruct->RecordArray);

	PrintRecArrayByte(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, 0xEE);			/* �����ڿ�ʼ��0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, *pRsspStruct->CycleNum);	/* ���ں� */ /*M_2010_05_10 */

	/*������մ���*/
	ret = RedunReceive(&pRsspStruct->OutnetQueueA, &pRsspStruct->OutnetQueueB, &pRsspStruct->RpInfoStru, &pRsspStruct->QueSig2Rs, pRsspStruct->RecordArray, pRsspStruct->RecordArraySize);
	/* wangpeipei 20120223 Mod E */
	if(0 == ret)
	{
		/* ��¼ */
		return 0;		
	}
	else
	{
		/* �������� */
	}

	/* RSSP���봦�� */
	ret = RsspReceive_WL(&pRsspStruct->QueSig2Rs, &pRsspStruct->DataToApp,pRsspStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return 0;
	}
	else
	{
		/* �������� */
	}
	
	return ret;
}

/***********************************************************************
 * ������   : Rssp2Output
 * �������� : ��Ӧ��������ݽ���RSSP��İ�ȫ���������
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP��ͳһ�ṹ��
 *											������У�pRsspStruct->OutputDataQueue
 *											������У�pRsspStruct->AppArray
 *													 pRsspStruct->VarArray
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 Rssp2Output(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 LocalName = 0;		/* ������ʶ�� */
	UINT16 DataLen = 0;			/* ������ݵĳ��� */
	UINT16 RsspVarLen = 0;		/* �������鳤�� */
    UINT8 index = 0;
	UINT16 devName = 0;
	Lnk_Info_Node_Stru_WL* lnkNodeArr = NULL;

	UINT8 lnkNodeNum = 0;	/* �ڵ��� */ 

	if(pRsspStruct == NULL)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	TypeId2DevName_WL(pRsspStruct->LocalType, pRsspStruct->LocalID, &LocalName);

	/* RSSP��������� */
	ret = RsspOutput_WL(&pRsspStruct->OutputDataQueue,&pRsspStruct->QueSig2Rs,pRsspStruct->VarArray+2,pRsspStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
   /*wangpeipei 20120223 Mod S */

	/* ������������ */
	ret = RedunOutput(&pRsspStruct->RpInfoStru,&pRsspStruct->QueSig2Rs);
	/* angpeipei 20120223 Mod E */
	if(0 == ret)
	{		   
		/* ��¼ */
		return ret;
	}
	else
	{
		/* wangpeipei 20111213 Mod S */
		/* ����·�Ͽ�ʱ������ɾ����· */
        GetLnkNodeArr_WL(&lnkNodeArr,&lnkNodeNum,pRsspStruct);
		for(index = 0;index < lnkNodeNum;index++)
		{
			/* ���ýڵ�Ϊ�ɸ��ǽڵ㣬���ʼ������ͳһ�ṹ�� */
			if((lnkNodeArr[index].NodeStatus == Replace_WL) && (lnkNodeArr[index].DestDevName != 0 ))
			{
				devName = lnkNodeArr[index].DestDevName;
				RedunLnkDelete(devName,&pRsspStruct->RpInfoStru);
			}
			else
			{
				/* Nothing */
			}
		}
        /* wangpeipei 20111213 Mod E */
		/* ������� RSSP��ͳһ�ṹ��.AppArray = ����ͳһ�ṹ��.AppArray */
		pRsspStruct->AppArray = pRsspStruct->RpInfoStru.AppArray;

		/* 
		 * �м�������鿽��
		 *  ------------------------------------------------------
		 *  �ܳ��� | RSSP�㳤�� | RSSP�м���� | �����м����	  |
		 *   2 �ֽ�|   2�ֽ�    | 
		 *  -------------------------------------------------------
		 * Rssp�㳤���Լ�RSSP���м������RSSP����������趨��
		 *
		 */
		RsspVarLen = ShortFromChar(pRsspStruct->VarArray+2);				/* Rssp���м�������� */
		DataLen = ShortFromChar(pRsspStruct->RpInfoStru.VarArray);		/* ����㳤�� */
		/* 
		 * �����ṹ���VarArray[2]��ʼ����������м������
		 * Rsr��ͳһ�ṹ���VarArray+4+Rssp���м�������Ⱥ��λ��
		 */
		RsspMemCopy_WL(pRsspStruct->RpInfoStru.VarArray+2,
				pRsspStruct->VarArray+4+RsspVarLen,DataLen);	
		
		/* �ܳ��� */
		ShortToChar((UINT16)(RsspVarLen+DataLen+2),pRsspStruct->VarArray);					
		
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : Rssp2Init
 * �������� : ͨ���ú�����ʵ�ֶ�RSSP��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSPͳһ�ṹ�壬����г�ʼֵ���á�

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 Rssp2Init(UINT8 *fileName,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT32 tmp = 0;
	UINT16 MaxMsgNum = 0;
	UINT16 InputSize = 0, OutputSize = 0;

	/* ������ */
	ret = CheckRsspStruct_WL(pRsspStruct);
	if(0 == ret)
	{
		return ret;
	}

	InputSize = pRsspStruct->InputSize;
	OutputSize = pRsspStruct->OutputSize;

	/* ��DSU���ѯ�������г�ʼ������ */
	ret = dsuRsspInitWl(fileName);
	if( 0 == ret )
	{
		/* DSU��ѯ�������ʼ��ʧ�� */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ����ͳһ�ṹ���ʼ�� */
	ret = InitRedunStruct_WL(pRsspStruct,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ����ʧ�� ʲô���� ���� */
		return ret;
	}
	else
	{
		/* ���� */
	}
	
	/* �����ʼ�� */
	ret = RedunInit(fileName,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ��ʼ��ʧ�� */
		return ret;
	}
	else
	{
		/* ����㴦��ɹ�  */		
	}

	/* RSSP���ʼ�� */
	ret = RsspInit_WL(pRsspStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ���������ʼ�� */
	MaxMsgNum = pRsspStruct->MaxNumLink *(pRsspStruct->MaxNumPerCycle + 2);		/* ������� */
	
	/* RSR�����ģ���������:ָ���������������ռ� */
	pRsspStruct->AppArraySize = pRsspStruct->RpInfoStru.AppArraySize;
	pRsspStruct->AppArray = pRsspStruct->RpInfoStru.AppArray;

	/* RSR�����ģ������м���������С */
	pRsspStruct->VarArraySize = pRsspStruct->RsspVarArraySize + pRsspStruct->RpInfoStru.VarArraySize + 32; 
	pRsspStruct->VarArray = (UINT8*)malloc(pRsspStruct->VarArraySize);
	if( NULL == pRsspStruct->VarArray)
	{
		/* ��¼ */
		return 0;
	}
	else
	{
		/* ʲô���� */
		CommonMemSet(pRsspStruct->VarArray,sizeof(UINT8)*pRsspStruct->VarArraySize,0,sizeof(UINT8)*pRsspStruct->VarArraySize);
	}

	/* Э�������Ӧ�õ����ݶ��г��� */
	pRsspStruct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* Э�������Ӧ�õ����ݶ��г��� */
	ret = QueueInitial(&pRsspStruct->DataToApp,pRsspStruct->DataToAppSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* Ӧ�ø�Э����������ݶ��г��� */
	pRsspStruct->OutputDataQueueSize= (OutputSize + 8 )*MaxMsgNum;
	ret = QueueInitial(&pRsspStruct->OutputDataQueue,pRsspStruct->OutputDataQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������ܶ��г��� */

	pRsspStruct->OutnetQueueSize = pRsspStruct->RpInfoStru.OutnetQueueSize;	/* �������Ľ��ܶ��г���һ�� */
	/* �������� */
	ret = QueueInitial(&pRsspStruct->OutnetQueueA,pRsspStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������� */
	ret = QueueInitial(&pRsspStruct->OutnetQueueB,pRsspStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ��ʱ���г��� */ 
	tmp = pRsspStruct->OutnetQueueSize > pRsspStruct->AppArraySize ? pRsspStruct->OutnetQueueSize:pRsspStruct->AppArraySize;
	ret = QueueInitial(&pRsspStruct->QueSig2Rs,tmp);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	 /*  ��¼����  */
	pRsspStruct->RecordArray = pRsspStruct->RpInfoStru.RecordArray;

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RsrFreeSpace
 * �������� : ͨ���ú������ͷ�RSR�㿪�ٵĿռ䡣
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  0�� ���󷵻� 1����������
 ***********************************************************************/
UINT8 Rssp2FreeSpace(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;

	if(pRsspStruct == NULL)
	{
		ret=0;
	}
	else
	{
		/* �����ռ��ͷ� */
		ret = RedunFreeSpace(&pRsspStruct->RpInfoStru);

		/* RSSP��ռ��ͷ� */
		ret = ret && (RsspFreeSpace_WL(pRsspStruct));
		
		/* ��������ռ��ͷ� */
			
		/* RSR�����ģ���������:ָ���������������ռ�  ��������ͷ� */
		pRsspStruct->AppArray = NULL;

		/* RSR�����ģ������м��������ռ��ͷ� */
		if( NULL != pRsspStruct->VarArray)
		{
			free(pRsspStruct->VarArray);
			pRsspStruct->VarArray = NULL;
		}
		else
		{
			/* ʲô���� */
		}

		/* Э�������Ӧ�õ����ݶ��г��� */	
		QueueElementFree(&pRsspStruct->DataToApp);

		/* Ӧ�ø�Э����������ݶ��г��� */
		QueueElementFree(&pRsspStruct->OutputDataQueue);

		/* �������ܶ��г��� */
		/* �������� */
		QueueElementFree(&pRsspStruct->OutnetQueueA);

		/* �������� */
		QueueElementFree(&pRsspStruct->OutnetQueueB);

		/* ��ʱ���г��� */ 	
		QueueElementFree(&pRsspStruct->QueSig2Rs);

		 /*  ��¼����  ����������ͷţ�������㹲���ڴ�ָ�� */
		pRsspStruct->RecordArray = NULL;
	}
	return ret;
}
/***********************************************************************
 * ������   : Rssp2LnkStatus
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� : 
 *	������			����			�������		����
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			Ŀ������
 *  destId			UINT8			IN			    Ŀ��ID
 *  pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���;
 *										0x17����·�����ݴ�������״̬DATA
 *	  								0x2b����·���޷���������״̬HALT
 ***********************************************************************/
UINT8 Rssp2LnkStatus(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;

	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* RSPP ��·���� */
	ret = RsspLnkStatus_WL(destType,destId,pRsspStruct);
	return ret;
}

/***********************************************************************
 * ������   : RsrLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������			����		�������		����
 *  --------------------------------------------------------------
 *	destType  		UINT8			IN  			Ŀ������
 *  destId			UINT8			IN			    Ŀ��ID
 *  pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
UINT8 Rssp2LnkDelete(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;
	UINT16 devName = 0;
    TypeId2DevName_WL(destType,destId, &devName);
	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* ɾ��RSSP����· */
	ret = RsspLnkDelete_WL(devName,pRsspStruct);	
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/* �޲��� */
	}

	/* ɾ�����ദ����· */
	ret = RedunLnkDelete(devName,&pRsspStruct->RpInfoStru);

	return ret;
}

/***********************************************************************
 * ������   : Rssp2Refresh
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSSP����м�������¸�ֵ��
 * ������� : 
 *	������					����		�������		����
 *  --------------------------------------------------------------
 *	sLen_Input  			UINT16			IN			�м�����Ĵ�С
 *	Input_Data_LnkMes		UINT8			IN			�м��������
 * 
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
UINT8 Rssp2Refresh(UINT16 sLen_Input, UINT8* Input_Data_LnkMes,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 RsspVarLen = 0;	/* Rssp���м�������� */
	UINT16 RpVarLen = 0;	/* ������м�������� */
	
	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* �ǿռ�� */
	if(NULL == Input_Data_LnkMes)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	RsspVarLen = ShortFromChar(Input_Data_LnkMes);

	/* ���ȼ�� */
	if((RsspVarLen+2) > sLen_Input)
	{
		/* ���Ȳ��Ϸ� */
		return ret;
	}
	else
	{
		/* ����ʲô���� */
	}

	/* 
	 * Input_Data_LnkMes��ʽ
	 *  ---------------------------------------------------
	 *  | RSSP�㳤�� | RSSP�м���� | ������м����	  |
	 *  |   2�ֽ�    |		N   	|   M                 |
	 *  ---------------------------------------------------
	 *
	 */

	/* Rssp���м����ˢ�� */
	ret = RsspRefresh_WL(RsspVarLen,Input_Data_LnkMes+2, pRsspStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret; 
	} 
	else
	{
		/* ʲô���� */
	}
	
	/* ����ˢ�� */
	RpVarLen = sLen_Input - RsspVarLen-2;		/* ������м�������� */
	ret = RedunRefresh(RpVarLen,
			Input_Data_LnkMes+2+RsspVarLen,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	ret = 1;
	return ret;
}


/***********************************************************************
 * ������   : InitRedunStruct_WL
 * �������� : ������ͳһ�ṹ����г�ʼ������
 * ������� : 
 *	������			����					�������	����
 *  --------------------------------------------------------------
 *	pRsspInfoStru  	const RSSP_INFO_STRU*	IN			RSSP��ͳһ�ṹ��
 *	pRpInfoStru		REDUN_INFO_STRU *			IN			�����ͳһ�ṹ��
 * 
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/

static UINT8 InitRedunStruct_WL(const RSSP_INFO_STRU *pRsspStruct, REDUN_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 0;
	pRedunInfoStru->LocalID = pRsspStruct->LocalID;			/* �����豸ID */
	pRedunInfoStru->LocalType = pRsspStruct->LocalType;		/* �����豸Type */

	pRedunInfoStru->MaxNumPerCycle = pRsspStruct->MaxNumPerCycle;		/* ������� */
	pRedunInfoStru->RecordArraySize = pRsspStruct->RecordArraySize;	/* ����¼�� */

	pRedunInfoStru->MaxNumLink = pRsspStruct->MaxNumLink;	  
	/* wangpeipei 20111205 MOD E */

	/* ��������������� */
	/* wangpeipei 20120223 Mod S*/
	/*pRpInfoStru->InputSize = pRsrInfoStru->InputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;*/	
    pRedunInfoStru->InputSize = pRsspStruct->InputSize + RSSP_FIXED_LEN_WL;
	/* �������������� */	
	/*pRpInfoStru->OutputSize = pRsrInfoStru->OutputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;*/
    pRedunInfoStru->OutputSize = pRsspStruct->OutputSize + RSSP_FIXED_LEN_WL;
	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : CheckRsspStruct_WL
 * �������� : ��ͳһ�ṹ��ĸ�����Ŀ���м��
 * ������� : 
 *	������			����					�������	����
 *  --------------------------------------------------------------
 *	pRsspStruct  	const RSSP_INFO_STRU*	IN			RSSP��ͳһ�ṹ��
 * 
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
static UINT8 CheckRsspStruct_WL(const RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP_MSG_WL -RSSP_FIXED_LEN_WL;		/* ���Ӧ�����ݴ�С��1400-22 */

	/* ͳһ�ṹ��ָ�벻Ϊ�� */
	if(pRsspStruct == NULL)
	{
		return ret;
	}

	/* ������Ŀ��� */
	if( (pRsspStruct->MaxNumLink == 0) || (pRsspStruct->MaxNumPerCycle == 0))
	{
		return ret;
	}
	
	/* �������Ӧ������ */
	if( (0 == pRsspStruct->InputSize) || (pRsspStruct->InputSize > MaxDataLen))
	{		
		return ret;
	}

	/* ������Ӧ������ */
	if( (0 == pRsspStruct->OutputSize ) || (pRsspStruct->OutputSize > MaxDataLen))
	{	
		return ret;
	}

	/* ���ں� */
	if( pRsspStruct->CycleNum == NULL)
	{
		return ret;
	}

	ret = 1;
	return ret;
}
/*--------------------------------------------------------------------
 * RSSP���ⲿʹ�ú������� End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
