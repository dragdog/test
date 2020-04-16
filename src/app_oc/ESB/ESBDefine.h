/********************************************************
*                                                                                                            
* 文 件 名： ESBConfigDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 紧急停车按钮配置头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _ESB_CONFIG_DEFINE_H_
#define _ESB_CONFIG_DEFINE_H_



/*紧急停车按钮总数最大值*/
#define ESB_SUM_MAX                      50     /*定义ESB数量最大值*/



/*紧急按钮状态*/
#define ESB_STATE_VALID_NO               0x55   /*紧急无效*/
#define ESB_STATE_VALID_YES              0xaa   /*紧急有效*/

#define ESB_VIRTUAL_ATTR_YES			0x55			/*虚拟ESB*/
#define ESB_VIRTUAL_ATTR_NO				0xaa			/*非虚拟ESB*/
#endif
