/*
*
* �ļ���   ��  SfpPrtcl.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.06.1
* ����     ��  ¥��Ϊ
* �������� ��  ͨ��Э���Լ�����ͨ�ſ�����
* ʹ��ע�� ��  
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*---------------------------------------------
* 1  2009/11/03	   ¥��ΰ		��װSFPЭ���Լ�ͨ�ſ�����
*
*/
#ifndef __CCOV_H_
#define __CCOV_H_
#include "CommonTypes.h"
#include "CommonQueue.h"
#include <winsock2.h>
#include <ws2tcpip.h>

const INT32 RCV_BUFF_SIZE = (INT32)1024*1024;

class CComm  
{
public:
	UINT8 Release();
	UINT8 Init_Comm();
	UINT8 Send(CHAR *Msg, INT32 Len, CHAR *host, UINT16 port);
	UINT8 Bind(UINT16 Port,CHAR *ServerIP);
	UINT8 StartRcv();	
	UINT8 ReadData(UINT16 *dataLen,UINT8 *data);
	virtual UINT8 RcvThread();
	CComm();
	virtual ~CComm();

protected:

	sockaddr_in client;	/* ���ܵ�sockaddr_in ���� */
	sockaddr_in srv;	/* ��������*/
	SOCKET ListenSocket;
	QueueStruct _rcvDataQueue;
	HANDLE _hRcvThread;		/* ���ܽ��̾�� */
	unsigned long _dwRcvThread;	/* ���̿�ʼ��־ */
	CHAR _prcvDataBuff[RCV_BUFF_SIZE];
	HANDLE hSemaphore;
};

class CbtcCcomm:public CComm
{
	virtual UINT8 RcvThread();
};
class LeuCcomm:public CComm
{
	virtual UINT8 RcvThread();
};
#endif