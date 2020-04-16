/* ***********************************************************************
*
* �ļ���		    ��  GlobalVar.h
* ��Ȩ˵��		��  �������ؿƼ��������ι�˾
* �汾��		    ��  1.0
* ����ʱ�� 		��  2015.12.09
* ����			��  ��һ��
* ��������		��  ����ͨ��OC���õ�Ӧ��ȫ�ֱ���ͷ�ļ�.
* ʹ��ע��		��  �����ڳ���ͨ����Ŀ��
* �޸ļ�¼		��  2015-6-17 			��һ��					����
*
*********************************************************************** */

#include "TypesDef.h"
#include "CommonQueue.h"
#include "kcg_types.h"
#include "protclStruct.h"
#include "GLOBALVAR.h"
/* ***********************   OC   ******************************************** */

OC_Struct                  g_stOC;                              /*OC�ṹ��*/
CONVERT_TABLE_STRU       g_stConverTable;					  /*ת����ṹ��*/

/*============================ȫ�ֽṹ������============================*/
CommandData_Struct         g_stOCRevCmdDataStru;                                      /*OC���յĿ�����������*/
ALLCommandResult_STRU		 g_stOCRevCmdResultStru;                                      /*OC���յĿ�����������*/
/*==============================ȫ�ֱ���=================================*/
UINT32  g_dwOCCycle = DEFAULT_ZERO;                                                 /*OC�������ڼ���*/
UINT32  g_dwOCCycle_PointSendLast = DEFAULT_ZERO;									/*��һ�����͵������������*/
UINT8	g_bResetSignal = FLAG_UNSET;												/*OC��λ�ź�*/
UINT8	g_bPowerLock = FLAG_SET;													/*�ϵ�����е�����ڶ�ռ����������������HCOC,���HCOC�����ϵ�������������������OC����������³��֡�*/
UINT16  g_wSwitchFaultNum = DEFAULT_ZERO;											/*�����������*/
UINT32  g_dwDataVersion = DEFAULT_ZERO;												/*OCʹ�����ݰ汾�ţ��ϵ����TMC���룩*/
UINT32  g_dwDataVersion_Train = DEFAULT_ZERO;												/*�г�ʹ�����ݰ汾�ţ�TMCʵʱ����*/
UINT8   g_DataVersionSuccess = FLAG_UNSET;											/*OC�汾��У��ɹ�*/
UINT8   g_WorkSuccess = FLAG_UNSET;													/*OC������������*/
UINT32  g_dwApplyDataCycle = DEFAULT_ZERO;											/*�����������ڣ����5����û���յ�ȷ�ϻ������Ϣ������������*/
UINT8   g_OutputIVOCOnce = FLAG_UNSET;
UINT8   g_MultSwitchInitSuccess = FLAG_UNSET;										/*�������سɹ��󣬽���������ݳ�ʼ���ɹ����ɹ��󲻼����������ݳ�ʼ��*/
UINT8   g_ConnectedInfo = FLAG_UNSET;												/*TMC������ϢΪ3��������ȷ���ǲ�������*/
UINT16  g_OCNO = 0;
UINT16  g_OCID = OCID;
/*==============================ȫ������=================================*/
UINT8  g_bszVOBCOut[SIZE_QUEUE_BUF];                                             /*VOBC���������֡����*/
UINT8  g_bszATSOut[SIZE_QUEUE_BUF];                                              /*ATS���������֡����*/
UINT8  g_bszLSOut[SIZE_QUEUE_BUF];                                               /*����ƽ̨���������֡����*/
UINT8  g_bszATSIN[SIZE_QUEUE_BUF];                                               /*ATS����������ݻ���*/
UINT8  g_bszVOBCIN1[SIZE_QUEUE_BUF];                                             /*�г�1����������ݻ���*/
UINT8  g_bszVOBCIN[SIZE_QUEUE_BUF];                                              /*�г�0����������ݻ���*/
UINT8  g_bszQueueBuffer[SIZE_QUEUE_BUF];                                         /*����������ݻ���*/
UINT8  g_bszLSCIN[SIZE_QUEUE_BUF];   								

ProtclConfigInfoStru g_stCbtcData;                           /*����Э��ʹ�õĽṹ��*/
QueueStruct  *g_stCurrencyRcvData;                        /*ͨ�ý������ݵĶ���ָ��*/
QueueStruct  *g_stCurrencySendData;                       /*ͨ�÷������ݵĶ���*/
UINT8  g_bszReadBuffer[SIZE_QUEUE_BUF];                  /*���Ե����������*/

UINT16 g_wSwitchFaultID[MAXSWITCHNum]={DEFAULT_ZERO};/*���ϵ���ID�ṹ*/
DataVersionStru g_stDataVersion; /*TMC�������г������������ݰ汾����Ϣ������Э�����ݰ汾�ŵ�*/

/*�����������������*/
kcg_uint16	g_wCommandNum_Switch_Send = 0;
/*������OC���յĵ��������������*/
Switch_SendCommandData_Struct g_stOCSendSwitchCmdDataStruct[CMD_MAX] = { 0 };

/*�������źŻ��������*/
kcg_uint16	g_wSignalCommandSendNum = 0;
/*������OC���յ��źŻ�������������*/
Signal_SendCommandData_Struct g_stOCSendSignalCmdDataStruct[CMD_MAX] = { 0 };

/*�������������������*/
kcg_uint16	g_wPsdCommandSendNum = 0;

/*ATS��������ʱ����*/
kcg_uint16	g_wATSQingLingNum = 0;
/*ATS��������ʱ���ݽṹ*/
ATSLockDelete_CountDown_Struct g_ATSLockDeleteCountDown[CMD_MAX] = { 0 };