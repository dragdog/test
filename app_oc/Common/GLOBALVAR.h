#ifndef _GLOBALVAR_H_
#define _GLOBALVAR_H_



#ifdef __cplusplus
extern "C" {
#endif



#include "TypesDef.h"
#include "CommonQueue.h"
#include "kcg_types.h"
	/* ***********************   OC����   ******************************************** */
	extern UINT32  g_dwOCCycle_PointSendLast;									/*��һ�����͵������������*/
	extern UINT16  g_wCommandNum_Switch_Send;									/*�����������������*/
	extern UINT16 g_wSwitchFaultNum;											/*�����������*/
	extern UINT32  g_dwDataVersion;												/*OCʹ�����ݰ汾�ţ��ϵ����TMC���룩*/
	extern UINT32  g_dwDataVersion_Train;									/*�г�ʹ�����ݰ汾�ţ�TMCʵʱ����*/

	extern UINT8   g_DataVersionSuccess;										/*OC�汾��У��ɹ�*/
	extern UINT32  g_dwApplyDataCycle ;											/*�����������ڣ����5����û���յ�ȷ�ϻ������Ϣ������������*/
	extern UINT8   g_OutputIVOCOnce;
	extern UINT32  g_dwOCCycle;                                               /*OC�������ڼ���*/
	extern	UINT8   g_MultSwitchInitSuccess;										/*�������سɹ��󣬽���������ݳ�ʼ���ɹ����ɹ��󲻼����������ݳ�ʼ��*/
	extern UINT8   g_WorkSuccess;													/*OC������������*/
	extern UINT8   g_ConnectedInfo;												/*TMC������ϢΪ3��������ȷ���ǲ�������*/
	extern UINT16  g_OCNO;
	extern UINT16  g_OCID;

	extern CommandData_Struct g_stOCRevCmdDataStru;
	/* ***********************   OC���鼰�ṹ��   ******************************************** */
	extern CONVERT_TABLE_STRU       g_stConverTable;					  /*ת����ṹ��*/
	extern OC_Struct                g_stOC;										/*OC�ṹ��*/
	extern ALLCommandResult_STRU	g_stOCRevCmdResultStru;                                      /*OC���յĿ�����������*/
	extern Switch_SendCommandData_Struct  g_stOCSendSwitchCmdDataStruct[CMD_MAX];	/*������OC���յĵ��������������*/
	extern Signal_SendCommandData_Struct g_stOCSendSignalCmdDataStruct[CMD_MAX];			/*������OC���յ��źŻ�������������*/
	extern ATSLockDelete_CountDown_Struct g_ATSLockDeleteCountDown[CMD_MAX]; /*ATS��������ʱ���ݽṹ*/
	extern UINT16 g_wSwitchFaultID[MAXSWITCHNum];/*���ϵ���ID�ṹ*/
	extern UINT16 g_wSignalCommandSendNum;
	extern kcg_uint16	g_wPsdCommandSendNum;
	extern kcg_uint16	g_wItsCommandSendNum;
	extern Its_SendCommandData_Struct g_stOCSendItsCmdDataStruct[CMD_MAX];
	extern DataVersionStru g_stDataVersion; /*TMC�������г������������ݰ汾����Ϣ������Э�����ݰ汾�ŵ�*/

#ifdef __cplusplus
}
#endif

#endif
