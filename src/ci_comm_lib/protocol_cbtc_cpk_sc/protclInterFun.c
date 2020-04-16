#include "protclInterFun.h"
#include "ProtclManage.h"


/*
* Э���ʼ��ģ��
* ������ 
* ���������    char*  fileName         ������ݵĵ�ַ
*    		   ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*   1 ����ʼ���ɹ�
*   0: ��ʼ��ʧ��
*/
UINT8 ProtclInitFunc(UINT8* fileName,ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret = 0;

	if (NULL == pUnifyInfo)
	{
		return ret ;
	}

	ret =Init(fileName,pUnifyInfo);

	return ret;
}
/*
* Э������Э��ģ��
* ������ 
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 ProtclEndFunc(void)
{
	UINT8 ret = 1;

	ret =FreeSpace();

	return ret;
}

/*
* ���ܣ�Э�鷢������
*
* ������ 
* �������������  ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    UINT8 1 ����������������
*/

UINT8 ProtclSendDataFunc(ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret =0;
	if (NULL != pUnifyInfo)
	{
		if ((pUnifyInfo->AppArray != NULL) && (pUnifyInfo->VarArray != NULL))
		{

			ret = SendAppData(pUnifyInfo);
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/*
* ���ܣ�Э���������
* ������
* �������������ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    1���������ݳɹ�
0��ʧ��
*/

UINT8 ProtclReceiveDataFunc (ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret =0;

	if (pUnifyInfo != NULL)
	{
		ret = ReceiveAppData(pUnifyInfo);
	}
	else
	{
		ret=0;
	}

	return ret;
}

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
UINT8 ProtclGetLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret =0;
	if (pUnifyInfo != NULL)
	{
		ret = GetLnkStatus(destType,destId,logId,pUnifyInfo);
	}

	return ret;
}


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

UINT8 ProtclDeleteLnkFunc(UINT8 DestType,UINT8 DestID,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retnVal =1;

	retnVal = DelLnkFunc(DestType,DestID,logId,pUnifyInfo);

	return retnVal;
}


/*
* ���ܣ�ˢ����·״̬
*
* ������ 
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ�� 
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ� 
*/
UINT8 ProtclReFreshFunc(UINT8* pLnkStatusData)
{
	UINT8 retVal =0;

	retVal =ReFresh(pLnkStatusData);

	return retVal;
}

/************************************************************************
���ܣ���Э�������TCP��·״̬��Ϣ
����: �û���Ҫÿ���ڵ����ҽ�����һ�α�����,ΪЭ������TCP��·״̬��Ϣ��
    ע�⣬����ȵ���Э����պ���ProtclReceiveDataFunc()������ñ���������
	�ܵ��±����ڵ�Э����պ����޷���ȷ��������������»���Ϊδ��ȷ����AU1
	��Ϣ���½���ʱ���ӳ�
���������
	UINT16 TcpStateDataLen,  TCP״̬��Ϣ�ֽ���
	UINT8* pTcpStateData	TCP״̬��Ϣ
����ֵ��0ʧ�� 1�ɹ�
************************************************************************/
UINT8 ProtclReceiveTcpStateFunc(UINT16 TcpStateDataLen,UINT8* pTcpStateData)
{
	UINT8 ret =0;
	ret = ReceiveTcpState(TcpStateDataLen,pTcpStateData);
	return ret;
}

/************************************************************************
���ܣ�Э������TCP��·������Ϣ
������ʹ��TCPͨ�ţ���RSSP2Э�飩ʱ���û���Ҫÿ���ڵ����ҽ�����һ�α�����,
    ��ȡЭ��������TCP��·������Ϣ����������Ϣ͸��ת����2��ͨ�ſ�������
	ע�⣬��������ͨ�ſ���������TCP��·������Ϣ������ͨ�ű��ġ�
�������:
	UINT16 *pTcpCtrlDataLen, TCP������Ϣ�ֽ���
	UINT8 *pTcpCtrlData      TCP������Ϣ
����ֵ��
************************************************************************/
UINT8 ProtclSendTcpCtrlFunc(UINT16 *pTcpCtrlDataLen,UINT8 *pTcpCtrlData)
{
	UINT8 ret = 0;
	ret = SendTcpCtrl(pTcpCtrlDataLen,pTcpCtrlData);
	return ret;
}