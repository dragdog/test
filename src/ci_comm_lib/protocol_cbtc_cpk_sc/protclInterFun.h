/*
*
* �ļ���   �� ProtclInterFun.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.15
* ����     ��  ������
* �������� ��  Э���������࣬��Ҫʵ���������Ե���Э�顣
* ʹ��ע�� ��  
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*---------------------------------------------
* 1  2011/06/07	   ¥��ΰ		Э�����
*/
#ifndef _PROTCL_Inter_Fun_H_
#define _PROTCL_Inter_Fun_H_

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "protclStruct.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
* Э���ʼ��ģ��
* ������ 
* ���������   unsigned char   PrintRecordFlg   ��ӡ��¼��ǡ�0��ʾ����ӡ 1:
*              char*  fileName         ������ݵĵ�ַ
*    		   ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*   1 ����ʼ���ɹ�
*   0: ��ʼ��ʧ��
*/
UINT8 ProtclInitFunc(UINT8* fileName,ProtclConfigInfoStru * pUnifyInfo);


/*
* Э������Э��ģ��
* ������ 
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 ProtclEndFunc(void);

/*
* ���ܣ�Э���������
* ������
* �������������ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    1���������ݳɹ�
0��ʧ��
*/
UINT8 ProtclReceiveDataFunc (ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ�Э�鷢������
*
* ������ 
* �������������  ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    UINT8 1 ����������������
*/

UINT8 ProtclSendDataFunc(ProtclConfigInfoStru * pUnifyInfo);


/*
* ���ܣ�ɾ����·
*
* ������ 
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ��̬ID
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    UINT8 1 ɾ���ɹ�
*          0 ɾ��ʧ��
*/

UINT8 ProtclDeleteLnkFunc(UINT8 DestType,UINT8 DestID,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ���ѯ��·״̬
*
* ������ 
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ�� 
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*     0x33����·��׼����������״̬
*     0x4e����·�ڵȴ���������״̬
*     0x01: ��ʾˢ�����õ���·
*/
UINT8 ProtclGetLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ�ˢ����·״̬
*
* ������ 
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ�� 
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ� 
*/
UINT8 ProtclReFreshFunc(UINT8* pLnkStatusData);

/************************************************************************
���ܣ���Э�������TCP��·״̬��Ϣ
����: ʹ��TCPͨ�ţ���RSSP2Э�飩ʱ���û���Ҫÿ���ڵ����ҽ�����һ�α�����,
    ΪЭ������TCP��·״̬��Ϣ��ע�⣬�����ȵ��ñ������������Э����պ�
	��ProtclReceiveDataFunc()��������ܵ��±����ڵ�Э����պ����޷���ȷ��
	������������»���Ϊδ����ȷ����AU1��Ϣ���½���ʱ���ӳ�
���������
	UINT16 TcpStateDataLen,  TCP״̬�����ֽ���
	UINT8* pTcpStateData	TCP״̬����
����ֵ��0ʧ�� 1�ɹ�
    TCP��·״̬�������ݽṹ��
        ���ȣ���������   |               �ڵ�1                                |  �ڵ�2  |  ...
             2�ֽ�         |  Զ���豸������ͨ���ETCSID(4�ֽ�)  ״̬��(1�ֽ�)  |  ...    |  ...
    ״̬�����ݽṹ��
	RRRRBBBB ��4λΪ����״̬��Ϣ����4λΪ����״̬��Ϣ��״̬��Ϣ��0�� 1������ 2������ 3�Ͽ��� 4�ѶϿ���
************************************************************************/
UINT8 ProtclReceiveTcpStateFunc(UINT16 TcpStateDataLen,UINT8* pTcpStateData);

/************************************************************************
���ܣ�Э������TCP��·������Ϣ
������ʹ��TCPͨ�ţ���RSSP2Э�飩ʱ���û���Ҫÿ���ڵ����ҽ�����һ�α�����,
    ��ȡЭ��������TCP��·������Ϣ����������Ϣ͸��ת����2��ͨ�ſ�������
	ע�⣬��������ͨ�ſ���������TCP��·������Ϣ������ͨ�ű��ġ�
�������:
	UINT16 *pTcpCtrlDataLen, TCP������Ϣ�ֽ���
	UINT8 *pTcpCtrlData      TCP������Ϣ
����ֵ��0ʧ�� 1�ɹ�
	TCP��·���Ʊ������ݽṹ��
		���ȣ���������   |               �ڵ�1                               |  �ڵ�2  |  ...
		     2�ֽ�         |  Զ���豸������ͨ���ETCSID(4�ֽ�) ������(1�ֽ�)  |  ...    |  ...
	���������ݽṹ��
		RRRRBBBB ��4λΪ��������ָ���4λΪ��������ָ�����ָ�0�� 1���� 2�������� 3�Ͽ���
************************************************************************/
UINT8 ProtclSendTcpCtrlFunc(UINT16 *pTcpCtrlDataLen,UINT8 *pTcpCtrlData);

#ifdef __cplusplus
}
#endif

#endif