
/*******************************************************************************
* 文件名    ：  Declare.h
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  此文件为协议公用头文件各部分的宏定义都在此文件当中声明，
                各文件都应包含此文件
* 使用注意  ： 
                无
* 修改记录  ：  
                无
*******************************************************************************/


#ifndef _SFPDECLARE_H
#define _SFPDECLARE_H


/********************************************************************************
宏定义
********************************************************************************/



/******链路参数宏定义**************/
#define ROLE_SPON                           0x00                    /*发起方*/
#define ROLE_FLW                            0x01                    /*跟随方*/	

#define STATUS_DATA                         0x17                    /*链路状态：数据状态*/
#define STATUS_HALT                         0x2b                    /*链路状态：通信中断状态*/
#define STATUS_PRECON                       0x33                    /*链路状态：准备建立连接状态*/
#define STATUS_WCONACK                      0x4e                    /*链路状态：等待建立连接状态*/

#define COMMTYPE_MULTICOMM                  0x33                    /*通信类型：组播*/
#define COMMTYPE_P2P                        0x55                    /*通信状态：点对点*/

/*帧类型*/
#define FRAME_RFC                           0x17                    /*连接请求帧*/
#define FRAME_ACK                           0x2b                    /*连接确认帧*/
#define FRAME_DATA                          0x35                    /*数据帧*/
#define FRAME_IDLE                          0x59                    /*维持连接报文*/

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
#define ERR                                 0x0800
#define WARN                                0x0C00

/*模块位置*/
#define  NET_STATUS                         0x0000                  /*红蓝网诊断*/
#define  REDUN                              0x0100                  /*冗余模块*/
#define  RECEIVE                            0x0200                  /*输入模块*/
#define  OUTPUT                             0x0300                  /*输出模块*/

/*各模块特有错误*/
#define ERROR_NULL                          0x0000
/*冗余*/
#define FRAMETYPE_ROLE_CONFLICT             0x0010                  /*帧类型与对象不符*/
#define SN_ILLEGAL                          0x0020                  /*序列号不合法*/
#define FRAMETYPE_STATUS_CONFLICT           0x0030                  /*帧类型和链路状态不合法*/
#define RE_RECV_RFC                         0x0040                  /*重复收到RFC*/
#define CRC_ERR                             0x0050                  /*CRC错误*/
#define OBJ_ILLEAGL                         0x0060                  /*通信对象不合法*/
#define DESTNAME_ERR                        0x0070                  /*目标Name错误*/
#define OUTNETDATA_TOO_LONG                 0x0080                  /*外网数据长度过长*/

/*输入*/
#define NODE_LOST                           0x0010                  /*输入入口数组中有数据但是链表中没有*/
/*输出*/
#define ROLE_STATUS_CONFLICT                0x0010                  /*本方角色和链路状态不相符*/
#define APPDATA_LENGTH_ERR                  0x0020                  /*应用给出的数据过长*/

/*函数出错处理，底层处理错误*/
#define TIMEOUT                             0x0001                  /*模块超时*/
#define QUEUE_READ                          0x0002                  /*队列读有错*/
#define QUEUE_WRITE                         0x0003                  /*队列写有错*/
#define QUEUE_INTEGRITY                     0x0004                  /*队列中数据不完整（包括scan有错，应用给出错误）*/
#define LNKMANAGE_PUSH                      0x0005                  /*链路管理表节点压栈错误*/
#define LNKMANAGE_POP                       0x0006                  /*链路管理表节点出栈错误*/
#define INPUT_LNK_PUSH                      0x0007                  /*一维数据表节点压栈错误*/
#define INPUT_LNK_POP                       0x0008                  /*一维数据表节点出栈错误*/
#define OUTPUT_LNK_PUSH                     0x0009                  /*输出数据链表压栈错误*/
#define OUTPUT_LNK_POP                      0x000a                  /*输出数据链表出栈错误*/
#define ARRAY_FULL                          0x000b                  /*数组溢出*/



/******错误码宏定义**************/
 
#define ERR_REDUN_INPUT_LNK_POP							0x00000001		/*冗余模块节点出栈错误*/
#define ERR_REDUN_TIMEOUT						    			0x00000002		/*冗余模块超时错误*/
#define WARN_REDUN_QUEUE_INTEGRITY		    			0x00000004		/*冗余模块队列中数据不完整错误*/
#define WARN_REDUN_OUTNETDATA_TOO_LONG    			0x00000008		/*冗余模块外网数据长度过长*/
#define WARN_REDUN_QUEUE_READ								0x00000010		/*冗余模块队列读有错误*/
#define WARN_REDUN_FRAMETYPE_ROLE_CONFLICT 		0x00000020		/*冗余模块帧类型与对象不符*/
#define WARN_REDUN_DESTNAME_ERR 							0x00000040		/*冗余模块目标对象错误*/
#define WARN_REDUN_SN_ILLEGAL								0x00000080		/*冗余模块序列号不合法*/
#define WARN_REDUN_FRAMETYPE_STATUS_CONFLICT 		0x00000100 		/*冗余模块帧类型和链路状态不合法*/
#define WARN_REDUN_RECV_RFC									0x00000200 		/*冗余模块重复收到RFC*/
#define WARN_REDUN_CRC_ERR										0x00000400		/*冗余模块CRC错误*/
#define WARN_REDUN_OBJ_ILLEAGL								0x00000800 		/*冗余模块通信对象不合法*/

