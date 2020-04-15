/********************************************************
*                                                                                                            
* �� �� ���� ParsePackPlatformData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2014-12-25
* ��    �ߣ� ������
* ���������� PSD���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_PLATFORM_DATA_H_
#define _PARSE_PACK_PLATFORM_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "CiInterfaceDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../PSD/PSDConfigDataManage.h"
#ifdef __cplusplus
extern "C" {
#endif



/*������ͨ�ŵ�Psd��Ϣ*/
typedef struct S_PsdToCiDataStruct
{
    UINT16 PsdId;                                /*Psd ID*/
	UINT8 doorHaveOrNot;						/*����������Ϣ���г�����Ϣ��0x55���޳�����Ϣ��0xaa*/
	UINT8  doorEditGroupInfo;                    /*�г�������Ϣ:����VOBC(��̬�ֶ�)*/
	UINT32 faultIsolationRequestInfo;            /*���ϸ�������:����VOBC(��̬�ֶ�)*/
	UINT32 faultIsolationReplyInfo;              /*���ϸ���ظ�:����PSDϵͳ(��̬�ֶ�)����ȫ�Ź���״̬*/
	UINT32 psdState;							/*bit0��bit23��ʾ1��24�������ŵĴ�״̬��bitΪ0��ʾ�ر�������״̬��Ϊ1��ʾ��״̬(�ǹر�������)��bit0-bit7Ϊ��1�ֽڣ���ʾ1-8�����Ŵ�״̬��*/
	UINT32 vobcToPsdfaultIsolationRequest[2];   /*���ϸ�������:����VOBC(��̬�ֶ�)��8�ֽ�*/
	UINT32 psdToVobcfaultIsolationReply[2];     /*���ϸ���״̬�ظ�:����PSDϵͳ(��̬�ֶ�)����ȫ�Ź���״̬��8�ֽڣ�1��ʾ�޹��ϣ�0��ʾ���ϣ�δ����bitĬ����д0*/
	UINT32 psdOpenCloseState[2];				/*bit0��bit31��ʾ1��32�������ŵĴ�״̬��bitΪ1��ʾ�ر�������״̬��Ϊ0��ʾ��״̬(�ǹر�������)��bit0-bit7Ϊ��1�ֽڣ���ʾ1-8�����Ŵ�״̬��δ����bitĬ����д0*/
} PsdToCiDataStruct;


typedef struct S_PlatformToCiDataStruct
{
	UINT8 stationId;                            				   /*��վ���:�����ֶ�*/
	UINT8 psdSum;                              					   /*����������: �����ֶ�*/
	PsdToCiDataStruct  psdDataStr[PLATFORM_INCLUDE_PSD_MAX_SUM];   /*��������Ϣ�ṹ������*/
}PlatformToCiDataStruct;



/*
* ���������� վ̨���ݽṹ��ʼ��
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 InitialPlatformData(void);


/*
* ���������� ����PSD����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParsePlatformToCiData(void);


/*
* ���������� ��������PSD��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParsePlatformToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);


/*
* ���������� ��PSD����
* ����˵���� QueueStruct* CiToPlatformDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToPlatformData(QueueStruct* CiToPlatformDataQueue,UINT8 deviceId);


/*
* ���������� ��ȡPlatform-CI�Ľṹ������
* ����˵���� void    
* ����ֵ  �� Platform-CI�ṹ������ָ��      
*/
PlatformToCiDataStruct * GetPlatformToCiDataStru(void);

/*
* ����������Ϊָ��PSD->CI�Ľṹ����ȱʡֵ(��ȫֵ) 
* ����˵���� platformId:PSDϵͳ��ID
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 SetPlatformToCiDefaultDataStru(UINT8 platformId);

/*
* ���������� ����ָ��ID������״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT32 isolateState, �����Ź��ϸ���״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdIsolateState(const UINT16 doorId, const UINT32 isolateState);

/*
* ���������� ��ȡָ��ID�����Ź��ϸ���״̬
* ����˵���� const UINT16 doorId, ������ID
* ����ֵ  �� 0: δ���ϸ���
*			 >0: ���ϸ���      
*/
UINT32 GetPsdIsolateState(const UINT16 doorId);

#ifdef __cplusplus
}
#endif

#endif
