/********************************************************
*                                                      
*  �ļ���   �� CbtcVersion.h         
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.23
*  ����     �� �з����������
*  �������� �� �汾�Ÿ�ʽת�����                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/ 

#ifndef _CBTCVERSION_H_
#define _CBTCVERSION_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	* ����˵�� : ���ģ��汾��ת����
	* ����˵�� : UINT16 productNo,��Ʒ���
	*            UINT16 moduleNo,���ģ����
	*            UINT16 mainVer,���汾��
	*            UINT16 subVer���Ӱ汾��
	* ����ֵ:    U32��ʾ�İ汾��
	*/
	UINT32 VerToU32SoftwareModule(UINT16 productNo,UINT16 moduleNo,UINT16 mainVer,UINT16 subVer);

	/*
	* ����˵�� : ���ģ��汾��ת����
	* ����˵�� : UINT16* pProductNo,��Ʒ���
	*            UINT16* pModuleNo,���ģ����
	*            UINT16* pMainVer,���汾��
	*            UINT16* pSubVer���Ӱ汾��
	*            UINT32 u32Ver,U32��ʾ�İ汾��
	*/
	void VerFromU32SoftwareModule(UINT16* pProductNo,UINT16* pModuleNo,UINT16* pMainVer,UINT16* pSubVer,UINT32 u32Ver);

	/*
	* ����˵�� : ������ɰ汾��ת����
	* ����˵�� : UINT16 projectNo,��Ŀ���
	*            UINT16 productNo,��Ʒ���
	*            UINT16 ocNo,��ִ���ļ����
	*            UINT16 integrationNo,���ɱ��
	* ����ֵ:    U32��ʾ�İ汾��
	*/
	UINT32 VerToU32SoftwareInt(UINT16 projectNo,UINT16 productNo,UINT16 ocNo,UINT16 integrationNo);

	/*
	* ����˵�� : ������ɰ汾��ת����
	* ����˵�� : UINT16* pProjectNo,��Ŀ���
	*            UINT16* pProductNo,��Ʒ���
	*            UINT16* pOcNo,��ִ���ļ����
	*            UINT16* pIntegrationNo�����ɱ��
	*            UINT32 u32Ver,U32��ʾ�İ汾��
	*/
	void VerFromU32SoftwareInt(UINT16* pProjectNo,UINT16* pProductNo,UINT16* pOcNo,UINT16* pIntegrationNo,UINT32 u32Ver);

	/*
	* ����˵�� : ����ģ��汾��ת����
	* ����˵�� : UINT16 projectNo,��Ŀ���
	*            UINT16 dataTypeNo,�������ͱ��
	*            UINT16 dataNo,���ݰ汾��
	*            UINT16 reserved,Ԥ����Ϣ
	* ����ֵ:    U32��ʾ�İ汾��
	*/
	UINT32 VerToU32DataModule(UINT16 projectNo,UINT16 dataTypeNo,UINT16 dataNo,UINT16 reserved);

	/*
	* ����˵�� : ����ģ��汾��ת����
	* ����˵�� : UINT16* pProjectNo,��Ŀ���
	*            UINT16* pDataTypeNo,�������ͱ��
	*            UINT16* pDataNo,���ݰ汾��
	*            UINT16* pReserved��Ԥ����Ϣ
	*            UINT32 u32Ver,U32��ʾ�İ汾��
	*/
	void VerFromU32DataModule(UINT16* pProjectNo,UINT16* pDataTypeNo,UINT16* pDataNo,UINT16* pReserved,UINT32 u32Ver);

	/*
	* ����˵�� : ���ݼ��ɰ汾��ת����
	* ����˵�� : UINT16 projectNo,��Ŀ���
	*            UINT16 dataFileNo,�����ļ����
	*            UINT16 dataIntNo,���ݼ��ɰ汾��
	*            UINT16 reserved,Ԥ����Ϣ
	* ����ֵ:    U32��ʾ�İ汾��
	*/
	UINT32 VerToU32DataInt(UINT16 projectNo,UINT16 dataFileNo,UINT16 dataIntNo,UINT16 reserved);

	/*
	* ����˵�� : ����ģ��汾��ת����
	* ����˵�� : UINT16* pProjectNo,��Ŀ���
	*            UINT16* pDataFileNo,�����ļ����
	*            UINT16* pDataIntNo,���ݼ��ɰ汾��
	*            UINT16* pReserved��Ԥ����Ϣ
	*            UINT32 u32Ver,U32��ʾ�İ汾��
	*/
	void VerFromU32DataInt(UINT16* pProjectNo,UINT16* pDataFileNo,UINT16* pDataIntNo,UINT16* pReserved,UINT32 u32Ver);

#ifdef __cplusplus
}
#endif

#endif


