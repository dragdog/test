/*******************************************************************************
*
* �ļ���  ��dsuRpFunc.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������RpЭ����ص�dsu��ѯ����������
* ʹ��ע�⣺��
* �޸ļ�¼����
*
*******************************************************************************/

#include <string.h>
#include "CommonMemory.h"
#include "dfsDataRead.h"
#include "dsuProtclFun.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"



/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE 22 
#define DSU_COMM_PROTCL_STRU_SIZE 3

DSU_PROTCL_BOOK_IP_INFO_STRU DsuProtclBookIpInfoStru;     /*��ʼ��֮�����ڱ����ʼ��֮������ݵı���*/

static void GetDevName(UINT8 devType,UINT16* dataBuff,UINT8 offIndex,UINT8* devNum);

/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
* ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuCommonInfoInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;

	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ���豸IP���ñ���豸IP��ַ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
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

	DsuProtclBookIpInfoStru.ipDevLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuProtclBookIpInfoStru.pProtclIpInfoStru=(DSU_PROTCL_IP_INFO_STRU*)malloc(sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuProtclBookIpInfoStru.pProtclIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuProtclBookIpInfoStru.pProtclIpInfoStru,sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen),0,sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen));
	}

	dataOff=0;
	/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
	for (i=0;i<DsuProtclBookIpInfoStru.ipDevLen;i++)
	{
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
		dataOff+=2;

		memcpy(DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].IpAddrBlue,pData+dataOff,DSU_LEN_IP_ADDR);
		dataOff+=DSU_LEN_IP_ADDR;

		memcpy(DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].IpAddrRed,pData+dataOff,DSU_LEN_IP_ADDR);
		dataOff+=DSU_LEN_IP_ADDR;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RsrBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RsrRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		/* ��Ҫ����RSSP2������� */
		#if 0
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].Rssp2BluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].Rssp2RedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		#endif
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

	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
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

	DsuProtclBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuProtclBookIpInfoStru.pCommProtclStru=(DSU_PROTCL_COMM_PROTCL_STRU*)malloc(sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuProtclBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuProtclBookIpInfoStru.pCommProtclStru,sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen));
	}

	dataOff=0;
	/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
	for (i=0;i<DsuProtclBookIpInfoStru.CommProtclStruLen;i++)
	{
		DsuProtclBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
		dataOff++;

		DsuProtclBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
		dataOff++;

		DsuProtclBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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
	/* ������Ҫ����RSSP2������Ϣ */

	/*����ִ��*/
	chReturnValue=1;
	return chReturnValue;
}

/*
* �����������ú������ڸ��ݱ����Լ��Է����豸���ͻ�ȡ��ʹ�á�
* ���������UINT16 LocalName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������UINT8* protclTye Э������
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuProtclGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye)
{
	UINT8 chReturnValue;/*��������ֵ*/
	UINT16 i;/*ѭ����*/

	UINT8 TempEmitterType;/*�������豸����*/
	UINT8 TempReceptorType;/*�������豸����*/

	DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

	/*��ȡ����������豸����*/
	TempEmitterType=dsuProtclGetDevType(LocalName);
	TempReceptorType=dsuProtclGetDevType(DestName);

	pTempCommProtclStru=DsuProtclBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuProtclBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)&&(pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			*protclTye=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuProtclBookIpInfoStru.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue=0;
		return chReturnValue;
	}

	chReturnValue =1;
	return chReturnValue;
}

