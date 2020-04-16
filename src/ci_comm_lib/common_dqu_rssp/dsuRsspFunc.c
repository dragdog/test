/*******************************************************************************
*
* �ļ���  ��dsuRsspFunc.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������RSRЭ����ص�dsu��ѯ����������
* ʹ��ע�⣺��
* �޸ļ�¼����
***	2013.02.15	������	ɾ��dsuRsspRGetLnkInfo������dsuRsspRGetLnkInfoWl
**	2013.02.15	������	ɾ��dsuRsspSGetLnkInfo������dsuRsspSGetLnkInfoWl
**	2013.02.15	������	����dsuInputdevvsOutputDevOutInfoWl
**	2013.02.15	������	����dsuInputdevvsOutputdevInInfoWl
**	2013.02.15	������	ɾ��ManageRsspData
**	2013.02.15	������	ɾ��dsuRsrInit������dsuRsspInitWl
**	2013.02.15	������	����dsuRsspRGetAddrWl
**	2013.02.15	������	����dsuRsspRGetName_WL
*******************************************************************************/
#include "string.h"
#include "CommonMemory.h"
#include "dsuRsspFunc.h"

#include "dsuRsspDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"


#define DSU_SSTY_INDEX_WL 0
#define DSU_SSID_INDEX_WL 1

DSU_RSSP_INFO_STRU_WL dsuRsrInfoStru_WL;	/*���ڱ���RSR����Ϣ�Ľṹ�����*/
DSU_RSSP_INFO_STRU_WL* pDsuRsrInfoStru_WL;     /*��ǰ���ڲ��������ݿ�.*/

/*
* ��������������������豸���Ͳ�ѯͨ����Ϣ
* ���������UINT8 localType,   �����豸����
*           UINT8 DestType,    Ŀ���豸����
* ���������DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType
* �� �� ֵ��1�ɹ���0ʧ��
*/
UINT8 dsuSetCommInfo_WL(DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*ѭ����*/
	DSU_DEVICE_COMM_INFO_STRU_WL* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuRsrInfoStru_WL->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru_WL->DeviceCommStruLen;i++)
	{
		if ((localType==pTempDevCommInfoStru->EmitterType)
			&&(DestType==pTempDevCommInfoStru->ReceptorType))
		{
			/*���������SSTy���ҵ����ڵ���*/
			pRsspLnkRInfo->MaxLost=pTempDevCommInfoStru->MaxLost;
			pRsspLnkRInfo->DLost=pTempDevCommInfoStru->DLost;
			pRsspLnkRInfo->WSsrOut=pTempDevCommInfoStru->WSsrOut;
			pRsspLnkRInfo->AntiDelayCount=pTempDevCommInfoStru->AntiDelayCount;
			pRsspLnkRInfo->TOut=pTempDevCommInfoStru->TOut;
			
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*���в���Ҫ���ҵ����ݣ���������*/
		}
		pTempDevCommInfoStru++;
	}

	/*δ�ҵ���Ӧͨ�Ź�ϵ�����ݣ���ϵ���Ϸ�*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuRsspGetDevTypeWl(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����սڵ���Ϣ����ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           UINT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkInfo��������Ϣ
*				��������ṹ���������������帳ֵ�������£�
*				UINT8 ROLE;��ʾĿ������Ƿ�Ϊ�ƶ����豸��
*				UINT8 LocalLogId;  ��ʾ����Դ��̬ID 
*				UINT16 DestName��ʾ����Ŀ���ʶ
*				UINT8 DestLogId;  ��ʾ���ĵ�Ŀ�궯̬ID 
*				LocalLogId ��0xFF
*				DestName = ���ݿ�����
*				DestLogId ��0xFF
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsspRGetLnkInfoWl(UINT16 RsspAddr, UINT16 SrcAddr, DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru_WL->RsspInfoStruLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->SseAddr==SrcAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorName;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuRsspGetDevTypeWl(pTempRsspLnkInfoStru->EmitterName);
			DestDevType=dsuRsspGetDevTypeWl(pTempRsspLnkInfoStru->ReceptorName);
			chReturnValue=dsuSetCommInfo_WL(pRsspLnkRInfo,LocalDevType,DestDevType);
			if (chReturnValue==0)
			{
				/*����ͨ�Ź�ϵ��������ʧ�ܣ���������ʧ��*/
				chReturnValue=0;
				return chReturnValue;
			}

			if (ChannelCount>=2)
			{
				/*����ͨ����Ϣ�����ҵ�����������*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��δ�ҵ�����ͨ������������*/
			}
		}
		else
		{
			/*��ǰ���Ŀ�����������*/
		}
		pTempRsspLnkInfoStru++;
	}


	/*δ�ҵ��豸����������ʧ��*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����ͽڵ���Ϣ����ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           UINT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkInfo��������Ϣ
*				��������ṹ���������������帳ֵ�������£�
*				UINT8 ROLE;��ʾĿ������Ƿ�Ϊ�ƶ����豸��
*				UINT8 LocalLogId;  ��ʾ����Դ��̬ID 
*				UINT16 DestName��ʾ����Ŀ���ʶ
*				UINT8 DestLogId;  ��ʾ���ĵ�Ŀ�궯̬ID 
*				Դ��Ŀ�����Ϊ�̶����豸
*				LocalLogId ��0xFF
*				DestName = ���ݿ�����
*				DestLogId ��0xFF
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsspSGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkSInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru_WL->RsspInfoStruLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			 &&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr) )
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorName;
			pRsspLnkSInfo->DestLogId=0xff;
			pRsspLnkSInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempRsspLnkInfoStru->SINIT_NUMBER;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;
			pRsspLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempRsspLnkInfoStru->DATAVER_1_Number;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;

			if (ChannelCount>=2)
			{
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��δ�ҵ�����ͨ������������*/
			}
		}
		else
		{
			/*��ǰ���Ŀ�����������*/
		}
		pTempRsspLnkInfoStru++;
	}


	/*δ�ҵ��豸����������ʧ��*/
	chReturnValue=0;
	return chReturnValue;
}


