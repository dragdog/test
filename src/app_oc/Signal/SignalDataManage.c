/********************************************************
*                                                                                                            
* 文 件 名： SignalDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 信号机数据管理  
* 修改记录：   
*
********************************************************/ 

#include "SignalDataManage.h"


SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];   /*定义信号机数据结构体*/


/*
* 功能描述： 获取指定ID信号数据
* 参数说明：    
* 返回值  ： 
            SignalDataStruct*
*/
SignalDataStruct *GetSignalData(void)
{
    return SignalDataStru;
}

/*
* 功能描述： 获取指定ID信号机封锁状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 封锁状态        
*/
UINT8 GetSignalFLockState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 fLockState = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		fLockState = SignalDataStru[bufIndex].FLockState;
	}
	return fLockState;
}


/*
* 功能描述： 设置指定ID信号机封锁状态
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalFLockState(const UINT16 signalId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_FENGSUO_NO == fLockState) || (SIGNAL_FENGSUO_YES == fLockState))
		{
			SignalDataStru[bufIndex].FLockState= fLockState;  /*信号机封锁状态*/

			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}


/*
* 功能描述： 获取指定ID信号机亮灭状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 亮灭状态        
*/
UINT8 GetSignalLightExtinctState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 lightExtinctState = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		lightExtinctState = SignalDataStru[bufIndex].LightExtinctState;
	}
	return lightExtinctState;
}


/*
* 功能描述： 设置指定ID信号机亮灭状态
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 ligheCloseState, 亮灭状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalLightExtinctState(const UINT16 signalId, const UINT8 lightExtinctState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_LIGHT_EXTICT_STATE_NO == lightExtinctState) || (SIGNAL_LIGHT_EXTICT_STATE_YES == lightExtinctState) || (SIGNAL_LIGHT_EXTICT_STATE_ALL_YES == lightExtinctState))
		{

			/*SignalDataStru[bufIndex].LightExtinctState = lightExtinctState;  信号机亮灭状态*/

			reVal = CI_SUCCESS;

			if (SIGNAL_LIGHT_EXTICT_STATE_ALL_YES == lightExtinctState)
			{/*如果是与ZC通信中断导致全站亮灯 则全部有效*/
				SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_YES;
			}
			else if (SIGNAL_LIGHT_EXTICT_STATE_YES == lightExtinctState)
			{/*本次解析状态为亮灯*/
				if (SignalDataStru[bufIndex].SignalLightOnStateCycCount >= SIGNAL_LIGHTON_MID_COUNT)
				{/*之前为亮灯*/
					if (SignalDataStru[bufIndex].SignalLightOnStateCycCount > (SIGNAL_LIGHTON_MID_COUNT + SIGNAL_LIGHTON_CHECK_VALUE))
					{/*超过了有效计数 重新设计为有效计数 防止越界*/
						SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT + SIGNAL_LIGHTON_CHECK_VALUE);

						SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_YES;
					} 
					else
					{
						SignalDataStru[bufIndex].SignalLightOnStateCycCount++;
					}
				} 
				else
				{/*之前为灭灯 则从1计数*/
					SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT + 1);
				}
			} 
			else
			{/*本次解析状态为灭灯*/
				if (SignalDataStru[bufIndex].SignalLightOnStateCycCount < SIGNAL_LIGHTON_MID_COUNT)
				{/*之前为灭灯*/
					if (SignalDataStru[bufIndex].SignalLightOnStateCycCount < (SIGNAL_LIGHTON_MID_COUNT - SIGNAL_LIGHTON_CHECK_VALUE))
					{/*超过了有效计数 重新设计为有效计数 防止越界*/
						SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT - SIGNAL_LIGHTON_CHECK_VALUE);

						SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_NO;
					} 
					else
					{
						SignalDataStru[bufIndex].SignalLightOnStateCycCount--;
					}
				} 
				else
				{/*之前为亮灯 则从1计数*/
					SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT - 1);
				}
			}
		}
	}
	return reVal;
}


/*
* 功能描述： 获取指定ID信号机之前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前显示颜色        
*/
UINT8 GetSignalPreColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].PreColor;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机当前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前显示颜色        
*/
UINT8 GetSignalCurColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].CurColor;
	}
	return reVal;
}


