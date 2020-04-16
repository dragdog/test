/********************************************************
*                                                                                                            
* 文件名  ： RsspConfigWl.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2010.05.10
* 作者    ： 车载及协议部
* 功能描述： Rsr层编译配置文件	: RSSP层记录打开
* 使用注意： 
*   
* 修改记录：   
*
********************************************************/ 
#ifndef _RSSP_CONFIG_WL_H_
#define _RSSP_CONFIG_WL_H_

#define VOBC_ATS_CYCLENUM_MULTIPLE 5	/*VOBC发到ATS本方周期号倍数*/
#define AOM_ATS_CYCLENUM_MULTIPLE 5		/*AOM发到ATS本方周期号倍数*/

/* 记录编译开关 */
/* #define RSSP_RECORD_ERR_ASCII	*/	/* 以字符方式记录 */

   #define RSSP_RECORD_SIMPLIFY_BIN	/* 以二进制方式记录 */

#endif