/*
* �����������ú������ڲ�ѯ�豸֮��ͨ����Ϣ��Ӧ�ã�>RSSPЭ�����ʱʹ�á�
* ���������UINT16 LocalName,��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT16 DestName,��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
* ���������UINT16* pRsspAddr,     rssp��ַ
*           UINT16* pBsdAddr,       bsd��ַ
*           UINT16* pSseAddr       sse��ַ
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuInputdevvsOutputDevOutInfoWl(UINT16 LocalName,UINT16 DestName,UINT16* pRsspAddr,UINT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;

	for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->EmitterName==LocalName)
			&&(pTempRsspLnkInfoStru->ReceptorName==DestName))
		{
			/*��ǰ���Դ��Ŀ���豸��������ϣ���ֵ����*/
			*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
			*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
			*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*��ǰ���Դ��Ŀ���豸�����벻���ϣ���������*/
		}
		pTempRsspLnkInfoStru++;
	}

	chReturnValue=0;
	return chReturnValue;
}


/*
* �����������ú������ڲ�ѯ�豸֮��ͨ����Ϣ��RSSPЭ��->Ӧ������ʱʹ�á�
* ���������UINT16 RsspAddr,  rssp��ַ
*           UINT16 BsdAddr,    bsd��ַ
* ���������UINT16* pLocalName����ʾ����Դ��ʾ��TYPE��ǰ��ID�ں�
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuInputdevvsOutputdevInInfoWl(UINT16 RsspAddr,UINT16 BsdAddr, UINT16* pLocalName)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;

	for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*��ǰ���RsspAddr ��BsdAddr��������ϣ���ֵ����*/
			*pLocalName=pTempRsspLnkInfoStru->EmitterName;

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*��ǰ���Դ��Ŀ���豸�����벻���ϣ���������*/
		}
		pTempRsspLnkInfoStru++;
	}

	chReturnValue=0;
	return chReturnValue;
}

