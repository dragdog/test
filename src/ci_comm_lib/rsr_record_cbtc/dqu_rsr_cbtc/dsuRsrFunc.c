/*******************************************************************************
*
* �ļ���  ��dsuRsrFunc.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������RSRЭ����ص�dsu��ѯ����������
* ʹ��ע�⣺��
* �޸ļ�¼����
*
*******************************************************************************/


#include "dsuRsrFunc.h"
#include "CommonMemory.h"
#include "Convert.h"
#include "string.h"
#include "dsuRsrDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"


#define DSU_SSTY_INDEX 0
#define DSU_SSID_INDEX 1

DSU_RSR_INFO_STRU dsuRsrInfoStru;	/*���ڱ���RSR����Ϣ�Ľṹ�����*/
DSU_RSR_INFO_STRU* pDsuRsrInfoStru;     /*��ǰ���ڲ��������ݿ�.*/

/*
* �����������ú������ڲ�ѯ�źŲ㱨���Ƿ�Ϸ�
*           ���������������ѯ���ݿ����Ƿ��и�ͨ�Ź�ϵ��
* ���������UINT16 localName, ��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT8 localLogId,��ʾ����Դ��̬ID
*           UINT16 DestName, ��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
*           UINT8 DestLogId, ��ʾ���ĵĶ�̬ID
*           UINT8 ItfVer, ��ʾ�ӿڰ汾�ţ��뱨�ķ������
*           UINT8 MsgId,��ʾ��ϢID���뱨�ķ�����ء�
* ���������UINT8* pIsLegalRole,��ʾͨ�Ź�ϵ�Ƿ���������ݿ���
*			1����ʾ�Ϸ������ݿ���ڸ�ͨ�Ź�ϵ��
*			0����ʾ���Ϸ������ݿ��в����ڸ�ͨ�Ź�ϵ��
* �� �� ֵ��1���ɹ����������ݲ�һ���Ϸ�
*           0��ʧ�ܣ�
*/
UINT8 dsuSigIsLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8 ItfVer, UINT8 MsgId,UINT8* pIsLegalRole)
{
	UINT8 ByLocalName[2];   /*����Դ��ʶ*/
	UINT8 ByDestName[2];    /*����Ŀ���ʶ��*/
	UINT16 i;               /*ѭ����*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	/*�ж�ͨ�Ź�ϵ�Ƿ�Ϸ�*/
	chReturnValue=dsuRsrCheckSigRelationLegal(LocalName,LocalLogId,DestName,DestLogId);
	if (chReturnValue==0)
	{
		/*ͨ�Ź�ϵ���Ϸ�*/
		* pIsLegalRole=DSURSR_FALSE;
		chReturnValue=1;
		return chReturnValue;
	}

	/*������ı�ʶ���ֳ�ssty��ssid*/
	ShortToChar(LocalName,ByLocalName);
	ShortToChar(DestName,ByDestName);

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
	{
		if ((ByLocalName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->EmitterType)
			&&(ByDestName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->ReceptorType))
		{
			/*���������SSTy���ҵ����ڵ���*/
			if ((pTempDevCommInfoStru->IftVer==ItfVer)&&(pTempDevCommInfoStru->MsgId==MsgId))
			{
				* pIsLegalRole=DSURSR_TRUE;
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*���ҵ����У��������ݲ��ԣ�˵�����Ϸ�*/
				* pIsLegalRole=DSURSR_FALSE;
				chReturnValue=1;
				return chReturnValue;
			}
			
			
		}
		else
		{
			/*���в���Ҫ���ҵ����ݣ���������*/
		}
		pTempDevCommInfoStru++;
	}

	/*δ�ҵ���Ӧͨ�Ź�ϵ�����ݣ���ϵ���Ϸ�*/
	* pIsLegalRole=DSURSR_FALSE;
	chReturnValue=1;
	return chReturnValue;
}

