/************************************************************************
*
* �ļ���   ��  DataPackage.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.01.14
* ����     ��  ������
* �������� ��  ��������֡���ĺϳ�һ֡����
* ʹ��ע�� ��  
* �޸ļ�¼ ��
************************************************************************/

#ifndef _DATA_PACKAGE_H_
#define _DATA_PACKAGE_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ��Ϣ���Ͷ��� */
typedef struct  
{
	UINT32 dataBuffLen;               	/* ���������ȣ���ʼ���ú��ܸı� */
	UINT8* dataBuff;               /* ������ݵ�ָ���ַ */
	UINT16 msgLen;               /* ������֡���� */
	UINT8 msgType;               /* ��Ϣ֡����*/
}MsgStru;

/*
 * ��֡���ݺϳ�һ�����ݷ���
 * ������ 
 * ���������   
            UINT8 msgCount  ���ĸ���
			MsgStru msgList[]  ���������б�
			UINT16 sndBuffLen  ���Ĵ�ŵ����ݻ�������С
 * ���������
 *          UINT8 snddataBuff[]  �ϲ�������֡
 * ����ֵ��
 *   >0���ϲ���ı��ĳ���
 *   0: ���ݳ���Ϊ��
 *   -1: û�з���ռ�
 *   -2: ����ռ䲻��
 *   -3:���鿽��ʧ��
 */
INT32 PackageMsg2SndBuff(UINT8 msgCount,MsgStru msgList[], UINT16 sndBuffLen,UINT8 snddataBuff[]);


/*
 * ���ܣ����յ�һ֡�������ݷֽ�ɶ�֡����
 *
 * ������ 
 * ���������  
            UINT16 rcvDataLen     �������ݳ���
            UINT8 rcvDataBuff[]   ����
            UINT8 msgListLen      MSG����������С
			MsgStru msgList[]     MSG������
 * ���������  
            MsgStru msgList[]     ���������б�
 * ����ֵ��
 *    >0 :��������
      0:����ʧ�ܻ���û������
 */
INT16 UnPackageRcvBuff2MsgList (UINT16 rcvDataLen,UINT8 rcvDataBuff[],UINT8 msgListLen,MsgStru msgList[]);

#ifdef __cplusplus
}
#endif
#endif	/* #ifdef */