/********************************************************
*                                                                                                            
* �� �� ���� ParseAtsData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� ATS���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_ATS_DATA_H_
#define _PARSE_ATS_DATA_H_

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "ParsePackLeuData.h"
#include "../System/CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../Indicator/IndicatorDataManage.h"
#include "CbtcVersion.h"



/*��������֡�ṹ��*/
typedef struct S_CommandDataStruct
{
	UINT8 CommandType;      /*��������*/
	UINT32 CommandId;		/*����ID*/
	UINT32 CommandParam;    /*�������*/
} CommandDataStruct;





#ifdef __cplusplus
extern "C" {
#endif


/*
* ���������� ����ATS���ֵ�����
* ������     QueueStruct* atsToCiDataQueue, �������ݵ�ַ
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�       
*/
UINT8 ParseAtsOrXianDiToCiData(QueueStruct* atsToCiDataQueue);

/*
* ���������� ����������������֡
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseItsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength);

/*
* ���������� ��ATS���ֵ�����
* ����˵���� QueueStruct* ciToAtsDataQueue, ������Ͷ��е�ַ   
*            const UINT8 deviceId, Ŀ��ϵͳID
*            const UINT8 targetDevType, Ŀ��ϵͳ����
* ����ֵ  �� 0: ���ʧ��
*			 1: ����ɹ�      
*/
UINT8 PackOcToItsDiData(QueueStruct* ciToXianDiDataQueue, const UINT8 deviceId, const UINT8 targetDevType);

/*
* ���������� ��ȡCI���յĲ�������ṹ�����鳤��
* ����˵���� void    
* ����ֵ  �� 0: ��ȡʧ��
*           >0: CI���ղ�������ṹ�����鳤��      
*/
UINT16 GetCiRevCmdStruCurSum(void);

/*
* ���������� ��ȡCI���յĲ�������ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: CI���ղ�������ṹ������ָ��      
*/
CommandDataStruct* GetCiRevCmdDataStruct(void);

/*
* ���������� ���OC���������
* ����˵���� const UINT32 CommandId, ����ID
*            const UINT8 CommandType, ��������
*            const UINT16 DeviceId, �豸ID
*			 const UINT8 commandParam, ��������
*            const UINT32 commandId, ����ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�       
*/
UINT8 SetOcSendCmdBackInfoData(const UINT8 CommandType, const UINT8 deviceType, const UINT16 deviceId, const UINT8 commandParam, const UINT32 commandId);

/*
* ���������� ���CI�����������ݶ���
* ����˵���� void    
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�      
*/
void ClearCiRevCmdData(void);

/*
* ���������� ���CI������������ݶ���
* ����˵���� void    
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�      
*/
void ClearOcSendCmdData(void);

/*
* ���������� ��ȡ��վ�������ֵ���λ����
* ����˵���� void    
* ����ֵ  �� ��λ����      
*/
UINT8 *GetCiToXianDiMaskData(void);

/*
* ���������� ��ȡ��վ�������ֵ���λ���ݳ���
* ����˵���� void    
* ����ֵ  �� ��λ���ݳ���     
*/
UINT16 GetCiToXianDiMaskDataLen(void);

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetPowerUnlockFirstCmdId(void);

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetPowerUnlockFirstCmdId(const UINT32 cmdId);

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetEliminateSwitchLockFirstCmdId(void);

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetEliminateSwitchLockFirstCmdId(const UINT32 cmdId);

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetRegionBlockRelieveFirstCmdId(void);

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetRegionBlockRelieveFirstCmdId(const UINT32 cmdId);

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetPhySecBlockRelieveFirstCmdId(void);

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetPhySecBlockRelieveFirstCmdId(const UINT32 cmdId);

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetDeleteItsElockFirstCmdId(void);

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetDeleteItsElockFirstCmdId(const UINT32 cmdId);

/*
* ���������� ��ȡCI���յ�ATS���ֵ��ź��ؿ���������ṹ�����鳤��
* ����˵���� void    
* ����ֵ  �� 0: ��ȡʧ��
*           >0: CI����ATS���ֵ��ź��ؿ���������ṹ�����鳤��      
*/
UINT16 GetATSorXDToCiCmdReturnToTRCurSum(void);

/*
* ���������� ��ȡCI���յ�ATS���ֵ��ź��ؿ���������ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: CI����ATS���ֵ��ź��ؿ���������ṹ������ָ��      
*/
CommandDataStruct* GetATSorXDToCiCmdReturnToTRDataStru(void);

/*
* ���������� ���ź��ؿ�����ṹ�������У�ɾ��ָ��λ�õ�����
* ����˵���� const UINT16 idx:�ź��ؿ�����ṹ�������λ������
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 RemoveReturnToTRCmdAt(const UINT16 idx);

/*
* ���������� �����ź��ؿ���Ϣ�ṹ��
* ����˵���� const CommandDataStruct* src:�����Ƶ��ź��ؿ�����ṹ��ָ��
			 CommandDataStruct* dst:Ŀ���ź��ؿ�����ṹ��ָ��
* ����ֵ  �� ��     
*/
void CopyReturnToTRCmd(const CommandDataStruct* src,CommandDataStruct* dst);
#ifdef __cplusplus
}
#endif

#endif
