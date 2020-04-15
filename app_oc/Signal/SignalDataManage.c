/********************************************************
*                                                                                                            
* �� �� ���� SignalDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �źŻ����ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "SignalDataManage.h"


SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];   /*�����źŻ����ݽṹ��*/


/*
* ���������� ��ȡָ��ID�ź�����
* ����˵����    
* ����ֵ  �� 
            SignalDataStruct*
*/
SignalDataStruct *GetSignalData(void)
{
    return SignalDataStru;
}

/*
* ���������� ��ȡָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSignalFLockState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 fLockState = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		fLockState = SignalDataStru[bufIndex].FLockState;
	}
	return fLockState;
}


/*
* ���������� ����ָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalFLockState(const UINT16 signalId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_FENGSUO_NO == fLockState) || (SIGNAL_FENGSUO_YES == fLockState))
		{
			SignalDataStru[bufIndex].FLockState= fLockState;  /*�źŻ�����״̬*/

			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}


/*
* ���������� ��ȡָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSignalLightExtinctState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 lightExtinctState = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		lightExtinctState = SignalDataStru[bufIndex].LightExtinctState;
	}
	return lightExtinctState;
}


/*
* ���������� ����ָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 ligheCloseState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalLightExtinctState(const UINT16 signalId, const UINT8 lightExtinctState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_LIGHT_EXTICT_STATE_NO == lightExtinctState) || (SIGNAL_LIGHT_EXTICT_STATE_YES == lightExtinctState) || (SIGNAL_LIGHT_EXTICT_STATE_ALL_YES == lightExtinctState))
		{

			/*SignalDataStru[bufIndex].LightExtinctState = lightExtinctState;  �źŻ�����״̬*/

			reVal = CI_SUCCESS;

			if (SIGNAL_LIGHT_EXTICT_STATE_ALL_YES == lightExtinctState)
			{/*�������ZCͨ���жϵ���ȫվ���� ��ȫ����Ч*/
				SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_YES;
			}
			else if (SIGNAL_LIGHT_EXTICT_STATE_YES == lightExtinctState)
			{/*���ν���״̬Ϊ����*/
				if (SignalDataStru[bufIndex].SignalLightOnStateCycCount >= SIGNAL_LIGHTON_MID_COUNT)
				{/*֮ǰΪ����*/
					if (SignalDataStru[bufIndex].SignalLightOnStateCycCount > (SIGNAL_LIGHTON_MID_COUNT + SIGNAL_LIGHTON_CHECK_VALUE))
					{/*��������Ч���� �������Ϊ��Ч���� ��ֹԽ��*/
						SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT + SIGNAL_LIGHTON_CHECK_VALUE);

						SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_YES;
					} 
					else
					{
						SignalDataStru[bufIndex].SignalLightOnStateCycCount++;
					}
				} 
				else
				{/*֮ǰΪ��� ���1����*/
					SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT + 1);
				}
			} 
			else
			{/*���ν���״̬Ϊ���*/
				if (SignalDataStru[bufIndex].SignalLightOnStateCycCount < SIGNAL_LIGHTON_MID_COUNT)
				{/*֮ǰΪ���*/
					if (SignalDataStru[bufIndex].SignalLightOnStateCycCount < (SIGNAL_LIGHTON_MID_COUNT - SIGNAL_LIGHTON_CHECK_VALUE))
					{/*��������Ч���� �������Ϊ��Ч���� ��ֹԽ��*/
						SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT - SIGNAL_LIGHTON_CHECK_VALUE);

						SignalDataStru[bufIndex].LightExtinctState = SIGNAL_LIGHT_EXTICT_STATE_NO;
					} 
					else
					{
						SignalDataStru[bufIndex].SignalLightOnStateCycCount--;
					}
				} 
				else
				{/*֮ǰΪ���� ���1����*/
					SignalDataStru[bufIndex].SignalLightOnStateCycCount = (SIGNAL_LIGHTON_MID_COUNT - 1);
				}
			}
		}
	}
	return reVal;
}


/*
* ���������� ��ȡָ��ID�źŻ�֮ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ��ʾ��ɫ        
*/
UINT8 GetSignalPreColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].PreColor;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ���ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ��ʾ��ɫ        
*/
UINT8 GetSignalCurColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].CurColor;
	}
	return reVal;
}


/*
* ���������� ����ָ��ID�źŻ���ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 curColor, ��ǰ��ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalCurColor(const UINT16 signalId, const UINT8 curColor)
{
	UINT16 bufIndex = 0;  /*�����±�*/

    UINT8 preDrvColor = 0;/*֮ǰ������ɫ*/
	UINT8 reVal = CI_ERROR;
    
    preDrvColor = GetSignalPreDriveColor(signalId);/*��ȡ֮ǰ������ɫ*/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        SignalDataStru[bufIndex].PreColor = SignalDataStru[bufIndex].CurColor;/*�źŻ�֮ǰ��ʾ��ɫ*/
		if (1 == CheckSignalShowColorValid(curColor))
		{
			SignalDataStru[bufIndex].CurColor = curColor;  /*�źŻ���ǰ��ʾ��ɫ*/
            
            /*�źŻ�����*/
            if ((curColor != SIGNAL_SHOWCOLOUR_RED) && (curColor != SIGNAL_SHOWCOLOUR_REDCLOSE))
            {
                SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_NO;/*����δ��˿��־*/
                SignalDataStru[bufIndex].RelayActaionStartCycNum = 0;/*�źŻ��̵���������ʼ��������*/
            }
            else
            {
				/*����Ϊ�˽������תΪ��������תΪ����֮��,�̵���������ʼ���ڲ����������*/
				SignalDataStru[bufIndex].RelayActaionStartCycNum = 0;/*�źŻ��̵���������ʼ��������*/
            }
		}
		else
		{/*��ʾ��ɫ��Ч*/

			switch (SignalDataStru[bufIndex].PreColor)
			{
            case SIGNAL_SHOWCOLOUR_RED:/*֮ǰ����*/
				if(SIGNAL_SHOWCOLOUR_REDYELLOW == SignalDataStru[bufIndex].PreDriveColor)
				{/*��״��Ϊ:������Ƶ�,���Ǻ�Ʒ�����˿*/
					if(GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_UP)
					{
	                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
					}
				}
				else if(SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if(RELAY_STATE_UP == GetRelayCollState(GetSignalFirstDsRelayId(signalId)))
					{/*��״��Ϊ:��������Ƶ�(���׵�)תΪ�������,����2DJδ��ʱ����*/
						if(RELAY_STATE_UP == GetRelayCollState(GetSignalSecondDsRelayId(signalId)))
						{
							if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
							{
								/*�̵���������ʼ����Ϊ0*/
								SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
							}
							else
							{
								if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
								{
									SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
								}
								else
								{
									/*�̵���������ʱ�ȴ�����*/
									SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
								}
							}
						}
						else
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ����*/
						}
					}
					else
					{						
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
					}
				}
				else if(SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if(SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
					else
					{
						if((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
						}
					}
				}
				else if(SIGNAL_SHOWCOLOUR_GREENCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*�����źŻ��̶�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else if(SIGNAL_SHOWCOLOUR_YELLOWCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*�����źŻ��̶�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_RED_WHITE == SignalDataStru[bufIndex].PreDriveColor)
				{/*��״��Ϊ:������׵�,���Ǻ�Ʒ�����˿*/
					if(GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_UP)
					{/*����2DJ�������ɼ�����Ч��ɫʱ���ֺ�ƣ��ȴ�2DJ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
					}
				}
				else
				{ 
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/										
				}
				break;
            case SIGNAL_SHOWCOLOUR_GREEN:/*֮ǰ����*/
				if(SIGNAL_SHOWCOLOUR_GREENCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREEN;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREEN;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*�����źŻ��̶�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*�����źŻ��̶�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else
				{
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENBREAK;/*�����źŻ��̶�*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/					
				}
                break;
            case SIGNAL_SHOWCOLOUR_YELLOW:/*֮ǰ����*/
				if(SIGNAL_SHOWCOLOUR_YELLOWCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
            		if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOW;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_RELAY_ACTION_TIMEMAX)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOW;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*�����źŻ��ƶ�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else if (SIGNAL_SHOWCOLOUR_REDCLOSE == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*�����źŻ��ƶ�*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
				else
				{
					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWBREAK;/*�����źŻ��ƶ�*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
				}
                break;
            case SIGNAL_SHOWCOLOUR_WHITE:/*֮ǰ����*/
                SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_WHITEBREAK;/*�����źŻ��׶�*/
                SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
                break;
            case SIGNAL_SHOWCOLOUR_REDYELLOW:/*֮ǰ�����*/
                if (GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_DOWN)
                {/*1��˿�̵�������*/

                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                    SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
                }
                else
                {
                	if(GetRelayCollState(GetSignalSecondDsRelayId(signalId)) == RELAY_STATE_DOWN)
                	{
						if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
						{
							/*�̵���������ʼ����Ϊ0*/
							SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDYELLOW;/*�����źŻ������*/
						}
						else
						{
							if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
							{
								/*�̵���������ʱ�ȴ�����*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
							}
							else
							{
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDYELLOW;/*�����źŻ������*/
							}
						}    	
                	}
                	else
                	{
                    	SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
                    }
                }

                break;
            case SIGNAL_SHOWCOLOUR_GREENCLOSE:/*֮ǰ����*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_GREEN)
                {
                	/*֮ǰ��������*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {
                    	/*�̵���������ʼ����Ϊ0*/
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENCLOSE;/*�����źŻ�����*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {
                        	/*�̵���������ʱ�ȴ�����*/
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_GREENCLOSE;/*�����źŻ�����*/
                        }
                    }
                }
				else if (SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                }
				break;
            case SIGNAL_SHOWCOLOUR_YELLOWCLOSE:/*֮ǰ����*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_YELLOW)
                {/*֮ǰ����Ϊ����*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {/*�̵���������ʼ����Ϊ0*/
                        
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWCLOSE;/*�����źŻ�����*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {/*�̵���������ʱ�ȴ�����*/
                            
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_YELLOWCLOSE;/*�����źŻ�����*/
                        }
                    }
                }
				else if (SIGNAL_SHOWCOLOUR_RED == SignalDataStru[bufIndex].PreDriveColor)
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{
						/*�̵���������ʼ����Ϊ0*/
						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) <= SIGNAL_OPEN_TO_CLOSE_TIME_DELAY)
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ����*/
						}
						else
						{
							/*�̵���������ʱ�ȴ�����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
							SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
						}
					}
				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                }
				break;
            case SIGNAL_SHOWCOLOUR_REDCLOSE:/*֮ǰ����*/
                if (preDrvColor == SIGNAL_DRIVECOLOUR_RED)
                {/*֮ǰ����Ϊ����*/
                    if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
                    {/*�̵���������ʼ����Ϊ0*/
                        
                        SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
                        SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
                    }
                    else
                    {
                        if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
                        {/*�̵���������ʱ�ȴ�����*/
                            
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                        }
                        else
                        {
                            SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
                        }
                    }
                }
				else if ((preDrvColor == SIGNAL_DRIVECOLOUR_YELLOW) || (preDrvColor == SIGNAL_DRIVECOLOUR_YELLOWCLOSE)
					|| (preDrvColor == SIGNAL_DRIVECOLOUR_GREEN) || (preDrvColor == SIGNAL_DRIVECOLOUR_GREENCLOSE))
				{
					if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
					{/*�̵���������ʼ����Ϊ0*/

						SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
					}
					else
					{
						if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
						{/*�̵���������ʱ�ȴ�����*/

							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
						}
						else
						{
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDCLOSE;/*�����źŻ�����*/
						}
					}

				}
                else
                {
                    SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                }
                break;
			case SIGNAL_SHOWCOLOUR_RED_WHITE:/*֮ǰ�����*/
				if (GetRelayCollState(GetSignalFirstDsRelayId(signalId)) == RELAY_STATE_DOWN)
				{/*1��˿�̵�������*/

					SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
					SignalDataStru[bufIndex].DsBreakFlag = SIGNAL_DS_BREAK_YES;/*���ö�˿��־*/
				}
				else
				{
					if(GetRelayCollState(GetSignalSecondDsRelayId(signalId)) == RELAY_STATE_DOWN)
					{/*�����źű����ڼ�2DJ��������*/
						if (SignalDataStru[bufIndex].RelayActaionStartCycNum == 0)
						{
							/*�̵���������ʼ����Ϊ0*/
							SignalDataStru[bufIndex].RelayActaionStartCycNum = Get2oo2CurCycleNum();/*���õ�ǰ����*/
							SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED_WHITE;/*�����źŻ������*/
						}
						else
						{
							if ((Get2oo2CurCycleNum() - SignalDataStru[bufIndex].RelayActaionStartCycNum) > SIGNAL_RELAY_ACTION_TIMEMAX)
							{
								/*�̵���������ʱ�ȴ�����*/
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
							}
							else
							{
								SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED_WHITE;/*�����źŻ������*/
							}
						}    	
					}
					else
					{
						SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_RED;/*�����źŻ�����*/
					}
				}
				break;
            default:
                SignalDataStru[bufIndex].CurColor = SIGNAL_SHOWCOLOUR_REDBREAK;/*�����źŻ����*/
                break;
			}
		}

        /*�źŻ�����������*/
        if (GetSignalOpenCloseState(curColor) == SIGNAL_OPEN)
        {   
            /*�����ź�������������*/
            SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
        }
        else
        {/*�ź��������ر�*/

            /*�ź������ر���������*/
            SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;
        }

        reVal = CI_SUCCESS;
	}
	return reVal;
}


/*
* ���������� ��ȡָ��ID�źŻ�֮ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalPreDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].PreDriveColor;
	}
	return reVal;
}

/*
* ���������� ����ָ��ID�źŻ�֮ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 driveColor, ������ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalPreDriveColor(const UINT16 signalId, const UINT8 driveColor)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].PreDriveColor = driveColor;
			
		reVal = CI_SUCCESS;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ���ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalCurDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].CurDriveColor;
	}
	return reVal;
}


/*
* ���������� ����ָ��ID�źŻ���ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 driveColor, ������ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalCurDriveColor(const UINT16 signalId, const UINT8 driveColor)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 curDrvColor = 0;/*��ǰ������ɫ*/
	UINT8 reVal = CI_SUCCESS;
    UINT8 sigDrvEnable = 0;/*�źŻ��Ƿ��������־*/
	UINT8 sigVirtAttr = 0 ;/*�źŻ���������*/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
	curDrvColor = GetSignalCurDriveColor(signalId);/*��ȡ��ǰ������ɫ*/
	sigDrvEnable = GetSignalDrvEnable(signalId);/*��ȡ�źŻ��Ƿ��������־*/
	sigVirtAttr = GetSignalVirtualAttr(signalId);/*��ȡ�źŻ���������*/


	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((CI_SUCCESS == CheckSignalDriveColorValid(driveColor)) && 
			((SIGNAL_DRV_ENABLE == sigDrvEnable)|| (SIGNAL_VIRTUAL_YES == sigVirtAttr)))
		{/*������ɫ��Ч�ң��źŻ�Ϊ�����źŻ����߿������������źŻ���*/
			if (driveColor == SIGNAL_DRIVECOLOUR_NULL)
			{
                /*�����ź�������ɫ��Ч*/
				SignalDataStru[bufIndex].CurDriveColor = SIGNAL_DRIVECOLOUR_NULL;
			}
            else if ((SIGNAL_DRIVECOLOUR_RED == driveColor) || (SIGNAL_DRIVECOLOUR_REDCLOSE == driveColor))
			{/*�źŻ�������ɫΪ���������*/
				
				SignalDataStru[bufIndex].CurDriveColor = driveColor;  /*�źŻ���ǰ������ʾ��ɫ*/
                
                if (SignalDataStru[bufIndex].DriveSigCloseStartCycNum == 0)
                {
                    if ((GetSignalType(signalId) == SIGNAL_TYPE_TWO_CHUKU_RG)
                        || (GetSignalType(signalId) == SIGNAL_TYPE_TWO_JINKU_RGY))
                    {
                        /*�����������*/
                        SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;
                    }
                    else
                    {
                        /*�����ź������ر�����*/
                        SignalDataStru[bufIndex].DriveSigCloseStartCycNum = Get2oo2CurCycleNum();
                    }
                    
                    
                }     
                else
                {
                    /*������*/
                }
                
                /*�ź�����������������*/
                SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
			}
			else if(SIGNAL_DRIVECOLOUR_NULL == curDrvColor)/*֮ǰΪ��Ч*/
			{/*�����ź�*/
				
				SignalDataStru[bufIndex].CurDriveColor = driveColor;  /*�źŻ���ǰ������ʾ��ɫ*/

                /*�ź������ر���������*/
                SignalDataStru[bufIndex].DriveSigCloseStartCycNum = 0;

				if (SignalDataStru[bufIndex].DriveSigOpenStartCycNum == 0)
                {

                    if ((GetSignalType(signalId) == SIGNAL_TYPE_TWO_CHUKU_RG)
                        || (GetSignalType(signalId) == SIGNAL_TYPE_TWO_JINKU_RGY))
                    {
                        /*�����������*/
                        SignalDataStru[bufIndex].DriveSigOpenStartCycNum = 0;
                    }
                    else
                    {
                        /*�����ź�������������*/
                        SignalDataStru[bufIndex].DriveSigOpenStartCycNum = Get2oo2CurCycleNum();
                    }
                     	
				}
                else
                {
                    /*������*/
                }                		
			}
			else
			{/*�������*/
				
				reVal = CI_ERROR;
			}					
		}
		else
		{
            /*�����ź�������ɫ��Ч*/
            SignalDataStru[bufIndex].CurDriveColor = SIGNAL_DRIVECOLOUR_NULL;
			reVal = CI_ERROR;
		}

		if(	(SignalDataStru[bufIndex].PreDriveColor != SignalDataStru[bufIndex].CurDriveColor)
			&&
			(SignalDataStru[bufIndex].CurDriveColor != SIGNAL_DRIVECOLOUR_NULL))
		{/*���ź�������ɫ����Ĳ��Ҹ�������ɫ��Чʱ,������������ɫ��ʼ���ں�,���±��֮ǰ��������ɫ.���δ�����Ϊ�˽�������̻�������ʱ,�ڼ����ֻƵƵ�����*/
			SignalDataStru[bufIndex].DriveNewSigStartCycNum = Get2oo2CurCycleNum();
			SignalDataStru[bufIndex].OldDriveColor = SignalDataStru[bufIndex].PreDriveColor;
		}
		else
		{
			/*�����κδ���*/
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
* ���������� ��ȡָ��ID�źŻ���˿��˿��־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��˿��־        
*/
UINT8 GetSignalDsBreakFlag(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DsBreakFlag;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ�����������ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������ʼ���ں�        
*/
UINT32 GetSignalDriveOpenStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveSigOpenStartCycNum;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ������ر���ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ر���ʼ���ں�        
*/
UINT32 GetSignalDriveCloseStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveSigCloseStartCycNum;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ��������ź���ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ź���ʼ���ں�        
*/
UINT32 GetSignalDriveNewSigStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DriveNewSigStartCycNum;
	}

	return reVal;
}

