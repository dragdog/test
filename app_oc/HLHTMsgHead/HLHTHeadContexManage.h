/********************************************************
*
* �� �� ���� HLHTHeadContexManage.h
* ��Ȩ˵���� �������ؿƼ����޹�˾
* ����ʱ�䣺 2016-12-17
* ��    �ߣ� ������Ʒ��
* ���������� ������ͨ����ͷ���������ݹ���
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��
*
********************************************************/

#ifndef _HLHT_HEAD_CONTEX_MANAGE_H_
#define _HLHT_HEAD_CONTEX_MANAGE_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*������ͨ����ͷ���������ݽṹ��*/
	typedef struct S_HLHTHeadContextStru
	{
		UINT16 sourceComPeriod;			/*��CI�������ݵ�һ����ͨ�����ڣ���λ��ms*/
		UINT8 sourceId;            /*��CI�������ݵ�һ����ID*/
		UINT16 logId;			/*��CI�������ݵ�һ�����߼�ID��Ŀǰ��������ͨ4IP VOBCʹ��*/
		UINT32 sourceCrntOwnMsgSN;		/*��CI�������ݵ�һ�����ͱ���ʱ��ǰ��SN*/
		UINT32 sourceRecvOppMsgSN;		/*��CI�������ݵ�һ���ϴ��յ�CI����ʱCI��SN*/
		UINT32 sourcePreOwnMsgSN;			/*��CI�������ݵ�һ���ϴ��յ�CI����ʱ��SN*/

		UINT32 ciPreRecvOwnSn;/*CI���յ�����ʱ�����SN*/
	} HLHTHeadContextStru;
	/*
	* ��������:��ȡĳ���豸���һ�θ�CI��������ʱ�䵱ǰ�����
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	ĳ���豸���һ�θ�CI��������ʱ�䵱ǰ�����,0Ϊ��Чֵ
	*/
	UINT32 GetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId);
	/*
	* ��������:��ȡĳ���豸���һ�θ�CI��������ʱ������д���ϴ��յ�CI�����䵱ʱ�����
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	ĳ���豸���һ�θ�CI��������ʱ������д���ϴ��յ�CI�����䵱ʱ�����,0Ϊ��Чֵ
	*/
	UINT32 GetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId);
	/*
	* ��������:��ȡCI����յ�ĳ���豸������ʱCI����ʱ�����
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	CI����յ�ĳ���豸������ʱ����ʱ�����,0Ϊ��Чֵ
	*/
	UINT32 GetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId);
	/*
	* ��������:����ĳ����CIͨ�ŵ��豸��ͨ������
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 period:ͨ������
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxComPeriod(const UINT8 devType, const UINT8 devId, const UINT16 period);
	/*
	* ��������:����CI����յ�ĳ���豸����ʱͨ��ͷ��������ĶԷ���ǰ�����ں�
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 srcCrntSN:�Է���ǰ�����ں�
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId, const UINT32 srcCrntSN);
	/*
	* ��������:����ĳ���豸��һ���յ�CI����ʱ���������CI��ǰ�����
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 srcOppSN:�Է���һ���յ�CI����ʱ���������CI��ǰ�����
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxSrcOppSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOppSN);
	/*
	* ��������:����ĳ���豸��һ���յ�CI����ʱ������ĵ�ǰ���
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 srcOwnSN:ĳ���豸��һ���յ�CI����ʱ������ĵ�ǰ���
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOwnSN);
	/*
	* ��������:����CI�յ�����ʱCI��������
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 ciRcvSn:CI�յ�����ʱCI��������
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId, const UINT32 ciRcvSn);
	/*
	* ��������:ɾ��ĳ���豸�Ļ�����ͨͷ
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 ClearHlhtHeadCtx(const UINT8 devType, const UINT8 devId);
	/*
	* ���������� ������������ͨͨ�ñ���ͷʱЧ��
	* ����˵����
	��
	* ����ֵ  ��
	��
	*/
	void ProcHlhtHeadCtxTimeOut(void);
	/*
	* ���������� �����豸���ͣ���þ߱���ʼͨ�Ż�����ͨͷ�������豸ID
	* ����˵����
	const UINT8 devType, �豸����
	UINT8* devIdBuf, �洢�����������豸��ŵ�����
	const UINT8 devIdBufMaxLen, devIdBuf����������ֹ�����ڷ���Խ��
	UINT8* devSum,���������������豸��ŵ�����
	UINT16* devLogIdBuf �洢�����������豸�߼�ID������
	* ����ֵ  ��
	CI_SUCCESS:��óɹ�
	CI_ERROR:���ʧ��
	*/
	UINT8 GetHlhtInitHeadDevsByDevType(const UINT8 devType, UINT8* devIdBuf, const UINT8 devIdBufMaxLen, UINT8* devSum, UINT16* devLogIdBuf);

	/*
	* ��������:��ȡĳ����CIͨ�ŵ��豸�Ļ�����ͨ����ͷ��������Ϣ�Ƿ���ڲ�����Ч
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	CI_ERROR:������
	*	CI_SUCCESS:����
	*/
	UINT8 GetHLHTHeadCtxValid(const UINT8 devType, const UINT8 devId);
		/*
	* ��������:��ȡĳ����CIͨ�ŵ��豸�Ļ�����ͨ����ͷ��������Ϣ�Ƿ����
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	* ����ֵ:
	CI_ERROR:������
	*	CI_SUCCESS:����
	*/
	UINT8 GetHLHTHeadCtxExistance(const UINT8 devType,const UINT8 devId);

	/*
	* ��������:����CI�յ�����ʱ�Է���LogId
	* ����˵��:
	const UINT8 devType:�豸����
	const UINT8 devId:�豸ID
	const UINT16 ciRcvLogId:CI�յ�����ʱ�Է���LogId
	* ����ֵ:
	CI_SUCCESS:�ɹ�
	CI_ERROR:ʧ��
	*/
	UINT8 SetHLHTHeadCtxCiRcvLogId(const UINT8 devType, const UINT8 devId, const UINT16 ciRcvLogId);
#ifdef __cplusplus
}
#endif

#endif
