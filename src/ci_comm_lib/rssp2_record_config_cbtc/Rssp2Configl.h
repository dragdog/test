/********************************************************
*                                                                                                            
* 文件名  ： Rssp2Config.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 
* 作者    ： 
* 功能描述： RSSP2记录打开
* 使用注意： 
*   
* 修改记录：   
*
********************************************************/ 
#ifndef _RSSP2_CONFIG_H_
#define _RSSP2_CONFIG_H_

/* WIN32 demo 模式开关 */
/*#define RSSP2_DEMO */

/* 记录编译开关 */
#define RSSP2_RECORD_ERR_ASCII		/* 以字符方式记录 */

#define RSSP2_RECORD_SIMPLIFY_BIN	/* 以二进制方式记录 */

#ifdef RSSP2_RECORD_SIMPLIFY_BIN	
#undef  RSSP2_RECORD_ERR_ASCII		/* 防止重复定义 */
#endif

#if CBTC_MOTOROLA
#define RSSP2_SIMP_REFRESH /*开启简单链路刷新*/
#endif

/* 在线调试开关 */
/* #define _BUILD_IN_TEST_ */		/* 开启在线调试功能 */

/* 记录等级定义 */
#define RSSP2_LOG_ERROR (1)
#define RSSP2_LOG_WARN  (2)
#define RSSP2_LOG_NOTICE (3)
#define RSSP2_LOG_INFO  (5)
#define RSSP2_LOG_CYC   (10)

#define RSSP2_LOG_LEVEL (10)			/* 日志记录等级 */

/* 专有记录码定义 */
#define RSSP2_LOG_CYC_START	  (0xFF)		/* 周期开始*/
#define RSSP2_LOG_NODE_RECV_START (0xFE)	/* 节点接收开始 */
#define RSSP2_LOG_NODE_SEND_START  (0xFD)	/* 节点发送开始 */
#define RSSP2_LOG_RED_RECV_START (0xF1)     /* 红王接收开始 */
#define RSSP2_LOG_BLUE_RECV_START (0xF2)    /* 蓝网接收开始 */
#define RSSP2_LOG_SEND_START (0xF3)         /* 发送开始 */

#endif

