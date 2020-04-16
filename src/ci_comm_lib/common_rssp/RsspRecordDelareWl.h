/********************************************************
*                                                                                                            
* 文件名  ： RsspRecordDelareWl.h 
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.12.16
* 作者    ： 车载及协议部
* 功能描述： RSR层记录相关宏定义。
* 使用注意： 
*   
* 修改记录： 
*	2010.05.10 修改		追加Rssp新周期开始标识。
*
********************************************************/ 


#ifndef _RSSP_RECORD_DECLARE_WL_H
#define _RSSP_RECORD_DECLARE_WL_H

#include "RsspConfigWl.h"

/********************************************************************************
宏定义
********************************************************************************/

/* 各个标志位 */
#define RSSP_CYCLE_TAG_WL		0xEE				/* RSSP新周期开始标识 A_2010_05_10 */
#define RSSP_RCV_TAG_WL		0xF4				/* RSSP输入单元标识 */
#define RSSP_OUT_TAG_WL		0xF5				/* RSR输出单元标识 */
#define RSSP_MSG_IN_TAG_WL		0x04				/* RSSP输入一帧消息起始标识(B 0000 0100) */
#define RSSP_MSG_OUT_TAG_WL	0x08				/* RSSP输出一帧消息起始标识(B 0000 1000) */
#define RSSP_NODE_OUT_TAG_WL	0x0C				/* RSSP输出一帧消息起始标识(B 0000 1100) */
#define RSSP_OUT_E_TAG_WL		0x0F				/* RSSP输出单元结束标识(B 0000 1111) */

/************************************************************************
 * 错误码编码原则，协议错误码一共16bit
 * D15~D12：保留
 * D11~D10：错误等级
 * D9~D8：模块位置
 * D7~D4：包括各个模块特有的错误，在模块不同时次4位代表的含义不同
 * 区别于队列链表等底层错误
 * D3~D0：函数出错处理，不论模块是否相同此4位代表的含义相同                                                                     
************************************************************************/
/* 错误等级 */
#define RSSP_ERR                                 0x0800
#define RSSP_WARN                                0x0400

/* 模块位置 */
#define  RSSP_RECEIVE                            0x0200		/* 输入模块 */
#define  RSSP_OUTPUT                             0x0100		/* 输出模块 */

/* 各模块特有错误 */
#define RSSP_ERROR_NULL                          0x0000
/* 输入 */
#define RSSP_MSG_TYPE_ILLEGAL                    0x0010      /* 不是RSSP消息 */
#define RSSP_MSG_OLD_ERR                         0x0020      /* 非最新报文 */
#define RSSP_DEL_MSG_LNK_ERR                     0x0030      /* 临时报文删除失败 */
#define RSSP_MSG_NOT_NODE_ERR                    0x0040      /* 报文信息与通信节点信息不一致 */
#define RSSP_MSG_LEN_ERR		                    0x0050      /* 报文长度不正 */
#define RSSP_MSG_FMT_ILLEGAL						0x0060		/* 报文格式不对 */
#define RSSP_SSR_NOT_ILLEGAL						0x0070		/* 不是对应SSR报文信息 */
#define RSSP_MSG_REPLACE_ERR						0x0080		/* 覆盖节点失败 */
#define RSSP_MSG_NO_REPLACENODE					0x0090		/* 不存在覆盖节点 */
#define RSSP_MSG_NO_INVALID						0x00A0		/* BSD解码失败 */
#define RSSP_MSG_LOST_INVALID					0x00B0		/* 丢包数超过 最大的丢包数*/
#define RSSP_MSG_TOUT_INVALID					0x00C0		/* 报文超时*/
#define RSSP_MSG_CRCM_INVALID					0x00D0		/* 检验CRCM无效*/

/* 输出 */
#define RSSP_DBGET_ERR							0x0050      /* 数据库查询错误 */
/* 函数出错处理，底层处理错误 */
#define RSSP_QUEUE_READ                          0x0001      /* 队列读有错 */
#define RSSP_QUEUE_WRITE                         0x0002      /* 队列写有错 */
#define RSSP_QUEUE_INTEGRITY                     0x0003		/* 队列中数据不完整（包括scan有错，应用给出错误）*/
#define RSSP_LNK_PUSH							0x0004		/* 链路管理表节点压栈错误 */
#define RSSP_LNK_POP								0x0005		/* 序列号缓存链表节点出栈错误 */


/******错误码宏定义**************/
#define RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR			0x00000001				/*接收模块报文信息与通信节点信息不一致*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_LEN_ERR					0x00000002				/*接收模块报文长度不正确*/		
#define RSSP_WARN_RECEIVE_RSSP_SSR_NOT_ILLEGAL			0x00000004				/*接收模块不是对应SSR报文信息*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_NO_INVALID			0x00000008				/*接收模块BSD解码失败*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_REPLACE_ERR			0x00000010				/*接收模块覆盖节点失败*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_NO_REPLACENODE		0x00000020				/*接收模块不存在覆盖节点*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_OLD_ERR					0x00000040				/*接收模块非最新报文*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_WRITE					0x00000080				/*接收模块队列写有错误*/	
#define RSSP_WARN_RECEIVE_RSSP_DEL_MSG_LNK_ERR			0x00000100				/*接收模块临时报文删除失败*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_READ					0x00000200				/*接收模块队列读有错*/	
#define RSSP_WARN_RECEIVE_RSSP_LNK_POP						0x00000400				/*接收模块序列号缓存链表节点出栈错误*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_TYPE_ILLEGAL			0x00000800				/*接收模块不是RSSP消息*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_INTEGRITY			0x00001000				/*接收模块队列中数据不完整*/	

#define RSSP_WARN_OUTPUT_RSSP_QUEUE_READ						0x00002000				/*发送模块队列读有错误*/	
#define RSSP_WARN_OUTPUT_RSSP_MSG_FMT_ILLEGAL			0x00004000				/*发送模块报文格式不对*/	
#define RSSP_WARN_OUTPUT_RSSP_MSG_REPLACE_ERR			0x00008000				/*发送模块覆盖节点失败*/	
#define RSSP_WARN_OUTPUT_RSSP_QUEUE_WRITE					0x00010000				/*发送模块队列写失败*/	



#endif

