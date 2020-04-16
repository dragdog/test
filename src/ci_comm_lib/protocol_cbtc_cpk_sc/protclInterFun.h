/*
*
* 文件名   ： ProtclInterFun.h
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.15
* 作者     ：  王佩佩
* 功能描述 ：  协议主处理类，主要实现了周期性调用协议。
* 使用注意 ：  
* 修改记录 ：
*    时间			修改人		修改理由
*---------------------------------------------
* 1  2011/06/07	   楼宇伟		协议基类
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
* 协议初始化模块
* 参数： 
* 输入参数：   unsigned char   PrintRecordFlg   打印记录标记。0表示不打印 1:
*              char*  fileName         存放数据的地址
*    		   ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*   1 ：初始化成功
*   0: 初始化失败
*/
UINT8 ProtclInitFunc(UINT8* fileName,ProtclConfigInfoStru * pUnifyInfo);


/*
* 协议销毁协议模块
* 参数： 
*    无
* 返回值：
*    1 ：销毁成功
*    0 ：销毁失败
*/
UINT8 ProtclEndFunc(void);

/*
* 功能：协议接收数组
* 参数：
* 输入输出参数：ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    1：接收数据成功
0：失败
*/
UINT8 ProtclReceiveDataFunc (ProtclConfigInfoStru * pUnifyInfo);

/*
* 功能：协议发送数据
*
* 参数： 
* 输入输出参数：  ProtclConfigInfoStru * pUnifyInfo   统一结构体
* 返回值：
*    UINT8 1 正常，其他不正常
*/

UINT8 ProtclSendDataFunc(ProtclConfigInfoStru * pUnifyInfo);


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

UINT8 ProtclDeleteLnkFunc(UINT8 DestType,UINT8 DestID,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo);

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
UINT8 ProtclGetLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo);

/*
* 功能：刷新链路状态
*
* 参数： 
* 输入参数： 	 UINT8* pLnkStatusData     协议状态数据
* 返回值： 
*     0：刷新链路失败
*     1：刷新链路成功 
*/
UINT8 ProtclReFreshFunc(UINT8* pLnkStatusData);

/************************************************************************
功能：向协议层输入TCP链路状态信息
描述: 使用TCP通信（如RSSP2协议）时，用户需要每周期调用且仅调用一次本函数,
    为协议层更新TCP链路状态信息，注意，建议先调用本函数，后调用协议接收函
	数ProtclReceiveDataFunc()。否则可能导致本周期的协议接收函数无法正确工
	作，极端情况下会因为未能正确接收AU1消息导致建链时间延长
输入参数：
	UINT16 TcpStateDataLen,  TCP状态报文字节数
	UINT8* pTcpStateData	TCP状态报文
返回值：0失败 1成功
    TCP链路状态报文数据结构：
        长度（不含自身）   |               节点1                                |  节点2  |  ...
             2字节         |  远端设备互联互通编号ETCSID(4字节)  状态字(1字节)  |  ...    |  ...
    状态字数据结构：
	RRRRBBBB 高4位为红网状态信息，低4位为蓝网状态信息（状态信息：0无 1建链中 2已连接 3断开中 4已断开）
************************************************************************/
UINT8 ProtclReceiveTcpStateFunc(UINT16 TcpStateDataLen,UINT8* pTcpStateData);

/************************************************************************
功能：协议层输出TCP链路控制信息
描述：使用TCP通信（如RSSP2协议）时，用户需要每周期调用且仅调用一次本函数,
    获取协议层输出的TCP链路控制信息，并将该信息透明转发给2个通信控制器，
	注意，建议先向通信控制器发送TCP链路控制信息，后发送通信报文。
输出参数:
	UINT16 *pTcpCtrlDataLen, TCP控制信息字节数
	UINT8 *pTcpCtrlData      TCP控制信息
返回值：0失败 1成功
	TCP链路控制报文数据结构：
		长度（不含自身）   |               节点1                               |  节点2  |  ...
		     2字节         |  远端设备互联互通编号ETCSID(4字节) 控制字(1字节)  |  ...    |  ...
	控制字数据结构：
		RRRRBBBB 高4位为红网控制指令，低4位为蓝网控制指令（控制指令：0无 1建链 2保持连接 3断开）
************************************************************************/
UINT8 ProtclSendTcpCtrlFunc(UINT16 *pTcpCtrlDataLen,UINT8 *pTcpCtrlData);

#ifdef __cplusplus
}
#endif

#endif