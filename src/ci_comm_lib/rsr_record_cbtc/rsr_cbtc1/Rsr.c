/** ******************************************************
*                                                                                                            
* �ļ���  �� Rsr.c   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.10.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rsr��Э��ʵ��  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
*	2009.12.16	¥��ΰ	��¼׷��
*   2011.12.06  ������  �޸�InitRpStruct�����������ͨ�ŵ���������
*   2011.12.06  ������  �޸�CheckRsrStruct���������ͼ��ʱ������ƽ̨��
*   2011.12.07  ������  ����RsrLnkDelete������ɾ���������·���ܣ��޸�RsspLnkDelete����
*   2011.12.13  ������  �޸�RsrOutput���������ӵ���·�Ͽ�ʱ��ɾ����·����
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsspIF.h"
#include "RpPlatform.h"
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "RsspCommon.h"
#include "Convert.h"
#include "sigInterface.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"
#include "RsspLnkNodeMgr.h"
#include "dsuRsrFunc.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSR���ڲ�ʹ�ú������� Start
 *--------------------------------------------------------------------*/
static UINT8 InitRpStruct(const RSR_INFO_STRU *pRsrInfoStru, RP_INFO_STRU *pRpInfoStru);
static UINT8 CheckRsrStruct(const RSR_INFO_STRU *pRsrInfoStru);
static void QueueCopy(QueueStruct *queFrom,QueueStruct *QueTo);
		

