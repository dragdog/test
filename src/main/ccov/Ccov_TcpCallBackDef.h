#pragma once

//tcp�����յ����ݴ�����ָ������
typedef void (*TcpConnectionRecvHandler) (unsigned char *rBuf,int rLen,unsigned int srcIp,unsigned short srcPort,LPVOID pRcvHandler);
//tcp����ɾ��������ָ������
typedef void (*ClientDelHandler) (unsigned int clientIp,unsigned short clientPort,LPVOID pRcvHandler);