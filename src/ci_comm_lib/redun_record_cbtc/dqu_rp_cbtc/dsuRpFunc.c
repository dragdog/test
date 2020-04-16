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
#include "dsuRpFunc.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"

#define DSURP_PROTCL_SFP   ((UINT8)1)
#define DSURP_PROTCL_RP    ((UINT8)2)
#define DSURP_PROTCL_RSR   ((UINT8)3)

/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE 22   
#define DSU_COMM_PROTCL_STRU_SIZE 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE 12
/*Դ��FAO��34��Ϊ(34+30)��BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE (34)

 /*FAO��HLT���ã�BY LJ,20161210*/
#define DSU_GATEWAY_INFO_STRU_SIZE 15

DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     /*��ʼ��֮�����ڱ����ʼ��֮������ݵı���*/

/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*�ϲ�˵������CPK���룬BY LJ,20161210
*/
static UINT8 dsuIpInit_CPK(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;

	/*��Ч�Լ��*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<DSU_MAX_ZCDSU_COMM_OBJ_NUM;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
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
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������UINT8* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*�ϲ�˵����FAO��HLT���ã�BY LJ,20161210
* ��������:201701
* ������:zlj
*/
static UINT8 dsuIpInit_DFT(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*����ֵ*/
	UINT8 tempRet;
	UINT8* pData=NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen=0;        /*���ݳ���*/
	UINT32 dataOff=0;        /*����ƫ����*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNum = 0;/*��ʱ��¼ZCDSU��Ŀ�����*/
	UINT8 cbtcVersionType=0;

	/*��Ч�Լ��*/
	if(NULL == FSName)
	{
		chReturnValue=0;
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

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
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

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
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

	
	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO);  /*�����������ݵĸ���*/
	
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		
		tmpDsuMaxZcDsuCommObjNum = DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO;  /*�����������ݵĸ���*/
		
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNum;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
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
			/*����ִ��*/
		}
	}

	

	/*hxq,��ʼ���豸���ر���Ϣ*/
	/*��ȡ����*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.GatewayInfoLen=(UINT16)(dataLen/DSU_GATEWAY_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pGatewayInfoStru=(DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pGatewayInfoStru==NULL)
	{
		/*�ռ����ʧ��*/
		chReturnValue=0;
	}
	else
	{
		/*����ִ��*/
		CommonMemSet(DsuBookIpInfoStru.pGatewayInfoStru,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen),0,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen));
		dataOff=0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
		{
			/*��� 2*/
			dataOff+=2;

			/*�豸���� 1*/
			DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceType = pData[dataOff];
			dataOff++;

			/*�豸�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*�豸�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*�������� 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrSubnetMask,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

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


/*��������: ���ʽӿ�
* �� �� ֵ��1���ɹ�
*           0��ʧ��
* ��������:201701
* ������:zlj
*/
UINT8 dsuIpInit(UINT8*FSName)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	/*��Ч�Լ��*/
	if(NULL != FSName)
	{			
		cbtcVersionType=GetCbtcSysType();

		if (DQU_CBTC_CPK == cbtcVersionType )
		{
		
			rtnValue = dsuIpInit_CPK(FSName);		
		} 
		else if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
		{
			rtnValue = dsuIpInit_DFT(FSName);
		}
		else
		{
			rtnValue = 0;		
		}
	}
	else
	{
		rtnValue = 0;		
	}
	
	return rtnValue; 
}

/*
* �����������ú������ڻ�ȡ�����ͨ�Žڵ��������Ϣ���ú���ר��������㡣
* ���������UINT16 LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestDevName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_RP_LINK_INFO_STRU* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRpGetLnkInfo(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo)
{
	UINT8 chReturnValue;/*��������ֵ*/
	UINT16 i;/*ѭ����*/

	UINT8 TempEmitterType;/*�������豸����*/
	UINT8 TempReceptorType;/*�������豸����*/

	UINT8 TempCommuType=0;

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

	DSU_IP_INFO_STRU *pTempIpInfoStru;
	UINT16 FindIPCount=0;  /*���ڱ����ҵ���IP��ַ������FindIPCountΪ2ʱ�����ҽ���*/

	/*��ȡ����������豸����*/
	TempEmitterType=dsuRpGetDevType(LocalName);
	TempReceptorType=dsuRpGetDevType(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)
			&&(pTempCommProtclStru->ReceptorType==TempReceptorType))
		{
			
			TempCommuType=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue=0;
		return chReturnValue;
	}
	
	/*ͨ��ѭ����������IP��ַ*/
	pTempIpInfoStru= DsuBookIpInfoStru.pIpInfoStru;
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		/*����Դ�豸*/
		if (pTempIpInfoStru->DevName==LocalName)
		{
			/*��ֵIP��ַ*/
			memcpy(pRpLnkInfo->LocalIPBlue,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->LocalIPRed,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));

			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if ( TempCommuType == DSURP_PROTCL_RP )
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_RSR )
			{
				/*��ǰ�豸ͨ��ʹ��RpЭ��*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP )
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*���ݴ���*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*�ҵ���IP��ַ����1*/
			FindIPCount++;
		}
		else
		{
			/*��ǰIP�豸Name������LocalDevName����ͬ��ʲôҲ����*/
		}

		/*����Ŀ���豸*/
		if(pTempIpInfoStru->DevName==DestName)
		{
			/*��ֵIP��ַ*/
			memcpy(pRpLnkInfo->DestIPBlue,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->DestIPRed,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));


			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if (TempCommuType==DSURP_PROTCL_RP)
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_RSR)
			{
				/*��ǰ�豸ͨ��ʹ��RpЭ��*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_SFP)
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*���ݴ���*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*�ҵ���IP��ַ����1*/
			FindIPCount++;
		}
		else
		{
			/*��ǰIP�豸Name������DestDevName����ͬ��ʲôҲ����*/
		}

		if(FindIPCount>=2)
		{
			/*���ҵ�Ŀ���ԴIP��ַ���˳�ѭ��*/
			break;
		}
		else
		{
			/*δȫ�ҵ�Ŀ���IP��ַ����������*/
		}
		pTempIpInfoStru++;
	}

	if (FindIPCount<2)
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



