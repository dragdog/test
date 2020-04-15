/********************************************************
*                                                                                                            
* �� �� ���� SystemParameterManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-21
* ��    �ߣ� ������
* ���������� ϵͳ��������  
* �޸ļ�¼��   
*
********************************************************/ 

#include "SystemParameterManage.h"



UINT8 LocalOcId = 0;                        /*����OCID*/
UINT16 ItsId = 0;                           /*�뱾OCͨ�ŵ�ITSID*/

SystemParameterStruct SystemParameterStru;	/*ϵͳ�����ṹ��*/

DquCiFuncSwitchStruct CiFuncSwitConfStru;/*�������ܿ������ýṹ��*/

UINT32 StartUnlockCount = 0;                   /*�ϵ�������������� (��ʼ���ֶ�)*/
UINT32 RouteXuanPaiCount = 0;                  /*��·ѡ�ų��������� (��ʼ���ֶ�)*/
UINT32 RouteLockCount = 0;                     /*��·���ճ��������� (��ʼ���ֶ�)*/
UINT32 OverlapLockCount = 0;                   /*�����������ճ��������� (��ʼ���ֶ�)*/
UINT32 RouteOpenSignalCount = 0;               /*��·���ų���ʱ��(��λ:s) (�����ֶ�)*/
UINT32 BlocRouteDelayCount = 0;                /*��ʽ��·��ʱ���������� (��ʼ���ֶ�)*/
UINT32 CbtcRouteDelayCount = 0;                /*CBTC��·��ʱ���������� (��ʼ���ֶ�)*/
UINT32 LeadRouteFaultOpenCount = 0;            /*������·�ڷ����ι����źſ��ų��������� (��ʼ���ֶ�)*/
UINT32 SecDelayUnlockCount = 0;                /*������ʱ���������� (��ʼ���ֶ�)*/
UINT32 SignalOpenCount = 0;                    /*�źſ��ų��������� (��ʼ���ֶ�)*/
UINT32 SignalCloseCount = 0;                   /*�źŹرճ��������� (��ʼ���ֶ�)*/
UINT32 SwitchMoveCount = 0;                    /*���������������� (��ʼ���ֶ�)*/
UINT32 SwitchDriveCount = 0;                   /*������������������ (��ʼ���ֶ�)*/
UINT32 ProtectSecUnlockCount = 0;              /*�������ν���ͣ�������� (��ʼ���ֶ�)*/
UINT32 OccupyToIdleCount = 0;                  /*ռ�ñ���м�ʱ������ (��ʼ���ֶ�)*/
UINT32 FaultUnlockCount = 0;                   /*�׹�����ʽ��ʱ������ (��ʼ���ֶ�)*/
UINT32 SectionResetCount = 0;                  /*���Ḵλ��ʱ������ (��ʼ���ֶ�)*/
UINT32 NonRouteXuanPaiCount = 0;               /*�ǽ�·ѡ���������� (��ʼ���ֶ�)*/
UINT32 NonRouteDelayUnlockCount = 0;           /*�ǽ�·��ʱ������������ (��ʼ���ֶ�)*/
UINT32 SwitchDriveCuoFengCount = 0;            /*��·ѡ�ŵ��������� (��ʼ���ֶ�)*/
UINT32 ZuheRouteXuanPaiCount = 0;              /*��Ͻ�·ѡ������ (��ʼ���ֶ�)*/
UINT32 CiAndZcTransmitBadEnsureCount = 0;      /*������ZCͨ���ж�ȷ������(��ʼ���ֶ�)*/
UINT32 CiAndCiTransmitBadEnsureCount = 0;      /*����������ͨ���ж�ȷ������(��ʼ���ֶ�)*/
UINT32 CiAndVobcTransmitBadEnsureCount = 0;    /*������Vobcͨ���ж�ȷ������(��ʼ���ֶ�)*/
UINT32 CiAndPsdTransmitBadEnsureCount = 0;     /*������PSDͨ���ж�ȷ������(��ʼ���ֶ�)*/
UINT32 WaitZCResponseCount = 0;                 /*�ȴ�ZC����ʱ�䣨����رճ����ţ�(��ʼ���ֶ�)*/
UINT32 SecJumpLockDelayUnlockCount = 0;			/*������Ծ������ʱ��������������ʼ���ֶΣ�*/
UINT32 GaragedoorOperateCount = 0;              /*�����Ŷ�����ʱ����������ʼ���ֶΣ�*/
UINT32 CtProtectSecUnlockCount = 0;				 /*ͨ�ų�ռ�÷������ν���ͣ�������� (��ʼ���ֶ�)*/
UINT32 SingleSwitchMoveCount = 0;				 /*���쵥�������������������� (��ʼ���ֶ�)*/
UINT32 ThreeSwitchMoveCount = 0;				 /*�������������������������� (��ʼ���ֶ�)*/
UINT32 FiveSwitchMoveCount = 0;				   	 /*�����忪������������������ (��ʼ���ֶ�)*/
UINT32 gapDetectTimeCount = 0;				   	 /*��϶̽��ʱ�䳬�������� (��ʼ���ֶ�)*/

UINT32	VobcOcCommunicationErrorCount = 0;/*VOBC-OCͨ�Ź���ȷ������*/
UINT32	TmcOcCommunicationErrorCount = 0;/*TMC-OCͨ�Ź���ȷ������*/
UINT32	AtsOcCommunicationErrorCount = 0;/*ATS-OCͨ�Ź���ȷ������*/
UINT32	OcPsdCommunicationErrorCount = 0;/*OC-PSDͨ�Ź���ȷ������*/

/*
* ���������� ϵͳ����ʱ��ת������
* ����˵���� void    
* ����ֵ  �� void      
*/
void SystemParaTimeToCycleCount(void)
{
	UINT32 appCycleTime = SystemParameterStru.AppCycleTime;

	StartUnlockCount = TimeToCycleCount(SystemParameterStru.StartUnlockTime * 1000, appCycleTime);                           /*�ϵ�������������� (��ʼ���ֶ�)*/
	RouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.RouteXuanPaiTime * 1000, appCycleTime);                         /*��·ѡ�ų��������� (��ʼ���ֶ�)*/
	RouteLockCount = TimeToCycleCount(SystemParameterStru.RouteLockTime * 1000, appCycleTime);                               /*��·���ճ��������� (��ʼ���ֶ�)*/
	OverlapLockCount = TimeToCycleCount(SystemParameterStru.OverlapLockTime * 1000, appCycleTime);                           /*�����������ճ��������� (��ʼ���ֶ�)*/
	RouteOpenSignalCount = TimeToCycleCount(SystemParameterStru.RouteOpenSigTime * 1000, appCycleTime);                      /*��·���ų���ʱ��(��λ:s) (�����ֶ�)*/
	BlocRouteDelayCount = TimeToCycleCount(SystemParameterStru.BlocRouteDelayTime * 1000, appCycleTime);                     /*��ʽ��·��ʱ���������� (��ʼ���ֶ�)*/
	CbtcRouteDelayCount = TimeToCycleCount(SystemParameterStru.CbtcRouteDelayTime * 1000, appCycleTime);                     /*CBTC��·��ʱ���������� (��ʼ���ֶ�)*/
	LeadRouteFaultOpenCount = TimeToCycleCount(SystemParameterStru.LeadRouteFaultOpenTime * 1000, appCycleTime);             /*������·�ڷ����ι����źſ��ų��������� (��ʼ���ֶ�)*/
	SecDelayUnlockCount = TimeToCycleCount(SystemParameterStru.SecDelayUnlockTime * 1000, appCycleTime);                     /*������ʱ���������� (��ʼ���ֶ�)*/
	SignalOpenCount = TimeToCycleCount(SystemParameterStru.SignalOpenTime * 1000, appCycleTime);                             /*�źſ��ų��������� (��ʼ���ֶ�)*/
	SignalCloseCount = TimeToCycleCount(SystemParameterStru.SignalCloseTime * 1000, appCycleTime);                           /*�źŹرճ��������� (��ʼ���ֶ�)*/
	SwitchMoveCount = TimeToCycleCount(SystemParameterStru.SwitchMoveTime * 1000, appCycleTime);                             /*���������������� (��ʼ���ֶ�)*/
	SwitchDriveCount = TimeToCycleCount(SystemParameterStru.SwitchDriveTime * 1000, appCycleTime);                           /*������������������ (��ʼ���ֶ�)*/
	ProtectSecUnlockCount = TimeToCycleCount(SystemParameterStru.ProtectSecUnlockTime * 1000, appCycleTime);                 /*�������ν���ͣ�������� (��ʼ���ֶ�)*/
	OccupyToIdleCount = TimeToCycleCount(SystemParameterStru.OccupyToIdleTime * 1000, appCycleTime);                         /*ռ�ñ���м�ʱ������ (��ʼ���ֶ�)*/
    FaultUnlockCount = TimeToCycleCount(SystemParameterStru.FaultUnlockTime * 1000, appCycleTime);                           /*�׹�����ʽ��ʱ������ (��ʼ���ֶ�)*/
    SectionResetCount = TimeToCycleCount(SystemParameterStru.SectionResetTime * 1000, appCycleTime);                         /*���Ḵλ��ʱ������ (��ʼ���ֶ�)*/
    NonRouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.NonRouteXuanPaiTime * 1000, appCycleTime);                   /*�ǽ�·ѡ���������� (��ʼ���ֶ�)*/
    NonRouteDelayUnlockCount = TimeToCycleCount(SystemParameterStru.NonRouteDelayUnlockTime * 1000, appCycleTime);           /*�ǽ�·��ʱ������������ (��ʼ���ֶ�)*/
    SwitchDriveCuoFengCount = TimeToCycleCount(SystemParameterStru.SwitchDriveCuoFengTime * 1000, appCycleTime);             /*��·ѡ�ŵ��������� (��ʼ���ֶ�)*/
    ZuheRouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.ZuheRouteXuanPaiTime * 1000, appCycleTime);                 /*��Ͻ�·ѡ������ (��ʼ���ֶ�)*/
    CiAndZcTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndZcTransmitBadEnsureTime, appCycleTime);        /*������ZCͨ���ж�ȷ������(��ʼ���ֶ�)*/
    CiAndCiTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndCiTransmitBadEnsureTime, appCycleTime);        /*����������ͨ���ж�ȷ������(��ʼ���ֶ�)*/
    CiAndVobcTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndVobcTransmitBadEnsureTime, appCycleTime);    /*������Vobcͨ���ж�ȷ������(��ʼ���ֶ�)*/
	CiAndPsdTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndPsdTransmitBadEnsureTime,appCycleTime);       /*������Psdͨ���ж�ȷ������(��ʼ���ֶ�)*/
    WaitZCResponseCount = TimeToCycleCount(SystemParameterStru.WaitZCResponseTime,appCycleTime);							 /*�ȴ�ZC����ʱ�䣨����رճ����ţ�(��ʼ���ֶ�)*/
	SecJumpLockDelayUnlockCount = TimeToCycleCount(SystemParameterStru.SecJumpLockDelayUnlockTime * 1000,appCycleTime);		 /*������Ծ������ʱ����������(��ʼ���ֶ�)*/
    GaragedoorOperateCount = TimeToCycleCount(SystemParameterStru.GaragedoorOperateTime * 1000,appCycleTime);                /*�����Ŷ�����ʱ����������ʼ���ֶΣ�*/
	CtProtectSecUnlockCount = TimeToCycleCount(SystemParameterStru.CtProtectSecUnlockTime * 1000, appCycleTime);              /*ͨ�ų�ռ�ý�·�����һ�������µķ������ν���ͣ�������� (��ʼ���ֶ�)*/
	SingleSwitchMoveCount = TimeToCycleCount(SystemParameterStru.SingleSwitchMoveTime * 1000, appCycleTime);				  /*���쵥�������������������� (��ʼ���ֶ�)*/
	ThreeSwitchMoveCount = TimeToCycleCount(SystemParameterStru.ThreeSwitchMoveTime * 1000, appCycleTime);					  /*���쵥�������������������� (��ʼ���ֶ�)*/
	FiveSwitchMoveCount = TimeToCycleCount(SystemParameterStru.FiveSwitchMoveTime * 1000, appCycleTime);					  /*���쵥�������������������� (��ʼ���ֶ�)*/
	gapDetectTimeCount = TimeToCycleCount(SystemParameterStru.gapDetectTime * 1000, appCycleTime);							  /*��϶̽�ⳬ�������� (��ʼ���ֶ�)*/

	VobcOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.VobcOcCommunicationErrorTime, appCycleTime);;/*VOBC-OCͨ�Ź���ȷ������(��ʼ���ֶ�)*/
	TmcOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.TmcOcCommunicationErrorTime, appCycleTime);;/*TMC-OCͨ�Ź���ȷ������(��ʼ���ֶ�)*/
	AtsOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.ItsOcCommunicationErrorTime, appCycleTime);;/*ATS-OCͨ�Ź���ȷ������(��ʼ���ֶ�)*/
	OcPsdCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.OcPsdCommunicationErrorTime, appCycleTime);;/*OC-PSDͨ�Ź���ȷ������(��ʼ���ֶ�)*/
}

