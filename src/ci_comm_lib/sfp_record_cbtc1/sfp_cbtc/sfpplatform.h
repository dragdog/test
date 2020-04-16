
/*******************************************************************************
* 文件名    ：  platform.h
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  此文件为协议为应用提供的头文件 
                 1. 协议为应用提供的函数声明都在此头文件中
                 2. 此文件包含统一结构体的定义
                 3. Windows平台下要应用提供获取当前时间计数值的函数，并且协议要根据
	                此函数修改PrtclCommon.h文件中的GetTick()函数
* 使用注意  ： 
                无
* 修改记录  ：  
    1.08.9.15
        将结构体中的数据库指针以及时间戳指针的数据类型定义成const UINT8*和const UINT32 *
    2009.1.20修改记录
        1.删除不同平台的头文件包含
        2.删除各对象宏定义;
        3.增加ProtclReceive函数申明
        4.在统一结构体里面增加 CommBuffer，BlueNetNoDataCycle，RedNetNoDataCycle参数
        5.统一结构体里面将CycleNum的数据类型由原来的unsigned long改为unsigned long*

    2009.1.21修改记录
        1.删除原先的输入以及冗余函数的申明， char Prtcl_Receive(struc_Unify_Info *pUnifyInfo);以及unsigned char Redundancy(struc_Unify_Info *pUnifyInfo);
          放到各子文件。
        2.将统一结构体中的OutnetQueueSize，PrtclSndBuffSize，PrtclRcvSize，DataToAppSize，OutputDataQueueSize数据类型有short改为long。

    2009-8-27 14:53:00修改记录
        1.删除统一结构体中NetStatusA、NetStatusB、PtrclRcv、BlueNetNoDataCycle、RedNetNoDataCycle
        2.修改类型，使用CommonTypes.h中的定义
	2012-8-15 15:54:00修改记录
	    1.增加SFP协议版本号宏定义
*******************************************************************************/

#ifndef _SFPPLATFORM_H
#define _SFPPLATFORM_H

#include "sfpPrtclStruct.h"
#include "CommonQueue.h"
#include "sfpConfig.h"


/* SFP协议版本号定义 */
#define SFPCODE_VER_PRODUCT_NO 66
#define SFPCODE_VER_MODULE_NO  1
#define SFPCODE_VER_MAIN_VER  2
#define SFPCODE_VER_SUB_VER  0

/**************************所有系统公用的部分**********************************/

/******************统一结构体变量定义*********************/
typedef struct Unify_Info
{
	UINT8  LocalID;                           /*设备ID，由应用按文档在初始化时填写*/
	UINT8  LocalType;                         /*设备type，由应用按文档在初始化时填写*/
	UINT32 *CycleNum;                         /*周期号，由应用按文档在每周期开始时填写*/
	UINT32 RadamSN;                           /*随机数，由应用按文档在每周期开始时填写*/
	UINT16 RedundTimeout;                     /*冗余模块执行时限（单位ms，为冗余执行的最大时间），由应用按文档在初始化时填写*/
	UINT16 OutputTimeout;                     /*输出模块执行时限（单位ms，为输出执行的最大时间），由应用按文档在初始化时填写*/
	struct S_DuanLianInfo *NumBuildLink;      /*断链结构体，参考断链结构体定义和文档说明*/
	struc_InputBuffer *InputBufferHead;       /*冗余链表头结点*/
	struc_Lnk_Mngr *LnkHead;                  /*链路管理表头结点*/
	struc_OutputDataBuff *OutputDataBuffHead; /*输出链表头结点*/
	UINT8  MaxNumLink;                        /*同时通信的最大链路数，由应用按文档在初始化时填写*/
    UINT8  MaxNumPerCycle;                    /*一周期允许每个对象的最大有效数据，由应用按文档在初始化时填写*/
	UINT16 InputSize;                         /*接受数据中最大一帧的应用数据长度，由应用按文档在初始化时填写*/
	UINT16 OutputSize;                        /*发送数据中最大一帧的应用数据长度，由应用按文档在初始化时填写*/
	UINT8 *DataAfterReduntancy;               /*冗余模块处理后给输入模块的数组*/
	UINT32 PrtclRcvSize;                      /*DataAfterReduntancy最大空间*/
	UINT8 *RecordArray;                       /*记录数组*/
	UINT16 RecordArraySize;                   /*记录数组大小*/
	UINT8 *PrtclSndBuff;                      /*输出模块的出口*/
	UINT32 PrtclSndBuffSize;                  /*输出模块的出口数组长度*/
	QueueStruct DataToApp;                    /*协议输入给应用的数据队列*/
	UINT32 DataToAppSize;                     /*协议输入给应用的数据队列长度*/
	QueueStruct OutputDataQueue;              /*应用给协议输出的数据队列*/
	UINT32 OutputDataQueueSize;               /*应用给协议输出的数据队列长度*/
	QueueStruct OutnetQueueA;                 /*内网接收队列A(内网->冗余红网队列)*/
	QueueStruct OutnetQueueB;                 /*内网接收队列B(内网->冗余蓝网队列)*/
	UINT32 OutnetQueueSize;                   /*内网接收队列长度，由应用按文档初始化时填写*/
	StackElement LnkStack;                    /*链路管理表堆栈*/
	StackElement InputStack;                  /*一维链表堆栈*/
	StackElement OutputStack;                 /*输出一维链表堆栈*/
	const UINT8  *DataBase;                   /*数据库指针*/    
    const UINT32 *TimeCounter;                /*计时变量，用于处理各个模块的计时*/
    UINT8  *CommBuffer;                       /*协议内部使用，输入临时变量09年增加*/
}struc_Unify_Info;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************/
/*********************函数声明***************************/
/********************************************************/ 
 