/*
* 功能描述： 设置指定ID信号机当前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 curColor, 当前显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalCurColor(const UINT16 signalId, const UINT8 curColor)
{
	UINT16 bufIndex = 0;  /*数组下标*/

    UINT8 preDrvColor = 0;/*之前驱动颜色*/
	UINT8 reVal = CI_ERROR;
    
    preDrvColor = GetSignalPreDriveColor(signalId);/*获取之前驱动颜色*/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        SignalDataStru[bufIndex].PreColor = SignalDataStru[bufIndex].CurColor;/*信号机之前显示颜色*/
		if (1 == CheckSignalShowColorValid(curColor))
		{
			SignalDataStru[bufIndex].CurColor = curColor;  /*信号机当前显示颜色*/
            
            /*信号机开放*/
            if ((curColor != SIGNAL_SHOWCOLOUR_RED) && (curColor != SIGNAL_SHOWCOLOUR_REDCLOSE))
            {
                SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_NO;/*设置未断丝标志*/
                SignalDataStru[bufIndex].RelayActaionStartCycNum = 0;/*信号机继电器动作开始周期清零*/
            }
            else
            {
				/*这是为了解决红亮转为红灭或红灭转为红亮之后,继电器动作开始周期不清零的问题*/
				SignalDataStru[bufIndex].RelayActaionStartCycNum = 0;/*信号机继电器动作开始周期清零*/
            }
		}
		else
		{/*显示颜色无效*/

			switch (SignalDataStru[bufIndex].PreColor)
			{
            case SIGNAL_SHOWCOLOUR_RED:/*之前红亮*/
				if(SIGNAL_SHOWCOLOUR_REDYELLOW == SignalDataStru[bufIndex].PreDriveColor)
				{/*该状况为:驱动红黄灯,但是红灯发生断丝*/
					if(GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_UP)
					{
	                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
					}
				}
				else if(SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if(RELAY_STATE_UP == GetRelayCollState(GetSignalFirstDsRelayId(signalId)))
					{/*该状况为:由驱动红黄灯(或红白灯)转为驱动红灯,但是2DJ未及时落下*/
						if(RELAY_STATE_UP == GetRelayCollState(GetSignalSecondDsRelayId(signalId)))
						{
							if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
							{
								/*继电器动作开始周期为0*/
								SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
							}
							else
							{
								if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
								{
									SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
								}
								else
								{
									/*继电器动作延时等待结束*/
									SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
								}
							}
						}
						else
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红灯*/
						}
					}
					else
					{						
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
					}
				}
				else if(SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if(SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
					else
					{
						if((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
						}
					}
				}
				else if(SIGNAL_SHOWCOLOUR_GREENCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*设置信号机绿断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else if(SIGNAL_SHOWCOLOUR_YELLOWCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*设置信号机绿断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_RED_WHITE == SignalDataStru[bufIndex].PreDriveColor)
				{/*该状况为:驱动红白灯,但是红灯发生断丝*/
					if(GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_UP)
					{/*由于2DJ缓吸，采集到无效颜色时保持红灯，等待2DJ吸起*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
					}
				}
				else
				{ 
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/										
				}
				break;
            case SIGNAL_SHOWCOLOUR_GREEN:/*之前绿亮*/
				if(SIGNAL_SHOWCOLOUR_GREENCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREEN;/*设置信号机绿亮*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREEN;/*设置信号机绿亮*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*设置信号机绿断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*设置信号机绿断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else
				{
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*设置信号机绿断*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/					
				}
                break;
            case SIGNAL_SHOWCOLOUR_YELLOW:/*之前黄亮*/
				if(SIGNAL_SHOWCOLOUR_YELLOWCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOW;/*设置信号机黄亮*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOW;/*设置信号机黄亮*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*设置信号机黄断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*设置信号机黄断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
				else
				{
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*设置信号机黄断*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
				}
                break;
            case SIGNAL_SHOWCOLOUR_WHITE:/*之前白亮*/
                SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_WHITEBREAK;/*设置信号机白断*/
                SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
                break;
            case SIGNAL_SHOWCOLOUR_REDYELLOW:/*之前红黄亮*/
                if (GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_DOWN)
                {/*1灯丝继电器落下*/

                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                    SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
                }
                else
                {
                	if(GetRelayCollState(GetSignalSecondDsRelayId(signalId)) == RELAY_STATE_DOWN)
                	{
						if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
						{
							/*继电器动作开始周期为0*/
							SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDYELLOW;/*设置信号机红黄亮*/
						}
						else
						{
							if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
							{
								/*继电器动作延时等待结束*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
							}
							else
							{
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDYELLOW;/*设置信号机红黄亮*/
							}
						}    	
                	}
                	else
                	{
                    	SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
                    }
                }

                break;
            case SIGNAL_SHOWCOLOUR_GREENCLOSE:/*之前绿灭*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_GREEN)
                {
                	/*之前驱动绿亮*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {
                    	/*继电器动作开始周期为0*/
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENCLOSE;/*设置信号机绿灭*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {
                        	/*继电器动作延时等待结束*/
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENCLOSE;/*设置信号机绿灭*/
                        }
                    }
                }
				else if (SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红灯*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红灯*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                }
				break;
            case SIGNAL_SHOWCOLOUR_YELLOWCLOSE:/*之前黄灭*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_YELLOW)
                {/*之前驱动为黄亮*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {/*继电器动作开始周期为0*/
                        
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWCLOSE;/*设置信号机黄灭*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {/*继电器动作延时等待结束*/
                            
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWCLOSE;/*设置信号机黄灭*/
                        }
                    }
                }
				else if (SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*继电器动作开始周期为0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红灯*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红灯*/
						}
						else
						{
							/*继电器动作延时等待结束*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
						}
					}
				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                }
				break;
            case SIGNAL_SHOWCOLOUR_REDCLOSE:/*之前红灭*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_RED)
                {/*之前驱动为红亮*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {/*继电器动作开始周期为0*/
                        
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {/*继电器动作延时等待结束*/
                            
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
                        }
                    }
                }
				else if ((preDrvColor == SIGNAL_DRIVECOLOUR_YELLOW) || (preDrvColor == SIGNAL_DRIVECOLOUR_YELLOWCLOSE)
					|| (preDrvColor == SIGNAL_DRIVECOLOUR_GREEN) || (preDrvColor == SIGNAL_DRIVECOLOUR_GREENCLOSE))
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{/*继电器动作开始周期为0*/

						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
						{/*继电器动作延时等待结束*/

							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
						}
						else
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*设置信号机红灭*/
						}
					}

				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                }
                break;
			case SIGNAL_SHOWCOLOUR_RED_WHITE:/*之前红白亮*/
				if (GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_DOWN)
				{/*1灯丝继电器落下*/

					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*设置断丝标志*/
				}
				else
				{
					if(GetRelayCollState(GetSignalSecondDsRelayId(signalId)) == RELAY_STATE_DOWN)
					{/*引导信号保持期间2DJ故障落下*/
						if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
						{
							/*继电器动作开始周期为0*/
							SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*设置当前周期*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED_WHITE;/*设置信号机红白亮*/
						}
						else
						{
							if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
							{
								/*继电器动作延时等待结束*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
							}
							else
							{
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED_WHITE;/*设置信号机红白亮*/
							}
						}    	
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*设置信号机红亮*/
					}
				}
				break;
            default:
                SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*设置信号机红断*/
                break;
			}
		}

        /*信号机已驱动开放*/
        if (GetSignalOpenCloseState(curColor) == SIGNAL_OPEN)
        {   
            /*设置信号驱动开放周期*/
            SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
        }
        else
        {/*信号已驱动关闭*/

            /*信号驱动关闭周期清零*/
            SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;
        }

        reVal = CI_SUCCESS;
	}
	return reVal;
}


