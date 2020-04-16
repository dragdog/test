
/************************************************************************
* 文件名：RpDeclare.h
* 文件描述：此文件为冗余层协议公用头文件各部分的宏定义都在此文件当中声明
* 创建时间：2009.11.16          
************************************************************************/

#ifndef _RPDECLARE_H
#define _RPDECLARE_H


/********************************************************************************
宏定义
********************************************************************************/


/************************************************************************
 * 错误码编码原则，协议错误码一共16bit
 * D15~D12：保留
 * D11~D10：错误等级
 * D9~D8：模块位置
 * D7~D4：包括各个模块特有的错误，在模块不同时次4位代表的含义不同
 * 区别于队列链表等底层错误
 * D3~D0：函数出错处理，不论模块是否相同此4位代表的含义相同                                                                     
************************************************************************/
/*错误等级*/
#define RP_ERR                          0x0800
#define RP_WARN                         0x0400

/*模块位置*/
#define  RP_RECEIVE                     0x0200                  /*输入模块*/
#define  RP_OUTPUT                      0x0100                  /*输出模块*/

/*各模块特有错误*/
#define RP_ERROR_NULL                   0x0000
/*输入*/
#define RP_EDITION_ILLEGAL              0x0010                  /*版本号非法*/
#define RP_SN_ILLEGAL                   0x0020                  /*序列号不合法*/
#define RP_DESTNAME_ERR                 0x0030                  /*目标Name错误*/
#define RP_QUEUE_OVLOAD                 0x0040                  /*出口队列空间不足*/
/*输出*/
#define RP_DBGET_ERR                    0x0050                  /*数据库查询错误*/
/*函数出错处理，底层处理错误*/
#define RP_QUEUE_READ                   0x0001                  /*队列读有错*/
#define RP_QUEUE_WRITE                  0x0002                  /*队列写有错*/
#define RP_QUEUE_INTEGRITY              0x0003                  /*队列中数据不完整（包括scan有错，应用给出错误）*/
#define RP_LNK_PUSH                     0x0004                  /*序列号缓存链表节点压栈错误*/
#define RP_LNK_POP                      0x0005                  /*序列号缓存链表节点出栈错误*/
#define RP_QUEUE_INTEGRITY_R            0x0006                  /*输入红网队列超长或不完整*/
#define RP_QUEUE_INTEGRITY_B            0x0007                  /*输入蓝网队列超长或不完整*/
#define RP_QUEUE_OVERLONG               0x0008                  /*输出队列帧数据超长*/


/******错误码宏定义**************/
#define RP_ERR_RECEIVE_RP_LNK_POP					 		0x00000001		/*接收模块序列号缓存链表节点出栈错误*/
#define RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_R	 	0x00000002		/*接收模块输入红网队列超长或不完整*/
#define RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_B		0x00000004		/*接收模块输入蓝网队列超长或不完整*/
#define RP_WARN_RECEIVE_RP_QUEUE_READ					0x00000008		/*接收模块队列读有错*/
#define RP_WARN_RECEIVE_RP_EDITION_ILLEGAL			0x00000010		/*接收模块版本号非法*/
#define RP_WARN_RECEIVE_RP_DESTNAME_ERR				0x00000020		/*接收模块目标名称错误*/
#define RP_WARN_RECEIVE_RP_SN_ILLEGAL					0x00000040		/*接收模块序列号不合法*/
#define RP_WARN_RECEIVE_RP_QUEUE_OVLOAD				0x00000080		/*接收模块出口队列空间不足*/

#define RP_ERR_OUTPUT_RP_LNK_POP							0x00000100		/*发送模块序列号缓存链表节点出栈错我*/
#define RP_WARN_OUTPUT_RP_QUEUE_OVERLONG				0x00000200		/*输出模块输出队列帧数据超长*/
#define RP_WARN_OUTPUT_RP_QUEUE_INTEGRITY			0x00000400		/*输出模块队列中数据不完整*/
#define RP_WARN_OUTPUT_RP_QUEUE_READ					0x00000800		/*输出模块队列读有错*/







#endif