/*--------------------------------------------------------------------
 * RSR���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSR�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsrReceive
 * �������� : �Ժ������������ݽ���RSR��İ�ȫ��������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR��ͳһ�ṹ��
 *											������У�pRsrStruct->OutnetQueueA				
 *													 pRsrStruct->OutnetQueueB
 *											������У�pRsrStruct->DataToApp
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsrReceive(RSR_INFO_STRU *pRsrStruct)
{
	UINT16 LocalName;
	UINT8 ret = 0;

	if(pRsrStruct == NULL)	/* �ǿռ�� */
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ����������� */
	}

	
	TypeId2DevName(pRsrStruct->LocalType,pRsrStruct->LocalID,&LocalName);		/* ��ʶ = Type + ID */
	
	/* ���������ݿ����� �����ͳһ�ṹ���� */
	QueueCopy(&pRsrStruct->OutnetQueueA,&pRsrStruct->RpInfoStru.OutnetQueueA);
	QueueCopy(&pRsrStruct->OutnetQueueB,&pRsrStruct->RpInfoStru.OutnetQueueB);

	/* ��¼���� */	
	ShortToChar(0,pRsrStruct->RecordArray);
	
	/* ��������봦�� */
	ret = RpReceive(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ��¼ */
		return 0;		
	}
	else
	{
		/* �������� */
	}

	/* �źŲ����봦�� */
	ret = SigReceive(LocalName,&pRsrStruct->RpInfoStru.DataToApp,&pRsrStruct->QueSig2Rs);
	if(0 == ret)
	{
		/* ��¼ */
		return 0;
	}
	else
	{
		/* �������� */
	}

	/* RSSP�����봦�� */
	ret = RsspReceive(&pRsrStruct->QueSig2Rs, &pRsrStruct->DataToApp,pRsrStruct);
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
 * ������   : RsrOutput
 * �������� : ��Ӧ��������ݽ���RSR��İ�ȫ���������
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR��ͳһ�ṹ��
 *											������У�pRsrStruct->OutputDataQueue
 *											������У�pRsrStruct->AppArray
 *													 pRsrStruct->VarArray
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsrOutput(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT16 LocalName = 0;		/* ������ʶ�� */
	UINT16 DataLen = 0;			/* ������ݵĳ��� */
	UINT16 RsspVarLen = 0;		/* �������鳤�� */
    UINT8 index = 0;
	UINT16 devName = 0;
	Lnk_Info_Node_Stru* lnkNodeArr = NULL;

	UINT8 lnkNodeNum = 0;	/* �ڵ��� */ 

	if(pRsrStruct == NULL)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	TypeId2DevName(pRsrStruct->LocalType, pRsrStruct->LocalID, &LocalName);

	/* RSSP��������� */
	ret = RsspOutput(&pRsrStruct->OutputDataQueue,&pRsrStruct->QueSig2Rs,pRsrStruct->VarArray+2,pRsrStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �źŲ�������� */
	ret = SigOutput(LocalName,&pRsrStruct->QueSig2Rs, &pRsrStruct->RpInfoStru.OutputDataQueue);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ������������ */
	ret = RpOutput(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{		   
		/* ��¼ */
		return ret;
	}
	else
	{
		/* wangpeipei 20111213 Mod S */
		/* ����·�Ͽ�ʱ�������ɾ����· */
        GetLnkNodeArr(&lnkNodeArr,&lnkNodeNum,pRsrStruct);
		for(index = 0;index < lnkNodeNum;index++)
		{
			/* ���ýڵ�Ϊ�ɸ��ǽڵ㣬���ʼ������ͳһ�ṹ�� */
			if((lnkNodeArr[index].NodeStatus == Replace) && (lnkNodeArr[index].DestDevName == 0))
			{
				devName = lnkNodeArr[index].DestDevName;
				RpLnkDelete(devName,&pRsrStruct->RpInfoStru);
			}
			else
			{
				/* Nothing */
			}
		}
        /* wangpeipei 20111213 Mod E */
		/* ������� RSR��ͳһ�ṹ��.AppArray = �����ͳһ�ṹ��.AppArray */
		pRsrStruct->AppArray = pRsrStruct->RpInfoStru.AppArray;

		/* 
		 * �м�������鿽��
		 *  ------------------------------------------------------
		 *  �ܳ��� | RSSP�㳤�� | RSSP�м���� | ������м����	  |
		 *   2 �ֽ�|   2�ֽ�    | 
		 *  -------------------------------------------------------
		 * Rssp�㳤���Լ�RSSP���м������RSSP����������趨��
		 *
		 */
		RsspVarLen = ShortFromChar(pRsrStruct->VarArray+2);				/* Rssp���м�������� */
		DataLen = ShortFromChar(pRsrStruct->RpInfoStru.VarArray);		/* ����㳤�� */
		/* 
		 * �����ṹ���VarArray[2]��ʼ����������м������
		 * Rsr��ͳһ�ṹ���VarArray+4+Rssp���м�������Ⱥ��λ��
		 */
		RsrMemCopy(pRsrStruct->RpInfoStru.VarArray+2,
				pRsrStruct->VarArray+4+RsspVarLen,DataLen);	
		
		/* �ܳ��� */
		ShortToChar((UINT16)(RsspVarLen+DataLen+2),pRsrStruct->VarArray);					
		
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RsrInit
 * �������� : ͨ���ú�����ʵ�ֶ�RSR��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR��ͳһ�ṹ�壬����г�ʼֵ���á�
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsrInit(UINT8 *fileName,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT32 tmp = 0;
	UINT16 MaxMsgNum = 0;
	UINT16 InputSize = 0, OutputSize = 0;

	/* ������ */
	ret = CheckRsrStruct(pRsrStruct);
	if(0 == ret)
	{
		return ret;
	}

	InputSize = pRsrStruct->InputSize;
	OutputSize = pRsrStruct->OutputSize;

	/* ��DSU���ѯ�������г�ʼ������ */
	ret = dsuRsrInit(fileName);
	if( 0 == ret )
	{
		/* DSU��ѯ�������ʼ��ʧ�� */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �����ͳһ�ṹ���ʼ�� */
	ret = InitRpStruct(pRsrStruct,&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ����ʧ�� ʲô���� ���� */
		return ret;
	}
	else
	{
		/* ���� */
	}
	
	/* ������ʼ�� */
	ret = RpInit(fileName,&pRsrStruct->RpInfoStru);
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
	ret = RsspInit(pRsrStruct);
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
	MaxMsgNum = pRsrStruct->MaxNumLink *(pRsrStruct->MaxNumPerCycle + 2);		/* ������� */
	
	/* RSR�����ģ���������:ָ���������������ռ� */
	pRsrStruct->AppArraySize = pRsrStruct->RpInfoStru.AppArraySize;
	pRsrStruct->AppArray = pRsrStruct->RpInfoStru.AppArray;

	/* RSR�����ģ������м���������С */
	pRsrStruct->VarArraySize = pRsrStruct->RsspVarArraySize + pRsrStruct->RpInfoStru.VarArraySize + 32; 
	pRsrStruct->VarArray = (UINT8*)malloc(pRsrStruct->VarArraySize);
	if( NULL == pRsrStruct->VarArray)
	{
		/* ��¼ */
		return 0;
	}
	else
	{
		/* ʲô���� */
		CommonMemSet(pRsrStruct->VarArray,sizeof(UINT8)*pRsrStruct->VarArraySize,0,sizeof(UINT8)*pRsrStruct->VarArraySize);
	}

	/* Э�������Ӧ�õ����ݶ��г��� */
	pRsrStruct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* Э�������Ӧ�õ����ݶ��г��� */
	ret = QueueInitial(&pRsrStruct->DataToApp,pRsrStruct->DataToAppSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* Ӧ�ø�Э����������ݶ��г��� */
	pRsrStruct->OutputDataQueueSize= (OutputSize + 8 )*MaxMsgNum;
	ret = QueueInitial(&pRsrStruct->OutputDataQueue,pRsrStruct->OutputDataQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������ܶ��г��� */
	pRsrStruct->OutnetQueueSize = pRsrStruct->RpInfoStru.OutnetQueueSize;	/* �������Ľ��ܶ��г���һ�� */
	/* �������� */
	ret = QueueInitial(&pRsrStruct->OutnetQueueA,pRsrStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������� */
	ret = QueueInitial(&pRsrStruct->OutnetQueueB,pRsrStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ��ʱ���г��� */ 
	tmp = pRsrStruct->OutnetQueueSize > pRsrStruct->AppArraySize ? pRsrStruct->OutnetQueueSize:pRsrStruct->AppArraySize;
	ret = QueueInitial(&pRsrStruct->QueSig2Rs,tmp);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* ʲô���� */
	}
	 /*  ��¼����  */
	pRsrStruct->RecordArray = pRsrStruct->RpInfoStru.RecordArray;

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RsrFreeSpace
 * �������� : ͨ���ú������ͷ�RSR�㿪�ٵĿռ䡣
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  0�� ���󷵻� 1����������
 ***********************************************************************/
UINT8 RsrFreeSpace(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	if(pRsrStruct == NULL)
	{
		return ret;
	}

	/* �����ռ��ͷ� */
	ret = RpFreeSpace(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* ����ʧ�� */
		return ret;
	}
	else
	{
		/* ����㴦��ɹ�  */		
	}

	/* RSSP��ռ��ͷ� */
	ret = RsspFreeSpace(pRsrStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ��������ռ��ͷ� */
		
	/* RSR�����ģ���������:ָ���������������ռ�  ��������ͷ� */
	pRsrStruct->AppArray = NULL;

	/* RSR�����ģ������м��������ռ��ͷ� */
	if( NULL != pRsrStruct->VarArray)
	{
		free(pRsrStruct->VarArray);
		pRsrStruct->VarArray = NULL;
	}
	else
	{
		/* ʲô���� */
	}

	/* Э�������Ӧ�õ����ݶ��г��� */	
	ret = QueueElementFree(&pRsrStruct->DataToApp);

	/* Ӧ�ø�Э����������ݶ��г��� */
	ret = QueueElementFree(&pRsrStruct->OutputDataQueue);
	
	/* �������ܶ��г��� */
	/* �������� */
	ret = QueueElementFree(&pRsrStruct->OutnetQueueA);

	/* �������� */
	ret = QueueElementFree(&pRsrStruct->OutnetQueueB);

	/* ��ʱ���г��� */ 	
	ret = QueueElementFree(&pRsrStruct->QueSig2Rs);

	 /*  ��¼����  ����������ͷţ�������㹲���ڴ�ָ�� */
	pRsrStruct->RecordArray = NULL;

	ret = 1;
	return ret;
}


/***********************************************************************
 * ������   : RsrLnkStatus
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� : 
 *	������			����			�������		����
 *  -------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSPԴ��ַ
 *  BsDAdd			INT16			IN				BSD��ַ
 *  SeDAdd			UINT16			IN				RSSPĿ���ַ��SSE��SSR��ַ��
 * ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���;
 *										0x17����·�����ݴ�������״̬DATA
 *	  								0x2b����·���޷���������״̬HALT
 ***********************************************************************/
UINT8 RsrLnkStatus(UINT16 RsSAdd,INT16 BsDAdd,UINT16 SeDAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	ret = RsspLnkStatus(RsSAdd,BsDAdd,SeDAdd,pRsrStruct);
	return ret;
}

/***********************************************************************
 * ������   : RsrLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������			����		�������		����
 *  --------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSPԴ��ַ
 *  BsDAdd			INT16			IN				BSD��ַ
 *  SeDAdd			UINT16			IN				RSSPĿ���ַ��SSE��SSR��ַ��
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
UINT8 RsrLnkDelete(UINT16 RsSAdd,INT16 BsDAdd,UINT16 SeDAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;
	UINT16 devName = 0;
	/* wangpeipei 20111207 ADD S */

	ret = RsspLnkDelete(RsSAdd,BsDAdd,SeDAdd,&devName,pRsrStruct);
	
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/* �޲��� */
	}

	/* ɾ���������· */
	ret = RpLnkDelete(devName,&pRsrStruct->RpInfoStru);
	/* wangpeipei 20111207 ADD S */
	return ret;
}

/***********************************************************************
 * ������   : RsrRefresh
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSR����м�������¸�ֵ��
 * ������� : 
 *	������					����		�������		����
 *  --------------------------------------------------------------
 *	sLen_Input  			UINT16			IN			�м�����Ĵ�С
 *	Input_Data_LnkMes		UINT8			IN			�м��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
UINT8 RsrRefresh(UINT16 sLen_Input, UINT8* Input_Data_LnkMes,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT16 RsspVarLen = 0;	/* Rssp���м�������� */
	UINT16 RpVarLen = 0;	/* ������м�������� */
	
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
	ret = RsspRefresh(RsspVarLen,Input_Data_LnkMes+2, pRsrStru);
	if(0 == ret)
	{
		/* ��¼ */
		return ret; 
	} 
	else
	{
		/* ʲô���� */
	}
	
	/* �����ˢ�� */
	RpVarLen = sLen_Input - RsspVarLen-2;		/* ������м�������� */
	ret = RpRefresh(RpVarLen,
			Input_Data_LnkMes+2+RsspVarLen,&pRsrStru->RpInfoStru);
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

/*--------------------------------------------------------------------
 * RSR�����ӿں������� End
 *--------------------------------------------------------------------*/
 

/*--------------------------------------------------------------------
 * RSR���ڲ�ʹ�ú������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : InitRpStruct
 * �������� : �������ͳһ�ṹ����г�ʼ������
 * ������� : 
 *	������			����					�������	����
 *  --------------------------------------------------------------
 *	pRsrInfoStru  	const RSR_INFO_STRU*	IN			RSR��ͳһ�ṹ��
 *	pRpInfoStru		RP_INFO_STRU *			IN			�����ͳһ�ṹ��
 * 
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/

static UINT8 InitRpStruct(const RSR_INFO_STRU *pRsrInfoStru, RP_INFO_STRU *pRpInfoStru)
{
	UINT8 DestType = 0,DestDevNum = 0;						/* �Է��豸�࣬�Է��豸���� */
	UINT8 ret = 0;
	pRpInfoStru->LocalID = pRsrInfoStru->LocalID;			/* �����豸ID */
	pRpInfoStru->LocalType = pRsrInfoStru->LocalType;		/* �����豸Type */

	pRpInfoStru->MaxNumPerCycle = pRsrInfoStru->MaxNumPerCycle;		/* ������� */
	pRpInfoStru->RecordArraySize = pRsrInfoStru->RecordArraySize;	/* ����¼�� */

	/* wangpeipei 20111205 MOD S */
	/* �ô�С��ʾ�뱾����ͨ�ŵ���������������ָ���������ͨ�Ŷ���
	 * �ɳ�վ�ͳ���׷�ӡ��ڴ��˷ѱȽ����ء�����VOBCʱ����N����վ����ͨ�����󣬵�һ��ʱ��ֻ����һ��ͨ�ţ�
	 * ���������˷ѡ���˸ĳ�ͬʱͨ�ŵ��������������˷ѡ�ͬʱ�������չ��
	 * �����޸ĺ���Ҫ�����Ҳ�ṩɾ����·����(��������кű�����ʹ��)
	 */
	pRpInfoStru->MaxNumLink = pRsrInfoStru->MaxNumLink;	  
	/* wangpeipei 20111205 MOD E */

	/* ��������������� */
	pRpInfoStru->InputSize = pRsrInfoStru->InputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;		
	/* �������������� */	
	pRpInfoStru->OutputSize = pRsrInfoStru->OutputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;

	ret = 1;
	return ret;
}
 /*
  *	��ͳһ�ṹ��ĸ�����Ŀ���м�顣
  */
static UINT8 CheckRsrStruct(const RSR_INFO_STRU *pRsrInfoStru)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP_MSG -RSSP_FIXED_LEN;		/* ���Ӧ�����ݴ�С��1400-22 */

	/* ͳһ�ṹ��ָ�벻Ϊ�� */
	if(pRsrInfoStru == NULL)
	{
		return ret;
	}

	/* ���ͼ�� */
	
	/* wangpeipei 20111205 Del S */
	/*
	 * ɾ�����ͼ�顣
	 */
	/* wangpeipei 20111205 Del E */

	/* ������Ŀ��� */
	if( (pRsrInfoStru->MaxNumLink == 0) || (pRsrInfoStru->MaxNumPerCycle == 0))
	{
		return ret;
	}
	
	/* �������Ӧ������ */
	if( (0 == pRsrInfoStru->InputSize) || (pRsrInfoStru->InputSize > MaxDataLen))
	{		
		return ret;
	}

	/* ������Ӧ������ */
	if( (0 == pRsrInfoStru->OutputSize ) || (pRsrInfoStru->OutputSize > MaxDataLen))
	{	
		return ret;
	}

	/* ���ں� */
	if( pRsrInfoStru->CycleNum == NULL)
	{
		return ret;
	}

	ret = 1;
	return ret;
}

 /*
  *	�Զ��н��п�����
  */
static void QueueCopy(QueueStruct *QueFrom,QueueStruct *QueTo)
{
	UINT8 data[1400] = {0};
	UINT32 Len = 0;
	Len =QueueStatus(QueFrom);
	while(Len > 0)
	{
		if( Len > 1400 )
		{
			QueueRead(1400,data,QueFrom);
			QueueWrite(1400,data,QueTo);
			Len = Len-1400;
		}
		else
		{
			QueueRead(Len,data,QueFrom);
			QueueWrite(Len,data,QueTo);
			Len = 0;
		}
	}
}


/*--------------------------------------------------------------------
 * RSR���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