/*
* 功能描述： 获取指定ID信号机之前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalPreDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].PreDriveColor;
	}
	return reVal;
}

/*
* 功能描述： 设置指定ID信号机之前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 driveColor, 驱动显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalPreDriveColor(const UINT16 signalId, const UINT8 driveColor)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].PreDriveColor = driveColor;
			
		reVal = CI_SUCCESS;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机当前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalCurDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].CurDriveColor;
	}
	return reVal;
}


/*
* 功能描述： 设置指定ID信号机当前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 driveColor, 驱动显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalCurDriveColor(const UINT16 signalId, const UINT8 driveColor)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 curDrvColor = 0;/*当前驱动颜色*/
	UINT8 reVal = CI_SUCCESS;
    UINT8 sigDrvEnable = 0;/*信号机是否可驱动标志*/
	UINT8 sigVirtAttr = 0 ;/*信号机虚拟属性*/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/
	curDrvColor = GetSignalCurDriveColor(signalId);/*获取当前驱动颜色*/
	sigDrvEnable = GetSignalDrvEnable(signalId);/*获取信号机是否可驱动标志*/
	sigVirtAttr = GetSignalVirtualAttr(signalId);/*获取信号机虚拟属性*/


	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((CI_SUCCESS == CheckSignalDriveColorValid(driveColor)) && 
			((SIGNAL_DRV_ENABLE == sigDrvEnable)|| (SIGNAL_VIRTUAL_YES == sigVirtAttr)))
		{/*驱动颜色有效且（信号机为虚拟信号机或者可驱动非虚拟信号机）*/
			if (driveColor == SIGNAL_DRIVECOLOUR_NULL)
			{
                /*设置信号驱动颜色无效*/
				SignalDataStru[bufIndex].CurDriveColor = SIGNAL_DRIVECOLOUR_NULL;
			}
            else if ((SIGNAL_DRIVECOLOUR_RED == driveColor) || (SIGNAL_DRIVECOLOUR_REDCLOSE == driveColor))
			{/*信号机驱动颜色为红亮或红灭*/
				
				SignalDataStru[bufIndex].CurDriveColor = driveColor;  /*信号机当前驱动显示颜色*/
                
                if (SignalDataStru[bufIndex].DriveSigCloseStartCycNum == 0)
                {
                    if ((GetSignalType(signalId) == SIGNAL_TYPE_TWO_CHUKU_RG)
                        || (GetSignalType(signalId) == SIGNAL_TYPE_TWO_JINKU_RGY))
                    {
                        /*清空驱动周期*/
                        SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;
                    }
                    else
                    {
                        /*设置信号驱动关闭周期*/
                        SignalDataStru[bufIndex].DriveSigCloseStartCycNum = Get2oo2CurCycleNum();
                    }
                    
                    
                }     
                else
                {
                    /*已设置*/
                }
                
                /*信号驱动开放周期清零*/
                SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
			}
			else if(SIGNAL_DRIVECOLOUR_NULL == curDrvColor)/*之前为无效*/
			{/*开放信号*/
				
				SignalDataStru[bufIndex].CurDriveColor = driveColor;  /*信号机当前驱动显示颜色*/

                /*信号驱动关闭周期清零*/
                SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;

				if (SignalDataStru[bufIndex].DriveSigOpenStartCycNum == 0)
                {

                    if ((GetSignalType(signalId) == SIGNAL_TYPE_TWO_CHUKU_RG)
                        || (GetSignalType(signalId) == SIGNAL_TYPE_TWO_JINKU_RGY))
                    {
                        /*清空驱动周期*/
                        SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
                    }
                    else
                    {
                        /*设置信号驱动开放周期*/
                        SignalDataStru[bufIndex].DriveSigOpenStartCycNum = Get2oo2CurCycleNum();
                    }
                     	
				}
                else
                {
                    /*已设置*/
                }                		
			}
			else
			{/*其他情况*/
				
				reVal = CI_ERROR;
			}					
		}
		else
		{
            /*设置信号驱动颜色无效*/
            SignalDataStru[bufIndex].CurDriveColor = SIGNAL_DRIVECOLOUR_NULL;
			reVal = CI_ERROR;
		}

		if(	(SignalDataStru[bufIndex].PreDriveColor != SignalDataStru[bufIndex].CurDriveColor)
			&&
			(SignalDataStru[bufIndex].CurDriveColor != SIGNAL_DRIVECOLOUR_NULL))
		{/*当信号驱动颜色变更的并且该驱动颜色有效时,更新驱动新颜色起始周期号,更新变更之前的驱动颜色.本段代码是为了解决红驱绿或绿驱红时,期间闪现黄灯的问题*/
			SignalDataStru[bufIndex].DriveNewSigStartCycNum = Get2oo2CurCycleNum();
			SignalDataStru[bufIndex].OldDriveColor = SignalDataStru[bufIndex].PreDriveColor;
		}
		else
		{
			/*不做任何处理*/
			;
		}
	}
	else
	{
		reVal = CI_ERROR;
	}
	return reVal;
}


