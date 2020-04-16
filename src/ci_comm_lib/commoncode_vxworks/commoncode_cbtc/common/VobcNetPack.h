
/************************************************************************

* �ļ���   ��  VobcNetPack.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.08.21
* ����     ��  ���ؼ�Э�鲿
* �������� ��  vobc������Ϣ��Э�麯��
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _VOBCNETPACK_H_
#define _VOBCNETPACK_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif




/*
*  ���������� ������Ϣ��������
*  ����˵���� IN UINT8 DataType��   ��������
*             IN UINT32 Seq,        �����������к�
*             IN const UINT8 *pData,Ӧ������
*             IN UINT16 DataLen,    Ӧ�����ݳ���
*             OUT UINT8 *pPackData,  �������ָ��
*  ����ֵ��   �����Ϣ�����ݳ���
*/
UINT16 VobcNetPack(IN UINT8 DataType,IN UINT32 Seq,IN const UINT8 *pData, IN UINT16 DataLen,OUT UINT8 *pPackData);

/*
*  ���������� ������Ϣ��������
*  ����˵���� IN const UINT8 *pPackData,     �������
*             IN const UINT16 PackDataLen,   ������ݳ���
*             OUT UINT8 *pDataType,          ��������
*             OUT UINT32 *pSeq,              �����������к�
*             OUT UINT8 *pData,              ������Ӧ������
*  ����ֵ��   >0,  ���������ݳ���
*             0,   ��ʾ�������,���߽��������ݳ���Ϊ0
*/
UINT16 VobcNetUnpack(IN const UINT8 *pPackData,IN const UINT16 PackDataLen,OUT UINT8 *pDataType,OUT UINT32 *pSeq,OUT UINT8 *pData);

#ifdef __cplusplus
}
#endif

#endif


