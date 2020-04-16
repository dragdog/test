#pragma once

//tcp连接收到数据处理函数指针类型
typedef void (*TcpConnectionRecvHandler) (unsigned char *rBuf,int rLen,unsigned int srcIp,unsigned short srcPort,LPVOID pRcvHandler);
//tcp连接删除处理函数指针类型
typedef void (*ClientDelHandler) (unsigned int clientIp,unsigned short clientPort,LPVOID pRcvHandler);