/*
* 功能描述： 获取指定ID信号机灯丝断丝标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 断丝标志        
*/
UINT8 GetSignalDsBreakFlag(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DsBreakFlag;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机驱动开放起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开放起始周期号        
*/
UINT32 GetSignalDriveOpenStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveSigOpenStartCycNum;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机驱动关闭起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动关闭起始周期号        
*/
UINT32 GetSignalDriveCloseStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveSigCloseStartCycNum;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机驱动新信号起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动新信号起始周期号        
*/
UINT32 GetSignalDriveNewSigStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveNewSigStartCycNum;
	}

	return reVal;
}

/*
* 功能描述： 获取指定ID信号机驱动信号机颜色变化之前,驱动信号机显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalOldDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].OldDriveColor;
	}

	return reVal;
}

/*
* 功能描述： 检查信号机显示颜色有效
* 参数说明： const UINT8 showColor, 信号机显示颜色
* 返回值  ： 0: 显示无效
*			 1: 显示有效      
*/
UINT8 CheckSignalShowColorValid(const UINT8 showColor)
{
	UINT8 reVal = CI_ERROR;

    if ((SIGNAL_SHOWCOLOUR_RED == showColor) || (SIGNAL_SHOWCOLOUR_GREEN == showColor) || (SIGNAL_SHOWCOLOUR_YELLOW == showColor)
        || (SIGNAL_SHOWCOLOUR_REDYELLOW == showColor) || (SIGNAL_SHOWCOLOUR_WHITE == showColor) || (SIGNAL_SHOWCOLOUR_REDCLOSE == showColor) 
        || (SIGNAL_SHOWCOLOUR_GREENCLOSE == showColor) || (SIGNAL_SHOWCOLOUR_YELLOWCLOSE == showColor) || (SIGNAL_SHOWCOLOUR_RED_WHITE == showColor))
    {
        reVal = CI_SUCCESS;  /*信号机显示有效*/
    }
	return reVal;
}


