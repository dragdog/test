/** ******************************************************
*                                                                                                            
* 文件名  ： CM_Define.h
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 声明日志记录相关函数  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef _CM_DEFINE_H_
#define _CM_DEFINE_H_

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */

#define LOG_LEVEL (10) /*%RELAX<MISRA_19_5>*/

#define LOG_ERROR (2) /*%RELAX<MISRA_19_5>*/
#define LOG_WARN (4) /*%RELAX<MISRA_19_5>*/
#define LOG_INFO (6) /*%RELAX<MISRA_19_5>*/

#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif