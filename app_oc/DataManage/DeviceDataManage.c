/********************************************************
* 
* �� �� ���� DeviceDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-09-21
* ��    �ߣ� ������
* ���������� �豸�����ݹ��� 
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 


#include "DeviceDataManage.h"
#include "../ParsePack/CiInterfaceDefine.h"

UINT8 AllSignalBrightFlag = CI_ALL_SIGNAL_BRIGHT_NO;					                  /*ȫվ���Ʊ�־*/

UINT8 AllSwitchYLockFlag = CI_ALL_SWITCH_YLOCK_NO;						                  /*ȫվ����������־*/

UINT8 StartFLockFlag = CI_START_FLOCK_YES;								                  /*ȫվ������־*/

UINT8 StartLockFlag = CI_START_LOCK_YES;								                  /*�ϵ����ձ�־*/

UINT8 LinShiXianSuCancelButtonIsValidFlag = CI_LINSHIXIANSU_CANCEL_BUTTON_VALID;          /*��ʱ�����˹������־*/
 
UINT8 LabourCancelLinShiXianState = LINSHIXIANSU_COMM_BAD_STATE;                          /*�˹�ȡ����ʱ����Ǩ��״̬*/

UINT16 JkjRelayId = 0;													                  /*Ӧ���̵̼���ID*/

UINT8 PlatOverFlag = CI_OVER_NO;										                  /*ƽ̨崻���־*/

UINT8 ciMode = CI_MODE_JJ_LOCAL;										                  /* ����վ�� */

UINT8 ciTrainTestStatus = CI_TRAIN_TEST_OFF;

UINT8 comm_Train_Test_SQAJ_SCX_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*�Գ����복���β���ͨ�Žӿ�ʱ�Գ���SQAJ״̬*/
UINT8 comm_Train_Test_YSKJ_SCX_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*�Գ����복���β���ͨ�Žӿ�ʱ�Գ���YSKJ״̬*/
UINT8 comm_Train_Test_SQJ_CLD_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*�Գ����복���β���ͨ�Žӿ�ʱ������SQJ״̬*/
UINT8 comm_Train_Test_TYSJ_CLD_State = COMM_TRAIN_TEST_DEV_STATE_INVALID;				/*�Գ����복���β���ͨ�Žӿ�ʱ������TYSJ״̬*/

UINT8 ZCToCIAutoRtnIndict_Mask_Flash_State = ZCTOCIAUTORTN_INDICT_MASK_INVALID;			/*ZC��CI�������۷�������״̬*/
UINT8 ZCToCIAutoRtnIndict_Mask_Steady_State = ZCTOCIAUTORTN_INDICT_MASK_INVALID;		/*ZC��CI�������۷����ȵ�״̬*/
UINT8 autoTriggerFlag = AUTO_TRIGGER_SET_NO;                                             /*�Զ�������־��ʼ��Ϊδ����*/
/******************************������־���ݹ���****************************/

/*
* ���������� ��ȡ�ϵ������־
* ����˵���� void
* ����ֵ  ��CI_START_LOCK_NO    
			CI_START_LOCK_YES
            CI_START_LOCK_ERROR
*/
UINT8 GetStartLockFlag(void)
{
	return StartLockFlag;
}

/*
* ���������� ��ȡȫվ���Ʊ�־
* ����˵���� void
* ����ֵ  ��CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
UINT8 GetAllSignalBrightFlag(void)
{
	return AllSignalBrightFlag;
}


/*
* ���������� ����ȫվ���Ʊ�־
* ����˵���� void
* ����ֵ  ��CI_ALL_SIGNAL_BRIGHT_NO   
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
        /*������*/
    }
}

/*
* ���������� ��ȡ����ģʽ��־
* ����˵���� void
* ����ֵ  ������ģʽ 0x55: վ�� 0xAA:�п� 0x71:����̨ 0xFF:����վ��
*/
UINT8 GetCiModeFlag(void)
{
	return ciMode;
}

/*
* ���������� ��������ģʽ��־
* ����˵���� ����ģʽ 0x55: վ�� 0xAA:�п� 0x71:����̨ 0xFF:����վ��
* ����ֵ  ��void
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
		/*������*/
	}
}