/*
* ���������� ʱ��ת������
* ����˵���� const UINT32 deviceTime, �豸ʱ��
*			 const UINT32 appTime, Ӧ��ʱ��   
* ����ֵ  �� ������      
*/
UINT32 TimeToCycleCount(const UINT32 deviceTime, const UINT32 appTime)
{
	UINT32 rev = 0;/*��*/

    /*appTime��Ϊ����*/
    if (appTime == 0)
    {
        /*��������Ϊ0*/

    }
    else
    {
        rev = deviceTime / appTime;
        
        if (deviceTime % appTime != 0)
        {/*��������*/
            
            rev++;
	    }
    }	

	return rev;
}

/*
* ���������� ��ȡ�������ں�
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT32 GetSystemParaAppCycleTime(void)
{
    return SystemParameterStru.AppCycleTime;
}

/*
* ���������� ��ȡ��������ID
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT8 GetSystemParaLocalOcId(void)
{
    return LocalOcId;
}


/*
* ���������� ��ȡ��������
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT8 GetSystemParaCiSum(void)
{
    return SystemParameterStru.CiSum;
}


/*
* ���������� ��ȡϵͳ�����ϵ��������������
* ����˵���� void    
* ����ֵ  �� �ϵ��������������      
*/
UINT32 GetSystemParaStartUnlockCycleCount(void)
{
    return StartUnlockCount;
}


