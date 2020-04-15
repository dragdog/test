/********************************************************
*                                                                                                            
* �� �� ���� HLHTCommonDataPacketHead.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2016-10-21
* ��    �ߣ� ������Ʒ��
* ���������� ������ͨͨ�ñ���ͷУ��ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _HLHTCOMMONDATAPACKETHEAD_H_
#define _HLHTCOMMONDATAPACKETHEAD_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#define HLHT_HEAD_SUCCESS (0x55) /*������ͨͷ�����ȷ*/
#define HLHT_HEAD_ERROR (0xaa) /*������ͨͷ������*/
#define HLHT_HEAD_INIT (0x33) /*����ͷ�ǻ�����ͨ��ʼͨ��֡*/

/*************************************ͨ�ñ���ͷ��ϢУ�����ԭ����**************************************/
#define GAL_COMPARE_INFO_TYPE_ERR 			  (UINT32)0x00000001 		/*GALͨ����Ϣ���нӿ���Ϣ���ʹ���*/
#define GAL_COMPARE_SEND_FLAG_ERR 			  (UINT32)0x00000002 		/*GALͨ����Ϣ���з��ͷ���ʶ��Ϣ����*/
#define GAL_COMPARE_RECV_FLAG_ERR 			  (UINT32)0x00000004 		/*GALͨ����Ϣ���н��շ���־��Ϣ����*/
#define GAL_COMPARE_EMAP_VER_COMP_ERR 		  (UINT32)0x00000008 		/*GALͨ����Ϣ���е��ӵ�ͼ�汾�űȽ�ʧ��*/
#define GAL_COMPARE_MSG_BDS_SN_ERR 		      (UINT32)0x00000010 	    /*GALͨ����Ϣ���б������Է����յ���һ����Ϣʱ�������кŴ���*/
#define GAL_COMPARE_MSG_SN_ERR 		          (UINT32)0x00000020 	    /*GALͨ����Ϣ���жԷ���Ϣ���кż��յ���һ����Ϣʱ�������кŴ���*/
#define GAL_COMPARE_PROTOCOL_VER_ERR 		  (UINT32)0x00000040 		/*GALͨ����Ϣ����Э��汾�Ŵ���*/
#define GAL_COMPARE_APP_LEN_ERR 			  (UINT32)0x00000080 		/*GALͨ����Ϣ����Ӧ�ò����ݳ�����Ч*/
#define GAL_COMPARE_COM_PERIOD (UINT32)0x00000100/*GALͨ����Ϣ����ͨ�����ڴ���*/

#ifdef __cplusplus
extern "C" {
#endif

	/*ͨ�ñ���ͷ*/
	typedef struct S_GALInfoStruct
	{
		UINT16 devInterInfoType;    /*�豸�ӿ���Ϣ����*/
		UINT16 totalDataLen;		/*Ӧ�ñ����ܳ���*/
		UINT16 comPeriod;			/*ͨ�����ڣ���λ��ms*/
		UINT8 prtclVer;				/*Э��汾��*/
		UINT32 sourceId;            /*ԴID�����ͷ���ʶ��Ϣ*/
		UINT32 destId;              /*Ŀ��ID�����շ���ʶ��Ϣ*/
		UINT32 emapVerCrc;			/*���ӵ�ͼ�汾У����Ϣ32λCRC*/
		UINT32 crntOwnMsgSN;		/*������Ϣ���кţ���¼���ͱ�����Ϣʱ�����������ڼ��������ͷ�ÿ���ڽ���������1*/	
		UINT32 recvOppMsgSN;		/*�Է���Ϣ���кţ���¼�յ��Է���һ����Ϣ�еĶԷ���Ϣ���к�*/
		UINT32 preOwnMsgSN;			/*�յ���һ����Ϣʱ�������кţ���¼�յ��Է���һ����Ϣʱ�����������ڼ���*/		
	} GALInfoStruct;

	/*************************************************************************************************************
	* �����������յ���ͨ����Ϣ��ʱ,�������Ϣ��ʣ����Ч��
	* ���������
	const UINT16 maxMsgValidTime	��ĿԼ���ĸ���Ϣ��Ч�� ;
	const UINT32 CurrentOwnSN �����ĵ�ǰ���ں�
	const UINT32 SNPeriod ������ͨ�����ڣ���CIϵͳ�У���Ϊ��Ӧ�õ�����
	const HLHTGALInfoStruct *pRecvCommonPackage  ͨ����Ϣ����ͷ
	const UINT32 srcRecvCiMsgSN,�Է��ϴ��յ��ı��������
	const UINT32 srcCrntOwnMsgSN,�Է������������ʱ�䵱ǰ�����
	const UINT32 srcPreOwnMsgSN,�Է��ϴ��յ���������ʱ�䵱ʱ�����
	const UINT32 srcPeriod,�Է���ͨ������
	* �����������.
	* ����ֵ��RemainValidity  ����Ϣ��ʣ����Ч��
	*************************************************************************************************************/
	INT32 HLHTCommonMsgRemainingValidity(const UINT32 MaxMsgValidTime, const UINT32 CurrentOwnSN, const UINT32 SNPeriod, const UINT32 srcRecvCiMsgSN, const UINT32 srcCrntOwnMsgSN, const UINT32 srcPreOwnMsgSN, const UINT32 srcPeriod);

	/*************************************************************************
	* ��������:    ���յ�CIͨ����Ϣ����ͷ��Ϣ�Ϸ��Լ��
	* �������:   UINT16 LocalDevName  ���ͷ��豸����
	*			  UINT16 RemoteDevName  ���շ��豸����
	*			  UINT16 DevInterfaceType  �豸���豸�ӿ�����
	*             UINT8 prtrlVer        �ӿ�Э��汾��
	*			  HLHTGALInfoStruct *pMICommonPackage  CI���͸�VOBC��ͨ����Ϣ����ͷ
	*             UINT16 logId        VOBC 1�˻�2�˱�ʶ
	* �����������:   UINT32 *pErrorCode  CI�ӿ��еĹ��Ϻ�.
	* �������:    ��.
	* ȫ�ֱ���:         ..
	* ����ֵ:
		HLHT_HEAD_SUCCESS (0x55) ������ͨͷ�����ȷ
		HLHT_HEAD_ERROR (0xaa) ������ͨͷ������
		HLHT_HEAD_INIT (0x33) ����ͷ�ǻ�����ͨ��ʼͨ��֡
	*************************************************************************/
	UINT8 HLHTRecvCommonPacketHeadCheck(UINT16 LocalDevName, UINT16 RemoteDevName, UINT16 DevInterfaceType, UINT8 prtrlVer, GALInfoStruct *pMICommonPackage, UINT32 *pErrorCode,UINT16 logId);

#ifdef __cplusplus
}
#endif

#endif
