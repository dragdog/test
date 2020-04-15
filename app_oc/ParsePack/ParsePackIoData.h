/********************************************************
*                                                                                                            
* �� �� ���� ParsePackIoData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� IO���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_IO_DATA_H_
#define _PARSE_PACK_IO_DATA_H_


#include "ParsePackDefine.h"
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "../Relay/RelayDefine.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../DataManage/DeviceDataManage.h"




/*IO���ݽṹ��*/
typedef struct S_IoDataStruct
{
    UINT8 BoardAddress;  /*���ӵ�ַ*/
	UINT8 PosValueBuf[BOARD_POS_MAX];  /*���Ӹ�λ������*/
} IoDataStruct;





#ifdef __cplusplus
extern "C" {
#endif


/*
* ���������� ����IO����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseWsToOcData(void);

/*
* ���������� ����IO��CI����֡
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseWsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength);

/*
* ���������� ��IO����
* ����˵���� void    
* ����ֵ  �� 0:���ʧ��
*			 1:����ɹ�      
*/
UINT8 PackOcToWsData(void);

/*
* ���������� ��ȡ�ɼ���ṹ�����鳤��
* ����˵���� void    
* ����ֵ  �� 0: ��ȡʧ��
*           >0: �ɼ���ṹ�����鳤��      
*/
UINT8 GetCollBroadStruCurSum(void);

/*
* ���������� ��ȡ�ɼ���ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: �ɼ���ṹ������ָ��      
*/
IoDataStruct* GetCollBoardDataStruct(void);

/*
* ���������� ����IO��CI����֡
* ����˵���� void    
* ����ֵ  �� void      
*/
void ResetIoToCiData(void);

#ifdef __cplusplus
}
#endif

#endif
