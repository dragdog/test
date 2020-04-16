/********************************************************
*                                                      
*  �ļ���   �� CbtcVersion.c         
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.23
*  ����     �� �з����������
*  �������� �� �汾�Ÿ�ʽת�����                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/ 

#include "CbtcVersion.h"


/*
* ����˵�� : ���ģ��汾��ת����
* ����˵�� : UINT16 productNo,��Ʒ���
*            UINT16 moduleNo,���ģ����
*            UINT16 mainVer,���汾��
*            UINT16 subVer���Ӱ汾��
* ����ֵ:    U32��ʾ�İ汾��
*/
UINT32 VerToU32SoftwareModule(UINT16 productNo,UINT16 moduleNo,UINT16 mainVer,UINT16 subVer)
{
	UINT32 U32Version = 0; /* ���ؽ�� */
	
	U32Version=subVer&0xFF;
	U32Version|=((mainVer&0x3F)<<8);
	U32Version|=((moduleNo&0x3FF)<<14);
	U32Version|=((productNo&0xFF)<<24);
	return U32Version;
}

/*
* ����˵�� : ���ģ��汾��ת����
* ����˵�� : UINT16* pProductNo,��Ʒ���
*            UINT16* pModuleNo,���ģ����
*            UINT16* pMainVer,���汾��
*            UINT16* pSubVer���Ӱ汾��
*            UINT32 u32Ver,U32��ʾ�İ汾��
*/
void VerFromU32SoftwareModule(UINT16* pProductNo,UINT16* pModuleNo,UINT16* pMainVer,UINT16* pSubVer,UINT32 u32Ver)
{

	*pProductNo=u32Ver>>24;
	*pModuleNo=(u32Ver&0x00FFC000)>>14;
	*pMainVer=(u32Ver&0x00003F00)>>8;
	*pSubVer=u32Ver&0x000000FF;

	return ;
}

/*
* ����˵�� : ������ɰ汾��ת����
* ����˵�� : UINT16 projectNo,��Ŀ���
*            UINT16 productNo,��Ʒ���
*            UINT16 ocNo,��ִ���ļ����
*            UINT16 integrationNo,���ɱ��
* ����ֵ:    U32��ʾ�İ汾��
*/
UINT32 VerToU32SoftwareInt(UINT16 projectNo,UINT16 productNo,UINT16 ocNo,UINT16 integrationNo)
{
	UINT32 U32Version = 0; /* ���ؽ�� */

	U32Version=integrationNo&0x01FF;
	U32Version|=((ocNo&0x7F)<<9);
	U32Version|=((productNo&0x3FF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* ����˵�� : ������ɰ汾��ת����
* ����˵�� : UINT16* pProjectNo,��Ŀ���
*            UINT16* pProductNo,��Ʒ���
*            UINT16* pOcNo,��ִ���ļ����
*            UINT16* pIntegrationNo�����ɱ��
*            UINT32 u32Ver,U32��ʾ�İ汾��
*/
void VerFromU32SoftwareInt(UINT16* pProjectNo,UINT16* pProductNo,UINT16* pOcNo,UINT16* pIntegrationNo,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pProductNo=(u32Ver&0x00FF0000)>>16;
	*pOcNo=(u32Ver&0x0000FE00)>>9;
	*pIntegrationNo=u32Ver&0x000001FF;

	return ;
}

/*
* ����˵�� : ����ģ��汾��ת����
* ����˵�� : UINT16 projectNo,��Ŀ���
*            UINT16 dataTypeNo,�������ͱ��
*            UINT16 dataNo,���ݰ汾��
*            UINT16 reserved,Ԥ����Ϣ
* ����ֵ:    U32��ʾ�İ汾��
*/
UINT32 VerToU32DataModule(UINT16 projectNo,UINT16 dataTypeNo,UINT16 dataNo,UINT16 reserved)
{
	UINT32 U32Version = 0; /* ���ؽ�� */

	U32Version=reserved&0x0F;
	U32Version|=((dataNo&0xFFF)<<4);
	U32Version|=((dataTypeNo&0xFF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* ����˵�� : ����ģ��汾��ת����
* ����˵�� : UINT16* pProjectNo,��Ŀ���
*            UINT16* pDataTypeNo,�������ͱ��
*            UINT16* pDataNo,���ݰ汾��
*            UINT16* pReserved��Ԥ����Ϣ
*            UINT32 u32Ver,U32��ʾ�İ汾��
*/
void VerFromU32DataModule(UINT16* pProjectNo,UINT16* pDataTypeNo,UINT16* pDataNo,UINT16* pReserved,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pDataTypeNo=(u32Ver&0x00FF0000)>>16;
	*pDataNo=(u32Ver&0x0000FFF0)>>4;
	*pReserved=u32Ver&0x0F;
	return ;
}

/*
* ����˵�� : ���ݼ��ɰ汾��ת����
* ����˵�� : UINT16 projectNo,��Ŀ���
*            UINT16 dataFileNo,�����ļ����
*            UINT16 dataIntNo,���ݼ��ɰ汾��
*            UINT16 reserved,Ԥ����Ϣ
* ����ֵ:    U32��ʾ�İ汾��
*/
UINT32 VerToU32DataInt(UINT16 projectNo,UINT16 dataFileNo,UINT16 dataIntNo,UINT16 reserved)
{
	UINT32 U32Version = 0; /* ���ؽ�� */

	U32Version=reserved&0x0F;
	U32Version|=((dataIntNo&0xFFF)<<4);
	U32Version|=((dataFileNo&0xFF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* ����˵�� : ����ģ��汾��ת����
* ����˵�� : UINT16* pProjectNo,��Ŀ���
*            UINT16* pDataFileNo,�����ļ����
*            UINT16* pDataIntNo,���ݼ��ɰ汾��
*            UINT16* pReserved��Ԥ����Ϣ
*            UINT32 u32Ver,U32��ʾ�İ汾��
*/
void VerFromU32DataInt(UINT16* pProjectNo,UINT16* pDataFileNo,UINT16* pDataIntNo,UINT16* pReserved,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pDataFileNo=(u32Ver&0x00FF0000)>>16;
	*pDataIntNo=(u32Ver&0x0000FFF0)>>4;
	*pReserved=u32Ver&0x0F;
	return ;
}