/*
* ������������ȫͨ��Э��rssp��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuRsrInfoStru_WL �ֽ�����ͷָ��
* ���������pDsuRsrInfoStru_WL ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuRsspInitWl(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;

	pDsuRsrInfoStru_WL=&dsuRsrInfoStru_WL;

	/*��ʼ��rssp lnk ��Ϣ��*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_RSSP_LNK_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}

	if(dataLen>0)
	{
		pDsuRsrInfoStru_WL->RsspInfoStruLen=(UINT16)(dataLen/DSU_RSSP_LNK_INFO_STRU_SIZE_WL);  /*�����������ݵĸ���*/
		pDsuRsrInfoStru_WL->pRsspLnkInfoStru=(DSU_RSSP_LNK_INFO_STRU_WL*)malloc(sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen)); /*Ϊ�ṹ������ռ�*/
		if (pDsuRsrInfoStru_WL->pRsspLnkInfoStru==NULL)
		{
			/*�ռ����ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
			CommonMemSet(pDsuRsrInfoStru_WL->pRsspLnkInfoStru,sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen),0,sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen));
		}

		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
		{
			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ReceptorName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].DATANUM=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].BsdAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
			dataOff+=4;

			for (j=0;j<DSURSR_MAX_K_NUM_WL;j++)
			{
				pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}

			for (j=0;j<DSURSR_MAX_K_NUM_WL;j++)
			{
				pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}
		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
		}
	}
	else
	{
		/* �������� */
	}

	/*��ʼ���豸ͨ�����Ա�*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_COMM_INFO_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}
    
	if(dataLen>0)
	{
		pDsuRsrInfoStru_WL->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE_WL);  /*�����������ݵĸ���*/
		pDsuRsrInfoStru_WL->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen)); /*Ϊ�ṹ������ռ�*/
		if (pDsuRsrInfoStru_WL->pDevCommInfoStru==NULL)
		{
			/*�ռ����ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
			CommonMemSet(pDsuRsrInfoStru_WL->pDevCommInfoStru,sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen),0,sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen));
		}

		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<pDsuRsrInfoStru_WL->DeviceCommStruLen;i++)
		{

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
			dataOff+=2;


			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].MaxLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].DLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].TOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].MsgId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].IftVer=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
		}
	}
	else
	{
		/* �������� */
	}

	/*��RsspLnk���ݷֳ�ZcVsCi,Vobc2Ci,Ci2Vobc��������*/
	/*ɾ�����ݷ��� 20130219 wangpeipei*/
	/*ManageRsspData();*/

	chReturnValue=1;
	return chReturnValue;
}


/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯRSSP��·����ȡԴ��ַ��Ŀ���ַ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������UINT16* rsspSrcAddr   Դ��ַ
UINT16* rsspDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuRsspRGetAddrWl(UINT16 destDevName,UINT16 locDevName ,UINT16* rsspSrcAddr,UINT16* rsspDestAddr)
{
	UINT8 ret = 0;
	UINT16 i; /*ѭ����*/
	
	for (i = 0;i < pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName == locDevName) && (pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ReceptorName == destDevName))
		{
			*rsspSrcAddr = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr;
			*rsspDestAddr = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuRsrInfoStru_WL->RsspInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}

/*
*���ܣ�����Դ��ַ��Ŀ���ַ����ѯRSSP��·����ȡ�����豸���ƺͶԷ��豸����
*���������������
                UINT16* rsspSrcAddr   Դ��ַ
                UINT16* rsspDestAddr  Ŀ���ַ				
       ���������
	            UINT16 destDevName   �Է���ʶ
                UINT16 locDevName    ������ʶ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuRsspRGetName_WL(UINT16 rsspSrcAddr,UINT16 rsspDestAddr,UINT16* destDevName)
{
	UINT8 ret = 0;

	UINT16 i; /*ѭ����*/
	
	for (i = 0;i < pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr == rsspSrcAddr) && (pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr == rsspDestAddr))
		{
			*destDevName = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuRsrInfoStru_WL->RsspInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}
