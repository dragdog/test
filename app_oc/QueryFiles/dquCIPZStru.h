/************************************************************************
*
* 文件名   ：  dquCIPZStru.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   联锁配置结构体定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "CommonTypes.h"
#include "CiConfigData.h"

#ifndef DQU_CIPZ_STRU
#define DQU_CIPZ_STRU


#define CI_SYSTEM_PARAMETER_DATA_TYPE  0x72

#define CI_SYSTEM_PARAMETER_DATA_ID    0x01
#define CI_SYSTEM_PARAMETER_DATA_LEN   62/*修改于20181121，系统参数表增加一个字段：间隙探测时间(s)，UDE2*/

#define CI_SYSTEM_CI_TRANSMIT_ID       0x02
#define CI_SYSTEM_CI_TRANSMIT_LEN      10/*修改于20170321，修改原因：联锁间接口数据增加一列：物理区段锁闭方向是否取反*/

#define CI_SYSTEM_OUTSTATIONSIG_RALATED_LOGSEC_ID  0x03
#define CI_SYSTEM_OUTSTATIONSIG_RALATED_LOGSEC_LEN 52

#define CI_SYSTEM_CI_FUNC_SWITCH_ID		0x04
#define CI_SYSTEM_CI_FUNC_SWITCH_LEN	15

#define CI_SYSTEM_CI_DIFF_LINE_DEV_ID		(0x05)
#define CI_SYSTEM_CI_DIFF_LINE_DEV_LEN		(5)

#define CI_SYSTEM_CI_VIRTUAL_DEV_ID		(0x06)
#define CI_SYSTEM_CI_VIRTUAL_DEV_LEN	(3)

#define CI_SYSTEM_OUTSTATIONSIG_RELATED_AXISSEC_ID  (0x07)	/*修改于20181121，新机场线需求：增加点式临时限速配置数据(计轴区段)表*/
#define CI_SYSTEM_OUTSTATIONSIG_RELATED_AXISSEC_LEN (82)

#endif

