
/*******************************************************************************
* 文件名    ：  PrtclStruct.h
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  协议公用结构体头文件
* 使用注意  ： 
                无
* 修改记录  ：  
    2008.7.8修改记录
        1.增加struc_Unify_Info统一结构体定义。
        2.增加函数  UINT32 GetTimeCounter();

    2008.7.29 修改记录
        1. 把DSU相关结构体定义移植到PrtclStruct.h中；

    2009.1.20修改记录
        1.删除declare.h

    2009-8-27 15:19:06修改记录
        1.修改类型，使用CommonTypes.h中的定义
        2.struc_LnkBasicInfo中删除unsigned char DestMultiCommIpRed[4];
                                  unsigned char DestMultiCommIpBlue[4];
                                  unsigned char DestGroup;
        3.struc_Lnk_Info中删除unsigned char CommType;
                              unsigned char DestGroup;
                              unsigned char DestMultiCommIpRed[4];
                              unsigned char DestMultiCommIpBlue[4];  
        4.struc_CommConfig中删除unsigned char MultiCommIpBlue[4];
                                unsigned char MultiCommIpRed[4];
                                unsigned char Group;
*******************************************************************************/


#ifndef _SFPPRTCLSTRUCT_H
#define _SFPPRTCLSTRUCT_H

#include "CommonTypes.h"

/*统计断链信息的结构体*/
typedef struct S_DuanLianInfo {
    UINT32 DevName;                          /*设备类型+ID*/
    INT32  nTotalNums_DuanLian;              /*累积断链次数*/
    INT32  nTotalNums_ReSnd[2];              /*累积重发次数*/
} DuanLianInfo;


/***冗余输入节点结构类型**/
typedef struct InputBufferNode
{
    UINT32 DevName;          /*设备ID*/
    UINT8  DevSn[4];         /*对方SN*/
    UINT8  *data;            /*报文数据*/
    UINT8  cycle;            /*该数据在三维链表中存在的周期数*/
    struct InputBufferNode *NextNode;/*本类型中下一个ID的通信对方在链路表中的首地址*/
} struc_InputBuffer ;        /*链路管理表*/


/************************************************************************/
/*       预定义的通信链路表数据结构                                     */
/************************************************************************/

typedef struct
{   
    UINT8  chRole;                     /*定义两个宏常量0和1用来标识发起方和跟随方*/
    UINT32 nDestDevName;               /*设备NAME(ID+TYPE)*/
    UINT8  chStatus;                   /*链路状态*/
	UINT8 destNum;                     /* 目标系个数 */
    UINT8  lastframe;                  /*收到最新数据的报文类型*/
    UINT8  chDestIP[4][4];             /* IP地址chIP[0]为对方红网IP, chIP[1]为对方蓝网Ip*/
    UINT16 nPort[4];                   /*端口号，0为对方红网，1为对方蓝网*/
    UINT32 lastCycle;                  /*收到最新数据的周期,暂时程序中没有用到*/
    UINT32 RfcSN;                      /*RFC初始序列号*/
    UINT32 AckSN;                      /*ACK初始序列号*/
    UINT32 lDestSN;                    /*对方序列号*/
    UINT32 lLocalSN;                   /*本方序列号*/
    UINT16 nCycle[2];                  /*报文发送周期，0为对方，1为本方*/
    UINT16 nSnWidth[2];                /*对方数据态报文序列号有效宽度，本方数据态报文序列号有效宽度*/ 
    UINT16 nTimeOut;                   /*本方数据报文超时时钟*/
    UINT16 nACKTOut;                   /*本方ACK报文超时时钟*/ 
    UINT8  chNoDataRcvCycle;           /*没有收到数据的周期数，如果此值为零，也说明本周期收到了数据*/
    UINT16 DestMaxDelay;               /*对方报文最大延时差(单位为ms)，暂时程序中没有用到*/
} struc_Lnk_Info;                      /*链路信息*/

/***链路管理表节点结构类型***/
typedef struct Lnk_Mngr_Node
{
    UINT32 DevName;                    /*设备NAME(ID+TYPE)*/
    struc_Lnk_Info Lnk_Info;           /*链路信息，此处为一个节点*/
    struct Lnk_Mngr_Node *NextNode;    /*本类型中下一个ID的通信对方在链路表中的首地址*/
} struc_Lnk_Mngr ;                     /*链路管理表*/



/******输出中间缓存*********/
typedef struct lOutputDataNode 
{
    UINT32 DevName;        /*设备NAME(ID+TYPE)*/
    UINT8  DataStatus;     /*数据的状态：SENT  (0x33),已经发送出去,说明数据为空;NEED2SEND (0x55) 需要发送,说明存在数据*/
    UINT32 DataCycleNum;   /*数据的周期号*/
    UINT16 DataLen;        /*应用数据长度*/
    UINT8  *chData;        /*数据*/
    struct lOutputDataNode *pNext;
} struc_OutputDataBuff;    /*数据存储空间链表,*/



/********堆栈通用*********/
typedef struct 
{
    UINT32 *pNode;         /*空闲节点值，表示的是存储数据的空间*/
    UINT16  size;          /*目前栈的大小.同时这个值也可以描述当前堆栈中,再填写元素要存放的下标.由于本堆栈是一个顺序栈,因此顺序栈中此值应当是初始为零,
                                                        之后逐步增大,即从0到STACK_MAXSIZE-1.*/
    UINT16  totalsize;     /*此堆栈能够存放节点的最大数*/
    UINT32  pFirstNode;    /*压入堆栈的地址都是一个数组的各个元素，在空间释放时直接将此数组对应的地址空间释放，此变量用于记录数组地址*/
} StackElement;



/*****输出入口变量*******/
typedef struct AppOutput
{
    UINT16  AppDataLen;    /*应用数据长度,纯数据的长度*/
    UINT32  DestDevName;   /*目标设备ID＋TYPE*/
    UINT8  *chAppData;     /*数据*/
} struc_AppOutput;         /*数据队列中的一个结构体节点*/


typedef struct {
    UINT8  ID;            /*记录编号,相当于记录的主键   1 byte*/
    UINT32 DevName;       /*设备类型 4 byte*/
    UINT8  IPBlue[4];     /*蓝网IP地址  4 bytes*/
    UINT16 PortBlue;      /*蓝网端口    2 bytes*/
    UINT8  IPRed[4];      /*红网IP地址  4 bytes*/
    UINT16 PortRed;       /*红网端口    2 bytes*/
} struc_CommConfig;       /*通信配置表    17 bytes*/

/************************************************************************/
/*      DSU相关结构体定义                                               */
/************************************************************************/
typedef struct{
    UINT8  ID;              /*记录编号,相当于记录的主键   1 byte*/
    UINT32 SponDevType;     /*发起方设备类型 1 byte*/
    UINT8  SponCommType;    /* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）   1 byte*/
    UINT16 SponCycle;       /*发起方报文发送周期长度(报文发送周期长度) 2 bytes*/
    UINT16 SponMaxDelay;    /*发起方报文最大延时差（设B为报文到达对方的延迟时间最小值，设Ａ为报文到达对方的延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT16 AckMaxDelay;     /*7.11增加，发起方收到Ack报文最大延时差（设B为Rfc报文到达对方的延迟时间最小值，设Ａ为收到Ack报文延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT32 FlwDevType;      /*跟随方设备类型  1 byte*/
    UINT8  FlwCommType;     /*跟随方发送数据通信类型（也是发送方接收数据的通信类型） 1 byte*/
    UINT16 FlwCycle;        /*跟随方运行周期长度(报文发送周期长度)  2 bytes*/
    UINT16 FlwMaxDelay;     /*跟随方报文最大延时差（设B为报文到达对方的延迟时间最小值，设Ａ为报文到达对方的延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT8  NCyclesPerPack;  /*此通信关系中成功发送一包数据所需最小周期数  1 byte*/
} struc_DiffDevRelations;   /*不同类型设备间通信约束关系  14 bytes*/

typedef struct{
    UINT8  ID;              /*记录编号,相当于记录的主键   1 byte*/
    UINT32 SponDevName;     /*发起方设备name 4 byte*/
    UINT8  SponCommType;    /* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）1 byte*/
    UINT16 SponCycle;       /*发起方报文发送周期长度(报文发送周期长度) 2 bytes*/
    UINT16 SponMaxDelay;    /*发起方报文最大延时差（设B为报文到达对方的延迟时间最小值，设Ａ为报文到达对方的延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT16 AckMaxDelay;     /*7.11增加，发起方收到Ack报文最大延时差（设B为Rfc报文到达对方的延迟时间最小值，设Ａ为收到Ack报文延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT32 FlwDevName;      /*跟随方设备name  4 byte*/
    UINT8  FlwCommType;     /*跟随方发送数据通信类型（也是发送方接收数据的通信类型） 1 byte*/
    UINT16 FlwCycle;        /*跟随方运行周期长度(报文发送周期长度)  2 bytes*/
    UINT16 FlwMaxDelay;     /*跟随方报文最大延时差（设B为报文到达对方的延迟时间最小值，设Ａ为报文到达对方的延迟时间最大值，此变量值为Ａ－Ｂ）2 bytes*/
    UINT8  NCyclesPerPack;  /*此通信关系中成功发送一包数据所需最小周期数  1 byte*/
} struc_SameDevRelations;   /*同类型设备间通信约束关系    20 bytes*/


#else

#endif

