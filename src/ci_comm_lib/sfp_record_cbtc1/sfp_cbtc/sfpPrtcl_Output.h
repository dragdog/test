
/*******************************************************************************
* 文件名    ：  Prtcl_Output.h
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  通信协议输出单元模块的头文件
* 使用注意  ： 
                无
* 修改记录  ：  
    2008.7.10
        1.修改BuildOutputData以及Prtcl_OutputInitial的形参
*******************************************************************************/

#ifndef _SFPPRTCL_OUTPUT_H
#define _SFPPRTCL_OUTPUT_H

#include "sfpplatform.h"
#include "sfpLinkManage.h"
#include "sfpDeclare.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "CRC32.h"
#include "dsuRelatedDeal.h"
#include "sfpPrtclCommon.h"

#define NULL_ITEM				((struc_OutputDataBuff*)0x00000001)				/*得不到节点的反应，不同于空节点*/



#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
/*   数据存储单元链表的操作                                             */
/************************************************************************/


/******************************************************************************************
* 功能描述      : 在输入两节点之间插入一个节点,从堆栈中推出一个单元用来存储数据
* 输入参数      : struc_OutputDataBuff* pLastNode     前节点
                  struc_OutputDataBuff* pCurNode      当前节点
                  UINT32 DevName                      设备type+ID（这里是指对方的）
                  UINT8  data_status                  数据状态(已发送  或   未发送)
                  UINT16 data_len                     数据长度(这里指的是纯数据的长度)
                  UINT8 data[]                        纯数据
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput   输出单元用到的全局统一结构体指针
* 返回值        : 插入成功返回当前指针,否则返回错误(插入操作返回空即为错)        
*******************************************************************************************/
static struc_OutputDataBuff* Insert_OutputDataBuff( struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode,
		UINT32 DevName,UINT8 data_status,UINT16 data_len,UINT8 data[]);  

/******************************************************************************************
* 功能描述      : 给定前节点和本节点,删除本节点
* 输入参数      : struc_OutputDataBuff *pLastNode   前节点
                  struc_OutputDataBuff *pCurNode    当前节点
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput   输出单元用到的全局统一结构体指针
* 返回值        : 删除成功返回入口中当前节点后的上一个节点(NULL也是正确结果),错误返回为NULL_ITEM        
*******************************************************************************************/
static struc_OutputDataBuff* Delete_OutputDataBuff(struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode);



/************************************************************************/
/* 协议的输出                                                           */
/************************************************************************/


/******************************************************************************************
* 功能描述      : 协议输出处理的初始化
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info *pUnifyInfo  调用初始化函数对象对应的统一结构体
* 输出参数      :
* 全局变量      : UINT32 nNumsPrtclSnd                       协议输出的数据的个数(字节为单位)
                  struc_Lnk_Mngr* pCurProc_Lnk               链路管理表中当前处理的的链路指针,这里是指针，不是实体
                  UINT8 chCurProcItemSrc                     当前要处理的元素的来源
                  struc_OutputDataBuff *pLastDataBuffItem    表示当前处理的数据存储单元的指针
                  struc_OutputDataBuff *pCurDataBuffItem     表示上一次处理的数据存储单元的指针
* 返回值        : 初始化成功返回1 失败返回0        
*******************************************************************************************/
UINT8 Prtcl_OutputInitial(struc_Unify_Info *pUnifyInfo);

/******************************************************************************************
* 功能描述      : 从队列中取出数据放入出口结构体元素中
* 输入参数      : 
* 输入输出参数  : 
* 输出参数      : struc_AppOutput* pOut                    应用的输出的1个对象的数据,指针变量
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput     应用->协议输出单元入口                  
* 返回值        : 读到1个对象的数据返回1,
                  应用输出队列为空和有错误返回0，错误原因这里会做记录        
*******************************************************************************************/
static INT8 DeQueue_OutputData(struc_AppOutput *pOut);

/******************************************************************************************
* 功能描述      : 将数据队列中的数据填充到相关的数据缓冲链表节点中去
* 输入参数      : 
* 输入输出参数  : 
* 输出参数      : 
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput     应用->协议输出单元入口                  
* 返回值        : 成功返回1,失败返回-1       
*******************************************************************************************/
static INT8 Queue2Buff(void);

/******************************************************************************************
* 功能描述      : 得到要处理的下一个通信对象:这里的思路是从数据缓冲链表和从链路管理表中分别取出各自
		          未处理对象中DeviceName数值最大的节点，然后比较取出的这两个对象的DeviceName数值，选择其中数值大的节点作为上层
		          循环中此次要处理的对象。处理原则为：当此通信对象只是链路管理表中的对象是，在链
                  路管理表中添加对应节点（这种情况只会出现在协议输入单元收到RFC建链了）；当此对象只是数据缓冲链表中存在时，
                  先不在此环节对链路管理表作任何修改（这种情况在应用第一次要求向某对象发数据或者应用删链中会出现）；当此对象
                  在链路管理表中和数据缓冲链表中都存在时，也不再此环节对链路管理表和数据缓冲链表作处理，其余交付下一部分进
                  行处理。此环节的处理结果以全局变量的形式反映到下一环节。
		          其中返回值主要表示能否取出有效对象，而实际要处理的对象通过
* 输入参数      : UINT8 NeedToInit		是否需要初始化,如果需要为真否则为假
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_OutputDataBuff *pLastDataBuffItem;      表示当前处理的数据存储单元的指针
*                 struc_OutputDataBuff *pCurDataBuffItem;       表示上一次处理的数据存储单元的指针
*                 UINT8 chCurProcItemSrc;                       当前要处理的元素的来源
*                 struc_Unify_Info *pUnifyInfoForOutput         协议输出模块用的统一结构体指针
* 返回值        : 成功返回非负值（当有节点要处理时返回1，无节点要处理时返回0）,失败返回-1        
*******************************************************************************************/
static INT8 GetNextItem2Proc(UINT8 NeedToInit);

/******************************************************************************************
* 功能描述      : 给定帧类型，链路管理表以及应用数据，根据帧格式组出相应的帧
* 输入参数      : UINT8 frametype						帧类型
*                 struc_OutputDataBuff* pSrcData		应用数据存放缓冲区
*                 struc_Lnk_Mngr* pSrcLnk		        链路管理表中的链路
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info *pUnifyInfoForOutput;	协议输出用到的统一结构体变量
*                 UINT32 nNumsPrtclSnd;                     协议输出的数据的个数(字节为单位)
* 返回值        : 成功返回1        
*******************************************************************************************/
static INT8 BuildFrame(UINT8 frametype,struc_OutputDataBuff *pSrcData,struc_Lnk_Mngr *pSrcLnk);

/******************************************************************************************
* 功能描述      : 将链路管理表的某一条链路的所有信息填充到一维数组中去
* 输入参数      : struc_Lnk_Mngr* CurLnk				当前处理的链路
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info *pUnifyInfoForOutput;	协议输出用到的统一结构体变量
*                 UINT32 nNumsPrtclSnd;                     协议输出的数据的个数(字节为单位)
* 返回值        : 成功返回1       
*******************************************************************************************/
static INT8 FillLnkMngr_into_PrtclSnd(struc_Lnk_Mngr *CurLnk);
#ifdef __cplusplus
}
#endif


#else

#endif