/*
* 功能描述： 检查信号机驱动颜色有效
* 参数说明： const UINT8 drvColor, 信号机驱动颜色
* 返回值  ： 0: 驱动无效
*			 1: 驱动有效      
*/
UINT8 CheckSignalDriveColorValid(const UINT8 drvColor)
{
	UINT8 reVal = CI_ERROR;


    if ((SIGNAL_DRIVECOLOUR_RED == drvColor) || (SIGNAL_DRIVECOLOUR_YELLOW == drvColor) || (SIGNAL_DRIVECOLOUR_REDYELLOW == drvColor)
        || (SIGNAL_DRIVECOLOUR_GREEN == drvColor) || (SIGNAL_DRIVECOLOUR_YELLOWCLOSE == drvColor) || (SIGNAL_DRIVECOLOUR_REDCLOSE == drvColor) 
        || (SIGNAL_DRIVECOLOUR_GREENCLOSE == drvColor) || (SIGNAL_DRIVECOLOUR_WHITE == drvColor)|| (SIGNAL_DRIVECOLOUR_RED_WHITE == drvColor)
		||(SIGNAL_DRIVECOLOUR_NULL == drvColor))
    {
        reVal = CI_SUCCESS; /*信号机驱动有效*/
    }

	return reVal;
}


/*
* 功能描述： 获取通信车跨压通过信号机状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalCrossState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_CROSS_STATE_NO == SignalDataStru[bufIndex].SignalCrossState) || (SIGNAL_CROSS_STATE_YES == SignalDataStru[bufIndex].SignalCrossState))
		{
			reVal = SignalDataStru[bufIndex].SignalCrossState;
		}		
	}
	return reVal;
}


/*
* 功能描述： 设置通信车跨压通过信号机状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 设置失败
*			>0: 设置成功      
*/
UINT8 SetSignalCrossState(const UINT16 signalId, const UINT8 signalCrossState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_CROSS_STATE_NO == signalCrossState) || (SIGNAL_CROSS_STATE_YES == signalCrossState))
		{
			SignalDataStru[bufIndex].SignalCrossState = signalCrossState;  /*设置通信车跨压通过信号机状态*/
			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}

/*
* 功能描述： 获取信号机颜色开放关闭状态
* 参数说明： UINT8 signalColor
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalOpenCloseState(UINT8 signalColor)
{
    UINT8 reVal = SIGNAL_CLOSE;

    if ((SIGNAL_SHOWCOLOUR_GREEN == signalColor) || 
        (SIGNAL_SHOWCOLOUR_YELLOW == signalColor) || 
        (SIGNAL_SHOWCOLOUR_REDYELLOW == signalColor) || 
        (SIGNAL_SHOWCOLOUR_WHITE == signalColor) || 
        (SIGNAL_SHOWCOLOUR_GREENCLOSE == signalColor) || 
        (SIGNAL_SHOWCOLOUR_YELLOWCLOSE == signalColor) ||
		(SIGNAL_SHOWCOLOUR_RED_WHITE == signalColor))
    { /*绿灯||黄灯||红黄灯||白灯||绿灭||黄灭||红白灯*/

        reVal = SIGNAL_OPEN;  /*信号开放*/
    }
    return reVal;
}

