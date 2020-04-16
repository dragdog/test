/*******************************************************************************
*
* �ļ���  ��dsuRedunFunc.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������RpЭ����ص�dsu��ѯ����������
* ʹ��ע�⣺��
* �޸ļ�¼����
*
*******************************************************************************/
#include "CommonMemory.h"
#include "dsuRedunFunc.h"
#include "Convert.h"
#include "string.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquCbtcType.h"

#define DSURP_PROTCL_SFP_WL   ((UINT8)1)
#define DSURP_PROTCL_RP_WL    ((UINT8)2)
#define DSURP_PROTCL_RSR_WL   ((UINT8)3)
#define DSURP_PROTCL_RSSP_WL   ((UINT8)4)

/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE_WL 22   
#define DSU_COMM_PROTCL_STRU_SIZE_WL 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE_WL 12
/*Դ��FAO��34��Ϊ(34+30)��BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL 34

DSU_BOOK_IP_INFO_STRU_WL DsuBookIpInfoStru_WL;     /*��ʼ��֮�����ڱ����ʼ��֮������ݵı���*/

/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuIpInit_WL(UINT8*FSName)
{
	UINT8 chReturnValue = 1 ; /*����ֵ*/
	UINT8 tempRet;
	UINT8* pData = NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen = 0;        /*���ݳ���*/
	UINT32 dataOff = 0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNumWL = 0;/*��ʱ��¼ZCDSU��Ŀ�����*/
	UINT8 cbtcVersionType = 0;

	if(NULL == FSName)
	{
		chReturnValue = 0;
	}
	cbtcVersionType = GetCbtcSysType();

	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru_WL.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE_WL);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru_WL.pIpInfoStru=(DSU_IP_INFO_STRU_WL*)malloc(sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru_WL.pIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru_WL.pIpInfoStru,sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru_WL.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR_WL);
			dataOff+=DSURP_LEN_IP_ADDR_WL;

			memcpy(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR_WL);
			dataOff+=DSURP_LEN_IP_ADDR_WL;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].RsrBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].RsrRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	

	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru_WL.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE_WL);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru_WL.pCommProtclStru=(DSU_COMM_PROTCL_STRU_WL*)malloc(sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru_WL.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru_WL.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru_WL.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru_WL.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru_WL.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru_WL.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	

	/*��ʼ��CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru_WL.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE_WL);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru_WL.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru_WL.pCcovCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru_WL.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru_WL.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{
			/*����ִ��*/
		}
	}	

	/*��ʼ��ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_HLT== cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
	{
		DsuBookIpInfoStru_WL.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL_FAO);  /*�����������ݵĸ���*/
	}
	else
	{
		DsuBookIpInfoStru_WL.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL);  /*�����������ݵĸ���*/
	}
	DsuBookIpInfoStru_WL.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru_WL.pZcdsuCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru_WL.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen));
		dataOff=0;

		if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_HLT== cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
		{
			tmpDsuMaxZcDsuCommObjNumWL = DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO;  /*�����������ݵĸ���*/
		}
		else 
		{
			tmpDsuMaxZcDsuCommObjNumWL = DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL;  /*�����������ݵĸ���*/
		}				
		
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru_WL.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNumWL;j++)
			{
				DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}

		if (dataOff!=dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue=0;
		}
		else
		{

		}
	}
	
	/*����ִ��*/
	return chReturnValue;
}

/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuRpGetDevType_WL(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* ������������ѯ�豸IP
* ���������UINT16 wLocalID���豸id
UINT8 chLocalType���豸����
* ���������UINT32 * RedIPInfo������ip
UINT32 * BlueIPInfo������ip
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckIPInfoOfObject_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo)
{
	UINT8 chReturnValue;/*��������ֵ*/
	UINT16 i;/*ѭ����*/
	UINT16 tempDevName; /*��ʱ����devName*/
	
	if (wLocalID>255)
	{
		/*�趨���������Ͳ�������Ҫ�����ش���*/
		chReturnValue=0;
		return chReturnValue;
	}
	
	/*����dev����*/
	tempDevName=(UINT16)(((chLocalType<<8)+wLocalID)&0xffff);
	
	/*ѭ�������豸ip��ַ*/
	for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
	{
		if (DsuBookIpInfoStru_WL.pIpInfoStru[i].DevName==tempDevName)
		{
			/*���ҵ���ǰ�豸*/
			*RedIPInfo=LongFromChar(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrRed);
			*BlueIPInfo=LongFromChar(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrBlue);
			break;
		}
		else
		{
			/*��������*/
		}
	}
	
	if(i==DsuBookIpInfoStru_WL.IpInfoStruLen)
	{
		/*δ�ҵ��豸*/
		chReturnValue=0;
	}
	else
	{
		/*���ҵ��豸*/
		chReturnValue=1;
	}
	
	return chReturnValue;
	
}