/*
* ���������� ��ȡָ��ID�źŻ������źŻ���ɫ�仯֮ǰ,�����źŻ���ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalOldDriveColor(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].OldDriveColor;
	}

	return reVal;
}

/*
* ���������� ����źŻ���ʾ��ɫ��Ч
* ����˵���� const UINT8 showColor, �źŻ���ʾ��ɫ
* ����ֵ  �� 0: ��ʾ��Ч
*			 1: ��ʾ��Ч      
*/
UINT8 CheckSignalShowColorValid(const UINT8 showColor)
{
	UINT8 reVal = CI_ERROR;

    if ((SIGNAL_SHOWCOLOUR_RED == showColor) || (SIGNAL_SHOWCOLOUR_GREEN == showColor) || (SIGNAL_SHOWCOLOUR_YELLOW == showColor)
        || (SIGNAL_SHOWCOLOUR_REDYELLOW == showColor) || (SIGNAL_SHOWCOLOUR_WHITE == showColor) || (SIGNAL_SHOWCOLOUR_REDCLOSE == showColor) 
        || (SIGNAL_SHOWCOLOUR_GREENCLOSE == showColor) || (SIGNAL_SHOWCOLOUR_YELLOWCLOSE == showColor) || (SIGNAL_SHOWCOLOUR_RED_WHITE == showColor))
    {
        reVal = CI_SUCCESS;  /*�źŻ���ʾ��Ч*/
    }
	return reVal;
}


