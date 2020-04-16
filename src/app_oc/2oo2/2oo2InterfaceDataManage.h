/********************************************************
*                                                                                                            
* �� �� ���� 2oo2InterfaceDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-26
* ��    �ߣ� ������
* ���������� ��ȫ������ӿ����ݹ���  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _2OO2_INTERFACE_DATA_MANAGE_H_
#define _2OO2_INTERFACE_DATA_MANAGE_H_


#include "../System/SystemConfigDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "../Transmit/TransmitStateDataManage.h"

#ifdef PLATFORM_2OO2
#include "sfpplatform.h"
#include "RsspWl.h"
#include "processor.h"
#include "FsfbProtcl.h"
#endif

#ifdef PLATFORM_LABVIEW_DEBUG
#include "../../Prtcl/sfp/sfpplatform.h"
#include "stdlib.h"
#endif


/*ͨ��״̬*/
#define TRANSMIT_2OO2_STATE_GOOD                  0x01      /*ͨ������*/
#define TRANSMIT_2OO2_STATE_LINK                  0x02      /*ά������*/
#define TRANSMIT_2OO2_STATE_BAD                   0x03      /*ͨ���ж�*/

/*�ϵ��ȡFSIO����������*/
#define START_GET_FSIO_CYCLE_NUM                   20        /*�ϵ��ȡFSIO����������*/


#ifdef __cplusplus
extern "C" {
#endif

/*ƽ̨ͨ��״̬�ṹ��*/
typedef struct S_PlatComStatusStruct
{
    UINT8* SystemType;   /*ͨ�ŵ�ϵͳ�豸����*/
    UINT8* SystemNum;   /*ͨ�ŵ�ϵͳ�豸����*/
    UINT8* SystemIdBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX];    /*ͨ�ŵ�ϵͳ�豸ID����*/
    UINT8* CommunicationStatusBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*ͨ��״̬���� ��ID�����Ӧ*/
} PlatComStatusStruct;

/*
* ���������� ƽ̨��ʼ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ATS��CI�ӿڶ���
*/
void Initial2oo2Interface(void);


/*
* ���������� ��ȡƽ̨ͨ��Э�鵽CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ͨ��Э�鵽CI�ӿڶ���
*/
QueueStruct* Get2oo2ProtocolToOcDataQueue(void);

/*
* ���������� ��ȡƽ̨CI��ͨ��Э��ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��ͨ��Э��ӿڶ���
*/
QueueStruct* Get2oo2OcToProtocolDataQueue(void);

#ifdef PLATFORM_LABVIEW_DEBUG
/*
* ���������� ��ȡƽ̨SFP��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨SFP��CI�ӿڶ���
*/
QueueStruct* Get2oo2SfpToCiDataQueue(void);

/*
* ���������� ��ȡƽ̨CI��SFP�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��SFP�ӿڶ���
*/
QueueStruct* Get2oo2CiToSfpDataQueue(void);

#endif

/*
* ���������� ��ȡƽ̨Fsfb��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��Fsfb�ӿڶ���
*/
QueueStruct* Get2oo2FsfbToCiDataQueue(void);

/*
* ���������� ��ȡƽ̨CI��FsfbP�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��Fsfb�ӿڶ���
*/
QueueStruct* Get2oo2CiToFsfbDataQueue(void);



/*
* ���������� ��ȡƽ̨ATS��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ATS��CI�ӿڶ���
*/
QueueStruct* Get2oo2AtsToCiDataQueue(void);

/*
* ���������� ��ȡƽ̨CI��ATS�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��ATS�ӿڶ���
*/
QueueStruct* Get2oo2CiToAtsDataQueue(void);

/*
* ���������� ��ȡƽ̨CI���ֵؽӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI���ֵؽӿڶ���
*/
QueueStruct* Get2oo2CiToXianDiDataQueue(void);

/*
* ���������� ��ȡƽ̨�ֵص�CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨�ֵص�CI�ӿڶ���
*/
QueueStruct* Get2oo2XianDiToCiDataQueue(void);

