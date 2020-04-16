/********************************************************
*                                                                                                            
* 文件名  ： FsfbRecordDelare.h 
* 版权说明： 北京交控科技有限工司 
* 版本号  ： 1.0
* 创建时间： 2013.09.09
* 作者    ： 王佩佩
* 功能描述： FSFB协议记录相关宏定义。
* 使用注意： 
*   
* 修改记录： 
*	2010.05.10 修改		追加FSFB新周期开始标识。
*
********************************************************/ 


#ifndef _FSFB_RECORD_DECLARE_H
#define _FSFB_RECORD_DECLARE_H


/********************************************************************************
宏定义
********************************************************************************/

/* 各个标志位 */
#define FSFB_CYCLE_TAG		0xDD				/* FSFB新周期开始标识 */
#define FSFB_RCV_TAG		0xF4				/* FSFB输入单元标识 */
#define FSFB_OUT_TAG		0xF5				/* FSFB输出单元标识 */
#define FSFB_MSG_IN_TAG		0x04				/* FSFB输入一帧消息起始标识(B 0000 0100) */
#define FSFB_MSG_OUT_TAG	0x08				/* FSFB输出一帧消息起始标识(B 0000 1000) */
#define FSFB_NODE_OUT_TAG	0x0C				/* FSFB输出一帧消息起始标识(B 0000 1100) */
#define FSFB_OUT_E_TAG		0x0F				/* FSFB输出单元结束标识(B 0000 1111) */

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
#define FSFB_ERR                                 0x0800
#define FSFB_WARN                                0x0400

/* 模块位置 */
#define  FSFB_RECEIVE                            0x0200		/* 输入模块 */
#define  FSFB_OUTPUT                             0x0100		/* 输出模块 */

/* 各模块特有错误 */
#define FSFB_ERROR_NULL                          0x0000
/* 输入 */
#define FSFB_MSG_TYPE_ILLEGAL                    0x0010      /* 不是FSFB消息 */
#define FSFB_MSG_OLD_ERR                         0x0020      /* 非最新报文 */
#define FSFB_DEL_MSG_LNK_ERR                     0x0030      /* 临时报文删除失败 */
#define FSFB_MSG_NOT_NODE_ERR                    0x0040      /* 报文信息与通信节点信息不一致 */
#define FSFB_MSG_LEN_ERR		                    0x0050      /* 报文长度不正 */
#define FSFB_MSG_FMT_ILLEGAL						0x0060		/* 报文格式不对 */
#define FSFB_SSR_NOT_ILLEGAL						0x0070		/* 不是对应SSR报文信息 */
#define FSFB_MSG_REPLACE_ERR						0x0080		/* 覆盖节点失败 */
#define FSFB_MSG_NO_REPLACENODE					0x0090		/* 不存在覆盖节点 */
#define FSFB_MSG_NO_INVALID						0x00A0		/* BSD解码失败 */
#define FSFB_MSG_CRC_ERR                        0x00B0
/* 输出 */
#define FSFB_DBGET_ERR							0x0050      /* 数据库查询错误 */
/* 函数出错处理，底层处理错误 */
#define FSFB_QUEUE_READ                          0x0001      /* 队列读有错 */
#define FSFB_QUEUE_WRITE                         0x0002      /* 队列写有错 */
#define FSFB_QUEUE_INTEGRITY                     0x0003		/* 队列中数据不完整（包括scan有错，应用给出错误）*/
#define FSFB_LNK_PUSH							0x0004		/* 链路管理表节点压栈错误 */
#define FSFB_LNK_POP								0x0005		/* 序列号缓存链表节点出栈错误 */



#endif

