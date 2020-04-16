/* ***********************************************************************
*
* �ļ���      ��  TypesDef.h
* ��Ȩ˵��    ��  �������ؿƼ��������ι�˾
* �汾��      ��  1.0
* ����ʱ��    ��  2016-6-13
* ����        ��  ��һ��
* ��������    ��  Ӧ�����ݽṹ����.
* ʹ��ע��    ��  
* �޸ļ�¼    ��  2016-6-13 			��һ��				  ����
                                                                      
*********************************************************************** */
#ifndef _TYPESDEF_H_
#define _TYPESDEF_H_
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "MacDefine.h"
#include "kcg_consts.h"
#include "dquVobcCommonStru.h"


/*================================�ṹ������===============================*/
/*  ����DSU���ݱ�ṹ����������͹���:                                     */
/*  UINT8:   ���ֱ�ʾƫ������������Ϣ�ı�����                              */
/*  UINT16�� ����ĸ��ֱ�������Ҫ��ID�š���š����κš������ȣ���          */
/*  UINT32:  �����豸�����ԡ�����״̬�ı�����                            */


#ifdef __cplusplus
extern "C" {
#endif

/*=============================�����ṹ��============================================*/



/*������Ϣ�����������ID��״̬*/
//typedef struct{
//	UCHAR   PointType;				/*��������ͣ����Կ��ǲ���*/
//	UINT16	PointId;				/*�����ID*/
//	UINT16	Lnk;					/*�ϰ����������κţ�D_ OBSTACLEL��*/
//	UINT32	Offset;					/*�ϰ�����������ƫ������D_ OBSTACLEO��*/
//	UCHAR	PointStatus;			/*�����״̬*/
//	UCHAR	PointExpectedStatus;	/*����Ӧ�õ�״̬*/
//}DQU_POINT_STATUS_STRU;



/*===========================վ̨��ȫ�����ݽṹ============================*/

/*վ̨��ȫ����Ϣ*/
struct PSDInfo
{
	UINT16  wPSDID;		                                /*վ̨��ȫ��ID*/
	UINT16  wCIOfOC;                                   /*վ̨��ȫ������OC*/
	UINT8  bPSDStatus_Now; 	                        /*վ̨��ȫ�ŵ�ǰ״̬*/
};
typedef struct PSDInfo struct_PSDInfo;                  /*վ̨��ȫ����Ϣ���ݽṹ����*/




/****************************************ʱ��ṹ��*****************************************/

/*  ������Ϣ */
typedef struct
{
	UINT8     Year;											/*  ��	 					 */
	UINT8     Month;										/*  ��	 					 */
	UINT8     Day;											/*  ��	 					 */
}DATE_STRU;

/*  ʱ����Ϣ */
typedef struct
{
	UINT8     Hour;											/*  Сʱ	 					 */
	UINT8     Minute;										/*  ��	 					 */
	UINT8     Second;										/*  ��	 					 */
}TIME_STRU;

/*  ʱ������	 */
typedef struct
{
    //CTCS_BOOL			stDateValid;							/*  ʱ����Ϣ��Ч״̬			 */
    DATE_STRU			stDate;								/*  ����						 */
    TIME_STRU			stTime;								/*  ʱ��						 */
    UINT32              dwCyclNum;                            /*������ں�*/
}CYCL_STRU;




/*  ***************************************OC�ṹ��*****************************************/
typedef	struct
{
	UINT16 wID_DA;                  /*�����ID*/
	UINT8  bDAStatus;               /*�����״̬*/
	/*0xaa:���С�0x55ռ��*/
	UINT8  bTrainDir;               /*��������˶���ķ���*/
	/*0xaa:���������÷����෴��0x55���������÷�����ͬ*/
}DAInfo_STRU;

typedef	struct
{
	UINT16 wID_CT;                  /*ͨ���г�ID*/
	UINT8  bContact_CT;              /*ͨ���г���������Ϣ*/
	                                /*0xaa:ɾ����0x55������0x1ά��,0x0δ�յ���Ϣ*/
	UINT8  bCycNum_LoseContact;                  /*OCδ�յ�ͨ���г���Ϣ��������*/
	UINT8  bIP_CT[4];                /*ͨ���г�dwIP��ַ*/
	UINT32     dwIP;                  /*ͨ���г�dwIP��ַ*/
	UINT32   dwCycle;                 /*�����г���Ϣ������*/
	UINT8    bDevPos;                   /*��ʾ�г��Ƿ���OC��Ͻ������*/
	UINT8  TrainMode;					/*�г�����ģʽ*/
	UINT8  PassFlag;				/*��ڼ��ͨ����־λ��55��ʾͨ����aa��ʾ��ͨ��*/
	UINT16 DetecAreaID;	/*�����ID*/
	UINT8  TrainDir;
	UINT16 TrainHeadLink;
	UINT32 TrainHeadOffset;
	UINT16 TrainTailLink;
	UINT32 TrainTailOffset;
}CT_STRU;

typedef	struct
{
	UINT16 wID_CT;                  /*ͨ���г�ID*/
	UINT8  bContact_CT;              /*ͨ���г���������Ϣ*/
	/*0xaa:ɾ����0x55������0x1ά��,0x0δ�յ���Ϣ*/
	UINT8  bCycNum_LoseContact;                  /*OCδ�յ�ͨ���г���Ϣ��������*/
	UINT8  bIP_CT[4];                /*ͨ���г�dwIP��ַ*/
	UINT32     dwIP;                  /*ͨ���г�dwIP��ַ*/
	UINT32   dwCycle;                 /*�����г���Ϣ������*/
	UINT8    bDevPos;                   /*��ʾ�г��Ƿ���OC��Ͻ������*/
	UINT8  UCTLineDir;
	UINT8  UCTType;        /*��ͨ���г����ͣ�01ΪRM��02Ϊʧȥͨ��*/
	UINT16 UCTLineHeadLink;
	UINT32 UCTLineHeadOffset;
	UINT16 UCTLineTailLink;
	UINT32 UCTLineTailOffset;
}UCT_STRU;

/*�������ṹ��*/
typedef	struct
{
	UINT16 wID_SLock;                  /*����������ID*/
	UINT8 bLEVEL_SLock;                 /*������Ȩ�޵ȼ�*/
	UINT8 bQ_SLock_SWITCH;              /*�������������εĵ���״̬ 0x55��ʾ��λ��0xaa��ʾ��λ*/
}SLock_STRU;

/*������߻������ṹ��*/
typedef	struct
{
	UINT16 wID_ULock;                  /*����������ID*/
	UINT8 bLEVEL_ULock;                 /*������Ȩ�޵ȼ�*/
	UINT8 bQ_ULock_SWITCH;              /*�������������εĵ���״̬,1����1δ��2����2λ*/
}ULock_STRU;

/*������״̬�ṹ��*/
typedef	struct
{
	UINT16 wID_PSD;
	UINT8 bQ_PSD;
}PSD_STRU;

/*��������״̬�ṹ��*/
typedef	struct
{
	UINT16 wID_AC;
	UINT8 bQ_AC;
}AC_STRU;

/*�����رսṹ��*/
typedef	struct
{
	UINT16 wID_EMP;
	UINT8 bQ_EMP;
}EMP_STRU;



/*����״̬�ṹ��*/
typedef	struct
{
	UINT16 wID_Switch;               /*����ID*/
	UINT8 bQ_Switch;                  /*����״̬*/
	UINT8 bLockQ_Switch;             /*����״̬���Ƿ����գ�*/
	UINT8 SwitchFault;				/*����״̬���Ƿ���ϣ�*/
	UINT8 bLockType;                  /*������*/
	UINT16 wID_ELock;                /*��ռ��ID*/
	UINT8 bLevel_ELock;               /*��ռ���ȼ�*/
	UINT8 bRQ_ELock_Switch;            /*��ռ��Ҫ�����״̬*/
	UINT8 bNum_SLock;                      /*����������*/
	SLock_STRU stSLock[MAX_SLock];     /*�������ṹ��*/

	UINT8 ChangeStatus;				/*����⶯״̬������������ڰᶯ�У��̶�ʱ���ڲ��������������ᶯ�У�0x55,δ�⶯��0xaa*/
	UINT8 SendQStatus;				/*���͵�������ʱ����Ҫ�ĵ���״̬0x55��λ��0xaa��λ*/
	UINT32 TimeHigh;
	UINT32 TimeLow;
	UINT32 CommandID;
	UINT32 SendCycle;				/*���͵�������������*/
	UINT32 CommandSourceID;			/*�����������Դ*/
}SWITCH_STRU;

/*�������״̬�ṹ��*/
typedef	struct
{
	UINT16 wID_Switch;               /*����ID*/
	UINT8 bQ_Switch;                  /*����״̬*/
	UINT8 bNum_ULock;                /*����������*/
	ULock_STRU stSLock[MAX_SLock];     /*��ʽ���ֻ������ṹ��*/
}MULTSWITCH_STRU;


typedef	struct
{
	UINT16 wID_Station;               /*ID*/
	UINT8 bQ_Button;                  /*״̬*/

}STATIONBUTTON_STRU;

//�źŻ�״̬
typedef	struct
{
	UINT16 wID_Signal;               /*ID*/
	UINT8 bQ_Signal;                  /*״̬*/
	UINT16 SourceID;				/*�������źŻ�����˭*/
	UINT16 SourceID_Old;				/*�������źŻ�����˭*/
	UINT16 SignalStatus_Need;		/*��Ҫ����״̬*/
	UINT16 SignalCtrlTime ;				/*�źŻ��������ʱ��*/
}SIGNAL_STRU;

/*��ʱ��������*/
//typedef	struct
//{
//	UINT8  TSRSpeed;                  /*��ʱ�����ٶ�*/
//	UINT16 TSRSLink;
//	UINT32 TSRSOff;
//	UINT8 TSRInterLinkNum;		
//	UINT16 TSRInterLinkID[MAX_LINK_NUM];
//	UINT16 TSRElink;
//	UINT32 TSREOff;
//	UINT8  TSRDir;
//	UINT8 Flag;				/*��ʱ������Ч�ԣ���OC����û����TMCͨ���ţ�����ʱ������Ч*/
//
//}TSR_STRU;

/*�����۷���ť�ṹ��*/
typedef	struct
{
	UINT16 wID_DRB;   /*�����۷���ťID*/
	UINT8 bQ_DRB;      /*�����۷���ť״̬*/
}DRB_STRU;
/*OC��������ṹ��*/
typedef	struct
{
	UINT16 wRegionID;               /*��������ID*/
	UINT16 wNum_CT;
	CT_STRU stCTMan[MAX_CT];              /*ͨ���г��ṹ��*/
	UINT8  bRegionBlockFlag;              /*���������־*/ 
}ManRegion_STRU;
/*�۳�����ṹ��*/
typedef	struct
{   
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wStationID;                    /*վ̨��ťID*/
	UINT16 wHoldTrainStatus;                /*�۳�����״̬:0x55Ϊ�۳���0xaaΪ��ͣ*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}
HoldTrain_CommandData_STRU;
/*��ͣ*/
typedef	struct
{   
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wStationID;                    /*վ̨��ťID*/
	UINT16 wJumpTrainStatus;                /*��ͣ����״̬0x55:ָ��վ̨��ͣ0xaa:ָ��վ̨ȡ����ͣ*/
	UINT16 wJumpTrainOrStation;             /*����Ϣ��������ͣ��Ϣ����Ը�վ̨�����г�������ĳ�ض��г�0x55:վ̨��ͣ 0xaa:�г���ͣ*/
    UINT16 wJumpTrainID;                    /*�����Ҫ��ͣ�ض��г������ֽڱ�ʾ�ض��г�ID*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}
JumpTrain_CommandData_STRU;
/*��ͣ��Ϣ�ṹ�壬�����ڲ��������������ݵ���*/
typedef	struct
{   

	UINT16 wStationID;                    /*վ̨ID*/
	UINT16 wJumpTrainNum;                  /*��ͣ�г����������Ϊ�㣬��ʾ��ͣ�����г�*/
	UINT16 wJumpTrainID[MAX_CT];                    /*�����Ҫ��ͣ�ض��г������ֽڱ�ʾ�ض��г�ID*/

}
JumpTrain_CommandData_Send_STRU;
/*ת�����豸��Ϣ�ṹ��*/
typedef	struct
{   

	UINT16 wGlobalID;                    /*�豸��ȫ���е�ID*/
	UINT16 wIDInOC;						 /*�豸��OC�ڵı��*/
	UINT16 wBelongOC;                    /*�豸����OC��ID*/
	UINT8  PointInOC[MAX_POINT_NUM];	/*�豸�Ŀ��Ƶ�λ*/
}
CONVERT_DEVICE_STRU;
/*ת������Ϣ�ṹ��*/
typedef	struct
{   

	UINT16 SwitchNum;								/*������ת�����е�����*/
	CONVERT_DEVICE_STRU SwitchConvertInfo[MAX_SWITCH];		/*������ת�����е�ת����ϵ*/
	UINT16 PSDNum;									/*��������ת�����е�����*/
	CONVERT_DEVICE_STRU PSDConvertInfo[MAX_PSD];				/*��������ת�����е�ת����ϵ*/
	UINT16 EMPNum;									/*����ͣ����ť��ת�����е�����*/
	CONVERT_DEVICE_STRU EMPConvertInfo[MAX_EMP];			/*����ͣ����ť��ת�����е�ת����ϵ*/
	UINT16 ACNum;								/*��������ת�����е�����*/
	CONVERT_DEVICE_STRU ACConvertInfo[MAX_AC];			/*��������ת�����е�ת����ϵ*/
	UINT16 MultSwitchNum;						/*���������ת�����е�����*/
	CONVERT_DEVICE_STRU MultSwitchConvertInfo[MAX_MULT_SWITCH];	/*���������ת�����е�ת����ϵ*/
	UINT16 DRBNum;						/*�����۷���ť��ת�����е�����*/
	CONVERT_DEVICE_STRU DRBConvertInfo[MAX_MULT_SWITCH];	/*�����۷���ť��ת�����е�ת����ϵ*/
	UINT16 SignalNum;						/*�źŻ���ת�����е�����*/
	CONVERT_DEVICE_STRU SignalConvertInfo[MAX_SIGNAL];	/*�źŻ���ת�����е�ת����ϵ*/
}
CONVERT_TABLE_STRU;

/*OC�ṹ��,����ýṹ���ȫ��ȫ�ֱ����������ת�����ձ�*/
typedef	struct
{
	UINT16  wID_OC;                  /*��ǰOC��ID*/
	UINT16  wNum_Switch;
	SWITCH_STRU stSwitch[MAX_SWITCH];  /*����״̬�ṹ��*/
	UINT16  wNum_PSD; 
	PSD_STRU stPSD[MAX_PSD];           /*������״̬�ṹ��*/
	UINT16 wNum_EMP;
	EMP_STRU stEMP[MAX_EMP];           /*�����رսṹ��*/
	UINT16 wNum_AC;
	AC_STRU stAC[MAX_AC];              /*��������״̬�ṹ��*/
	UINT16 wNum_DRB;
	DRB_STRU stDRB[MAX_AC];              /*�����۷���ť״̬�ṹ��*/
	UINT16 wNum_CT;						/*��Ҫͨ�ŵ��г�*/
	UINT16 wNum_MAN_CT;					/*�ڹ�Ͻ�����ڵ��г�*/
	CT_STRU stCT[MAX_CT];              /*ͨ���г��ṹ�� �ڲ�OCͨ����*/
	UINT16 wNum_LoseCommunicationTrain;
	UCT_STRU stLoseCommunicationTrain[MAX_CT];              /*��ʧ�г��ṹ�� �ڲ�OCͨ����*/

	UINT16 wNum_MAN_VT;					/*�ڹ�Ͻ�����ڵ�VBTC�г� ����*/
	CT_STRU stVT[MAX_CT];              /*VBTC�г��ṹ�� ����*/
	UINT16 wNum_UCT;						 /*UT�г��ṹ�� ����*/
	UCT_STRU stUT[MAX_CT];              /*UT�г��ṹ�� ����*/

	UINT8  bRegionBlockFlag;              /*���������־ 0x55��ʾ������0xaa��ʾ���*/ 
	UINT8  bRegionBlockPosition;              /*��ʾ���������������Ϣ*/ 
	UINT16 wNum_ManRegion;
	ManRegion_STRU stManRegion[MAX_ManRegion];     /*��������ṹ��*/
	UINT8 bWSLoseInputFlag;                   /*����δ�������ݵ�������*/
	UINT8 bTMCLoseInputFlag;                   /*TMC�������ݵ�������*/
	UINT8 bRRBfITS;								/*��TMC�Ͽ��������յ�����ITS�ķ��������Ϣ��־λ����ʱ�������*/
	UINT8 TMCInputFlag;							/*�յ���TMC����Ϣ��־λ��0x55��ʾ�ϵ��յ���TMC���ݣ�0xaa��ʾ�ϵ�δ�յ���TMC����*/						
	UINT16 wDetectAreaNum;              /*���������*/
	DAInfo_STRU stDAInfo[MAX_DA];               /*�������Ϣ�ṹ��*/
	UINT16 wCommandNum_HoldTrain;													 /*�۳��������*/
	HoldTrain_CommandData_STRU     stOCRevHoldTrainCmdDataStru[CMD_MAX];
	UINT16 wCommandNum_JumpTrain;													 /*��ͣ�������*/
	JumpTrain_CommandData_Send_STRU     stOCRevJumpTrainCmdDataStru[CMD_MAX];			 /*��ͣ��������*/
	UINT16 wNum_MultSwitch;                 /*������ߵ�����*/
	MULTSWITCH_STRU   stOCMultSWitchDataStru[MAX_SWITCH];			 /*�������״̬����*/
	UINT16 wNum_StationIn;
	STATIONBUTTON_STRU stStationInButton[MAX_EMP];
	UINT16 wNum_StationOut;
	STATIONBUTTON_STRU stStationOutButton[MAX_EMP];
	UINT16 wNum_Signal;								/*�źŻ�������*/
	SIGNAL_STRU stSignal[MAX_SIGNAL];
	UINT16 ElecPowerState;	/*�ϵ����״̬0x01:δ�յ��ϵ��������
								0x02:�յ��ϵ����һ������
								0x03���յ��ϵ����������������ϵ����������*/
	UINT16 wNum_TSR;
	TSR_STRU stTSRDataStru[MAX_LINK_NUM];
	UINT16 TSR_ValidFlag;		/*0xff����ʱ������Ϣ��Ч�����ϵ�δ��TMCͨ�ţ�
								0x55������ʱ������Ϣ��
								0xaa��û����ʱ������Ϣ*/

	
}OC_STRU;


/*  ***************************************����ṹ��*****************************************/
/*�����������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/  
	UINT16 wSwitchID;                 /*������Ƶĵ���ID���������豸���ͣ�*/
	UINT8 bCC_PositionNormal_Reverse;               /*�������ݣ��⵽��λ���Ƿ�λ��*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}Switch_CommandData_STRU;

/*�����������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/  
	UINT16 wSwitchID;                 /*������Ƶĵ���ID���������豸���ͣ�*/
	UINT8 bCC_LockAdd_Delete;              /*����������ɾ����;0x55��ʾ��������0XAA��ʾɾ����*/
	UINT8 bCommandState;               /*�������ݣ��⵽1λ����2λ��*/
	UINT8 All_Self;                   /*���������������Դ������������н��������Դ��,0x55�����������Դ������Ȩ��ֻ��ATS�С�0XAA������*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}MultSwitch_CommandData_STRU;

/*ATS����������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wSwitchID;                /*�����Ƶĵ���ID*/
	UINT8 bCC_LockAdd_Delete;              /*����������ɾ����;0x55��ʾ��������0XAA��ʾɾ����*/
	UINT8 bCC_ELcok_SLock;              /*��ռ�����ǹ�������0x55��ʾ��ռ����0XAA��ʾ������*/
	UINT8 bCC_LockPositionNormal_Reverse;              /*��λ�����Ƿ�λ����0x55��ʾ��λ����0xAA��ʾ��λ��*/
	UINT16 wDestID;                  /*����ɾ���������ɾ�������������ID*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}ATSLock_CommandData_STRU;

/*VOBC����������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wSwitchID;                /*�����Ƶĵ���ID*/
	UINT8 bCC_LockAdd_Delete;              /*����������ɾ����*/
	UINT8 bCC_ELcok_SLock;              /*��ռ�����ǹ�����*/
	UINT8 bCC_LockPositionNormal_Reverse;              /*��λ�����Ƿ�λ��*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}VOBCLock_CommandData_STRU;

/*�����ſ�������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wPSDID;                    /*������Ƶ�������ID*/
	UINT8 bCC_Open_Close;               /*�������ݣ��򿪻��ǹرգ�*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}PSD_CommandData_STRU;

/*��������������ṹ��*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wACID;                     /*������Ƶļ�������ID*/
	UINT8 bCC_ResetAct;               /*�������θ�λ�����δ����*/
}AC_CommandData_STRU;

/*�г������������ṹ��*/
typedef	struct
{
	UINT16 wOCRegionID;               /*OC��������ID*/
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT8 bCC_TrainContact;               /*�г�������Ϣ*/
	UINT32 dwIP;                       /*�г�dwIP��Ϣ*/
}CTMan_CommandData_STRU;

/*ATS��0*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wSwitchID;                /*�����Ƶĵ���ID*/
	UINT16 wZeroLable;               /*�����־λ���ñ�־λΪ55��ʾ�������*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}ATSLOCKDELETE_CommandData_STRU;

/*����λ*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wSwitchID;                /*�����Ƶĵ���ID*/
	UINT16 wResetLable;               /*����λ��־λ���ñ�־λΪ55��ʾ��λ����*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}SWITCH_RESET_CommandData_STRU;
/*�������*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 RegionID;                /*���������ID*/
	UINT16 wBlockLable;               /*���������־λ���ñ�־λΪ55��ʾ�������ñ�־ΪAA��ʾ���*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}REGION_BLOCK_CommandData_STRU;
/*�źŻ�����*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 SignalID;                  /*�źŻ���ID*/
	UINT16 SignalType;                /*�źŻ���������0x01:�źŻ������
									  0x02:�źŻ����̵�
									  0x03:�źŻ����Ƶ�
									  0x04���źŻ�������״̬
										*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}SIGNAL_CTRL_CommandData_STRU;

/*��ʱ��������*/
typedef	struct
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT8  TSRSpeed;                  /*��ʱ�����ٶ�*/
	UINT16 TSRType;					/*0x55��������ʱ���٣�0xaa��ȡ����ʱ����*/
	UINT16 TSRSLink;
	UINT32 TSRSOff;
	UINT8 TSRInterLinkNum;		
	UINT16 TSRInterLinkID[MAX_LINK_NUM];
	UINT16 TSRElink;
	UINT32 TSREOff;
	UINT8  TSRDir;
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}TSR_CTRL_CommandData_STRU;

/*ATS��0����ʱ*/
typedef	struct
{
	UINT16 g_dwSwitchID_ATSQingLing;				/*ATS���������������*/
	UINT32 g_dwTime_ATSQingLing;					/*ATS�������ʼ����ʱ��(������Ϊ��λ)*/

}ATSLOCKDELETE_COUNTDOWN_STRU;
typedef	struct
{   
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wDRBID;                    /*�����۷���ťID*/
	UINT16 wDRBStatus;                /*�����۷�����״̬*/
}
DRB_CommandData_STRU;

/*ɾ��/���ӷ�ͨ���г�*/
typedef struct  
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT8 DelOrAdd;						/*���ӻ���ɾ����ͨ�ų���0X55��ʾ���ӣ�0Xaa��ʾɾ��*/
	UINT16 wIVOCID;                   /*��Ҫɾ��/���ӵ�ʧ���г�ID*/
	UINT16 wDeleteTrainOneorAll;      /*ɾ�������ض��г���������ʧ���г�*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}DeleteAddTrain_CommandData_STRU;
/*�ϵ����*/
typedef struct  
{
	UINT16 wSourceID;                 /*Դ��������ͺ�ID��Ϣ*/
	UINT16 wOCID;                     /*��Ҫ�ϵ������OCID*/
	UINT8  bPowerLock;				  /*�ϵ�������������0x55Ϊ��Ч*/
	UINT8  bOnceOrTwice;				/*�ϵ����һ��������߶�������*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}PowerLock_CommandData_STRU;
typedef	struct
{   
	UINT16 wCommandNum_Switch;														 /*�����������*/
	Switch_CommandData_STRU  stOCRevSwitchCmdDataStru[CMD_MAX];						 /*OC���յĵ��������������*/

	UINT16 wCommandNum_MultSwitch;														 /*��������������������*/
	MultSwitch_CommandData_STRU  stOCRevMultSwitchCmdDataStru[CMD_MAX];						 /*OC���յĽ�����߻�����������������*/

	UINT16 wCommandNum_ATSLock;														 /*ATS��Դ���������*/
	ATSLock_CommandData_STRU stOCRevATSLockCmdDataStru[CMD_MAX];					 /*ATS��Դ���ṹ��*/
	UINT16 wCommandNum_VOBCLock;													 /*VOBC��Դ���������*/
	VOBCLock_CommandData_STRU    stOCRevVOBCLockCmdDataStru[CMD_MAX];				 /*OC���յ�VOBC��Դ��������������*/
	UINT16 wCommandNum_HCOCLock;													 /*HCOC��Դ���������*/
	VOBCLock_CommandData_STRU    stOCRevTMCLockCmdDataStru[CMD_MAX];				 /*OC���յ�VOBC��Դ��������������*/
	UINT16 wCommandNum_TMCLock;													 /*HCOC��Դ���������*/
	ATSLock_CommandData_STRU    stOCRevHCOCLockCmdDataStru[CMD_MAX];				 /*OC���յ�HCOC��Դ��������������*/
	UINT16 wCommandNum_PSD;															 /*�������������*/
    PSD_CommandData_STRU     stOCRevPSDCmdDataStru[CMD_MAX];						 /*OC���յ������ſ�����������*/   
	UINT16 wCommandNum_AC;															 /*���������������*/
    AC_CommandData_STRU     stOCRevACCmdDataStru[CMD_MAX];							 /*OC���յļ������ο�����������*/  
	UINT16 wCommandNum_CTMan;														 /*�г���������������*/
    CTMan_CommandData_STRU     stOCRevCTManCmdDataStru[CMD_MAX];					 /*�г����������������*/  
	UINT16 wCommandNum_ATSLockDelete;												 /*ATS��0�������*/
    ATSLOCKDELETE_CommandData_STRU     stOCRevATSLockDeleteCmdDataStru[CMD_MAX];	 /*ATS��0�������*/ //???ע��Ϊ��Ϊ�г����㣿by hdh/*�г���0��������*/  
	UINT16 wCommandNum_DRB;															 /*�����۷���ť�������*/
    DRB_CommandData_STRU     stOCRevDRBCmdDataStru[CMD_MAX];						 /*�г���0��������*/  
	UINT16 wCommandNum_HoldTrain;													 /*�۳��������*/
    HoldTrain_CommandData_STRU     stOCRevHoldTrainCmdDataStru[CMD_MAX];			 /*�۳���������*/  
	UINT16 wCommandNum_JumpTrain;													 /*��ͣ�������*/
    JumpTrain_CommandData_STRU     stOCRevJumpTrainCmdDataStru[CMD_MAX];			 /*��ͣ��������*/  
	UINT16 wCommandNum_DeleteAddTrain;                                                  /*ɾ��ʧ���г��������*/
	DeleteAddTrain_CommandData_STRU  stOCRevDeleteAddTrainCmdDataStru[CMD_MAX];            /*ɾ��ʧ���г���������*/
	PowerLock_CommandData_STRU    stOCRevPowerLockCmdDataStru;                       /*�ϵ������������ṹ��*/
	UINT16 wCommandNum_SwitchReset;													 /*����λ�������*/
	SWITCH_RESET_CommandData_STRU    stOCRevATSSwitchResetCmdDataStru[CMD_MAX];	     /*����λ�������*/ 
	
	UINT16 wCommandNum_RegionBlock;  
	REGION_BLOCK_CommandData_STRU    stOCRevRegionBlockCmdDataStru[CMD_MAX];                       /*������������ṹ��*/
	
	UINT16 wCommandNum_SignalCtrl;  
	SIGNAL_CTRL_CommandData_STRU    stOCRevSignalCtrlCmdDataStru[CMD_MAX];                       /*������������ṹ��*/

	UINT16 wCommandNum_TSRCtrl;
	TSR_CTRL_CommandData_STRU	stOCRevTSRCtrlCmdDataStru[CMD_MAX];

}CommandData_STRU;

typedef struct
{
	UINT8 Result;		/*����ִ�н��*/
	UINT8 Reason;		/*����ʧ��ԭ��*/
	UINT32 TimeHigh;		/*�����ʱ��,��4�ֽ�*/
	UINT32 TimeLow;		/*�����ʱ��,��4�ֽ�*/
	UINT32 CommandID;	/*������*/
	UINT16 SouceID;		/*�����Դ*/
}CommandResult_STRU;

typedef	struct
{   
	UINT16 wCommandNum_Switch;														 /*�����������*/
	CommandResult_STRU  stOCRevSwitchCmdResultStru[CMD_MAX];						 /*OC���յĵ��������������*/

	UINT16 wCommandNum_MultSwitch;														 /*��������������������*/
	CommandResult_STRU  stOCRevMultSwitchCmdResultStru[CMD_MAX];						 /*OC���յĽ�����߻�����������������*/

	UINT16 wCommandNum_Lock;														 /*��Դ���������*/
	CommandResult_STRU stOCRevLockCmdResultStru[CMD_MAX];					 /*��Դ���ṹ��*/

	UINT16 wCommandNum_PSD;															 /*�������������*/
	CommandResult_STRU     stOCRevPSDCmdResultStru[CMD_MAX];						 /*OC���յ������ſ�����������*/ 

	UINT16 wCommandNum_AC;															 /*���������������*/
	CommandResult_STRU     stOCRevACCmdResultStru[CMD_MAX];							 /*OC���յļ������ο�����������*/


	UINT16 wCommandNum_ATSLockDelete;												 /*ATS��0�������*/
	CommandResult_STRU     stOCRevATSLockDeleteCmdResultStru[CMD_MAX];	 /*ATS��0�������*/ 


	UINT16 wCommandNum_HoldTrain;													 /*�۳��������*/
	CommandResult_STRU     stOCRevHoldTrainCmdResultStru[CMD_MAX];			 /*�۳���������*/

	UINT16 wCommandNum_JumpTrain;													 /*��ͣ�������*/
	CommandResult_STRU     stOCRevJumpTrainCmdResultStru[CMD_MAX];			 /*��ͣ��������*/ 

	UINT16 wCommandNum_DeleteAddTrain;                                                  /*ɾ��ʧ���г��������*/
	CommandResult_STRU  stOCRevDeleteTrainCmdResultStru[CMD_MAX];            /*ɾ��ʧ���г���������*/

	CommandResult_STRU    stOCRevPowerLockCmdResultStru;                       /*�ϵ������������ṹ��*/
	
	UINT16 wCommandNum_RegionBlock;  
	CommandResult_STRU    stOCRevRegionBlockCmdResultStru[CMD_MAX];                       /*������������ṹ��*/

	UINT16 wCommandNum_SwitchReset;													 /*����λ�������*/
	CommandResult_STRU    stOCRevATSSwitchResetCmdResultStru[CMD_MAX];	     /*����λ�������*/ 

	UINT16 wCommandNum_SignalCtrl;													 /*�źŻ������������*/
	CommandResult_STRU    stOCRevSignalCtrlCmdResultStru[CMD_MAX];	     /*�źŻ������������*/ 

	UINT16 wCommandNum_TSRCtrl;													 /*��ʱ���ٿ����������*/
	CommandResult_STRU    stOCRevTSRCtrlCmdResultStru[CMD_MAX];	     /*��ʱ���ٿ����������*/ 

}ALLCommandResult_STRU;


/* Э��ͳһ�ṹ�� */
//typedef struct  
//{
//	UINT8 bDevID;            		/* �豸ID*/
//	UINT8 bDevType;            	/* �豸type*/
//	UINT32* pdwCycleNum;               /* ���ں� */
//	UINT16 wInputSize; 				/* �������������һ֡��Ӧ�����ݳ���*/
//	UINT16 wOutputSize; 				/* �������������һ֡��Ӧ�����ݳ���*/
//	QueueStruct stOutnetQueueA;		/* ���ն���A */
//	QueueStruct stOutnetQueueB;		/* ���ն���B */
//	UINT32 dwOutnetQueueSize;			/* ���ն��г��ȣ���Э���ʼ��ʱ��д */
//	QueueStruct stDataToApp;			/* Э�������Ӧ�õ����ݶ��� */
//	UINT32 dwDataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */
//	QueueStruct stOutputDataQueue;	/* Ӧ�ø�Э����������ݶ��� */
//	UINT32 dwOutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */
//	UINT16 wVarArraySize;		    /* �м�������鳤�� */
//	UINT8 *pbVarArray;			    /* �м�������� */
//	UINT8 *pbAppArray;				/* �����Ӧ�õ����� */
//	UINT32 dwAppArraySize;			/* �����Ӧ�õ����鳤�� */
//	UINT8 *pbRecordArray;				/* Э���¼���� */
//	UINT16 wRecordArraySize;			/* Э���¼�����С */
//}ProtclConfigInfoStru;

typedef struct
{
	UINT32 DataVersion_Map;     /*���ӵ�ͼ���ݰ汾��*/
	UINT32 DataVersion_Confi;	/*�����������ݰ汾��*/
	UINT32 DataVersion_IP;		/*IP���ݰ汾��*/
	UINT32 DataVersion_Dyn;		/*��̬���ݰ汾��*/
	UINT32 DataVersion_Prot;	/*Э�����ݰ汾��*/
}DataVersionStru;


#ifdef __cplusplus
}
#endif

#endif