/*
* ���������� ����źŻ�������ɫ��Ч
* ����˵���� const UINT8 drvColor, �źŻ�������ɫ
* ����ֵ  �� 0: ������Ч
*			 1: ������Ч      
*/
UINT8 CheckSignalDriveColorValid(const UINT8 drvColor)
{
	UINT8 reVal = CI_ERROR;


    if ((SIGNAL_DRIVECOLOUR_RED == drvColor) || (SIGNAL_DRIVECOLOUR_YELLOW == drvColor) || (SIGNAL_DRIVECOLOUR_REDYELLOW == drvColor)
        || (SIGNAL_DRIVECOLOUR_GREEN == drvColor) || (SIGNAL_DRIVECOLOUR_YELLOWCLOSE == drvColor) || (SIGNAL_DRIVECOLOUR_REDCLOSE == drvColor) 
        || (SIGNAL_DRIVECOLOUR_GREENCLOSE == drvColor) || (SIGNAL_DRIVECOLOUR_WHITE == drvColor)|| (SIGNAL_DRIVECOLOUR_RED_WHITE == drvColor)
		||(SIGNAL_DRIVECOLOUR_NULL == drvColor))
    {
        reVal = CI_SUCCESS; /*�źŻ�������Ч*/
    }

	return reVal;
}