/*
* �����������ú������ڻ�ȡ�źŲ�İ汾���Լ���ϢID
* ���������UINT16 localName, ��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT8 localLogId,��ʾ����Դ��̬ID
*           UINT16 DestName, ��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
*           UINT8 DestLogId, ��ʾ���ĵĶ�̬ID
* ���������UINT8* pItfVer, ��ʾ�ӿڰ汾�ţ��뱨�ķ������
*           UINT8* pMsgId,��ʾ��ϢID���뱨�ķ�����ء�
* �� �� ֵ��1���ɹ�
*           0��ʧ��,δ�ҵ�ͨ�Ź�ϵ��
*/
UINT8 dsuSigGetInfo(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8* pItfVer, UINT8* pMsgId)
{
	UINT8 ByLocalName[2];   /*����Դ��ʶ*/
	UINT8 ByDestName[2];    /*����Ŀ���ʶ��*/
	UINT16 i;               /*ѭ����*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;


	/*�ж�ͨ�Ź�ϵ�Ƿ�Ϸ�*/
	chReturnValue=dsuRsrCheckSigRelationLegal(LocalName,LocalLogId,DestName,DestLogId);
	if (chReturnValue==0)
	{
		/*ͨ�Ź�ϵ���Ϸ�*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*������ı�ʶ���ֳ�ssty��ssid*/
	ShortToChar(LocalName,ByLocalName);
	ShortToChar(DestName,ByDestName);

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
	{
		if ((ByLocalName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->EmitterType)
			&&(ByDestName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->ReceptorType))
		{
			/*���������SSTy���ҵ����ڵ���*/
			* pItfVer=pTempDevCommInfoStru->IftVer;
			* pMsgId=pTempDevCommInfoStru->MsgId;
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
* ��������������������豸���Ͳ�ѯͨ����Ϣ
* ���������UINT8 localType,   �����豸����
*           UINT8 DestType,    Ŀ���豸����
* ���������DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType
* �� �� ֵ��1�ɹ���0ʧ��
*/
UINT8 dsuSetCommInfo(DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*ѭ����*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
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
UINT8 dsuRsrGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* ������������ȡ�豸ID
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸Id
*/
UINT8 dsuRsrGetDevId(UINT16 DevName)
{
	UINT8 devId;
	devId=(UINT8)(DevName&0xff);

	return devId;
}

/*
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����սڵ���Ϣ���Լ����źŲ��ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           INT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_RINFO_STRU* pRsspLnkInfo��������Ϣ
*				��������ṹ���������������帳ֵ�������£�
*				UINT8 ROLE;��ʾĿ������Ƿ�Ϊ�ƶ����豸��
*				UINT8 LocalLogId;  ��ʾ����Դ��̬ID 
*				UINT16 DestName��ʾ����Ŀ���ʶ
*				UINT8 DestLogId;  ��ʾ���ĵ�Ŀ�궯̬ID 
*				��ROLE Ϊ0x00ʱ��Դ��Ŀ�����Ϊ�̶����豸
*				LocalLogId ��0xFF
*				DestName = ���ݿ�����
*				DestLogId ��0xFF
*
*				��ROLRΪ0x55ʱ��ԴΪ�̶����豸��Ŀ�����Ϊ�ƶ����豸
*				LocalLogId ��0xFF
*				DestName = 0xFFFF
*				DestLogId �����ݿ�����
*
*				��ROLRΪ0xFFʱ��ԴΪ�ƶ����豸��Ŀ�����Ϊ�̶����豸
*				LocalLogId �����ݿ�����
*				DestName =���ݿ�����
*				DestLogId ��0xFF
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsspRGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_VOBC2CI;
			pRsspLnkRInfo->LocalLogId=(UINT8)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));
			
			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,DSURSR_DEVTYPE_VOBC,DSURSR_DEVTYPE_CI);
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

	/*����Ϊ�ƶ���,��Ci2Vobc���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_CI2VOBC;
			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=0xffff;
			pRsspLnkRInfo->DestLogId=(UINT8)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,DSURSR_DEVTYPE_CI,DSURSR_DEVTYPE_VOBC);
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

	/*����Ϊ�ƶ���,��ZcvsCi���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_ZC_CI;
			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuRsrGetDevType(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			DestDevType=dsuRsrGetDevType(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,LocalDevType,DestDevType);
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
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����ͽڵ���Ϣ���Լ����źŲ��ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           INT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_SINFO_STRU* pRsspLnkInfo��������Ϣ
*				��������ṹ���������������帳ֵ�������£�
*				UINT8 ROLE;��ʾĿ������Ƿ�Ϊ�ƶ����豸��
*				UINT8 LocalLogId;  ��ʾ����Դ��̬ID 
*				UINT16 DestName��ʾ����Ŀ���ʶ
*				UINT8 DestLogId;  ��ʾ���ĵ�Ŀ�궯̬ID 
*				��ROLE Ϊ0x00ʱ��Դ��Ŀ�����Ϊ�̶����豸
*				LocalLogId ��0xFF
*				DestName = ���ݿ�����
*				DestLogId ��0xFF
*
*				��ROLRΪ0x55ʱ��ԴΪ�̶����豸��Ŀ�����Ϊ�ƶ����豸
*				LocalLogId ��0xFF
*				DestName = 0xFFFF
*				DestLogId �����ݿ�����
*
*				��ROLRΪ0xFFʱ��ԴΪ�ƶ����豸��Ŀ�����Ϊ�̶����豸
*				LocalLogId �����ݿ�����
*				DestName =���ݿ�����
*				DestLogId ��0xFF
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsspSGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU* pRsspLnkSInfo)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;
	
	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_VOBC2CI;
			pRsspLnkSInfo->LocalLogId=(UINT8)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
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

	/*����Ϊ�ƶ���,��Ci2Vobc���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_CI2VOBC;
			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=0xffff;
			pRsspLnkSInfo->DestLogId=(UINT8)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
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

	/*����Ϊ�ƶ���,��ZcvsCi���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

	/*�ҵ���Channel��Ϊ0*/
	ChannelCount=0;

	/*ѭ������*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_ZC_CI;
			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
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
* �����������ú������ڻ�ȡCI���߼�վ̨����Ϣ����վ̨����
* ���������UINT16 CIDevName,    CI�豸��ʶ
* ���������UINT8* pLogicIDNum,  վ̨��Ŀ
*           UINT8 LogicID[],     վ̨ID,��ׯ��ÿ��CI����Ͻ��վ̨������Ϊ6��վ̨��
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ�δ�ҵ�CIID
*/
UINT8 dsuRsspGetLogicId(UINT16 CIDevName, UINT8* pLogicIDNum,UINT8 LogicID[])
{
	UINT16 i; /*ѭ����*/
	UINT16 j; /*ѭ����*/
	DSU_CI_PLATFORM_INFO_STRU *pTempCIPlatFormInfoStru;
	UINT8 ByCIName[2];   /*�ֽڱ�ʶ*/
	UINT8 chReturnValue;

	/*������ı�ʶ���ֳ�ssty��ssid*/
	ShortToChar(CIDevName,ByCIName);

	pTempCIPlatFormInfoStru=pDsuRsrInfoStru->pCIPlatFormInfoStru;
	for (i=0;i<pDsuRsrInfoStru->CIPlatFormInfoStruLen;i++)
	{
		/*�жϵ�ǰCI Id������CIID�Ƿ���ͬ*/
		if (ByCIName[DSU_SSID_INDEX]==pTempCIPlatFormInfoStru->CIId)
		{
			/*��ǰCIID������CIID��ͬ����ֵ����*/
			*pLogicIDNum=pTempCIPlatFormInfoStru->CIPlatformNum;
			for (j=0;j<pTempCIPlatFormInfoStru->CIPlatformNum;j++)
			{
				LogicID[j]=pTempCIPlatFormInfoStru->CIPlatformId[j];
			}

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*��ǰCIID������CIID����ͬ����������*/
		}

		pTempCIPlatFormInfoStru++;
	}

	/*û���ҵ�CIID���ڵ����ݣ���������ʧ��*/
	chReturnValue=0;
	return chReturnValue;

}

/*
* �����������ú������ڲ�ѯZC��CIͨ����Ϣ��Ӧ�ã�>RSRЭ�����ʱʹ�á�
* ���������UINT16 LocalName,��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT16 DestName,��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
* ���������UINT16* pRsspAddr,     rssp��ַ
*           INT16* pBsdAddr,       bsd��ַ
*           UINT16* pSseAddr       sse��ַ
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuZCvsCIOutInfo(UINT16 LocalName,UINT16 DestName,UINT16* pRsspAddr,INT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;

	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;i++)
	{
		if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
			&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
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
* �����������ú������ڲ�ѯZC��CIͨ����Ϣ��RSRЭ��->Ӧ������ʱʹ�á�
* ���������UINT16 RsspAddr,  rssp��ַ
*           INT16 BsdAddr,    bsd��ַ
* ���������UINT16* pLocalName����ʾ����Դ��ʾ��TYPE��ǰ��ID�ں�
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuZCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* pLocalName)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;

	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*��ǰ���RsspAddr ��BsdAddr��������ϣ���ֵ����*/
			*pLocalName=pTempRsspLnkInfoStru->EmitterNameOrLogicId;

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
* �����������ú������ڲ�ѯVOBC��CIͨ����Ϣ��Ӧ�ã�>RSRЭ�����ʱʹ�á�
* ���������UINT8 PlatId,  ��ʾվ̨ID��
*           UINT8 LocalRole,��ʾ�����Ƿ�Ϊ�ƶ��塣
*                  1������Ϊ�ƶ���,��ʱվ̨ID�����ݿ���ΪԴ��
*                  0������Ϊ�̶��壬��ʱվ̨ID�����ݿ���ΪĿ�ꡣ
* ���������UINT16* pRsspAddr,     rssp��ַ
*           INT16* pBsdAddr,       bsd��ַ
*           UINT16* pSseAddr       sse��ַ
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuVOBCvsCIOutInfo(UINT16 PlatId, UINT8 LocalRole,UINT16* pRsspAddr, INT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ByPlatId;

	/*�˴�����Ӧ�û�õ�PlatIdΪ2byte��Э��PlatIdΪ1byte���˴���һ��ǿ������ת��*/
	if(PlatId>255)
	{
		/*�����PlatId���Ϸ�*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*�����PlatId�Ϸ����������ִ��*/

	}
	ByPlatId=(UINT8)(PlatId);


	if (LocalRole==1)
	{
		/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

		/*ѭ������PlatId*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if (pTempRsspLnkInfoStru->EmitterNameOrLogicId==PlatId)
			{
				/*��ǰ���PlatId��������ϣ���ֵ����*/
				*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
				*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
				*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��ǰ���PlatId�����벻���ϣ���������*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else
	{
		/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

		/*ѭ������PlatId*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if (pTempRsspLnkInfoStru->ReceptorNameOrLogicId==PlatId)
			{
				/*��ǰ���PlatId��������ϣ���ֵ����*/
				*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
				*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
				*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��ǰ���PlatId�����벻���ϣ���������*/
			}
			pTempRsspLnkInfoStru++;
		}

	}

	/*δ�ҵ��豸����������ʧ��*/
	chReturnValue=0;
	return chReturnValue;
}



/*
* �����������ú������ڲ�ѯVOBC��CIͨ����Ϣ��RSRЭ��->Ӧ������ʱʹ�á�
* ���������UINT16 RsspAddr,  rssp��ַ
*           INT16 BsdAddr,    bsd��ַ
* ���������UINT8* pPlatId��  ��ʾվ̨ID
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuVOBCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* pPlatId)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;

	/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

	/*ѭ������PlatId*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/

			/*�˴�����Ӧ�û�õ�PlatIdΪ2byte��Э��PlatIdΪ1byte���˴���һ��ǿ������ת��*/
			*pPlatId=(UINT16)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*��ǰ���Ŀ�����������*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*����Ϊ�̶���,��Ci2Vobc���в���*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*ѭ������PlatId*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*�Ѿ����ҵ�Ŀ��*/

			/*�˴�����Ӧ�û�õ�PlatIdΪ2byte��Э��PlatIdΪ1byte���˴���һ��ǿ������ת��*/
			*pPlatId=(UINT16)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);

			chReturnValue=1;
			return chReturnValue;
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
* �������������ݼ�麯�����ڲ�������dsu��������Ƿ�������
* �����������
* �����������
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsrCheckData(void)
{
	UINT8 chReturnValue=1;  /*��������ֵ*/
	return chReturnValue;
}

/*
* ��������������豸�Ƿ�Ϸ��������źŲ����ͨ��˫����Name�Ƿ�Ϸ���
* �����������
* �����������
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsrCheckSigRelationLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId)
{
	UINT8 chReturnValue; /*����������*/
	UINT16 i; /*ѭ����*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 LocalDevType;
	UINT8 DestDevType;
	UINT8 LocalDevId;
	UINT8 DestDevId;

	/*��ȡ�豸���ͺ�Id*/
	LocalDevType=dsuRsrGetDevType(LocalName);
	DestDevType=dsuRsrGetDevType(DestName);

	LocalDevId=dsuRsrGetDevId(LocalName);
	DestDevId=dsuRsrGetDevId(DestName);

	/*���ݲ�ͬ���豸���ͷ��ദ��*/
	if (LocalDevType==DSURSR_DEVTYPE_VOBC)
	{
		/*���̶����豸ssid��logic id�Ƿ���ͬ*/
		if (DestDevId!=DestLogId)
		{
			/*�̶����豸Id��LogicId�������*/
			chReturnValue=0;
			return chReturnValue;
		}

		/*����Ϊ�ƶ���,��Vobc2Ci���в���*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

		/*ѭ������*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalLogId)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
			{
				/*�Ѿ����ҵ�Ŀ��,ͨ�Ź�ϵ�Ϸ�*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��ǰ���Ŀ�����������*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else if (DestDevType==DSURSR_DEVTYPE_VOBC)
	{

		/*���̶����豸ssid��logic id�Ƿ���ͬ*/
		if (LocalDevId!=LocalLogId)
		{
			/*�̶����豸Id��LogicId�������*/
			chReturnValue=0;
			return chReturnValue;
		}

		/*�Է�Ϊ�ƶ���,��Ci2Vobc���в���*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;


		/*ѭ������*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestLogId))
			{
				/*�Ѿ����ҵ�Ŀ��*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��ǰ���Ŀ�����������*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else
	{
		/*���̶����豸ssid��logic id�Ƿ���ͬ*/
		if ((LocalDevId!=LocalLogId)||(DestDevId!=DestLogId))
		{
			/*�̶����豸Id��LogicId�������*/
			chReturnValue=0;
			return chReturnValue;
		}


		/*˫����Ϊ�̶���,��ZcvsCi���в���*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

		/*ѭ������*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
			{
				/*�Ѿ����ҵ�Ŀ��*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*��ǰ���Ŀ�����������*/
			}
			pTempRsspLnkInfoStru++;
		}

	}

	/*δ�ҵ�ͨ�Ź�ϵ��ͨ�Ź�ϵ���Ϸ�*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* ������������ȫͨ��Э��rsr��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuRsrInfoStru �ֽ�����ͷָ��
* ���������pDsuRsrInfoStru ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuRsrInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;

	pDsuRsrInfoStru=&dsuRsrInfoStru;

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
		pDsuRsrInfoStru->RsspInfoStruLen=(UINT16)(dataLen/DSU_RSSP_LNK_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
		pDsuRsrInfoStru->pRsspLnkInfoStru=(DSU_RSSP_LNK_INFO_STRU*)malloc(sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen)); /*Ϊ�ṹ������ռ�*/
		if (pDsuRsrInfoStru->pRsspLnkInfoStru==NULL)
		{
			/*�ռ����ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
			CommonMemSet(pDsuRsrInfoStru->pRsspLnkInfoStru,sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen),0,sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen));
		}

		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLen;i++)
		{
			pDsuRsrInfoStru->pRsspLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].EmitterNameOrLogicId=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].ReceptorNameOrLogicId=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].DATANUM=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].RsspAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].BsdAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SseAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
			dataOff+=4;

			for (j=0;j<DSURSR_MAX_K_NUM;j++)
			{
				pDsuRsrInfoStru->pRsspLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}

			for (j=0;j<DSURSR_MAX_K_NUM;j++)
			{
				pDsuRsrInfoStru->pRsspLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
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

	/*��ʼ��ƽ̨���ݽṹ���*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_CI_PLATFORM_ID, &pData , &dataLen);
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
		pDsuRsrInfoStru->CIPlatFormInfoStruLen=(UINT16)(dataLen/DSU_CI_PLATFORM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
		pDsuRsrInfoStru->pCIPlatFormInfoStru =(DSU_CI_PLATFORM_INFO_STRU*)malloc(sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen)); /*Ϊ�ṹ������ռ�*/
		if (pDsuRsrInfoStru->pCIPlatFormInfoStru==NULL)
		{
			/*�ռ����ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
			CommonMemSet(pDsuRsrInfoStru->pCIPlatFormInfoStru,sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen),0,sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen));
		}

		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<pDsuRsrInfoStru->CIPlatFormInfoStruLen;i++)
		{
			pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIPlatformNum=pData[dataOff];
			dataOff++;

			for (j=0;j<MAX_PLATFORM_PER_CI;j++)
			{
				pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIPlatformId[j]=pData[dataOff];
				dataOff++;
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
		pDsuRsrInfoStru->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
		pDsuRsrInfoStru->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen)); /*Ϊ�ṹ������ռ�*/
		if (pDsuRsrInfoStru->pDevCommInfoStru==NULL)
		{
			/*�ռ����ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*����ִ��*/
			CommonMemSet(pDsuRsrInfoStru->pDevCommInfoStru,sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen),0,sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen));
		}

		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
		{

			pDsuRsrInfoStru->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
			dataOff+=2;


			pDsuRsrInfoStru->pDevCommInfoStru[i].MaxLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].DLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].TOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].MsgId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].IftVer=pData[dataOff];
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
	ManageRsspData();

	chReturnValue=1;
	return chReturnValue;
}