/*
* �����������ú������ڸ��ݱ����豸���ͻ�ȡ���ʹ��Э�����Ϣ��
* ���������UINT16 LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_PROTCCL_TYPE_STRU* dsuProtclTypeInfo Э������
* �� �� ֵ��1:�������� 0�����󷵻�
* ��ע��1��ʹ��Э����豸�����ܸ������ܳ���255
*       2��������������Ϊ���𷽣��������ͨ�ŵĶԷ����豸���͵ĸ������ܳ���10����
*/
UINT8 dsuGetProtclTypeInfo(UINT16 locDevName,DSU_PROTCL_TYPE_STRU* dsuProtclTypeInfo)
{
	UINT8 retVal = 0;
	UINT16 i = 0;/* ѭ��ʹ��*/
    UINT16 j = 0;/* ѭ��ʹ��*/
	UINT8 index = 0;
	UINT8 tempDevNum = 0;
	UINT8 tempDestType = 0;/*��ʱ�豸����*/
	UINT8 TempEmitterType = 0;/*�����豸����*/
	DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru = NULL;/*ͨ��Э�����ʱ����*/
    DSU_PROTCL_COMM_PROTCL_STRU pLocalComProtclStru[10]; /* ����Э������ */
	DSU_PROTCL_IP_INFO_STRU *pDevIpInfoStru = NULL;/*IP����Ϣ */
	UINT16 *hasDesTypeIds = NULL; /*���е�Ŀ������ID����*/
	UINT16 hasDesTypeIdsLen = 0; /*���е�Ŀ������ID����*/
	UINT8 isHas = 0; /*�Ƿ�����Ŀ������ID*/
	UINT16 k = 0; /* ѭ��ʹ��*/

	CommonMemSet(pLocalComProtclStru, sizeof(DSU_PROTCL_COMM_PROTCL_STRU), 0, sizeof(DSU_PROTCL_COMM_PROTCL_STRU));

	/*��ȡ����������豸����*/
	TempEmitterType = dsuProtclGetDevType(locDevName);

	if(NULL != dsuProtclTypeInfo)
	{
		pTempCommProtclStru = DsuProtclBookIpInfoStru.pCommProtclStru;
		pDevIpInfoStru = DsuProtclBookIpInfoStru.pProtclIpInfoStru;
		(*dsuProtclTypeInfo).redunProtclNum = 0;
		(*dsuProtclTypeInfo).rsrProtclNum = 0;
		(*dsuProtclTypeInfo).rsspProtclNum = 0;
		(*dsuProtclTypeInfo).sfpProtclNum = 0;
		(*dsuProtclTypeInfo).rssp2ProtclNum = 0;
	
		for(i = 0;i < DsuProtclBookIpInfoStru.CommProtclStruLen; i++)/*�����豸�������룩ͨ��Э�飨��ϵ����*/
		{
			if(pTempCommProtclStru->EmitterType == TempEmitterType)/* ����Ϊ���ͷ� */
			{
				if(10 <= index)
				{
					retVal = 0;
					return retVal;
				}
				
				pLocalComProtclStru[index].CommuType=pTempCommProtclStru->CommuType;/* Э������ */
				pLocalComProtclStru[index].EmitterType=pTempCommProtclStru->EmitterType;
				pLocalComProtclStru[index].ReceptorType=pTempCommProtclStru->ReceptorType;
				index++;
	
				/*��ʼ��hasDesTypeIds*/
				hasDesTypeIds = (UINT16*)malloc(sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen);
				CommonMemSet(hasDesTypeIds, sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen, 0, sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen);
				hasDesTypeIdsLen = 0;
	
				tempDevNum=0;
				for(j = 0;j < DsuProtclBookIpInfoStru.ipDevLen; j++)/* ����IP�� */
				{
					tempDestType=dsuProtclGetDevType(pDevIpInfoStru[j].DevName);/* ��ȡIP���е��豸���� */
					if(tempDestType == pTempCommProtclStru->ReceptorType)/* �豸������Ŀ���豸������ͬ����Ϊ�뱾���豸ͨ�ŵ��豸 */
					{
						/*�������е�Ŀ������ID����,�鿴�Ƿ��Ѵ���*/
						isHas = 0;
						for(k = 0; k < hasDesTypeIdsLen; k++)
						{
							if(pDevIpInfoStru[j].DevName == hasDesTypeIds[k])
							{
								isHas = 1;
								break;
							}
						}
						
						/*δ�мӵ� ���е�Ŀ������ID������,��·����1*/
						if(!isHas)
						{
							hasDesTypeIds[hasDesTypeIdsLen] = pDevIpInfoStru[j].DevName;
							hasDesTypeIdsLen++;
							tempDevNum++;
						}
					}
					else
					{
						/* ���� */
					}
				}
				
				/*�ͷ�hasDesTypeIds*/
				free(hasDesTypeIds);
				hasDesTypeIds = NULL;
	
				switch(pTempCommProtclStru->CommuType)/* ����Э������ͣ���ȡ��ӦЭ�����͵ļ��� */
				{
					case DSU_PROTCL_SFP:
						(dsuProtclTypeInfo->sfpProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSSP:
						(dsuProtclTypeInfo->rsspProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSR:
						(dsuProtclTypeInfo->rsrProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RP:
						(dsuProtclTypeInfo->redunProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSSP2:
						(dsuProtclTypeInfo->rssp2ProtclNum) += tempDevNum;
						break;
					default:
						break;
				}
			}
			pTempCommProtclStru++;
		}		
		retVal = 1;
	}

	return retVal;
}

/*
* ������������ȡ�Է��豸�������Լ����豸���͵�ͨ���豸����
* ���������UINT16 devType �豸����
*           UINT8 offIndex  ����ƫ����
* ���������dataBuff �Է��豸��������  UINT16*
*           devNum  ���豸���͵�ͨ���豸����  UINT8*
* �� �� ֵ����
*/
static void GetDevName(UINT8 devType,UINT16* dataBuff,UINT8 offIndex,UINT8* devNum)
{
    UINT16 j=0;/* ѭ��ʹ�� */ 
	UINT8 tempDestType =0;
    DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/
	*devNum=0;
	pTempCommProtclStru=DsuProtclBookIpInfoStru.pCommProtclStru;
	for(j=0;j<DsuProtclBookIpInfoStru.ipDevLen;j++)
	{
		tempDestType=dsuProtclGetDevType(DsuProtclBookIpInfoStru.pProtclIpInfoStru[j].DevName);
		if(tempDestType == devType)
		{
			dataBuff[offIndex++]=DsuProtclBookIpInfoStru.pProtclIpInfoStru[j].DevName;
			(*devNum)++;
		}
		else
		{
			/* ���� */
		}
	}
}

/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuProtclGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 GetRedNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
    UINT16 i=0;/* ѭ��ʹ�� */
	UINT8 flag=0;
    UINT16 ipInfoLen=0;
	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrRed[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrRed[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrRed[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpRedPort)||(port==pTempIpInfoStru[i].RsrRedPort)||
			(port==pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	return retnVal;
}

/*
* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 GetBlueNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
	UINT16 i=0;/* ѭ��ʹ�� */
	UINT8 flag=0;
	UINT16 ipInfoLen=0;
	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrBlue[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrBlue[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrBlue[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpBluePort)||(port==pTempIpInfoStru[i].RsrBluePort)||
			(port==pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	return retnVal;
}

/*
* ���������������豸���ƻ�ȡ��������IP��ַ�Ͷ˿ں�
* ���������UINT16 devName �豸����
* ����������豸IP����Ϣ*
* �� �� ֵ��1:�������� 0�����󷵻�
* ��������:201701
* ������:zlj
*/
UINT8 GetIpInfoAcordDevName(UINT8 protclType,UINT16 destDevName,PROTCL_IP_INFO_STRU*pIpInfo)
{
	UINT8 retnVal = 0;
	UINT8 tempI = 0;
	UINT8 index = 0;
	UINT16 i=0;/* ѭ��ʹ�� */
	UINT16 ipInfoLen=0;

	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;
	if(NULL != pIpInfo)
	{
		for(i=0;i<ipInfoLen;i++)
		{
			if(destDevName == pTempIpInfoStru->DevName)
			{

				for (tempI =0;tempI <4;tempI++)
				{
					pIpInfo->DstIpInfo[index].RedIp[tempI] = pTempIpInfoStru->IpAddrRed[tempI];
				}

				/*����Э�����ͻ�ȡ��Ӧ�ĺ����˿ں�*/
				pIpInfo->DstIpInfo[index].PortRed = GetPortAcordProtclType(protclType, 1, pTempIpInfoStru);
				
				for (tempI =0;tempI <4;tempI++)
				{
					pIpInfo->DstIpInfo[index].BlueIp[tempI] = pTempIpInfoStru->IpAddrBlue[tempI];
				}
				/*����Э�����ͻ�ȡ��Ӧ�������˿ں�*/
				pIpInfo->DstIpInfo[index].PortBlue = GetPortAcordProtclType(protclType, 2, pTempIpInfoStru);

				index++;
				retnVal = 1;
			}
			
			pTempIpInfoStru++;
		}
		pIpInfo->dstNum = index;
	}
	else
	{
		retnVal = 0;
	}

	return retnVal;
}

/*
* ��������������Э�����ͻ�ȡ��Ӧ�Ķ˿ں�
* ���������UINT8 protclType Э������
			UINT8 redOrBlue ����������(1��ʾ������2��ʾ����)
			DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru IP��ַ�Ͷ˿ں���Ϣ
* ���������
* �� �� ֵ���˿ں�
* ��������:201701
* ������:zlj
*/
UINT16 GetPortAcordProtclType(UINT8 protclType, UINT8 redOrBlue, DSU_PROTCL_IP_INFO_STRU*pTempIpInfoStru)
{
	UINT16 protclPort = 0;

	if(NULL != pTempIpInfoStru)
	{
		switch (protclType)
		{

			case DSU_PROTCL_SFP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->SfpRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->SfpBluePort;
				}
				break;

			case DSU_PROTCL_RP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RpRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RpBluePort;
				}
				break;

			case DSU_PROTCL_RSR:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RsrRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RsrBluePort;
				}
				break;

			case DSU_PROTCL_RSSP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RsrRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RsrBluePort;
				}
				break;

			case DSU_PROTCL_NTP:
				protclPort = NTP_PORT;
				break;

			default:
				protclPort = 0;
				break;

		}
	}
	else
	{
		protclPort = 0;
	}

	return protclPort;
}