/*
* ���������� ��ȡϵͳ������·ѡ�ų���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteXuanPaiCycleCount(void)
{
    return RouteXuanPaiCount;
}

/*
* ���������� ��ȡϵͳ������·���ճ���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteLockCycleCount(void)
{
    return RouteLockCount;
}

/*
* ���������� ��ȡϵͳ���������������ճ���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaOverlapLockCycleCount(void)
{
    return OverlapLockCount;
}

/*
* ���������� ��ȡϵͳ������·�����źų���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteOpenSignalCycleCount(void)
{
    return RouteOpenSignalCount;
}

/*
* ���������� ��ȡϵͳ������ʽ��·��ʱ����������
* ����˵���� void    
* ����ֵ  �� ��ʽ��·��ʱ����������      
*/
UINT32 GetSystemParaBlocRouteDelayCycleCount(void)
{
    return BlocRouteDelayCount;
}


/*
* ���������� ��ȡϵͳ����CBTC��·��ʱ����������
* ����˵���� void    
* ����ֵ  �� CBTC��·��ʱ����������      
*/
UINT32 GetSystemParaCbtcRouteDelayCycleCount(void)
{
    return CbtcRouteDelayCount;
}


/*
* ���������� ��ȡϵͳ����������·�ڷ����ι����źſ��ų���������
* ����˵���� void    
* ����ֵ  �� ������·�ڷ����ι����źſ��ų���������      
*/
UINT32 GetSystemParaLeadFaultOpenCycleCount(void)
{
    return LeadRouteFaultOpenCount;
}