/*	��RsspLnk���ݷֳ�ZcVsCi,Vobc2Ci,Ci2Vobc��������*/
void ManageRsspData(void)
{
	UINT16 i; /*ѭ���ñ���*/
	UINT16 EmitterType;
	UINT16 ReceptorType;
	pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen=0;
	pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen=0;
	pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen=0;

	/*ѭ����RsspLnk�е����ݷ���,�����ݷֳ����࣬CIvsZC,VOBC2CI��CI2VOBC��CI��CIͨ�ŵ������Ѷ���*/
	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLen;i++)
	{
		EmitterType=dsuRsrGetDevType(pDsuRsrInfoStru->pRsspLnkInfoStru[i].EmitterNameOrLogicId);
		ReceptorType=dsuRsrGetDevType(pDsuRsrInfoStru->pRsspLnkInfoStru[i].ReceptorNameOrLogicId);
		if (((EmitterType==DSURSR_DEVTYPE_CI)&&(ReceptorType==DSURSR_DEVTYPE_ZC))
			||((EmitterType ==DSURSR_DEVTYPE_ZC )&&(ReceptorType==DSURSR_DEVTYPE_CI)))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf+pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen++;
		}
		else if ((EmitterType==DSURSR_DEVTYPE_PLATFORM)&&(ReceptorType==DSURSR_DEVTYPE_CI))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf+pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen++;
		}
		else if ((EmitterType==DSURSR_DEVTYPE_CI )&&(ReceptorType==DSURSR_DEVTYPE_PLATFORM))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf+pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			
			pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen++;
		}
		else
		{
			/*������������ͨ�����ͣ���¼������*/
		}
	}

	return;
}


