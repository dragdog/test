/********************************************************
*                                                                                                            
* 文件名  ： RsrPlatform.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.10.25
* 作者    ： 车载及协议部
* 功能描述： Rsr层编译配置文件
* 使用注意： 
*   
* 修改记录：
* 2010.05.10 楼宇伟		改名为RsrPlatform.h，统一命名
* 2010.05.10 楼宇伟 	把记录开关定义到集成配置RsrConfig.h文件里。
*
*2012-8-15 15:54:00 王佩佩 修改记录
*	    1.增加RSR协议版本号宏定义
********************************************************/ 
#ifndef _RSR_PLATFORM_H_
#define _RSR_PLATFORM_H_

#include "CommonTypes.h"
#include "RsrConfig.h"

#define RSR_VER_NORMAL	0x01			/* 正常版 Release版 */ 
#define RSR_VER_RP_ONLY 0x02			/* 只集成冗余层 */

/* RSR协议软件版本号 */
#define RSRCODE_VER_PRODUCT_NO 66
#define RSRCODE_VER_MODULE_NO  3
#define RSRCODE_VER_MAIN_VER  2
#define RSRCODE_VER_SUB_VER  0

#define RSR_VERSION RSR_VER_NORMAL		/* 当前使用版本 */

/* 平台区分 */
/* 需要根据实际的情况来区别具体平台 */
#define ENM_ZC	(CBTC_VXWORKS)			/* ZC */
#define ENM_CC  (CBTC_MOTOROLA)			/* VOBC */
#define ENM_CI	(CBTC_WINDOWS)			/* 仿真CI */

/* 只集成 冗余层 */
/* #define RSR_RP_ONLY	*/
/* 正常 */
#define RSR_NORMAL

#endif

