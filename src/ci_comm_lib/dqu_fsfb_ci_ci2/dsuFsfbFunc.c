/*******************************************************************************
*
* �ļ���  ��dsuFsfbFunc.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������FSFBЭ����ص�dsu��ѯ����������
* ʹ��ע�⣺��
* �޸ļ�¼����
*******************************************************************************/
#include "string.h"
#include "dsuFsfbFunc.h"
#include "dsuFsfbDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"


#define DSU_SSTY_INDEX 0
#define DSU_SSID_INDEX 1

DSU_FSFB_INFO_STRU dsuFsfbInfoStru;	/*���ڱ���Fsfb����Ϣ�Ľṹ�����*/
DSU_FSFB_INFO_STRU* pDsuFsfbInfoStru;     /*��ǰ���ڲ��������ݿ�.*/

/*
* ��������������������豸���Ͳ�ѯͨ����Ϣ
* ���������UINT8 localType,   �����豸����
*           UINT8 DestType,    Ŀ���豸����
* ���������DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType
* �� �� ֵ��1�ɹ���0ʧ��
*/
UINT8 dsuFsfbSetCommInfo(DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*ѭ����*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuFsfbInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuFsfbInfoStru->DeviceCommStruLen;i++)
	{
		if ((localType==pTempDevCommInfoStru->EmitterType)
			&&(DestType==pTempDevCommInfoStru->ReceptorType))
		{
			/*���������SSTy���ҵ����ڵ���*/
			pFSFBLnkRInfo->MaxLost=pTempDevCommInfoStru->MaxLost;
			pFSFBLnkRInfo->DLost=pTempDevCommInfoStru->DLost;
			pFSFBLnkRInfo->WSsrOut=pTempDevCommInfoStru->WSsrOut;
			pFSFBLnkRInfo->AntiDelayCount=pTempDevCommInfoStru->AntiDelayCount;
			pFSFBLnkRInfo->TOut=pTempDevCommInfoStru->TOut;
			
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
UINT8 dsuFsfbGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* �����������ú������ڲ�ѯFSFB�����·�����Ϣ�����սڵ���Ϣ����ӳ�����ݡ�
* ���������UINT16 FSFBAddr, FSFB��ַ
*           UINT16 BSDAddr,   bsd��ַ
* ���������DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkInfo��������Ϣ
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
UINT8 dsuFsfbRGetLnkInfo(UINT16 locDevName,UINT16 destDevName,UINT8 applyCat, DSU_FSFB_LINK_RINFO_STRU* pFsfbLnkRInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_FSFB_LNK_INFO_STRU* pTempFsfbLnkInfoStru;
	UINT16 pTempFsfbLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	pTempFsfbLnkInfoStru=pDsuFsfbInfoStru->pfsfbLnkInfoStru;
	pTempFsfbLnkInfoStruLen=pDsuFsfbInfoStru->fsfbInfoStruLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempFsfbLnkInfoStruLen;i++)
	{
		if ((pTempFsfbLnkInfoStru->ReceptorName ==locDevName)
			&&(pTempFsfbLnkInfoStru->EmitterName==destDevName)
			&& (pTempFsfbLnkInfoStru->ApplyCat == applyCat))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempFsfbLnkInfoStru->ChannelNumber-1;
			ChannelCount++;
			pFsfbLnkRInfo->DestName = pTempFsfbLnkInfoStru->ReceptorName;
			pFsfbLnkRInfo->srcAdd = pTempFsfbLnkInfoStru->srcAddr;
			pFsfbLnkRInfo->destAdd = pTempFsfbLnkInfoStru->destAddr;
			pFsfbLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_E;
			pFsfbLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_R;
			pFsfbLnkRInfo->SINIT_NUMBER[CurrentChannel]=pTempFsfbLnkInfoStru->SINIT_NUMBER;
			pFsfbLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempFsfbLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pFsfbLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempFsfbLnkInfoStru->POST_RXDADA_K,sizeof(pFsfbLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pFsfbLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempFsfbLnkInfoStru->PREC_SINIT_K,sizeof(pFsfbLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuFsfbGetDevType(pTempFsfbLnkInfoStru->EmitterName);
			DestDevType=dsuFsfbGetDevType(pTempFsfbLnkInfoStru->ReceptorName);
			chReturnValue=dsuFsfbSetCommInfo(pFsfbLnkRInfo,LocalDevType,DestDevType);
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
		pTempFsfbLnkInfoStru++;
	}


	/*δ�ҵ��豸����������ʧ��*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* �����������ú������ڲ�ѯFSFB�����·�����Ϣ�����ͽڵ���Ϣ����ӳ�����ݡ�
* ���������UINT16 LocalDevName, �����豸����
*           UINT16 destName,   �Է��豸����
*           UINT8 applyCat   Application Cat
* ���������DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo��������Ϣ
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuFsfbSGetLnkInfo(UINT16 LocalDevName, UINT16 destName,UINT8 applyCat,DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_FSFB_LNK_INFO_STRU* pTempFsfbLnkInfoStru;
	UINT16 pTempFsfbLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	pTempFsfbLnkInfoStru=pDsuFsfbInfoStru->pfsfbLnkInfoStru;
	pTempFsfbLnkInfoStruLen=pDsuFsfbInfoStru->fsfbInfoStruLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempFsfbLnkInfoStruLen;i++)
	{
		if ((pTempFsfbLnkInfoStru->ReceptorName==destName)
			 &&(pTempFsfbLnkInfoStru->EmitterName==LocalDevName)
			 &&(pTempFsfbLnkInfoStru->ApplyCat == applyCat))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempFsfbLnkInfoStru->ChannelNumber-1;
			ChannelCount++;
			pFsfbLnkSInfo->DestName=pTempFsfbLnkInfoStru->ReceptorName;
			pFsfbLnkSInfo->srcAdd = pTempFsfbLnkInfoStru->srcAddr;
			pFsfbLnkSInfo->destAdd = pTempFsfbLnkInfoStru->destAddr;
			pFsfbLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_E;
			pFsfbLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_R;
			pFsfbLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempFsfbLnkInfoStru->SINIT_NUMBER;
			pFsfbLnkSInfo->DATANUM[CurrentChannel]=pTempFsfbLnkInfoStru->DATANUM;
			pFsfbLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempFsfbLnkInfoStru->DATAVER_1_Number;
			pFsfbLnkSInfo->DATANUM[CurrentChannel]=pTempFsfbLnkInfoStru->DATANUM;

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
		pTempFsfbLnkInfoStru++;
	}


	/*δ�ҵ��豸����������ʧ��*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* ������������ȫͨ��Э��FSFB��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuFsfbInfoStru �ֽ�����ͷָ��
* ���������pDsuFsfbInfoStru ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuFsFbInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;
	pDsuFsfbInfoStru=&dsuFsfbInfoStru;

	/* ��ʼ��IP ��ַ�ṹ */
    /* ��ȡ���� */
	TempRet = dquGetConfigData((char*)FSName,DSU_DEVICE_IP_CONFIG_TYPE,DSU_DATA_IP_IP_ID,&pData,&dataLen);

	if (TempRet != 1)
	{
		/* ���ݶ�ȡʧ�� */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* ���� ���� */
	}
	pDsuFsfbInfoStru->IpInfoStruLen = (UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);
	pDsuFsfbInfoStru->pIpInfoStru = (DSU_FSFB_IP_INFO_STRU*)malloc(sizeof(DSU_FSFB_IP_INFO_STRU)*pDsuFsfbInfoStru->IpInfoStruLen);
	if (pDsuFsfbInfoStru->pIpInfoStru == NULL)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/* ����ִ�� */
	}

	dataOff = 0;

	for (i=0;i<pDsuFsfbInfoStru->IpInfoStruLen;i++)
	{
		pDsuFsfbInfoStru->pIpInfoStru[i].devName = ShortFromChar(pData+dataOff);
		dataOff+=2;
		memcpy(pDsuFsfbInfoStru->pIpInfoStru[i].IPAddrBlue,pData+dataOff,DSU_IPADDR_LEN);
		dataOff+=DSU_IPADDR_LEN;

		memcpy(pDsuFsfbInfoStru->pIpInfoStru[i].IPAddrRed,pData+dataOff,DSU_IPADDR_LEN);
		dataOff+=DSU_IPADDR_LEN;

		pDsuFsfbInfoStru->pIpInfoStru[i].SfpBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].SfpRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RpBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RpRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RsrAndRsspBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RsrAndRsspRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		/*pDsuFsfbInfoStru->pIpInfoStru[i].FsfbBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].FsfbRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;*/
	}

	if (dataOff != dataLen)
	{
		/* ���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣 */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* ����ִ�� */
	}
	/*  ��ʼ���豸������ͨ��Э���ϵ�� */
	TempRet = dquGetConfigData((char*)FSName,DSU_DEVICE_IP_CONFIG_TYPE,DSU_DATA_IP_COMM_PROTCL_ID,&pData,&dataLen);
	if (TempRet !=1)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/* ����ִ�� */
	}

	pDsuFsfbInfoStru->ComProtclStruLen =(UINT16)(dataLen/DSU_COM_PROTCL_STRU_SIZE);
	pDsuFsfbInfoStru->pComProtclStr = (DSU_COM_PROTCL_STRU*)malloc(sizeof(DSU_COM_PROTCL_STRU)*pDsuFsfbInfoStru->ComProtclStruLen);
	if (pDsuFsfbInfoStru->pComProtclStr ==NULL)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/*����ִ�� */
	}

	dataOff =0;
	for (i=0;i<pDsuFsfbInfoStru->ComProtclStruLen;i++)
	{
		pDsuFsfbInfoStru->pComProtclStr[i].EmitterType=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pComProtclStr[i].ReceptorType = pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pComProtclStr[i].CommType = pData[dataOff];
		dataOff++;
	}

	if (dataOff != dataLen)
	{
		/* ���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣 */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* ����ִ�� */
	}

	/*��ʼ��FSFB lnk ��Ϣ�� */
	/*��ȡ����*/
	//TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_FSFB_LNK_ID, &pData , &dataLen);
	//if (TempRet!=1)
	//{
	//	/*���ݶ�ȡʧ��*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*����ִ��*/
	//}

	//pDsuFsfbInfoStru->fsfbInfoStruLen=(UINT16)(dataLen/DSU_FSFB_LNK_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	//pDsuFsfbInfoStru->pfsfbLnkInfoStru=(DSU_FSFB_LNK_INFO_STRU*)malloc(sizeof(DSU_FSFB_LNK_INFO_STRU)*(pDsuFsfbInfoStru->fsfbInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	//if (pDsuFsfbInfoStru->pfsfbLnkInfoStru==NULL)
	//{
	//	/*�ռ����ʧ��*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*����ִ��*/
	//}

	//dataOff=0;
	///*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
	//for (i=0;i<pDsuFsfbInfoStru->fsfbInfoStruLen;i++)
	//{
	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ApplyCat=pData[dataOff];
	//	dataOff+=1;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].EmitterName=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ReceptorName=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].DATANUM=pData[dataOff];
	//	dataOff++;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].srcAddr=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].destAddr=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	for (j=0;j<DSUFSFB_MAX_K_NUM;j++)
	//	{
	//		pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
	//		dataOff+=4;

	//	}

	//	for (j=0;j<DSUFSFB_MAX_K_NUM;j++)
	//	{
	//		pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
	//		dataOff+=4;
	//	}
	//}

	//if (dataOff!=dataLen)
	//{
	//	/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*����ִ��*/
	//}

	///*��ʼ��ƽ̨���ݽṹ���*/
	///*��ȡ����*/
	//TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_CI_PLATFORM_ID, &pData , &dataLen);
	//if (TempRet!=1)
	//{
	//	/*���ݶ�ȡʧ��*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*����ִ��*/
	//}

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

	pDsuFsfbInfoStru->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	pDsuFsfbInfoStru->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuFsfbInfoStru->DeviceCommStruLen)); /*Ϊ�ṹ������ռ�*/
	if (pDsuFsfbInfoStru->pDevCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}

	dataOff=0;
	/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
	for (i=0;i<pDsuFsfbInfoStru->DeviceCommStruLen;i++)
	{

		pDsuFsfbInfoStru->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
		dataOff+=2;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
		dataOff+=2;


		pDsuFsfbInfoStru->pDevCommInfoStru[i].MaxLost=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].DLost=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].TOut=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].MsgId=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].IftVer=pData[dataOff];
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

	chReturnValue=1;
	return chReturnValue;
}


