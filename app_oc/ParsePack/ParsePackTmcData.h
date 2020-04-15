/********************************************************
*                                                                                                            
* �� �� ���� ParsePackVobcData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Vobc���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_TMC_DATA_H_
#define _PARSE_PACK_TMC_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "CiInterfaceDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../ParsePack/ParsePackAtsData.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../ParsePack/ParsePackPlatformData.h"
#include "../PSD/PSDConfigDataManage.h"
#include "../PSD/PSDDataManage.h"
#include "../System/SystemParameterManage.h"
#include "dquIdmapQuery.h"


#define OC_TMC_INFO_TYPE_CONTROL 0x0101 /*OC->TMC�ӿڣ����ڷ���*/
#define TMC_OC_INFO_TYPE_CONTROL 0x0102 /*TMC->OC�ӿڣ����ڷ���*/

#define TMC_ID 0x01 /* TMC ID */

#define OC_TMC_INTERFACE_INFOTYPE    0x0101 /*TMC-OC�ӿ���Ϣ����*/
#define OC_TMC_INFO_TYPE   0x0101 /*OC->TMC��Ϣ�����ڷ���*/
#define TMC_OC_INFO_TYPE   0x0102 /*TMCC->OC��Ϣ�����ڷ���*/
#define OC_TMC_PRTCL_VER   0x10 /*OC_TMCЭ��汾��*/


#ifdef __cplusplus
extern "C" {
#endif


#ifndef SFP_PROTOCOL


/*������ͨ�ŵ�Vobc��Ϣ*/
typedef struct S_TmcToOcDataStruct
{
	UINT16 TmcId;                                           /*Tmc ID*/
	UINT16 appInfoType;										/*Ӧ����Ϣ����*/
} TmcToOcDataStruct;


/*
* ���������� ����VOBC����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseVobcToCiData(void);

#endif

/*
* ���������� ��������IVOC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseIvocToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId,const UINT16 logId);

/*
* ���������� ��������TMC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseTmcToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);


/*
* ���������� ��TMC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackOcToTmcData(QueueStruct* ocToTmcDataQueue);


/*
* ���������� ��VOBC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue);

/*
* ���������� ��ȡVOBC��CI����ṹ����������
* ����˵���� void    
* ����ֵ  �� ��������    
*/
UINT8 GetVobcToCiSum(void);

/*
* ���������� ��ȡ�����ű�����Ϣ
* ����˵���� const UINT16 psdId :������ID    
* ����ֵ  �� 01:�ı���
*            02:������
*            CI_ERROR: ���Ϸ�
*/
UINT8 GetPsdEditGroupInfo(const UINT16 psdId);


#ifdef __cplusplus
}
#endif

#endif
