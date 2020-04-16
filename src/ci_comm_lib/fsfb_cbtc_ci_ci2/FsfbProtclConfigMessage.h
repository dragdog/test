/************************************************************************
*
* 文件名   ：  FsfbProtclConfigMessage.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.05
* 作者     ：  王佩佩
* 功能描述 ：  FSFB/2协议中各种配置信息
* 使用注意 ： 无
* 修改记录 ：
* 时间			修改人		修改理由
************************************************************************/
#ifndef	_FSFB_PROTCL_CONFIG_MESSAGE_H_
#define	_FSFB_PROTCL_CONFIG_MESSAGE_H_

#include "CommonTypes.h"

/* system check word */
#define COM_SYS_CHK_WD_1 0xAE390B5AL
#define COM_SYS_CHK_WD_2 0xC103589CL	

#define FSFB_CI_DEV 60      /*连锁设备*/
#define FSFB_LEU_DEV 94      /*连锁设备*/

/* 消息类型 */
#define FSFB_SSE_MSG	((UINT8)0x90)						/* SSE 消息 */
#define FSFB_SSR_MSG	((UINT8)0x91)						/* SSR 消息 */
#define FSFB_TSD_MSG	((UINT8)0x80)						/* TSD 消息 */
#define FSFB_SBT_MSG	((UINT8)0xA0)						/* SBT 消息 */
#define FSFB_EBT_MSG	((UINT8)0xA1)						/* EBT 消息 */
#define FSFB_ABT_MSG	((UINT8)0xA2)						/* ABT 消息 */
#define FSFB_BTC_MSG	((UINT8)0xA3)						/* BTC 消息 */
#define FSFB_BTD_MSG	((UINT8)0xA4)						/* BTD 消息 */

/* 消息帧长度 */
#define TSD_SIZE     58 
#define SSE_SIZE     20
#define SSR_SIZE     25
#define SBT_SIZE    138
#define EBT_SIZE     22
#define ABT_SIZE     22
#define BTC_SIZE     30
#define BTD_SIZE     52

/* 报文data feild Size */
#define TSD_DF_SIZE  36
#define BTC_DF_SIZE   8
#define SBT_DF_SIZE 116
#define ABT_DF_SIZE   0
#define EBT_DF_SIZE   0
#define BTD_DF_SIZE  30

/* Safety Data Size */
#define TSD_SDS      44
#define BTC_SDS      16
#define ABT_SDS       8
#define BTD_SDS      38
#define EBT_SDS       8
#define SBT_SDS     124


/* 地址定义 */
#define ADDR_BROADCAST 0xFFFF
#define ADDR_LEU       0x0006
#define ADDR_BTM       0x00FF
#define ADDR_CI      0x0003


#define MAX_TSD_LOST  30  /* TSD通讯最大允许的消息丢失数量*/
#define MAX_BTM_LOST  10  /* BTM通讯最大允许的消息丢失数量*/
#define Delta_Time_Counter_max   0x40

/* 应用层数据分类定义*/
#define APPLCAT_TDM  0x01  /* TD通讯类型*/
#define APPLCAT_BTM  0x10  /* BTM通讯类型*/

#define FSFB_CHANNEL_NOSIGNAL 0  /* 通道无信号*/
#define FSFB_CHANNEL_SIGNAL   1  /* 通道有信号*/
#define FSFB_CHANNEL_LOCKED   2  /* 通道被锁定*/

/* 共用长度 */
#define FSFB_HEAD_LEN	6								/* 帧头长度 */
#define FSFB_TC_LEN			4								/* 计数器的长度 */
#define FSFB_CRC_16_LEN		2								/* CRC 16位 的长度 */

/* 帧头 */
#define FSFB_APP_CTGRY_POS	0							/* 帧头应用域位置*/
#define	FSFB_MSG_TYPE_POS	(FSFB_APP_CTGRY_POS+1)		/* 帧头消息类型 */
#define FSFB_SRC_ADD_POS	(FSFB_MSG_TYPE_POS+1)		/* 消息源地址 */
#define FSFB_DEST_ADD_POS	(FSFB_SRC_ADD_POS+2)		/* 消息目标地址 */