/*
* ���������� ��ȡZC��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ZC��CI�ӿڶ���
*/
QueueStruct* Get2oo2ZcToCiDataQueue(void);

/*
* ���������� ��ȡCI��ZC�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��ZC�ӿڶ���
*/
QueueStruct* Get2oo2CiToZcDataQueue(void);

/*
* ���������� ��ȡƽ̨CI������CI���սӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI����CI�ӿڶ���
*/
QueueStruct* Get2oo2CiRecvCiDataQueue(void);

/*
* ���������� ��ȡCI���͵�����CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI����CI�ӿڶ���
*/
QueueStruct* Get2oo2CiSendCiDataQueue(void);

/*
* ���������� ��ȡƽ̨IO��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨IO��CI�ӿڶ���
*/
QueueStruct* Get2oo2IoToCiDataQueue(void);

/*
* ���������� ��ȡƽ̨CI��IO�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��IO�ӿڶ���
*/
QueueStruct* Get2oo2CiToIoDataQueue(void);

/*
* ���������� ��ȡLeu��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: Leu��CI�ӿڶ���
*/
QueueStruct* Get2oo2LEUToCiDataQueue(void);

/*
* ���������� ��ȡCI��Leu�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Leu�ӿڶ���
*/
QueueStruct* Get2oo2CiToLEUDataQueue(void);

/*
* ���������� ��ȡCI��Vobc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2VobcToCiDataQueue(void);

/*
* ���������� ��ȡCI��Vobc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2CiToVobcDataQueue(void);


/*
* ���������� ��ȡOc��Tmc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2OcToTmcDataQueue(void);


/*
* ���������� ��ȡTmc��Oc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2TmcToOcDataQueue(void);


/*
* ���������� ��ȡPSD��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: PSD��CI�ӿڶ���
*/
QueueStruct* Get2oo2PlatformToCiDataQueue(void);

/*
* ���������� ��ȡCI��Psd�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Psd�ӿڶ���
*/
QueueStruct* Get2oo2CiToPlatformDataQueue(void);

/*
* ���������� ��ȡƽ̨���ں�
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨���ں�
*/
UINT32 Get2oo2CurCycleNum(void);

/*
* ���������� ��ȡƽ̨����ID
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨����ID
*/


UINT8 Get2oo2ComputerId(void);

/*
* ���������� ��ȡƽ̨��������ģʽ
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨��������ģʽ
*/
UINT8 Get2oo2ProcessMode(void);

/*
* ���������� ��ȡCI�������豸ͨ�ŵĸ��豸����
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� ͨ���豸��Ŀ
*/
UINT8 Get2oo2CiAndOtherSysCommNum(const UINT8 systemType);

/*
* ���������� ��ȡCI�������豸ͨ��״̬
* ����˵���� const UINT8 systemType, ϵͳ����
*            const UINT8 index, ״̬�����±�
* ����ֵ  �� ͨ��״̬
*/
UINT8 Get2oo2CiAndOtherSysCommState(const UINT8 systemType, const UINT8 index);

/*
* ���������� ��ȡCI��IOͨ��״̬
* ����˵���� void
* ����ֵ  �� 
*/
UINT8 Get2oo2CiAndIoCommState(void);

/*
* ���������� ��ȡ��ʼ���ں�
* ����˵���� void
* ����ֵ  �� 
*/
UINT32 Get2oo2StartCycleNum(void);

/*
* ���������� ��ȡƽ̨�豸����
* ����˵���� void
* ����ֵ  �� 
*/
UINT8 Get2oo2SheBeiType(void);

#ifdef PLATFORM_LABVIEW_DEBUG

/*
* ���������� ��ȡ���͵�ά�޻�����
* ����˵���� void
* ����ֵ  �� CI��ά�޻��ӿڶ���
*/
QueueStruct* GetLabviewCiToMaintainDataQueue(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