/******************************************************************************************
* 功能描述      : 协议初始化函数
                   1. 在调用此函数之前，应用要将统一结构体中InputSize，OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize等变量赋值
* 输入参数      : UINT8 *   fileName  存放数据的文件名或地址
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 正常初始化返回1，否则返回0        
*******************************************************************************************/
UINT8 ProtclInit(UINT8 *fileName,struc_Unify_Info *pUnifyInfo);



/******************************************************************************************
* 功能描述      : 协议输入函数
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 正确返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclReceive(struc_Unify_Info *pUnifyInfo);


/******************************************************************************************
* 功能描述      : 协议输出单元的主函数,根据协议的规则,将应用传来的数据组帧,
                  并为数据比较单元准备好必需的数据,同时维护链路管理表
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      : UINT32 nNumsPrtclSnd                       协议输出的数据的个数(字节为单位)
                  struc_Lnk_Mngr *pCurProc_Lnk               链路管理表中当前处理的的链路指针,这里是指针，不是实体*
                  UINT8 chCurProcItemSrc                     当前要处理的元素的来源 ITEMSRC_BOTH (数据队列和链路管理表) 0x17;ITEMSRC_DATAQUEUE (数据队列) 0x33;ITEMSRC_LNKMNGR(链路管理表) 0x55
                  struc_OutputDataBuff *pLastDataBuffItem    表示当前处理的数据存储单元的指针
                  struc_OutputDataBuff *pCurDataBuffItem     表示上一次处理的数据存储单元的指针
                  struc_Unify_Info *pUnifyInfoForOutput      输出用平台变量结构体指针
* 返回值        : 正确返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclOutput(struc_Unify_Info *pUnifyInfo);


/******************************************************************************************
* 功能描述      : 为应用提供的在链路管理表中删除节点的操作——删除链路管理表中指定节点
                   1. 应用调用此函数来删除相应Type和ID的链路
* 输入参数      : UINT8 DestType              对方设备类型
                  UINT8 DestID                对方设备ID
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo        平台提供的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclDeleteLnk(UINT8 DestType,UINT8 DestID,struc_Unify_Info *LnkUnifyInfo);

/******************************************************************************************
* 功能描述      : 查询链路管理表中某条链路在本周期的链路状态
                   1. 应用根据设备类型和ID查询链路管理表中此链路本周期的链路状态
* 输入参数      : UINT8 DestType             对方设备类型  
                  UINT8 DestID               对方设备ID
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo     平台提供的统一结构体
* 输出参数      : 
* 全局变量      :
* 返回值        : 指定节点的链路状态，错误返回0        
*******************************************************************************************/
UINT8 ProtclCheckLnkStatus(UINT8 DestType,UINT8 DestID,struc_Unify_Info *LnkUnifyInfo );
  
  	  
 
/******************************************************************************************
* 功能描述      : 刷新备通道链路管理表，由平台应用在需要更新链路管理表示调用
* 输入参数      : INT16 sLen_Input           输入数组长度  
                  UINT8 *Input_Data_LnkMes   输入数组头指针
* 输入输出参数  : struc_Unify_Info *LnkUnifyInfo     平台提供的统一结构体
* 输出参数      : 
* 全局变量      :
* 返回值        : 正常返回1，异常返回0        
*******************************************************************************************/
UINT8 ProtclFreshLnkMngr(INT16 sLen_Input,UINT8 *Input_Data_LnkMes, struc_Unify_Info *LnkUnifyInfo );



/******************************************************************************************
* 功能描述      : 释放协议在PrtclInit函数中分配的内存空间
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 正确返回1        
*******************************************************************************************/
UINT8 ProtclFreeSpace(struc_Unify_Info *pUnifyInfo);

#ifdef __cplusplus
}
#endif

#else
#endif
