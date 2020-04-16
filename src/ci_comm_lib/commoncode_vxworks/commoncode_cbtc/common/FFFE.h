/************************************************************************
*
* �ļ���   ��  FFFE.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0  
* ����ʱ�� ��  2009.08.17
* ����     ��  ���ؼ�Э�鲿
* �������� ��  FFFEЭ��ת�弰�����ļ� 
* ʹ��ע�� ��  1.���ܴ���ĳ���Ϊ65535����Ϊ�ܳ��ȵ�����UINT16���ơ�
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef _FFFE_H
#define _FFFE_H

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  
{
  UINT8 *pRawDataRcvBuf;/*δ��ת�����ݽ��ջ�����*/
  UINT16 RawDataRcvBufferSize;/*δ��ת�����ݽ��ջ�������С*/
  UINT8 RcvState ;/*FFFE���ݽ���״̬*/
  UINT8 GotFrame ;/*FFFE�Ƿ��յ�������֡*/
  UINT16 DataCount ;/* �յ�����֮֡ǰ�����յ���FFFE�����ܳ��ȣ�ÿ���յ�����֡������*/
}FFFE_STATE_STRU;

/*
* ���������� �����ݽ���FFFEת�崦������ͷFFFE��βFFFD����
* ��������� const UINT8 *pSrc    ����(��FFFEת��)
*            UINT16 SrcLen        ����(��FFFEת��)�ĳ���
* ��������� UINT8 *pTgt          ����(����FFFEת��)
* ����ֵ��   ת���ĳ���   
*/
UINT16 TranslateToFFFE(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen);


/*
* ����������  ������FFFEת�崦�������ݣ��Ѿ�ȥ����ͷFFFE��βFFFD����ԭΪԭʼ����
* ���������  const UINT8 *pSrc,    ����(FFFEת��)
*             UINT16 SrcLen,        ����(FFFEת��)�ĳ���
* ���������  UINT8 *pTgt,          ����(ȥ��FFFEת��)
* ����ֵ��    ����0,  ת��ɹ�,����ת�������ݳ���
*             0,   �в���ת����ַ���ת��ʧ�ܡ�����ת�������ݳ���Ϊ0   
*/
UINT16 TranslateFromFFFE(const UINT8 *pSrc,UINT8 *pTgt,UINT16 SrcLen);

/*
* ����������  FFFE�ṹ���ʼ������
* ���������  UINT16 FFFEBufferSize, ���ڽ���һ֡���ݵĻ���������   
*             PORT_STATE_STRU *pFFFEStatStru, FFFE״̬�ṹ��
* ����ֵ��    0,����ִ��ʧ��
*             1,����ִ�гɹ�
*/
UINT8 FFFEInit(FFFE_STATE_STRU *pFFFEStatStru,UINT16 FFFEBufferSize);

/*
* ����������   ���ַ����մ���ת������ݣ������֡FFFE���ģ�����FFFE��FFFD����
* ���������   UINT8 RcvChar,      ��ǰ���յ��ַ�
*              PORT_STATE_STRU *pFFFEStatStru,
* ���������   UINT8 *pRawDataRcv,  ���ݽ��ջ�����ָ��
* ����ֵ��     ����0,  ��ʾ�յ�����֡�����ݳ��ȣ������յ�������֡���������ջ�������
*              0,   δ�յ���֡���ݣ� 
*/

UINT16 FFFERcv(FFFE_STATE_STRU *pFFFEStatStru,UINT8 RcvChar, UINT8 *pRawDataRcv);

/*
* ����������  FFFE״̬������
* ���������  PORT_STATE_STRU *pFFFEStatStru, FFFE״̬�ṹ��
* ����ֵ��     
*/
void FFFEFree(FFFE_STATE_STRU *pFFFEStatStru);

/*
* ���������� �����ݽ���FFFEת�崦��,��ͷFFFE��βFFFD
* ��������� const UINT8 *pSrc    ����(��FFFEת��)
*            UINT16 SrcLen        ����(��FFFEת��)�ĳ���
* ��������� UINT8 *pTgt          ����(����FFFEת��),�����FFFEͷ��FFFDβ
* ����ֵ��   ת���ĳ���   
*/
UINT16 FFFEPack(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen);

#ifdef __cplusplus
}
#endif

#endif