#define ERR_OUTPUT_TIMEOUT										0x00001000		/*输出模块超时*/
#define ERR_OUTPUT_OUT_LNK_POP								0x00002000 		/*输出模块输出数据链表出栈错误*/
#define ERR_OUTPUT_OUT_LNK_PUSH							0x00004000 		/*输出模块输出数据链表压栈错误*/
#define ERR_OUTPUT_QUEUE_INTEGRITY						0x00008000 		/*输出模块队列中数据不完整错误*/
#define ERR_OUTPUT_APPDATA_LENGTH_ERR  				0x00010000 		/*输出模块应用给出的数据过长*/
#define ERR_OUTPUT_QUEUE_READ       						0x00020000		/*输出模块队列读有错*/
#define ERR_OUTPUT_ARRAY_FULL								0x00040000 		/*输出模块数组溢出*/
#define ERR_OUTPUT_LNKMANAGE_POP							0x00080000 		/*输出模块链路管理表节点出栈错误*/
#define ERR_OUTPUT_ROLE_STATUS_CONFLICT 				0x00100000 		/*输出模块本方角色和链路状态不相符*/
#define WARN_OUTPUT_OBJ_ILLEAGL							0x00200000 		/*输出模块通信对象不合法*/

#define ERR_RECEIVE_LNKMANAGE_POP							0x00400000 		/*接收模块链路管理链表出栈错误*/
#define WARN_RECEIVE_FRAMETYPE_STATUS_CONFLICT	0x00800000 		/*接收模块帧类型和链路状态不合法*/
#define WARN_RECEIVE_QUEUE_WRITE							0x01000000 		/*接收模块队列写有错误*/
#define WARN_RECEIVE_STATUS_DATATOHALT				0x02000000 		/*接收模块超时由数据帧转为中断*/
#define WARN_RECEIVE_DEL_LNK									0x04000000 		/*超时删链*/

/************************************************/


/****************************协议冗余处理使用的宏定义*********************/
/*外网接收到的内网的内容*/
#define RCV_DATA_LEN                        0                       /*0,从内网接收数据的长度*/
#define RCV_SRC_SN                          (RCV_DATA_LEN+2)        /*2,从内网接收数据的发送方序列号*/
#define RCV_AIM_SN                          (RCV_SRC_SN+4)          /*6,从内网接收数据的接收方序列号*/
#define RCV_FRAME_TYPE                      (RCV_AIM_SN+4)          /*10,从内网接收数据的报文类型*/
#define	RCV_AIM_DN                          (RCV_FRAME_TYPE+1)      /*11,从内网接收数据的目标设备NAME*/
#define	RCV_SRC_DN                          (RCV_AIM_DN+4)          /*15,从内网接收数据的源设备NAME*/
#define	RCV_APP_DATA                        (RCV_SRC_DN+4)          /*19,从内网接收数据*/

/*经冗余处理后放入一维链表中的数据*/
#define LINK_DATA_LEN                       0                       /*0,一维链表节点中的数据的长度*/
#define LINK_SRC_SN                         (LINK_DATA_LEN+2)       /*2,一维链表节点中的数据的发送方序列号*/
#define	LINK_AIM_SN                         (LINK_SRC_SN+4)         /*6,一维链表节点中的数据的接收方序列号*/
#define	LINK_FRAME_TYPE                     (LINK_AIM_SN+4)         /*10,一维链表节点中的数据的报文类型*/
#define	LINK_SRC_DN                         (LINK_FRAME_TYPE+1)     /*11,一维链表节点中的数据的源设备ID*/
#define	LINK_APP_DATA                       (LINK_SRC_DN+4)         /*15,一维链表节点中的数据的源设备类型*/
/***********************************/


/**************协议输入宏定义*****************************************/
/************协议输入接口一维数组中各元素下标*******/
#define NODATA                              0                       /*接收数组中没有应用数据*/
#define LEN_IDX                             0                       /*接收数组中本帧数据长度下标*/
#define SENDSN_IDX                          2                       /*接收数组中发送序列号下标*/
#define REVSN_IDX                           6                       /*接收数组中发送序列号下标*/
#define FRAMETYPE_IDX                       10                      /*接收数组中报文类型下标*/
#define DESTID_IDX                          11                      /*接收数组中源逻辑ID下标*/
#define DESTTYPE_IDX                        14                      /*接收数组中源逻辑ID下标*/
#define DESTData_IDX                        DESTID_IDX+4            /*接受数组中应用数据下标*/
/*#define MAX_TIMEOUTREV                      0x04             */       /*跟随方最大超时周期数，暂定10个*/

/*报错*/
#define COM_ERROR                           0                       /*平台报错*/
#define DONE                                0x01                    /*协议输入处理完成*/
/******************************/



/*******************协议输出用宏定义*******************/
#define ITEMSRC_BOTH                        0x17                    /*操作对象的来源来自数据队列和链路管理表*/
#define ITEMSRC_DATABUFF                    0x33                    /*操作对象的来源来自数据队列*/
#define ITEMSRC_LNKMNGR                     0x55                    /*操作对象的来源来自链路管理表*/

#define NEED2SEND                           0x33                    /*需要发送数据*/
#define SENT                                0x55                    /*已经发送过*/
/**********************************/


#define LNKFRAMELEN                         0x24                    /* 中间变量一帧数据长度 */
#endif