/*
* �������������뱾ZC��ID�������ZC����ͨ�Ŷ�����Ϣ��������ZC��DSU��ATS��CI��ʱ�ӷ������ȡ�
* ���������UINT16 wLocalID�����豸id
UINT8 chLocalType���豸����
* ���������UINT32 * wObjectNum, ͨ�Ŷ�������
UINT16 *pwObjectInfo,ͨ�Ŷ�������Ϣ������ID��Ϣ��������Ϣ
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckCommObjectInfo_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo)
{
	UINT16 localName;  /*���ݱ����豸id��type����������豸����*/
    UINT16 i;
	UINT8 bRet;
	
	if (wLocalID>255)
	{
		/*�趨���������Ͳ�������Ҫ�����ش���*/
		bRet=0;
		return bRet;
	}
	
	/*�����豸type���豸id�����豸����*/
	localName=(chLocalType<<8)+(wLocalID&0xff);
	
	/*ѭ�������豸*/
	for (i=0;i<DsuBookIpInfoStru_WL.ZcdsuCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].LocalDevName==localName)
		{
			/*�ҵ��豸�������������*/
			* wObjectNum=DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjNum;
			memcpy(pwObjectInfo,DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjBuf,(* wObjectNum)*2);
			
			/*�������سɹ�*/
			break;
			
		}
	}
	
	/*ѭ���������ж��Ƿ��ҵ��豸*/
	if (i==DsuBookIpInfoStru_WL.ZcdsuCommInfoLen)
	{
		/*û���ҵ��豸������ʧ��*/
		bRet=0;
	}
	else
	{
		/*�ҵ��豸�����سɹ�*/
		bRet=1;
	}
	
	return bRet;
}

/*
* �����������ú������ڻ�ȡ���ദ��ͨ�Žڵ��������Ϣ���ú���ר����RSSPЭ�顣
* ���������UINT16 LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestDevName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRpGetLnkInfoWl(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo)
{
	UINT8 chReturnValue;/*��������ֵ*/
	UINT16 i;/*ѭ����*/

	UINT8 TempEmitterType;/*�������豸����*/
	UINT8 TempReceptorType;/*�������豸����*/

	UINT8 TempCommuType=0;

	DSU_COMM_PROTCL_STRU_WL *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

	DSU_IP_INFO_STRU_WL *pTempIpInfoStru;


	/*��ȡ����������豸����*/
	TempEmitterType=dsuRpGetDevType_WL(LocalName);
	TempReceptorType=dsuRpGetDevType_WL(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru_WL.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru_WL.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)
			&&(pTempCommProtclStru->ReceptorType==TempReceptorType))
		{

			TempCommuType=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru_WL.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue=0;
		return chReturnValue;
	}

	pRpLnkInfo->srcNum=0;
	pRpLnkInfo->dstNum=0;

	/*ͨ��ѭ����������IP��ַ*/
	pTempIpInfoStru= DsuBookIpInfoStru_WL.pIpInfoStru;
	for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
	{
		/*����Դ�豸*/
		if (pTempIpInfoStru->DevName==LocalName)
		{
			/*��ֵIP��ַ*/
			
			memcpy(pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].BlueIp,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].RedIp,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));
			

			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if ( TempCommuType == DSURP_PROTCL_RP_WL )
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->RpRedPort;

			}
			else if ((TempCommuType == DSURP_PROTCL_RSR_WL)||(TempCommuType == DSURP_PROTCL_RSSP_WL))
			{
				/*��ǰ�豸ͨ��ʹ��RsrЭ��*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->RsrBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->RsrRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP_WL )
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*���ݴ���*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*�ҵ���IP��ַ����1*/
			pRpLnkInfo->srcNum++;
		}
		else
		{
			/*��ǰIP�豸Name������LocalDevName����ͬ��ʲôҲ����*/
		}

		/*����Ŀ���豸*/
		if(pTempIpInfoStru->DevName==DestName)
		{
			/*��ֵIP��ַ*/
			memcpy(pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].BlueIp,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].RedIp,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));


			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if (TempCommuType==DSURP_PROTCL_RP_WL)
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->RpRedPort;

			}
			else if ((TempCommuType==DSURP_PROTCL_RSR_WL)||(TempCommuType==DSURP_PROTCL_RSSP_WL))
			{
				/*��ǰ�豸ͨ��ʹ��RpЭ��*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->RsrBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->RsrRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_SFP_WL)
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*���ݴ���*/
				chReturnValue=0;
				return chReturnValue;
			}


			pRpLnkInfo->dstNum++;
		}
		else
		{
			/*��ǰIP�豸Name������DestDevName����ͬ��ʲôҲ����*/
		}

		pTempIpInfoStru++;
	}

	if ((pRpLnkInfo->dstNum==0)||(pRpLnkInfo->srcNum==0))
	{
		/*Ŀ�����ԴIP��ַδ�ҵ�����������ʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*���ҵ���Ӧ��ip���豸��Ϣ*/
		chReturnValue=1;
		return chReturnValue;
	}

}