/*
* ���������� ��ȡϵͳ����������ʱ����������
* ����˵���� void    
* ����ֵ  �� ������ʱ����������      
*/
UINT32 GetSystemParaSecDelayUnlockCycleCount(void)
{
    return SecDelayUnlockCount;
}


/*
* ���������� ��ȡϵͳ�����źſ��ų���������
* ����˵���� void    
* ����ֵ  �� �źſ��ų���������      
*/
UINT32 GetSystemParaSignalOpenCycleCount(void)
{
    return SignalOpenCount;
}


/*
* ���������� ��ȡϵͳ�����źŹرճ���������
* ����˵���� void    
* ����ֵ  �� �źŹرճ���������      
*/
UINT32 GetSystemParaSignalCloseCycleCount(void)
{
    return SignalCloseCount;
}


/*
* ���������� ��ȡϵͳ����������������
* ����˵���� void    
* ����ֵ  �� ������������      
*/
UINT32 GetSystemParaSwitchMoveCycleCount(void)
{
    return SwitchMoveCount;
}


/*
* ���������� ��ȡϵͳ������������������
* ����˵���� void    
* ����ֵ  �� ��������������      
*/
UINT32 GetSystemParaSwitchDriveCycleCount(void)
{
    return SwitchDriveCount;
}


/*
* ���������� ��ȡϵͳ�����������ν���ͣ��������
* ����˵���� void    
* ����ֵ  �� �������ν���ͣ��������
*/
UINT32 GetSystemParaProtectSecUnlockCycleCount(void)
{
    return ProtectSecUnlockCount;
}


/*
* ���������� ��ȡCT��ռ�ý�·�����һ����������µĽ���ͣ��������
* ����˵���� void   
* ����ֵ  �� ͨ�ų�ռ�÷������ν���ͣ��������
*/
UINT32 GetSystemParaCtProtectSecUnlockCycleCount(void)
{
    return CtProtectSecUnlockCount;
}

/*
* ���������� ��ȡϵͳ����ռ�ñ���м�ʱ������
* ����˵���� void    
* ����ֵ  �� ռ�ñ���м�ʱ������      
*/
UINT32 GetSystemParaOccuToIdleCycleCount(void)
{
    return OccupyToIdleCount;
}

/*
* ���������� ��ȡϵͳ�����׹�����ʽ��ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaFaultUnlockCycleCount(void)
{
    return FaultUnlockCount;
}

/*
* ���������� ��ȡϵͳ�������Ḵλ��ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaSectionResetCount(void)
{
    return SectionResetCount;
}

/*
* ���������� ʱ�������ʱ���
* ����˵���� UINT32 StartCount ��ʼʱ��
*			 UINT32 MaxCount   ���ʱ�� 
* ����ֵ  �� CI_TIME_NO        δ��ʱ
*			 CI_TIME_IN        δ��ʱ          
*			 CI_TIME_OUT       ��ʱ
*/
UINT8 SystemOverTimeCheck(UINT32 StartCount, UINT32 MaxCount)
{
    UINT32 nowTime = 0;
    UINT32 subTime = 0;
	UINT8 retVal = CI_TIME_OUT;

    nowTime = Get2oo2CurCycleNum();

    if (StartCount == 0)
    {
        /*δ��ʱ*/
        retVal = CI_TIME_NO;
    }
    else
    {
        subTime = nowTime - StartCount;
        if ((subTime <= MaxCount) && (subTime >= 0))
        {
            /*δ��ʱ*/
            retVal = CI_TIME_IN;
        }
        else
        {
            /*��ʱ*/
            retVal = CI_TIME_OUT;
        }
    }
	return retVal;
}


