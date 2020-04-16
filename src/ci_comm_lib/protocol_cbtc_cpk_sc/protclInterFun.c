#include "protclInterFun.h"
#include "ProtclManage.h"


/*
* 协议初始化模块
* 参数： 
* 输入参数：    char*  fileName         存放数据的地址
*    		   ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*   1 ：初始化成功
*   0: 初始化失败
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
* 协议销毁协议模块
* 参数： 
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 ProtclEndFunc(void)
{
	UINT8 ret = 1;

	ret =FreeSpace();

	return ret;
}

/*
* 功能：协议发送数据
*
* 参数： 
* 输入输出参数：  ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    UINT8 1 正常，其他不正常
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
* 功能：协议接收数组
* 参数：
* 输入输出参数：ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    1：接收数据成功
0：失败
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
* 功能：查询链路状态
*
* 参数： 
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      若卡斯柯的CI与VOBC通信，需要填写VOBC的站台号。其他的系统填0。
*             ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值： 
*     0：不存在该条链路，返回错误
*     0x17：链路在数据传输正常状态
*     0x2b：链路在通信中断状态
*     0x33：链路在准备建立链接状态
*     0x4e：链路在等待建立链接状态
*     0x01: 表示刷新无用的链路
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
* 功能：删除链路
*
* 参数： 
* 输入参数：  UINT8 DestType    对方设备类型
* 			  UINT8 DestID      对方设备ID
*             UINT16 logId      动态ID
*             ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    UINT8 1 删链成功
*          0 删链失败
*/

UINT8 ProtclDeleteLnkFunc(UINT8 DestType,UINT8 DestID,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retnVal =1;

	retnVal = DelLnkFunc(DestType,DestID,logId,pUnifyInfo);

	return retnVal;
}


/*
* 功能：刷新链路状态
*
* 参数： 
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值： 
*     0：刷新链路失败
*     1：刷新链路成功 
*/
UINT8 ProtclReFreshFunc(UINT8* pLnkStatusData)
{
	UINT8 retVal =0;

	retVal =ReFresh(pLnkStatusData);

	return retVal;
}

/************************************************************************
功能：向协议层输入TCP链路状态信息
描述: 用户需要每周期调用且仅调用一次本函数,为协议层更新TCP链路状态信息，
    注意，如果先调用协议接收函数ProtclReceiveDataFunc()，后调用本函数，可
	能导致本周期的协议接收函数无法正确工作，极端情况下会因为未正确接收AU1
	消息导致建链时间延长
输入参数：
	UINT16 TcpStateDataLen,  TCP状态信息字节数
	UINT8* pTcpStateData	TCP状态信息
返回值：0失败 1成功
************************************************************************/
UINT8 ProtclReceiveTcpStateFunc(UINT16 TcpStateDataLen,UINT8* pTcpStateData)
{
	UINT8 ret =0;
	ret = ReceiveTcpState(TcpStateDataLen,pTcpStateData);
	return ret;
}

/************************************************************************
功能：协议层输出TCP链路控制信息
描述：使用TCP通信（如RSSP2协议）时，用户需要每周期调用且仅调用一次本函数,
    获取协议层输出的TCP链路控制信息，并将该信息透明转发给2个通信控制器，
	注意，建议先向通信控制器发送TCP链路控制信息，后发送通信报文。
输出参数:
	UINT16 *pTcpCtrlDataLen, TCP控制信息字节数
	UINT8 *pTcpCtrlData      TCP控制信息
返回值：
************************************************************************/
UINT8 ProtclSendTcpCtrlFunc(UINT16 *pTcpCtrlDataLen,UINT8 *pTcpCtrlData)
{
	UINT8 ret = 0;
	ret = SendTcpCtrl(pTcpCtrlDataLen,pTcpCtrlData);
	return ret;
}