/** ******************************************************
*                                                                                                            
* 文件名  ： CM_RingQueue.c
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 环形队列处理类  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef __CM_RING_QUEUE__H
#define  __CM_RING_QUEUE__H

#include "CM_Base.h"



/*#define MUTEX_QUEUE_OPEN*/

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */


#define TIMEOUT_SINGLE_LOCK_RING_BUFF (50) /* 单锁超时 */  /*%RELAX<MISRA_19_5>*/

#define MAX_QUEUE_BUFFER_LENTH (1024 * 64U) /* 缓冲列队最大长度 64KB  防止初始化过大 系统溢出 */ /*%RELAX<MISRA_19_5>*/
#define  _DEFAULT_QEUEUE_RING_BUFFER_LENTH (32 * 1024U) /*默认缓冲区的最大长度 以CM_UINT8为单位*/ /*%RELAX<MISRA_19_5>*/

#ifndef  MAX_UNSIGNED_CHAR_SIZE
#define MAX_UNSIGNED_CHAR_SIZE 256 /* 一个CM_UINT8 总共 256种 */ /*%RELAX<MISRA_19_5>*/
#endif

	typedef     enum  QUEUE_RING_STATE_ENUM_
	{
		QUEUE_RING_STATE_EXCUTE_FAIL = 0x00,                                /* 表示动作执行失败 */
		QUEUE_RING_STATE_EXCUTE_OK = 0x01,                                  /* 表示上一动作执行成功 */
		QUEUE_RING_STATE_Empty = 0x10,                                   /*队列空*/
		QUEUE_RING_STATE_READ_OK = QUEUE_RING_STATE_EXCUTE_OK,              /*读取成功*/
		QUEUE_RING_STATE_Data = 0x11,                                     /*数据正常*/
		QUEUE_RING_STATE_WRITE_OK = QUEUE_RING_STATE_EXCUTE_OK,              /*写入成功*/
		QUEUE_RING_STATE_MSG_BUF_ERR = 0x10,                                     /*缓冲区异常*/
		QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH = 0x20,                   /*数据长度超过列队最大缓冲区长度 丢弃数据前段 只留和列队大小相同的数据*/
		QUEUE_RING_STATE_MSG_NOFIND = 0x30,                                      /*消息未定义*/
		QUEUE_RING_STATE_FULL = 0x40,                                         /*队列满*/
		QUEUE_RING_STATE_FULL_WRITE = 0x50,                                 /* 列队满并且覆盖写入 */
		QUEUE_RING_STATE_LOCK_ERR = 0x60,                                        /*队列锁定异常*/
		QUEUE_RING_STATE_READ_ERROR = 0x70,                                          /*队列读取异常*/
		QUEUE_RING_STATE_WRITE_ERROR = 0x80,                                        /*队列写入异常*/
		QUEUE_RING_STATE_JAM = 0x90,                                 /* 列队锁定超时 堵塞 */
		QUEUE_RING_STATE_NORMAL = 0xA1                                         /*队列状态正常*/
	}QUEUE_RING_STATE;

	typedef struct QUEUE_RING_DATA_STRUCT_
	{
		CM_BOOL   m_bIsInited;    /*初始化标记*/
		UINT8 * m_pDataCache;     /* 数据储存指针 */
		UINT32  m_dwMaxBufferLenth;    /* 循环列队的缓冲区最大长度 也就是列队能处理的最大信息长度 */
		UINT32  m_dwDataLenth;    /* 有效数据长度 */
		UINT32  m_pHeadOffset;    /* 头部位置指针 */
		UINT32  m_pTailOffset;    /* 尾部位置指针 */
		CM_BOOL    m_bUseMutex;     /* 是否使用数据互斥保护*/
		UINT8   m_IfCoverQueueFull;   /*标记当列队满时是否覆盖原先的数据 或者丢包不处理*/
		UINT8   m_QueueState;   /* 列队状态 */
#ifdef RQ_TASK_SAFE
		CM_THREAD  m_hMutexHanle; /**/
		MUTEX_CM  m_DataMutex;    /* 数据操作互斥保护 */
#endif
	}QUEUE_RING_DATA_STRUCT;

	typedef QUEUE_RING_DATA_STRUCT * QueueRingID;

	/*环形队列初始化 输入结构体指针 指定队列大小
	输入：QueueRingID pQueue 环形队列结构体指针
	const UINT32 mMaxBufferLenth 初始化环形队列的大小
	输出：无
	返回值：1为成功 其他值异常*/
	CM_BOOL QUEUE_RING_Init(/*环形队列结构体指针*/IN QueueRingID pQueue,
		/*初始化环形队列的大小*/IN const UINT32 mMaxBufferLenth);


	/*释放指定结构体的环形队列占用内存
	输入：QueueRingID pQueue 环形队列结构体指针
	输出：无
	返回值：1为成功 其他值异常*/
	CM_BOOL          QUEUE_RING_Release(/*环形队列结构体指针*/IN QueueRingID pQueue);

	/* 设置当队列满时 新数据覆盖老数据 或者 丢弃新数据 返回1为成功 其他值异常*/
	/* CM_UINT8         QUEUE_RING_SetCover(IN QueueRingID pQueue, IN const CM_BOOL ifCover);*/

	/* 清空队列 
	输入：QueueRingID pQueue 环形队列结构体指针
	输出：无
	返回值：1为成功 其他值异常*/
	UINT8         QUEUE_RING_SetEmpty(/*环形队列结构体指针*/IN QueueRingID pQueue);

	/* 将数据写入列队的尾部 如果满 则覆盖
	输入：IN QueueRingID pQueue, 环形队列结构体指针
		IN const UINT8 * mbuffer, 要写入队列的数据
		IN const UINT32 mLenth 数据长度
	输出：
	返回值：为状态信息enum _RING_QUEUE_STATE_ 返回1为成功 其他值异常*/
	UINT8         QUEUE_RING_Write(/*环形队列结构体指针*/IN QueueRingID pQueue,
		/*要写入队列的数据*/IN const UINT8 * mbuffer,
		/*数据长度*/IN const UINT32 mLenth);

	/*
	*从列队头部读取一定数量的数据 但是不清除列队中的数据
	输入：
	IN QueueRingID pQueue, 环形队列结构体指针		
		IN const UINT32 mOutBufMaxLenth, 为mOutBuffer的内存长度的最大值
		IN const UINT32  mReadLenth, 为打算读取的数据的长度
		IN const UINT32 mReadStartoff 读取数据时指针的偏移 默认0 
	输出：
	OUT UINT8 * mOutBuffer, 为外部声明的buffer 将会把数据复制进去
	OUT UINT32 * mOutReadLen, 读取出的buff 数量
	返回值：1为成功 其他值异常  */
	UINT8         QUEUE_RING_ReadOnly( /*环形队列结构体指针*/IN QueueRingID pQueue,
		/*为外部声明的buffer 将会把数据复制进去*/OUT UINT8 * mOutBuffer,
		/*读取出的buff 数量*/OUT UINT32 * mOutReadLen,
		/*为mOutBuffer的内存长度的最大值*/IN const UINT32 mOutBufMaxLenth,
		/*为打算读取的数据的长度*/IN const UINT32  mReadLenth,
		/*读取数据时指针的偏移 默认0*/IN const UINT32 mReadStartoff
	);

	/* 读取列队中的数据并清除相应长度  实际是移动指针偏移量 
	输入：
	IN QueueRingID pQueue, 环形队列结构体指针
	IN const UINT32 mOutBufMaxLenth, 为mOutBuffer的内存长度的最大值
	IN const UINT32  mReadLenth, 为打算读取的数据的长度
	IN const UINT32 mReadStartoff读取数据时指针的偏移 默认0
	输出：
	OUT UINT8 * mOutBuffer, 为外部声明的buffer 将会把数据复制进去
	OUT UINT32 * mOutReadLen, 读取出的buff 数量
	返回值：1为成功 其他值异常*/
	UINT8         QUEUE_RING_Read(     /*环形队列结构体指针*/IN QueueRingID pQueue,
		/*为外部声明的buffer 将会把数据复制进去*/OUT UINT8 * mOutBuffer,
		/*读取出的buff 数量*/OUT UINT32 * mOutReadLen,
		/*为mOutBuffer的内存长度的最大值*/IN const UINT32 mOutBufMaxLenth,
		/*为打算读取的数据的长度*/IN const UINT32  mReadLenth,
		/*读取数据时指针的偏移 默认0*/IN const UINT32 mReadStartoff
	);

	/* 移动读指针 删除数据
	* 实际上只是移动读指针的位置 不释放任何数据
	输入:
	IN QueueRingID pQueue, 环形队列结构体指针
	IN const UINT32 mDeletedLenth, 要处理清除的数据长度
	输出：
	OUT UINT32 * mDeletedLen返回实际删除的数据长度 做确认 NULL表示不返回
	返回值1为成功 其他值异常*/
	UINT8         QUEUE_RING_Delete(/*环形队列结构体指针*/IN QueueRingID pQueue,
		/*要处理清除的数据长度*/ IN const UINT32 mDeletedLenth,
		/*返回实际删除的数据长度 做确认 NULL表示不返回*/  OUT UINT32 * mDeletedLen);

	/* 得到环形队列中的数据长度
	输入：IN QueueRingID pQueue 环形队列结构体指针
	输出：无
	返回值：UINT32长度
	*/
	UINT32        QUEUE_RING_GetLenth(/*环形队列结构体指针*/IN QueueRingID pQueue);
	/* 判断队列是否为空 
	输入：IN QueueRingID pQueue 环形队列结构体指针
	输出：无
	返回值：CM_BOOL true-是 false-否
	*/
	CM_BOOL         QUEUE_RING_IsEmpty(/*环形队列结构体指针*/IN QueueRingID pQueue);

	/* 判断队列是否状态为满
	输入：IN QueueRingID pQueue 环形队列结构体指针
	输出：无
	返回值：CM_BOOL true-是 false-否*/
	CM_BOOL         QUEUE_RING_IsFull(/*环形队列结构体指针*/IN QueueRingID pQueue);

	/*    const CM_UINT8 * QUEUE_RING_GetPrePrt(IN QueueRingID pQueue, IN const CM_UINT8 * pNowPrt);*/
	/*     const CM_UINT8 * GetNextBytePtrByOff(IN const CM_UINT8 * pNowPrt,IN const CM_UINT32 moffset);返回该指针数据的下 moffset 地址 如果为NULL 表示已经到数据队尾 没有有效数据了
	*/

	/* 搜索函数
	输入：
	IN QUEUE_RING_DATA_STRUCT * pQueue,环形队列结构体指针
	IN const CM_UINT8 *msubBytes,搜索字符串
	IN const CM_UINT32 msubLenth,字串长度
	IN const CM_UINT32 mSearchOffset 搜索偏移
	IN const CM_BOOL bSearchDir  指定正反方向的搜索
	输出：
	返回值：为字串对应主串的偏移量 如果不存在返回-1
	);*/
	INT32        QUEUE_RING_SearchByte(/*环形队列结构体指针*/IN QueueRingID pQueue,
		/*搜索字符串*/IN const UINT8 *          msubBytes,
		/*字串长度*/IN const UINT32           msubLenth,
		/*搜索向量表  _TableDir 查找关键字的向量表 需要用preCompute 初始化 大小为256*/
		IN const UINT8 *         _TableDir,
		/*搜索偏移*/IN const UINT32           mSearchOffset,
		/*指定正反方向的搜索 CM_TRUE 正搜*/IN const CM_BOOL bSearchDir
	);

	/*初始化搜索向量表
	输入：
	IN const UINT8* msubBytes, 搜索字符串
	IN const UINT32 subLenth,字串长度
	IN const CM_BOOL bSearchDir 指定正反方向的搜索 CM_TRUE 正搜
	输出：
	OUT UINT8 *_TableDir,搜索向量表
	返回值：
	*/
	void            QUEUE_RING_preCompute(/*搜索字符串*/IN const UINT8* msubBytes,
		/*字串长度*/ IN const UINT32 subLenth,
		/*搜索向量表*/OUT UINT8 *_TableDir,
		/*指定正反方向的搜索 CM_TRUE 正搜*/ IN const CM_BOOL bSearchDir
	);


#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif 