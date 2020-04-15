/* ***********************************************************************
*
* 文件名      ：  PointCommandManage.c
* 版权说明    ：  北京交控科技有限责任公司
* 版本号      ：  1.0
* 创建时间    ：  2016-6-22
* 作者        ：  闻一龙
* 功能描述    ：  
* 使用注意    ：  
* 修改记录    ：  2016-6-22 			闻一龙				  创建
                                                                      
*********************************************************************** */

#include <stdio.h>
#include "TypesDef.h"
#include "CommonTypes.h"
#include "PointCommandManage.h"
#include "GLOBALVAR.h"
/*===============================extern全局数组===============================*/
extern CommandData_Struct         g_stOCRevCmdDataStru;                                      /*OC接收的控制命令数组*/
extern ATSLock_CommandData_STRU    stOCRevATSLockCmdDataStru[CMD_MAX];                   /*OC接收的ATS资源锁控制命令数组*/
extern VOBCLock_CommandData_STRU    stOCRevVOBCLockCmdDataStru[CMD_MAX];                 /*OC接收的VOBC资源锁控制命令数组*/
extern PSD_CommandData_STRU     stOCRevPSDCmdDataStru[CMD_MAX];                          /*OC接收的屏蔽门控制命令数组*/


/**=============================================================================
    函数名: PointBufferSendManage()
      描述: 道岔缓存区发送判断函数      
      功能: 判断缓存区数组中的道岔命令是否可以发送。如果不能发送（控制用户没有
	  该道岔的独占锁）则删除该道岔命令。
      参数：无
    返回值: 可以发送：1
	        不可以发送：0  by hdh
==============================================================================*/
UINT8 PointBufferSendManage(void)
{
	UINT16 wPointIndex = DEFAULT_ZERO;           /*道岔缓冲区数组下标*/
	UINT16 wOCIndex = DEFAULT_ZERO;              /*OC结构体数组循环下标*/
	UINT16 wSwitchID = DEFAULT_ZERO;             /*道岔缓冲区的道岔ID*/
	UINT16 wSwitchIDConvert = DEFAULT_ZERO;
	UINT16 wSourceID = DEFAULT_ZERO;             /*命令控制源对象ID*/
	UINT16 i = DEFAULT_ZERO;                     /*删除命令的循环下标*/
	UINT8  IDFlage = DEFAULT_ZERO;               /*道岔命令ID与OC管理的道岔ID是否匹配的标志*/

	UINT8  Rtn = RETURN_SUCCESS;
	/*初始化标志*/
	IDFlage = DEFAULT_ZERO;
	wSwitchID = g_stOCSendSwitchCmdDataStruct[wPointIndex].SwitchID;
	wSourceID = g_stOCSendSwitchCmdDataStruct[wPointIndex].SourceID;
	/*找到与道岔控制命令匹配的道岔*/
	for(wOCIndex = 0; wOCIndex < g_stOC.SwitchNum; wOCIndex++)
	{
		if(wSwitchID == g_stOC.SwitchStateArray[wOCIndex].Id)
		{
			/*判断道岔命令的源对象是否具有该道岔的独占锁,判断该对象是否为上位机所属，并为上电上锁状态，如果是，则不转发*/
			if((g_stOC.SwitchELockArray[wOCIndex].ELockId == wSourceID) && (g_stOC.SwitchELockArray[wOCIndex].ELockLevel != 4))
			{
				Rtn = RETURN_SUCCESS;
				/*保留该命令*/	
			}
			else
			{
				Rtn = RETURN_FAILURE;
				/*删除该命令*/
				for(i = wPointIndex; i < g_wCommandNum_Switch_Send; i++)
				{
					g_stOCSendSwitchCmdDataStruct[i].SourceID = g_stOCSendSwitchCmdDataStruct[i+1].SourceID;
					g_stOCSendSwitchCmdDataStruct[i].SwitchID = g_stOCSendSwitchCmdDataStruct[i+1].SwitchID;
					g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse = g_stOCSendSwitchCmdDataStruct[i+1].LockPositionNormal_Reverse;

				}
				/*减少道岔命令的数量*/
				if(g_wCommandNum_Switch_Send)
				{
					g_wCommandNum_Switch_Send--;
				}	
				else
				{
					debug_infor_printf("\ng_wCommandNum_Switch_Send:%d--, failed", g_wCommandNum_Switch_Send);
				}
			}
			IDFlage = FLAG_SET;
			break;
		}
		else
		{

		}
	}
	/*如果找到该ID道岔，不处理*/
	if(IDFlage == FLAG_SET)
	{
		;/*什么也不做*/
	}
	/*道岔命令中的控制对象不在OC配置范围内*/
	else
	{
		/*删除该命令*/
		Rtn = RETURN_FAILURE;
		for(i = wPointIndex; i < g_wCommandNum_Switch_Send; i++)
		{
			g_stOCSendSwitchCmdDataStruct[i].SourceID = g_stOCSendSwitchCmdDataStruct[i+1].SourceID;
			g_stOCSendSwitchCmdDataStruct[i].SwitchID = g_stOCSendSwitchCmdDataStruct[i+1].SwitchID;
			g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse = g_stOCSendSwitchCmdDataStruct[i+1].LockPositionNormal_Reverse;

			/*减少道岔命令的数量*/
			if(g_wCommandNum_Switch_Send)
			{
				g_wCommandNum_Switch_Send--;
			}	
			else
			{
				debug_infor_printf("\ng_wCommandNum_Switch_Send:%d--, failed", g_wCommandNum_Switch_Send);
			}
		}		
	}

	return Rtn;

}
/**=============================================================================
    函数名: PointFaultManage()
      描述: 道岔故障管理函数      
      功能: 判断道岔是否故障，故障后将道岔转入故障状态，即不能接受任何控制。
	  道岔故障的几种情形:
	  1、此时处于定位共享锁，但道岔转至反位（可能人为扳动）
      参数：无
	  返回值: 判断成功：1
	 判断失败 0
==============================================================================*/
UINT8 PointFaultManage(void)
{
	UINT16 wPointIndex=DEFAULT_ZERO;           /*道岔缓冲区数组下标*/
	UINT16 wLockIndex=DEFAULT_ZERO;            /*锁数组下标*/
	UINT16 wOCIndex=DEFAULT_ZERO;              /*OC结构体数组循环下标*/
	UINT16 i=DEFAULT_ZERO;                     /*删除命令的循环下标*/
	UINT8  IDFlage=FLAG_SET;               /*共享锁要求的道岔状态与道岔本身的状态是否一致*/
	UINT8  Rtn = RETURN_SUCCESS;
	UINT8 FindSwitch = DEFAULT_ZERO;
	

	/*故障情形1：此时处于定位共享锁，但道岔转至反位（可能人为扳动）*/
	for (wOCIndex = 0; wOCIndex < g_stOC.SwitchNum ; wOCIndex++)
	{
		IDFlage = FLAG_SET;
		/*道岔有共享锁*/
		if (DEFAULT_ZERO != g_stOC.SwitchSLockNumArray[wOCIndex])
		{
			for (wLockIndex = 0; wLockIndex < g_stOC.SwitchSLockNumArray[wOCIndex];wLockIndex++)
			{
				if (g_stOC.SwitchStateArray[wOCIndex].Position != LockQSwitchStatus2SwitchStatus(g_stOC.SwitchSLockArray[wOCIndex][wLockIndex].SLockSwitchState))
				{
					IDFlage = FLAG_UNSET;
					if (DEFAULT_ZERO == g_wSwitchFaultNum)
					{
						g_wSwitchFaultID[g_wSwitchFaultNum] = g_stOC.SwitchStateArray[wOCIndex].Id;
						g_wSwitchFaultNum++;
					} 
					else
					{
						for (i = 0 ; i < g_wSwitchFaultNum;i++)
						{
							if (g_wSwitchFaultID[i] = g_stOC.SwitchStateArray[wOCIndex].Id)
							{
								FindSwitch = FLAG_SET;
								break;
							} 
							else
							{

							}
						}
						if (FLAG_SET != FindSwitch)
						{
							g_wSwitchFaultID[g_wSwitchFaultNum] = g_stOC.SwitchStateArray[wOCIndex].Id;
							g_wSwitchFaultNum++;
						} 
						else
						{
						}
					}					
					break;
				}
				else
				{
					/*共享锁与道岔状态一直，正常。什么也不做*/
				}
			}			
		}
		else
		{
			/*没有共享锁，不需要考虑道岔状态*/
		}

		if (FLAG_UNSET == IDFlage)
		{
			g_stOC.SwitchFaultStateArray[wOCIndex] = SWTICH_FAULT;
			/*DebugPrint(PRINT_PERIOD,"道岔管理：道岔出现故障，共享锁要求状态与道岔实际状态不一致\r\n");*/
		}
		else
		{
			g_stOC.SwitchFaultStateArray[wOCIndex] = SWTICH_NORMAL;
		}
	}
	if ((DEFAULT_0XFF_UIN16 != g_wSwitchFaultNum)&&(DEFAULT_ZERO != g_wSwitchFaultNum))
	{
		if (NUM_ONE == g_wSwitchFaultNum)
		{
			/*DebugPrint(PRINT_PERIOD,"道岔管理：道岔出现故障，有一个共享锁要求状态与道岔实际状态不一致\r\n");*/
		}
		else if (NUM_TWO == g_wSwitchFaultNum)
		{
			/*DebugPrint(PRINT_PERIOD,"道岔管理：道岔出现故障，有两个共享锁要求状态与道岔实际状态不一致\r\n");*/
		}
		else
		{
			/*DebugPrint(PRINT_PERIOD,"道岔管理：道岔出现故障，有大于两个共享锁要求状态与道岔实际状态不一致\r\n");*/
		}
	}
	else
	{
		/*do nothing*/
	}
	return Rtn;

}
/**=============================================================================
    函数名: LockQSwitchStatus2SwitchStatus()
      描述: 锁结构里的道岔状态类型转换成道岔自身状态类型，为了锁要求道岔状态与自身道岔状态做比较    
      功能: 锁结构里的道岔状态类型转换成道岔自身状态类型，为了锁要求道岔状态与自身道岔状态做比较  
      参数：锁结构里的道岔状态
	  返回值: 成功：返回内部道岔状态 ；失败：返回0
==============================================================================*/
UINT8 LockQSwitchStatus2SwitchStatus(UINT8 LockQSwitchStatus)
{
	UINT8 ReturnSwitchStatus = DEFAULT_ZERO;
	if (LOCK_Q_SWITCH_STATUS_MAIN == LockQSwitchStatus)
	{
		ReturnSwitchStatus = SWITCH_POSITION_MAIN;
	}
	else if (LOCK_Q_SWITCH_STATUS_SIDE == LockQSwitchStatus)
	{
		ReturnSwitchStatus = SWITCH_POSITION_SIDE;
	}
	else
	{
		/*道岔状态错误，四开或者挤岔*/
	}
	return ReturnSwitchStatus;
}

void SwitchSucces(void)
{
	UINT8 i = DEFAULT_ZERO;

	for (i = 0 ;i < g_stOC.SwitchNum;i++)
	{
		if (FLAG_SET == g_stOC.SwitchInfoArray[i].MoveState)
		{
			if(g_dwOCCycle - g_stOC.SwitchInfoArray[i].SendCycle >= SWITCH_CHANGE_TIME)
			{
				g_stOC.SwitchInfoArray[i].MoveState = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].SendCommondState = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].TimeHigh = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].TimeLow = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].CommandID = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].SendCycle = DEFAULT_ZERO;
				g_stOC.SwitchInfoArray[i].CommandSourceID = DEFAULT_ZERO;
			}
			else
			{
				if (LockQSwitchStatus2SwitchStatus(g_stOC.SwitchInfoArray[i].SendCommondState) == g_stOC.SwitchStateArray[i].Position)
				{
					g_stOC.SwitchInfoArray[i].MoveState = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].SendCommondState = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].TimeHigh = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].TimeLow = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].CommandID = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].SendCycle = DEFAULT_ZERO;
					g_stOC.SwitchInfoArray[i].CommandSourceID = DEFAULT_ZERO;
				}
			}
		} 
		else
		{

		}
	}
}
