/** ******************************************************
*                                                                                                            
* 文件名  ： PrtclBaseParse.h
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 协议初始化  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef _PROTOCOL_BASE_PARSE_H_
#define _PROTOCOL_BASE_PARSE_H_

#include "CM_RingQueue.h"

#ifdef __cplusplus
extern "C" {
#endif
    /* 初始化并计算包头信息 需要先设置好包头
	1、	由应用初始化函数调用接口Prtcl_Init ；
    2、	模块内依次调用各个子协议的初始化函数，同时记录是否初始化成功，如果有一个失败就结束返回失败；
    3、	返回成功，结束流程。 
	*/
#define MAX_PACKAGE_HEADorTAIL_Lenth 10  /* /包头或者包尾的最大长度 */
	typedef struct _PRTCL_BASE_CONF
	{
		UINT8 m_Head[MAX_PACKAGE_HEADorTAIL_Lenth];
		UINT8 m_HeadTable[MAX_UNSIGNED_CHAR_SIZE];   /* 搜索的时候使用的向量表*/
		UINT16 m_HeadLen;                            /* 包头长度*/
		CM_BOOL m_SearchDir;                            /* bSearchDir 为搜索方向  TRUE 为 正序  FALSE为倒序*/

		CM_BOOL m_bCheckCRC;                            /* 标记是否进行CRC校验 默认 crc都会在整包数据的最后2位*/

		UINT8 uReadLen;                               /*包头+数据长度的字节数 */
		UINT8 uLenByte;                               /*数据长度由几个字节表示 */ /*目前最多处理两个字节*/
		UINT8 uAddLen;                                /*整包数据长度=数据长度+AddLen */

														   /*  /*暂时不用尾部信息
														   CM_UINT8 m_Tail[MAX_PACKAGE_HEADorTAIL_Lenth];
														   CM_UINT8 m_TailTable[MAX_UNSIGNED_CHAR_SIZE]; */
	} PRTCL_BASE_CONF, *PRTCL_BASE_CONFID;
    
    CM_BOOL Prtcl_Base_Init(/*通用协议结构体指针*/IN OUT PRTCL_BASE_CONFID pHeadID);	
	
	/* 通用协议解析模块 使用已经初始化的协议信息 对队列中数据进行解析
	输入：
	IN QueueRingID pQueue,储存缓冲数据的环形队列
	IN const PRTCL_BASE_CONFID pHeadID,初始化过的协议包头
	IN const UINT32 dwBufMax,指定输出缓冲区的大小
	输出：
	OUT UINT8 *pBuf,输出解析出来的完整一包
	OUT UINT32 *pBufLen 指定解析出的完整一包的大小
	返回值：是否解析成功 成功TURE 其他FALSE*/
    CM_BOOL Prtcl_Base_Check(/* 储存缓冲数据的环形队列*/IN QueueRingID pQueue,
		                           /* 初始化过的协议包头*/IN const PRTCL_BASE_CONFID pHeadID,
                                   /* 输出解析出来的完整一包*/ OUT UINT8 *pBuf,
		                           /* 指定输出缓冲区的大小*/IN const UINT32 dwBufMax,
		                           /* 指定解析出的完整一包的大小*/OUT UINT32 *pBufLen
                            );

	/* 用于RSSP2协议解析，解析初始化 
	输入：无
	输出：OUT PRTCL_BASE_CONF *pHeadID 待初始化的协议包头
	返回值：是否初始化成功 成功TURE 其他FALSE*/
	CM_BOOL Prtcl_RSSP2_CheckInit(/* 待初始化的协议包头 */ OUT PRTCL_BASE_CONF *pHeadID);
#if 0
	/* 用于RSSP2协议解析，对队列中数据进行解析
	输入：
	IN QueueRingID pQueue,储存缓冲数据的环形队列
	IN const UINT32 dwBufMax,指定输出缓冲区的大小
	输出：
	OUT UINT8 *pBuf,输出解析出来的完整一包
	OUT UINT32 *pBufLen指定解析出的完整一包的大小
	返回值:是否解析成功 成功TURE 其他FALSE*/
	CM_BOOL Ptrtcl_RSSP2_Check(/* 储存缓冲数据的环形队列*/IN QueueRingID pQueue,
		/* 输出解析出来的完整一包*/ OUT UINT8 *pBuf,
		/* 指定输出缓冲区的大小*/IN const UINT32 dwBufMax,
		/* 指定解析出的完整一包的大小*/OUT UINT32 *pBufLen);
#endif
#ifdef __cplusplus
}
#endif

#endif 
