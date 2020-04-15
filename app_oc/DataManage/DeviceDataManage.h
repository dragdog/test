/********************************************************
* 
* 文 件 名： DeviceDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-09-21
* 作    者： 联锁组
* 功能描述： 设备间数据管理头文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*		20141205 燕房线需求追加SPKS对应封锁处理
*
********************************************************/ 
#ifndef _DEVICE_DATA_MANAGE_H_
#define _DEVICE_DATA_MANAGE_H_

#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../error/ErrorDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"

#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"

#include "../Signal/SignalDataManage.h"

#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../spks/SpksConfigDataManage.h"
#include "../spks/SpksDataManage.h"
/*#include "../ParsePack/ParsePackZcData.h"*/

#include "../esb/ESBDataManage.h"

#include "../PSD/PSDConfigDataManage.h"
#include "../PSD/PSDDataManage.h"
/*--------------------------------------------------------------------------
 * 其他联锁条件状态定义
 *--------------------------------------------------------------------------*/
/* 其他联锁条件检查配置状态相关定义 */
/* 1	信号机	
开放0xaa
关闭0x55
同级开放0xbb（该信号机开放颜色与始端信号机相同）
*/
#define OTHER_CI_SIGNAL_STATUS_OPEN 0x55
#define OTHER_CI_SIGNAL_STATUS_CLOSE 0xAA
#define OTHER_CI_SIGNAL_STATUS_SAME 0xbb

/* 2	道岔	定位0x55 反位0xaa */
#define OTHER_CI_SWTICH_STATUS_DINGWEI 0x55
#define OTHER_CI_SWTICH_STATUS_FANWEI 0xAA

/* 3	屏蔽门	打开0x55 关闭0xaa 正常VOBC打开屏蔽门 0xff*/
#define OTHER_CI_PSD_STATUS_OPEN 0x55
#define OTHER_CI_PSD_STATUS_CLOSE 0xAA
#define OTHER_CI_PSD_STATUS_OCCUPY_OPEN 0xFF

/* 4	紧急关闭按钮	实施0xaa 未实施0x55 */
#define OTHER_CI_ESB_STATUS_VALID 0xAA
#define OTHER_CI_ESB_STATUS_UNVALID 0x55


/* 5	封锁作业开关	实施0x55 未实施0xaa */
#define OTHER_CI_SPKS_YES 0x55
#define OTHER_CI_SPKS_NO 0xAA

/* 6	车库门	打开0x55 关闭0xAA */
#define OTHER_CI_GARAGEDOOR_OPEN 0x55
#define OTHER_CI_GARAGEDOOR_CLOSE 0xAA


/* 7	洗车机归位	已归位至限界外0x55 未归位0xaa */
#define OTHER_CI_WASH_READ_YES 0x55
#define OTHER_CI_WASH_READ_NO 0xAA

/* 8	物理区段占用状态	空闲0x55 占用0xaa */
#define OTHER_CI_PHYSICAL_OCCU_YES 0xAA
#define OTHER_CI_PHYSICAL_OCCU_NO  0x55

#define ZCTOCIAUTORTN_INDICT_MASK_VALID 0x55		/*ZC给CI的无人折返灯码位有效*/
#define ZCTOCIAUTORTN_INDICT_MASK_INVALID 0xAA		/*ZC给CI的无人折返灯码位无效*/

/******************************联锁标志数据管理****************************/

/*
* 功能描述： 获取上电解锁标志
* 参数说明： void
* 返回值  ：CI_START_LOCK_NO    
			CI_START_LOCK_YES 
*/
UINT8 GetStartLockFlag(void);

/*
* 功能描述： 获取全站亮灯标志
* 参数说明： void
* 返回值  ：CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
UINT8 GetAllSignalBrightFlag(void);


/*
* 功能描述： 设置全站亮灯标志
* 参数说明： void
* 返回值  ：CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
void SetAllSignalBrightFlag(UINT8 setFlag);

/******************************模式相关计算****************************/
/*
* 功能描述： 获取联锁模式标志
* 参数说明： void
* 返回值  ：联锁模式 0x55: 站控 0xAA:中控 0x71:控制台 0xFF:紧急站控
*/
UINT8 GetCiModeFlag(void);

/*
* 功能描述： 设置联锁模式标志
* 参数说明： 联锁模式 0x55: 站控 0xAA:中控 0x71:控制台 0xFF:紧急站控
* 返回值  ：void
*/
void SetCiModeFlag(UINT8 setFlag);

#endif