/*
* ���������� ��ȡͨ�ų���ѹͨ���źŻ�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
*/
UINT8 GetSignalCrossState(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
	
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
* ���������� ����ͨ�ų���ѹͨ���źŻ�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ����ʧ��
*			>0: ���óɹ�      
*/
UINT8 SetSignalCrossState(const UINT16 signalId, const UINT8 signalCrossState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_CROSS_STATE_NO == signalCrossState) || (SIGNAL_CROSS_STATE_YES == signalCrossState))
		{
			SignalDataStru[bufIndex].SignalCrossState = signalCrossState;  /*����ͨ�ų���ѹͨ���źŻ�״̬*/
			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}

/*
* ���������� ��ȡ�źŻ���ɫ���Źر�״̬
* ����˵���� UINT8 signalColor
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
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
    { /*�̵�||�Ƶ�||��Ƶ�||�׵�||����||����||��׵�*/

        reVal = SIGNAL_OPEN;  /*�źſ���*/
    }
    return reVal;
}

/*
* ���������� ��ȡ���������źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
*/
UINT8 GetSignalYLockOpenState(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;
    
    bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
    
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
* ���������� �������������źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ״̬
* ����ֵ  �� 0: ����ʧ��
*			>0: ���óɹ�      
*/
UINT8 SetSignalYLockOpenState(const UINT16 signalId, const UINT8 setState)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 reVal = CI_ERROR;
    
    bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/
    
    if (bufIndex < SIGNAL_SUM_MAX)
    {
        if ((SIGNAL_OPEN == setState) && 
            (SignalDataStru[bufIndex].SignalLeadProperty == SIGNAL_LEAD_PROPERTY_YES) &&
            (GetSignalOpenCloseState(GetSignalCurColor(signalId)) == SIGNAL_CLOSE)
            )
        {
            /*�źſ����� �� �źŻ���ǰ�ر�*/
            SignalDataStru[bufIndex].SignalYLockOpenState = setState;  /*����״̬*/
            reVal = CI_SUCCESS;
        }
        else if (SIGNAL_CLOSE == setState)
        {
            SignalDataStru[bufIndex].SignalYLockOpenState = setState;  /*����״̬*/
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
* ���������� ��ȡָ��ID�źŻ��۳���־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �۳���־      
*/
UINT8 GetSignalDetainFlag(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        if (SignalDataStru[bufIndex].AtsDetainFlag == SIGNAL_DETAIN_NO)
        {/*ATSδ�۳�*/

            if (SignalDataStru[bufIndex].XiandiDetainFlag == SIGNAL_DETAIN_NO)
            {/*�ֵ�δ�۳�*/

                reVal = SIGNAL_DETAIN_NO;/*�����ֵء�ATS��δ�۳�*/
            }
            else
            {
                /*�ֵؿ۳�*/
                reVal = SIGNAL_DETAIN_XIANDI;/*�����ֵؿ۳���ATSδ�۳�*/
            }
        }
        else
        {/*ATS�۳�*/

            if (SignalDataStru[bufIndex].XiandiDetainFlag == SIGNAL_DETAIN_NO)
            {/*�ֵ�δ�۳�*/
                
                reVal = SIGNAL_DETAIN_ATS;/*�����ֵ�δ�۳���ATS���۳�*/
            }
            else
            {
                /*�ֵؿ۳�*/
                reVal = SIGNAL_DETAIN_YES;/*�����ֵؿ۳���ATS���۳�*/
            }
        }
	}
	return reVal;
}

/*
* ���������� ����ָ��ID�źŻ��۳�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainState(const UINT16 signalId, const UINT8 setState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if (setState == SIGNAL_DETAIN_YES)
		{/*ATS���ֵؾ��۳�*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_YES;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_NO)
		{/*ATS���ֵؾ�δ�۳�*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_NO;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_XIANDI)
		{/*�ֵذ���۳�*/

			SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
			SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
			reVal = CI_SUCCESS;
		}
		else if (setState == SIGNAL_DETAIN_ATS)
		{/*ATS����۳�*/

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
* ���������� ����ָ��ID�źŻ��۳���־
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainFlag(const UINT16 signalId, const UINT8 setState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
        if (setState == SIGNAL_DETAIN_ATS)
        {/*ats����۳�*/

            SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_YES;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_ATS_NO)
        {/*atsȡ���۳�*/

            SignalDataStru[bufIndex].AtsDetainFlag = SIGNAL_DETAIN_NO;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_XIANDI)
        {/*�ֵذ���۳�*/

            SignalDataStru[bufIndex].XiandiDetainFlag = SIGNAL_DETAIN_YES;
            reVal = CI_SUCCESS;
        }
        else if (setState == SIGNAL_DETAIN_XIANDI_NO)
        {/*�ֵ�ȡ���۳�*/

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
* ���������� ��ȡ�Զ��źŶ�Ӧ�Ľ�·ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ�ID      
*/
UINT16 GetSignalAutoRouteId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 returnValue = CI_ERROR;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡָ���źŻ���������*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		returnValue = SignalDataStru[bufIndex].AutoRouteId;
	}

	return returnValue;
}

/*
* ���������� �����Զ��źŶ�Ӧ���Զ��źŻ�ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalAutoRouteId(const UINT16 signalId,const UINT16 routeId)
{
	UINT8 ret = CI_SUCCESS;
	UINT16 bufIndex = 0;  /*�����±�*/
	
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡָ���źŻ���������*/
	
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
* ���������� ��ȡָ��ID�źŻ���ʱ�������ñ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʱ��������״̬      
*/
UINT8 GetSignalLinShiXianSuSetFlag(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8  funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

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
* ���������� ����ָ��ID�źŻ���ʱ���ٱ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalLinShiXianSuSetFlag(const UINT16 signalId, const UINT8 linShiXianSuSetState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if ((SIGNAL_LINSHIXIANSUSET_STATE_NO == linShiXianSuSetState) ||
            (SIGNAL_LINSHIXIANSUSET_STATE_YES == linShiXianSuSetState) ||
            (SIGNAL_LINSHIXIANSUSET_STATE_INVALID == linShiXianSuSetState))
		{
			SignalDataStru[bufIndex].LinShiXianSuSetFlag = linShiXianSuSetState;  /*�źŻ���ʱ���ٱ�־*/

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
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/
	
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
* ���������� ����ָ��ID�źŻ��쳣�رձ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
			 const UINT8 curColor, ��ǰ��ʾ��ɫ
			 const UINT8 setState����ǰ����״̬
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �쳣��־      
*/
UINT8 SetSignalPLockCommand(const UINT16 signalId, const UINT8 pLockCommand)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;
	
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/
	
	if (bufIndex < SIGNAL_SUM_MAX)
	{
		SignalDataStru[bufIndex].PLockCommand = pLockCommand;  /*�߼�����֮ǰռ��״̬*/

		funRetVal = CI_SUCCESS;

	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;	
}

/*
* ���������� ��ȡ�źŻ�������ͨ����������Ч��
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������Ч������
*/
UINT8 GetSignalProSecValid(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

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
* ���������� �����źŻ�������ͨ����������Ч��
* ����˵����
const UINT16 signalId, �źŻ�ID
const UINT8 proSecValid:����������Ч��
* ����ֵ:
*		0: ����ʧ��
*		1: ���óɹ�
*/
UINT8 SetSignalProSecValid(const UINT16 signalId, const UINT8 proSecValid)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

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
* ���������� �����źŻ���ɫ�Ѿ����ţ�������Ŀ�������������ɫ����¼�ù���̬����ʼ����
* ����˵����
	const UINT16 signalId, �źŻ�ID
	const UINT32 cycNum:��ʼ���ں�
* ����ֵ:
*		0: ����ʧ��
*		1: ���óɹ�
*/
UINT8 SetSignalDiffColorOpenStartCycNum(const UINT16 signalId,const UINT32 cycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

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
* ���������� ��ȡ�źŻ���ɫ�Ѿ����ţ�������Ŀ�������������ɫ����¼�ù���̬����ʼ����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʼ���ں�        
*/
UINT32 GetSignalDiffColorOpenStartCycNum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 reVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		reVal = SignalDataStru[bufIndex].DiffColorOpenStartCycNum;
	}

	return reVal;
}