/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯFSFB��·����ȡԴ��ַ��Ŀ���ַ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������UINT16* FSFBSrcAddr   Դ��ַ
UINT16* FSFBDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuFsfbRGetAddr(UINT16 destDevName,UINT16 locDevName,UINT8 applyCat,UINT16* srcAddr,UINT16* destAddr)
{
	UINT8 ret = 0;
	UINT16 i; /*ѭ����*/
	
	for (i = 0;i < pDsuFsfbInfoStru->fsfbInfoStruLen;i++)
	{
		if ((pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].EmitterName == locDevName) 
			&& (pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ReceptorName == destDevName)
			&&(pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ApplyCat == applyCat))
		{
			*srcAddr = pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].srcAddr;
			*destAddr = pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].destAddr;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuFsfbInfoStru->fsfbInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}

/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯ�豸ͨ�Ź�ϵ���IP���ñ��ȡIp��Ϣ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������DSU_FSFB_LNK_INFO_STRU* pLnkInfo   IP��Ϣ
UINT16* FSFBDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuFsfbGetLnkInfo(UINT16 locDevName,UINT16 destDevName,DSU_FSFB_IP_LNK_INFO_STRU* pLnkInfo)
{
	UINT8 retVal =0;
	UINT16 i=0;/* ѭ��ʹ�� */

	UINT8 tempEmitterType = 0;
	UINT8 tempReceptorType = 0;
	UINT8 commType =0;

	DSU_COM_PROTCL_STRU* pTempComProtclStru = NULL; /* ͨ��Э����ʱ�� */
	DSU_FSFB_IP_INFO_STRU *pTempIpInfoStru = NULL;
	UINT16 tempComProtclStruLen=0;
	UINT16 tempIpInfoStruLen =0;

	/* ��ȡ����������豸���� */
	tempEmitterType =dsuFsfbGetDevType(locDevName);
	tempReceptorType = dsuFsfbGetDevType(destDevName);

	pTempComProtclStru=pDsuFsfbInfoStru->pComProtclStr;
    tempComProtclStruLen=pDsuFsfbInfoStru->ComProtclStruLen;

	for(i=0;i<tempComProtclStruLen;i++)
	{
		if ((pTempComProtclStru->EmitterType == tempEmitterType)
			&&(pTempComProtclStru->ReceptorType == tempReceptorType))
		{
			commType=pTempComProtclStru->CommType;
			break;
		}
		pTempComProtclStru++;
	}

	if (i== tempComProtclStruLen)
	{
		retVal = 0;
		return retVal;
	}

	pLnkInfo->srcNum =0;
	pLnkInfo->destNum =0;

	pTempIpInfoStru = pDsuFsfbInfoStru->pIpInfoStru;
    tempIpInfoStruLen = pDsuFsfbInfoStru->IpInfoStruLen;

	for(i=0;i<tempIpInfoStruLen;i++)
	{
		/* ����Դ�豸 */
		if (pTempIpInfoStru->devName == locDevName)
		{
			/* ��ֵIP��ַ */
			memcpy(pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].BlueIp,pTempIpInfoStru->IPAddrBlue,DSU_IPADDR_LEN);
			memcpy(pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].RedIp,pTempIpInfoStru->IPAddrRed,DSU_IPADDR_LEN);

			/* ͨ��Э�������Ѿ��ҵ� */
			/* ����Э�����͵Ĳ�ͬ��ȡ��ͬ�Ķ˿ں� */
			if(commType == DSU_PROTCL_SFP)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->SfpBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->SfpRedPort;
			}
			else if(commType == DSU_PROTCL_RP)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->RpBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->RpRedPort;
			}
			else if((commType == DSU_PROTCL_RSR)||(commType == DSU_PROTCL_RSSP))
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->RsrAndRsspBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->RsrAndRsspRedPort;
			}
			/*else if(commType == DSU_PROTCL_FSFB)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->FsfbBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->FsfbRedPort;
			}*/
			else
			{
				/* ���ݴ��� */
				retVal =0;
				return retVal;
			}
			/* IP��ַ��+1*/
			pLnkInfo->srcNum++;
		}
		else
		{
			/* �������� */
		}

		if (pTempIpInfoStru->devName == destDevName)
		{
			/* ��ֵIP��ַ */
			memcpy(pLnkInfo->DestIpInfo[pLnkInfo->destNum].BlueIp,pTempIpInfoStru->IPAddrBlue,DSU_IPADDR_LEN);
			memcpy(pLnkInfo->DestIpInfo[pLnkInfo->destNum].RedIp,pTempIpInfoStru->IPAddrRed,DSU_IPADDR_LEN);

			/* ͨ��Э�������Ѿ��ҵ� */
			/* ����Э�����͵Ĳ�ͬ��ȡ��ͬ�Ķ˿ں� */
			if(commType == DSU_PROTCL_SFP)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->SfpBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->SfpRedPort;
			}
			else if(commType == DSU_PROTCL_RP)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->RpBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->RpRedPort;
			}
			else if((commType == DSU_PROTCL_RSR)||(commType == DSU_PROTCL_RSSP))
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->RsrAndRsspBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->RsrAndRsspRedPort;
			}
			/*else if(commType == DSU_PROTCL_FSFB)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->FsfbBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->FsfbRedPort;
			}*/
			else
			{
				/* ���ݴ��� */
				retVal =0;
				return retVal;
			}
			/* IP��ַ��+1*/
			pLnkInfo->destNum++;
		}
		else
		{
			/* �������� */
		}

		pTempIpInfoStru++;
	}

	if ((pLnkInfo->srcNum == 0)||(pLnkInfo->destNum == 0))
	{
		retVal = 0;
		return retVal;
	}
	else
	{
		/* �������� */
	}

	retVal =1;
	return retVal;
}