/*
* 功能描述： 获取引导总锁信号机开放状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalYLockOpenState(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;
    
    bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/
    
    if (bufIndex < SIGNAL_SUM_MAX)
    {
        if ((SIGNAL_OPEN == SignalDataStru[bufIndex].SignalYLockOpenState) || (SIGNAL_CLOSE == SignalDataStru[bufIndex].SignalYLockOpenState))
        {
            reVal = SignalDataStru[bufIndex].SignalYLockOpenState;
        }
    }
	return reVal;
}


/*
* 功能描述： 设置引导总锁信号机开放状态
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 状态
* 返回值  ： 0: 设置失败
*			>0: 设置成功      
*/
UINT8 SetSignalYLockOpenState(const UINT16 signalId, const UINT8 setState)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 reVal = CI_ERROR;
    
    bufIndex = GetSignalConfigBufIndex(signalId);  /*获取道岔数组下标*/
    
    if (bufIndex < SIGNAL_SUM_MAX)
    {
        if ((SIGNAL_OPEN == setState) && 
            (SignalDataStru[bufIndex].SignalLeadProperty == SIGNAL_LEAD_PROPERTY_YES) &&
            (GetSignalOpenCloseState(GetSignalCurColor(signalId)) == SIGNAL_CLOSE)
            )
        {
            /*信号可引导 且 信号机当前关闭*/
            SignalDataStru[bufIndex].SignalYLockOpenState = setState;  /*设置状态*/
            reVal = CI_SUCCESS;
        }
        else if (SIGNAL_CLOSE == setState)
        {
            SignalDataStru[bufIndex].SignalYLockOpenState = setState;  /*设置状态*/
            reVal = CI_SUCCESS;
        }
        else
        {
            reVal = CI_ERROR;
        }		
    }
	return reVal;
}

/*
* 功能描述： 获取指定ID信号机扣车标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 扣车标志      
*/
UINT8 GetSignalDetainFlag(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        if (SignalDataStru[bufIndex].AtsDetainFlag == SIGNAL_DETAIN_NO)
        {/*ATS未扣车*/

            if (SignalDataStru[bufIndex].XiandiDetainFlag == SIGNAL_DETAIN_NO)
            {/*现地未扣车*/

                reVal = SIGNAL_DETAIN_NO;/*返回现地、ATS均未扣车*/
            }
            else
            {
                /*现地扣车*/
                reVal = SIGNAL_DETAIN_XIANDI;/*返回现地扣车、ATS未扣车*/
            }
        }
        else
        {/*ATS扣车*/

            if (SignalDataStru[bufIndex].XiandiDetainFlag == SIGNAL_DETAIN_NO)
            {/*现地未扣车*/
                
                reVal = SIGNAL_DETAIN_ATS;/*返回现地未扣车、ATS均扣车*/
            }
            else
            {
                /*现地扣车*/
                reVal = SIGNAL_DETAIN_YES;/*返回现地扣车、ATS均扣车*/
            }
        }
	}
	return reVal;
}

/*
* 功能描述： 设置指定ID信号机扣车状态
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainState(const UINT16 signalId, const UINT8 setState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if (setState == SIGNAL_DETAIN_YES)
		{/*ATS、现地均扣车*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_YES;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_NO)
		{/*ATS、现地均未扣车*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_NO;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_XIANDI)
		{/*现地办理扣车*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_ATS)
		{/*ATS办理扣车*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_YES;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_NO;
			reVal = CI_SUCCESS;
		}
		else
		{
			reVal = CI_ERROR;
		}	
	}
	return reVal;
}

/*
* 功能描述： 设置指定ID信号机扣车标志
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainFlag(const UINT16 signalId, const UINT8 setState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        if (setState == SIGNAL_DETAIN_ATS)
        {/*ats办理扣车*/

            SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_YES;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_ATS_NO)
        {/*ats取消扣车*/

            SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_XIANDI)
        {/*现地办理扣车*/

            SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_XIANDI_NO)
        {/*现地取消扣车*/

            SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_NO;
            reVal = CI_SUCCESS;
        }
        else
        {
            reVal = CI_ERROR;
	    }	
	}
	return reVal;
}