/*
* ���������� ��ȡϵͳ�����ǽ�·ѡ����������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaNonRouteXuanPaiCount(void)
{
    return NonRouteXuanPaiCount;
}


/*
* ���������� ��ȡϵͳ�����ǽ�·��ʱ������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaNonRouteDelayUnlockCount(void)
{
    return NonRouteDelayUnlockCount;
}

/*
* ���������� ��ȡ��·ѡ�ŵ�������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaSwitchDriveCuoFengCount(void)
{
    return SwitchDriveCuoFengCount;
}


/*
* ���������� ��ȡ��Ͻ�·ѡ����������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaZuheRouteXuanPaiCount(void)
{
    return ZuheRouteXuanPaiCount;
}

/*
* ���������� ��ȡ������ZCͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndZcTransmitBadEnsureCount(void)
{
    return CiAndZcTransmitBadEnsureCount;
}

/*
* ���������� ��ȡ����������ͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndCiTransmitBadEnsureCount(void)
{
    return CiAndCiTransmitBadEnsureCount;
}

/*
* ���������� ��ȡ������Vobcͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndVobcTransmitBadEnsureCount(void)
{
    return CiAndVobcTransmitBadEnsureCount;
}

/*
* ���������� ��ȡ������PSDͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndPsdTransmitBadEnsureCount(void)
{
    return CiAndPsdTransmitBadEnsureCount;
}

/*
* ���������� ��ȡ����������
* ����˵���� void    
* ����ֵ  �� ����������    
*/
UINT8 GetLocalCiType(void)
{
    return SystemParameterStru.CiType;
}


/*
* ���������� ��ȡ�ȴ�ZC����ʱ��
* ����˵���� void    
* ����ֵ  �� �ȴ�ZC����ʱ��    
*/
UINT32 GetWaitZCResponseCount(void)
{
    return WaitZCResponseCount;
}

/*
* ���������� ��ȡϵͳ����������Ծ������ʱ����������
* ����˵���� void    
* ����ֵ  �� ������Ծ������ʱ����������      
*/
UINT32 GetSystemParaSecJumpLockDelayUnlockCycleCount(void)
{
	return SecJumpLockDelayUnlockCount;
}


/*
* ���������� ��ȡָ���������ܿ�������ֵ
* ����˵���� INT32 FunctionConfigIndex ָ���������ܿ�����������
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	���������ر��ѿ����ź�
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	�����źŻ���ر��ѿ����ź�
	CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF	��������ʱ����������
	CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF			��������ʱ��������
	
* ����ֵ  �� ָ���������ܿ�������ֵ0xAA��0x55��0xFF
*/
UINT8 GetCiFunctionSwitchConfig(UINT8 FunctionConfigIndex)
{
	UINT8 retVal = FUNC_SWITCH_UNDEF;

	if((CI_FUNCTION_SWITCH_SUM_MAX > FunctionConfigIndex) && (0 <= FunctionConfigIndex))
	{
		retVal = CiFuncSwitConfStru.wCiFuncSwitchBuf[FunctionConfigIndex];
	}
	else
	{
		retVal = FUNC_SWITCH_UNDEF;
	}
	
	return retVal;
}

/*
* ���������� ��ȡ����������ͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� ����������ͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetCiAndCiTransmitBadEnsureTime(void)
{
	return SystemParameterStru.CiAndCiTransmitBadEnsureTime;
}
/*
* ���������� ��ȡ������Vobcͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� ������Vobcͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetCiAndVobcTransmitBadEnsureTime(void)
{
	return SystemParameterStru.CiAndVobcTransmitBadEnsureTime;
}

/*
* ���������� ��ȡOC��IVOCͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� OC��IVOCͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetOcAndIvocTransmitBadEnsureTime(void)
{
	return SystemParameterStru.VobcOcCommunicationErrorTime;
}

/*
* ���������� ��ȡϵͳ���������Ŷ�����ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaGaragedoorOperateCount(void)
{
	return GaragedoorOperateCount;
}
/*
* ���������� ��ȡ���쵥��������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSingleSwitchMoveCount(void)
{
	return SingleSwitchMoveCount;
}
/*
* ���������� ��ȡ��������������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetThreeSwitchMoveCount(void)
{
	return ThreeSwitchMoveCount;
}
/*
* ���������� ��ȡ�����忪������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetFiveSwitchMoveCount(void)
{
	return FiveSwitchMoveCount;
}

/*
* ���������� ��ȡ��϶̽�ⳬ��������
* ����˵���� void    
* ����ֵ  �� ��϶̽�ⳬ��������      
*/
UINT32 GetGapDetectCount(void)
{
	return gapDetectTimeCount;
}