UINT8 AccordDevNameGetIpAddr(UINT16 devName,UINT8* pIpaddr)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if(pTempIpStru[i].devName == devName)
		{
			memcpy(pIpaddr,pTempIpStru[i].IPAddrRed,DSU_IPADDR_LEN);
			break;
		}
		else
		{
			/* �������� */
		}
	}

	if(i==dataLen)
		{
			retVal =0;
			pIpaddr = NULL;
		}
	else
		{
		retVal =1;
		}

    
	return retVal;
}

UINT8 AccordRedIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if((pTempIpStru[i].IPAddrRed[commIdBit] == commId)&&(devId ==pTempIpStru[i].IPAddrRed[devIdBit])
			&&(pTempIpStru[i].IPAddrRed[0]==0)&&(pTempIpStru[i].IPAddrRed[1]==0))
		{
			*devName=pTempIpStru[i].devName;
			break;
		}
		else
		{
			/* �������� */
		}
	}

		if(i==dataLen)
		{
			retVal =0;
			*devName = 0;
		}
	else
		{
		retVal =1;
		}
	return retVal;
}


UINT8 AccordBlueIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if((pTempIpStru[i].IPAddrBlue[commIdBit] == commId)&&(devId ==pTempIpStru[i].IPAddrBlue[devIdBit])
			&&(pTempIpStru[i].IPAddrBlue[0]==0)&&(pTempIpStru[i].IPAddrBlue[1]==0))
		{
			*devName=pTempIpStru[i].devName;
			break;
		}
		else
		{
			/* �������� */
		}
	}

    		if(i==dataLen)
		{
			retVal =0;
			*devName = 0;
		}
	else
		{
		retVal =1;
		}
	return retVal;
}