/*
* 功能描述： 获取自动信号对应的进路ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机ID      
*/
UINT16 GetSignalAutoRouteId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 returnValue = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取指定信号机的索引号*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		returnValue = SignalDataStru[bufIndex].AutoRouteId;
	}

	return returnValue;
}

/*
* 功能描述： 设置自动信号对应的自动信号机ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalAutoRouteId(const UINT16 signalId,const UINT16 routeId)
{
	UINT8 ret = CI_SUCCESS;
	UINT16 bufIndex = 0;  /*数组下标*/
	
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取指定信号机的索引号*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].AutoRouteId = routeId;
	}
	else
	{
		ret = CI_ERROR;
	}

	return ret;
}


/*
* 功能描述： 获取指定ID信号机临时限速设置标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 临时限速设置状态      
*/
UINT8 GetSignalLinShiXianSuSetFlag(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8  funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        funRetVal = SignalDataStru[bufIndex].LinShiXianSuSetFlag;
    }
    else
    {
        funRetVal = CI_ERROR;
    }	
	return funRetVal;
}


/*
* 功能描述： 设置指定ID信号机临时限速标志
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalLinShiXianSuSetFlag(const UINT16 signalId, const UINT8 linShiXianSuSetState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_LINSHIXIANSUSET_STATE_NO == linShiXianSuSetState) ||
            (SIGNAL_LINSHIXIANSUSET_STATE_YES == linShiXianSuSetState) ||
            (SIGNAL_LINSHIXIANSUSET_STATE_INVALID == linShiXianSuSetState))
		{
			SignalDataStru[bufIndex].LinShiXianSuSetFlag = linShiXianSuSetState;  /*信号机临时限速标志*/

			funRetVal = CI_SUCCESS;
		}
		else
		{
			funRetVal = CI_ERROR;
		}
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;	
}

UINT8 GetSignalPLockCommand(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		funRetVal = SignalDataStru[bufIndex].PLockCommand;
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;	
}


/*
* 功能描述： 设置指定ID信号机异常关闭标志
* 参数说明： const UINT16 signalId, 信号机ID
			 const UINT8 curColor, 当前显示颜色
			 const UINT8 setState，当前设置状态
* 返回值  ： 0: 获取数据失败
*			>0: 异常标志      
*/
UINT8 SetSignalPLockCommand(const UINT16 signalId, const UINT8 pLockCommand)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].PLockCommand = pLockCommand;  /*逻辑区段之前占用状态*/

		funRetVal = CI_SUCCESS;

	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;	
}

/*
* 功能描述： 获取信号机互联互通保护区段有效性
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 保护区段有效性属性
*/
UINT8 GetSignalProSecValid(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		funRetVal = SignalDataStru[bufIndex].ProSecValid;
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;
}


/*
* 功能描述： 设置信号机互联互通保护区段有效性
* 参数说明：
const UINT16 signalId, 信号机ID
const UINT8 proSecValid:保护区段有效性
* 返回值:
*		0: 设置失败
*		1: 设置成功
*/
UINT8 SetSignalProSecValid(const UINT16 signalId, const UINT8 proSecValid)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_PRO_SEC_VALID == proSecValid) || (SIGNAL_PRO_SEC_INVALID == proSecValid))
		{
			SignalDataStru[bufIndex].ProSecValid = proSecValid;

			funRetVal = CI_SUCCESS;
		}
		else
		{
			funRetVal = CI_ERROR;
		}
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;
}
/*
* 功能描述： 设置信号机颜色已经开放，但不是目标的驱动开放颜色，记录该过渡态的起始周期
* 参数说明：
	const UINT16 signalId, 信号机ID
	const UINT32 cycNum:起始周期号
* 返回值:
*		0: 设置失败
*		1: 设置成功
*/
UINT8 SetSignalDiffColorOpenStartCycNum(const UINT16 signalId,const UINT32 cycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].DiffColorOpenStartCycNum = cycNum;

		funRetVal = CI_SUCCESS;
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;
}

/*
* 功能描述： 获取信号机颜色已经开放，但不是目标的驱动开放颜色，记录该过渡态的起始周期
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 起始周期号        
*/
UINT32 GetSignalDiffColorOpenStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DiffColorOpenStartCycNum;
	}

	return reVal;
}