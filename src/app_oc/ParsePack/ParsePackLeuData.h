/********************************************************
*                                                                                                            
* �� �� ���� ParsePackLeuData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Leu���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_LEU_DATA_H_
#define _PARSE_PACK_LEU_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"


#ifdef __cplusplus
extern "C" {
#endif

/*������ͨ�ŵ�LEU��Ϣ*/
typedef struct S_LeuToCiDataStruct
{
   UINT8 reservedByte;/*�����ֽ�*/
   UINT8 idInfo[3];/*ID��ʾ����Ϣ��idInfo[0]δ֪��idInfo[1]LEU�豸���ͣ�idInfo[2]LEU ID*/
   UINT8 allState;/*����״̬*/
   UINT8 leuType;/*LEU���ͣ�02͸�����䣬03��͸������*/
   UINT8 toQianjiComState;/*��ǰ��ͨ��״̬*/
   UINT8 boardState;/*�弶״̬*/
   UINT8 outState[4];/*�����״̬�������1-�����4*/
   UINT8 maskInfo[12];/*��λ��Ϣ��12�ֽ�*/
} LeuToCiDataStruct;

/*
* ���������� ����LEU����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���   
* ����ֵ  �� void       
*/
void SaveLeuToCiData(const UINT8* dataBuf, const UINT16 dataLength);

/*
* ���������� ��LEU����
* ����˵���� QueueStruct* CiToLeuDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToLeuData(QueueStruct* CiToLeuDataQueue, UINT8 deviceId);

/*
* ���������� ���LEU����
* ����˵���� void    
* ����ֵ  �� void       
*/
void ClearLeuToCiData(void);

/*
* ���������� ��ȡLEU���ݳ���
* ����˵���� void    
* ����ֵ  �� ���ݳ���       
*/
UINT16 GetLeuDataLen(void);

/*
* ���������� ��ȡLEU����
* ����˵���� void    
* ����ֵ  �� ����     
*/
UINT8* GetLeuData(void);


/*
* ���������� ��������LEU��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseLeuToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);

/*
* ���������� ��ȡ��CIͨ�ŵ�LEU��Ŀ
* ����˵���� void    
* ����ֵ  �� ��CIͨ�ŵ�LEU��Ŀ       
*/
UINT16 GetLeuToCiSum(void);

/*
* ���������� ��ȡLEU��CIͨ�����ݽṹ��
* ����˵���� void    
* ����ֵ  �� LEU��CIͨ�����ݽṹ��ָ��       
*/
LeuToCiDataStruct* GetLeuToCiDataStru();

#ifdef __cplusplus
}
#endif

#endif
