/********************************************************
* 
* 文 件 名： DeviceDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-09-21
* 作    者： 联锁组
* 功能描述： 设备间数据管理 
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 


#include "DeviceDataManage.h"
#include "../ParsePack/CiInterfaceDefine.h"

UINT8 AllSignalBrightFlag = CI_ALL_SIGNAL_BRIGHT_NO;					                  /*全站亮灯标志*/

UINT8 AllSwitchYLockFlag = CI_ALL_SWITCH_YLOCK_NO;						                  /*全站引导总锁标志*/

UINT8 StartFLockFlag = CI_START_FLOCK_YES;								                  /*全站封锁标志*/

UINT8 StartLockFlag = CI_START_LOCK_YES;								                  /*上电锁闭标志*/

UINT8 LinShiXianSuCancelButtonIsValidFlag = CI_LINSHIXIANSU_CANCEL_BUTTON_VALID;          /*临时限速人工解除标志*/
 
UINT8 LabourCancelLinShiXianState = LINSHIXIANSU_COMM_BAD_STATE;                          /*人工取消临时限速迁移状态*/

UINT16 JkjRelayId = 0;													                  /*应急盘继电器ID*/

UINT8 PlatOverFlag = CI_OVER_NO;										                  /*平台宕机标志*/

UINT8 ciMode = CI_MODE_JJ_LOCAL;										                  /* 紧急站控 */

UINT8 ciTrainTestStatus = CI_TRAIN_TEST_OFF;

UINT8 comm_Train_Test_SQAJ_SCX_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*试车线与车辆段采用通信接口时试车线SQAJ状态*/
UINT8 comm_Train_Test_YSKJ_SCX_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*试车线与车辆段采用通信接口时试车线YSKJ状态*/
UINT8 comm_Train_Test_SQJ_CLD_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*试车线与车辆段采用通信接口时车辆段SQJ状态*/
UINT8 comm_Train_Test_TYSJ_CLD_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*试车线与车辆段采用通信接口时车辆段TYSJ状态*/

UINT8 ZCToCIAutoRtnIndict_Mask_Flash_State = ZCTOCIAUTORTN_INDICT_MASK_INVALID;			/*ZC给CI的无人折返灯闪灯状态*/
UINT8 ZCToCIAutoRtnIndict_Mask_Steady_State = ZCTOCIAUTORTN_INDICT_MASK_INVALID;		/*ZC给CI的无人折返灯稳灯状态*/
UINT8 autoTriggerFlag = AUTO_TRIGGER_SET_NO;                                             /*自动触发标志初始化为未设置*/
/******************************联锁标志数据管理****************************/

/*
* 功能描述： 获取上电解锁标志
* 参数说明： void
* 返回值  ：CI_START_LOCK_NO    
			CI_START_LOCK_YES
            CI_START_LOCK_ERROR
*/
UINT8 GetStartLockFlag(void)
{
	return StartLockFlag;
}

/*
* 功能描述： 获取全站亮灯标志
* 参数说明： void
* 返回值  ：CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
UINT8 GetAllSignalBrightFlag(void)
{
	return AllSignalBrightFlag;
}


/*
* 功能描述： 设置全站亮灯标志
* 参数说明： void
* 返回值  ：CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
void SetAllSignalBrightFlag(UINT8 setFlag)
{
    if (setFlag == CI_ALL_SIGNAL_BRIGHT_YES)
    {
        AllSignalBrightFlag = setFlag;
    }
    else if (setFlag == CI_ALL_SIGNAL_BRIGHT_NO)
    {
        AllSignalBrightFlag = setFlag;
    }
    else
    {
        /*不处理*/
    }
}

/*
* 功能描述： 获取联锁模式标志
* 参数说明： void
* 返回值  ：联锁模式 0x55: 站控 0xAA:中控 0x71:控制台 0xFF:紧急站控
*/
UINT8 GetCiModeFlag(void)
{
	return ciMode;
}

/*
* 功能描述： 设置联锁模式标志
* 参数说明： 联锁模式 0x55: 站控 0xAA:中控 0x71:控制台 0xFF:紧急站控
* 返回值  ：void
*/
void SetCiModeFlag(UINT8 setFlag)
{
	if ((CI_MODE_ATS == setFlag) || (CI_MODE_CONSOLE == setFlag)
		||(CI_MODE_JJ_LOCAL == setFlag) ||(CI_MODE_LOCAL == setFlag))
	{
		ciMode = setFlag;
	}
	else
	{
		/*不处理*/
	}
}