/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuRpGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/***********************************************************************
 * ������   : dsuRpTypeId2DevName
 * �������� : ����Type��ID ȡ�����Ӧ���豸��ʶ
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	 Type		UINT8			IN				����
 *	 ID			UINT8			IN				Id
 *	 pDevName	UINT16*			OUT				��ʶ
 * 
 * ����ֵ   : ��
 ***********************************************************************/
void dsuRpTypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName)
{
	*pDevName = Type;
	*pDevName = (*pDevName)*0x100 + Id;
}

/*
* �����������ú������ڸ��ݱ����Լ��Է����豸���ͻ�ȡ��ʹ�á�
* ���������UINT16 LocalName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������UINT8* protclTye Э������
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsurRpGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye)
{
	UINT8 chReturnValue;/*��������ֵ*/
	UINT16 i;/*ѭ����*/

	UINT8 TempEmitterType;/*�������豸����*/
	UINT8 TempReceptorType;/*�������豸����*/

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

	/*��ȡ����������豸����*/
	TempEmitterType=dsuRpGetDevType(LocalName);
	TempReceptorType=dsuRpGetDevType(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)&&(pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			*protclTye=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue=0;
		return chReturnValue;
	}

	chReturnValue =1;
	return chReturnValue;
}

/*
* ������������ѯ�豸IP
* ���������UINT16 wLocalID���豸id
            UINT8 chLocalType���豸����
* ���������UINT32 * RedIPInfo������ip
            UINT32 * BlueIPInfo������ip
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckIPInfoOfObject(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo)
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
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		if (DsuBookIpInfoStru.pIpInfoStru[i].DevName==tempDevName)
		{
			/*���ҵ���ǰ�豸*/
			*RedIPInfo=LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed);
			*BlueIPInfo=LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue);
			break;
		}
		else
		{
			/*��������*/
		}
	}

	if(i==DsuBookIpInfoStru.IpInfoStruLen)
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
UINT8 CheckCommObjectInfo(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo)
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
	for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName==localName)
		{
			/*�ҵ��豸�������������*/
			* wObjectNum=DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum;
			memcpy(pwObjectInfo,DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf,(* wObjectNum)*2);

			/*�������سɹ�*/
			break;

		}
	}

	/*ѭ���������ж��Ƿ��ҵ��豸*/
	if (i==DsuBookIpInfoStru.ZcdsuCommInfoLen)
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

/**=============================================================================
      ������: UINT8 CheckLocalGatewayInfo
      ����: ��ѯCC��Ӧ�ĺ���������      
	  ����: ����������豸���ͺͺ�������������ѯ�õ���Ӧ������
	  ���������UINT8 localType���豸����
	            UINT8 computerLocalId���豸����
	  ���ز�����UINT8 localGateWayBuff[DSURP_LEN_IP_ADDR]    ����IP
	  ����ֵ:   UINT8 rtnValue   0x00    �����豸����ʧ�ܣ�   
				                 0x01    �����豸���سɹ��� 
	  �ϲ�˵������FAO��HLT���ã�BY LJ,20161210
==============================================================================*/
UINT8 CheckLocalGatewayInfo(UINT8 localType , UINT8 computerLocalId ,  UINT8 localGateWayBuff[DSURP_LEN_IP_ADDR])
{
	UINT8  i = 0;                /*ѭ�����Ʊ���*/
	UINT8  gatewayFlag = 0x55;   /*��ѯ�����ر�־λ*/
	UINT8  rtnValue = 0x00;      /*��������ֵ*/
	UINT8  j = 0;                /*ѭ�����Ʊ���*/

	/*ѭ�������豸*/
	for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceType == localType)
		{
			if (ZHUJI_5 == computerLocalId)
			{
				/*�豸��������*/
				memcpy(localGateWayBuff,DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,DSURP_LEN_IP_ADDR);
				/*�ҵ��豸�����سɹ�*/
				gatewayFlag = 0xaa;
			}
			else if (ZHUJI_6 == computerLocalId)
			{
				/*�豸��������*/
				memcpy(localGateWayBuff,DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,DSURP_LEN_IP_ADDR);
				/*�ҵ��豸�����سɹ�*/
				gatewayFlag = 0xaa;
			}
			else
			{
				/*����ʧ��*/
				gatewayFlag = 0x55;
			}

			/*����ѭ��*/
			break;
		}
	}

	/*ѭ���������ж��Ƿ��ҵ��豸*/
	if ((i == DsuBookIpInfoStru.GatewayInfoLen)||(0x55 == gatewayFlag))
	{
		/*û���ҵ��豸������ʧ��*/
		rtnValue = 0;
	}
	else
	{
		/*�ҵ��豸�����سɹ�*/
		rtnValue = 1;
	}

	return rtnValue;
}