#define FSFB_BTM_HEAD_LEN    20        /* BTM帧帧头 */

#define	FSFB_TC_POS			FSFB_HEAD_LEN				/* FSFB的计数器位置 */

/* TSD或者BTM报文 */
#define FSFB_CRCM_LEN		4										/* crc 验证的字段的大小 */
#define	FSFB_DATASIZE_LEN	2										/* 数据长度字段的大小 */
#define	FSFB_DATA_SIZE_POS	(FSFB_TC_POS + FSFB_TC_LEN)				/* 报文中安全数据长度保存位置 */
#define FSFB_CRCM_1_POS		(FSFB_DATA_SIZE_POS + FSFB_DATASIZE_LEN)	/* 报文中CRCM_1的位置 */
#define FSFB_CRCM_2_POS		(FSFB_CRCM_1_POS + FSFB_CRCM_LEN)		/* 报文中CRCM_2的位置 */
#define	FSFB_DATA_POS		(FSFB_CRCM_2_POS + FSFB_CRCM_LEN)		/* 安全数据长度保存位置 */


#define FSFB_ENQ_LEN	4										    /* 报文中 SEQENQ长度 */
/* SSE报文 */

#define	FSFB_SSE_ENQ_1_POS	(FSFB_TC_POS+FSFB_TC_LEN)					/* SSE报文中 SEQENQ_1位置 */
#define	FSFB_SSE_ENQ_2_POS	(FSFB_SSE_ENQ_1_POS+FSFB_ENQ_LEN)	/* SSE报文中 SEQENQ_2位置 */

/* SSR报文 */
#define FSFB_SSR_NUM_LEN	1										/* SSR报文中Num DataVer长度 */
#define	FSFB_SSR_TCENQ_POS	(FSFB_TC_POS+FSFB_TC_LEN)					/* SSR报文中 请求方 计数器位置 */
#define	FSFB_SSR_ENQ_1_POS	(FSFB_SSR_TCENQ_POS+FSFB_TC_LEN)				/* SSR报文中 SEQINI_1位置 */
#define	FSFB_SSR_ENQ_2_POS	(FSFB_SSR_ENQ_1_POS+FSFB_ENQ_LEN)	/* SSR报文中 SEQINI_2位置 */
#define	FSFB_SSR_NUM_POS	(FSFB_SSR_ENQ_2_POS+FSFB_ENQ_LEN)	/* SSR报文中Num DataVer 位置 */

/*BTC data feild 格式 */
#define FSFB_BTC_OT_POS    (FSFB_DATA_POS)  /* operation Type */
#define FSFB_BTC_BT_POS    (FSFB_BTC_OT_POS+2)  /* LEU STATUS DATA */
#define FSFB_BTC_SNS_POS   (FSFB_BTC_BT_POS+2)   /* First sub_buffer number requested */
#define FSFB_BTC_NST_POS   (FSFB_BTC_SNS_POS+2)  /* NUmber Sub_buffer to Transmit*/
/*BTC data feild 值 */
#define FSFB_BTC_OT_VALUE    0x0000
#define FSFB_BTC_BT_VALUE    0x000A
#define FSFB_BTC_SNS_VALUE   0x0001
#define FSFB_BTC_NST_VALUE   0xFFFF

/* BTD data Feild 格式 */
#define FSFB_BTD_BT_POS   (FSFB_DATA_POS)       /*buffer Type */
#define FSFB_BTD_SBN_POS  (FSFB_BTD_BT_POS+2)   /* sub_buffer Number */
#define FSFB_BTD_TNM_POS  (FSFB_BTD_SBN_POS+2)  /* Total Number of sub_buffer */
#define FSFB_BTD_SBD_POS  (FSFB_BTD_TNM_POS+2)  /* sub-buffer data */

#define FSFB_FIXED_LEN 22 /* 帧固定部分长度 */

/* 常量声明 */
#define FSFB_SEND_SBT_NUM		3		/* 等待SSR超时后，最多发送SSE次数 */
#define FSFB_NONE_SEND_APP_DATA	3		/* 无法送应用数据周期数 */
#endif