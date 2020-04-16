/* ***********************************************************************
*
* 文件名      ：  TypesDef.h
* 版权说明    ：  北京交控科技有限责任公司
* 版本号      ：  1.0
* 创建时间    ：  2016-6-13
* 作者        ：  闻一龙
* 功能描述    ：  应用数据结构定义.
* 使用注意    ：  
* 修改记录    ：  2016-6-13 			闻一龙				  创建
                                                                      
*********************************************************************** */
#ifndef _TYPESDEF_H_
#define _TYPESDEF_H_
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "MacDefine.h"
#include "kcg_consts.h"
#include "dquVobcCommonStru.h"


/*================================结构定义区===============================*/
/*  根据DSU数据表结构定义变量类型规则:                                     */
/*  UINT8:   各种表示偏移量及距离信息的变量；                              */
/*  UINT16： 其余的各种变量（主要有ID号、编号、区段号、数量等）；          */
/*  UINT32:  各种设备的属性、方向、状态的变量；                            */


#ifdef __cplusplus
extern "C" {
#endif

/*=============================公共结构体============================================*/



/*道岔信息：包括道岔的ID及状态*/
//typedef struct{
//	UCHAR   PointType;				/*道岔的类型，可以考虑不用*/
//	UINT16	PointId;				/*道岔的ID*/
//	UINT16	Lnk;					/*障碍物所在区段号（D_ OBSTACLEL）*/
//	UINT32	Offset;					/*障碍物所在区段偏移量（D_ OBSTACLEO）*/
//	UCHAR	PointStatus;			/*道岔的状态*/
//	UCHAR	PointExpectedStatus;	/*道岔应该的状态*/
//}DQU_POINT_STATUS_STRU;



/*===========================站台安全门数据结构============================*/

/*站台安全门信息*/
struct PSDInfo
{
	UINT16  wPSDID;		                                /*站台安全门ID*/
	UINT16  wCIOfOC;                                   /*站台安全门所属OC*/
	UINT8  bPSDStatus_Now; 	                        /*站台安全门当前状态*/
};
typedef struct PSDInfo struct_PSDInfo;                  /*站台安全门信息数据结构申明*/




/****************************************时间结构体*****************************************/

/*  日期信息 */
typedef struct
{
	UINT8     Year;											/*  年	 					 */
	UINT8     Month;										/*  月	 					 */
	UINT8     Day;											/*  日	 					 */
}DATE_STRU;

/*  时间信息 */
typedef struct
{
	UINT8     Hour;											/*  小时	 					 */
	UINT8     Minute;										/*  分	 					 */
	UINT8     Second;										/*  秒	 					 */
}TIME_STRU;

/*  时间数据	 */
typedef struct
{
    //CTCS_BOOL			stDateValid;							/*  时间信息有效状态			 */
    DATE_STRU			stDate;								/*  日期						 */
    TIME_STRU			stTime;								/*  时刻						 */
    UINT32              dwCyclNum;                            /*软件周期号*/
}CYCL_STRU;




/*  ***************************************OC结构体*****************************************/
typedef	struct
{
	UINT16 wID_DA;                  /*检测区ID*/
	UINT8  bDAStatus;               /*检测区状态*/
	/*0xaa:空闲、0x55占用*/
	UINT8  bTrainDir;               /*检测区中运动物的方向*/
	/*0xaa:与检测区作用方向相反、0x55与检测区作用方向相同*/
}DAInfo_STRU;

typedef	struct
{
	UINT16 wID_CT;                  /*通信列车ID*/
	UINT8  bContact_CT;              /*通信列车的连接信息*/
	                                /*0xaa:删除、0x55建立，0x1维持,0x0未收到信息*/
	UINT8  bCycNum_LoseContact;                  /*OC未收到通信列车信息的周期数*/
	UINT8  bIP_CT[4];                /*通信列车dwIP地址*/
	UINT32     dwIP;                  /*通信列车dwIP地址*/
	UINT32   dwCycle;                 /*接收列车信息周期数*/
	UINT8    bDevPos;                   /*表示列车是否在OC管辖区域内*/
	UINT8  TrainMode;					/*列车运行模式*/
	UINT8  PassFlag;				/*入口检测通过标志位，55表示通过；aa表示不通过*/
	UINT16 DetecAreaID;	/*检测区ID*/
	UINT8  TrainDir;
	UINT16 TrainHeadLink;
	UINT32 TrainHeadOffset;
	UINT16 TrainTailLink;
	UINT32 TrainTailOffset;
}CT_STRU;

typedef	struct
{
	UINT16 wID_CT;                  /*通信列车ID*/
	UINT8  bContact_CT;              /*通信列车的连接信息*/
	/*0xaa:删除、0x55建立，0x1维持,0x0未收到信息*/
	UINT8  bCycNum_LoseContact;                  /*OC未收到通信列车信息的周期数*/
	UINT8  bIP_CT[4];                /*通信列车dwIP地址*/
	UINT32     dwIP;                  /*通信列车dwIP地址*/
	UINT32   dwCycle;                 /*接收列车信息周期数*/
	UINT8    bDevPos;                   /*表示列车是否在OC管辖区域内*/
	UINT8  UCTLineDir;
	UINT8  UCTType;        /*非通信列车类型：01为RM；02为失去通信*/
	UINT16 UCTLineHeadLink;
	UINT32 UCTLineHeadOffset;
	UINT16 UCTLineTailLink;
	UINT32 UCTLineTailOffset;
}UCT_STRU;

/*共享锁结构体*/
typedef	struct
{
	UINT16 wID_SLock;                  /*共享锁归属ID*/
	UINT8 bLEVEL_SLock;                 /*共享锁权限等级*/
	UINT8 bQ_SLock_SWITCH;              /*共享锁锁闭区段的道岔状态 0x55表示定位，0xaa表示反位*/
}SLock_STRU;

/*交叉渡线互斥锁结构体*/
typedef	struct
{
	UINT16 wID_ULock;                  /*互斥锁归属ID*/
	UINT8 bLEVEL_ULock;                 /*互斥锁权限等级*/
	UINT8 bQ_ULock_SWITCH;              /*互斥锁锁闭区段的道岔状态,1代表1未；2代表2位*/
}ULock_STRU;

/*屏蔽门状态结构体*/
typedef	struct
{
	UINT16 wID_PSD;
	UINT8 bQ_PSD;
}PSD_STRU;

/*计轴区段状态结构体*/
typedef	struct
{
	UINT16 wID_AC;
	UINT8 bQ_AC;
}AC_STRU;

/*紧急关闭结构体*/
typedef	struct
{
	UINT16 wID_EMP;
	UINT8 bQ_EMP;
}EMP_STRU;



/*道岔状态结构体*/
typedef	struct
{
	UINT16 wID_Switch;               /*道岔ID*/
	UINT8 bQ_Switch;                  /*道岔状态*/
	UINT8 bLockQ_Switch;             /*道岔状态（是否锁闭）*/
	UINT8 SwitchFault;				/*道岔状态（是否故障）*/
	UINT8 bLockType;                  /*锁类型*/
	UINT16 wID_ELock;                /*独占锁ID*/
	UINT8 bLevel_ELock;               /*独占锁等级*/
	UINT8 bRQ_ELock_Switch;            /*独占锁要求道岔状态*/
	UINT8 bNum_SLock;                      /*共享锁数量*/
	SLock_STRU stSLock[MAX_SLock];     /*共享锁结构体*/

	UINT8 ChangeStatus;				/*道岔扳动状态，如果道岔正在搬动中，固定时间内不允许操作，道岔搬动中：0x55,未扳动：0xaa*/
	UINT8 SendQStatus;				/*发送道岔命令时，需要的道岔状态0x55定位；0xaa反位*/
	UINT32 TimeHigh;
	UINT32 TimeLow;
	UINT32 CommandID;
	UINT32 SendCycle;				/*发送道岔命令周期数*/
	UINT32 CommandSourceID;			/*发送命令的来源*/
}SWITCH_STRU;

/*交叉渡线状态结构体*/
typedef	struct
{
	UINT16 wID_Switch;               /*道岔ID*/
	UINT8 bQ_Switch;                  /*道岔状态*/
	UINT8 bNum_ULock;                /*互斥锁个数*/
	ULock_STRU stSLock[MAX_SLock];     /*复式交分互斥锁结构体*/
}MULTSWITCH_STRU;


typedef	struct
{
	UINT16 wID_Station;               /*ID*/
	UINT8 bQ_Button;                  /*状态*/

}STATIONBUTTON_STRU;

//信号机状态
typedef	struct
{
	UINT16 wID_Signal;               /*ID*/
	UINT8 bQ_Signal;                  /*状态*/
	UINT16 SourceID;				/*点亮该信号机的是谁*/
	UINT16 SourceID_Old;				/*点亮该信号机的是谁*/
	UINT16 SignalStatus_Need;		/*需要点亮状态*/
	UINT16 SignalCtrlTime ;				/*信号机控制最大时长*/
}SIGNAL_STRU;

/*临时限速设置*/
//typedef	struct
//{
//	UINT8  TSRSpeed;                  /*临时限速速度*/
//	UINT16 TSRSLink;
//	UINT32 TSRSOff;
//	UINT8 TSRInterLinkNum;		
//	UINT16 TSRInterLinkID[MAX_LINK_NUM];
//	UINT16 TSRElink;
//	UINT32 TSREOff;
//	UINT8  TSRDir;
//	UINT8 Flag;				/*临时限速有效性，当OC起来没有与TMC通过信，则临时限速无效*/
//
//}TSR_STRU;

/*无人折返按钮结构体*/
typedef	struct
{
	UINT16 wID_DRB;   /*无人折返按钮ID*/
	UINT8 bQ_DRB;      /*无人折返按钮状态*/
}DRB_STRU;
/*OC管理区域结构体*/
typedef	struct
{
	UINT16 wRegionID;               /*管理区域ID*/
	UINT16 wNum_CT;
	CT_STRU stCTMan[MAX_CT];              /*通信列车结构体*/
	UINT8  bRegionBlockFlag;              /*区域封锁标志*/ 
}ManRegion_STRU;
/*扣车命令结构体*/
typedef	struct
{   
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wStationID;                    /*站台按钮ID*/
	UINT16 wHoldTrainStatus;                /*扣车命令状态:0x55为扣车，0xaa为跳停*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}
HoldTrain_CommandData_STRU;
/*跳停*/
typedef	struct
{   
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wStationID;                    /*站台按钮ID*/
	UINT16 wJumpTrainStatus;                /*跳停命令状态0x55:指定站台跳停0xaa:指定站台取消跳停*/
	UINT16 wJumpTrainOrStation;             /*该信息决定该跳停信息是针对该站台所有列车，还是某特定列车0x55:站台跳停 0xaa:列车跳停*/
    UINT16 wJumpTrainID;                    /*如果需要跳停特定列车，该字节表示特定列车ID*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}
JumpTrain_CommandData_STRU;
/*跳停信息结构体，用于内部处理，及发送数据调用*/
typedef	struct
{   

	UINT16 wStationID;                    /*站台ID*/
	UINT16 wJumpTrainNum;                  /*跳停列车数量，如果为零，表示跳停所有列车*/
	UINT16 wJumpTrainID[MAX_CT];                    /*如果需要跳停特定列车，该字节表示特定列车ID*/

}
JumpTrain_CommandData_Send_STRU;
/*转换表单设备信息结构体*/
typedef	struct
{   

	UINT16 wGlobalID;                    /*设备在全局中的ID*/
	UINT16 wIDInOC;						 /*设备在OC内的编号*/
	UINT16 wBelongOC;                    /*设备所在OC的ID*/
	UINT8  PointInOC[MAX_POINT_NUM];	/*设备的控制点位*/
}
CONVERT_DEVICE_STRU;
/*转换表信息结构体*/
typedef	struct
{   

	UINT16 SwitchNum;								/*道岔在转换表中的数量*/
	CONVERT_DEVICE_STRU SwitchConvertInfo[MAX_SWITCH];		/*道岔在转换表中的转换关系*/
	UINT16 PSDNum;									/*屏蔽门在转换表中的数量*/
	CONVERT_DEVICE_STRU PSDConvertInfo[MAX_PSD];				/*屏蔽门在转换表中的转换关系*/
	UINT16 EMPNum;									/*紧急停车按钮在转换表中的数量*/
	CONVERT_DEVICE_STRU EMPConvertInfo[MAX_EMP];			/*紧急停车按钮在转换表中的转换关系*/
	UINT16 ACNum;								/*计轴器在转换表中的数量*/
	CONVERT_DEVICE_STRU ACConvertInfo[MAX_AC];			/*计轴器在转换表中的转换关系*/
	UINT16 MultSwitchNum;						/*交叉渡线在转换表中的数量*/
	CONVERT_DEVICE_STRU MultSwitchConvertInfo[MAX_MULT_SWITCH];	/*交叉渡线在转换表中的转换关系*/
	UINT16 DRBNum;						/*无人折返按钮在转换表中的数量*/
	CONVERT_DEVICE_STRU DRBConvertInfo[MAX_MULT_SWITCH];	/*无人折返按钮在转换表中的转换关系*/
	UINT16 SignalNum;						/*信号机在转换表中的数量*/
	CONVERT_DEVICE_STRU SignalConvertInfo[MAX_SIGNAL];	/*信号机在转换表中的转换关系*/
}
CONVERT_TABLE_STRU;

/*OC结构体,存入该结构体的全是全局变量（相对于转换对照表）*/
typedef	struct
{
	UINT16  wID_OC;                  /*当前OC的ID*/
	UINT16  wNum_Switch;
	SWITCH_STRU stSwitch[MAX_SWITCH];  /*道岔状态结构体*/
	UINT16  wNum_PSD; 
	PSD_STRU stPSD[MAX_PSD];           /*屏蔽门状态结构体*/
	UINT16 wNum_EMP;
	EMP_STRU stEMP[MAX_EMP];           /*紧急关闭结构体*/
	UINT16 wNum_AC;
	AC_STRU stAC[MAX_AC];              /*计轴区段状态结构体*/
	UINT16 wNum_DRB;
	DRB_STRU stDRB[MAX_AC];              /*无人折返按钮状态结构体*/
	UINT16 wNum_CT;						/*需要通信的列车*/
	UINT16 wNum_MAN_CT;					/*在管辖区域内的列车*/
	CT_STRU stCT[MAX_CT];              /*通信列车结构体 内部OC通信用*/
	UINT16 wNum_LoseCommunicationTrain;
	UCT_STRU stLoseCommunicationTrain[MAX_CT];              /*丢失列车结构体 内部OC通信用*/

	UINT16 wNum_MAN_VT;					/*在管辖区域内的VBTC列车 对外*/
	CT_STRU stVT[MAX_CT];              /*VBTC列车结构体 对外*/
	UINT16 wNum_UCT;						 /*UT列车结构体 对外*/
	UCT_STRU stUT[MAX_CT];              /*UT列车结构体 对外*/

	UINT8  bRegionBlockFlag;              /*区域封锁标志 0x55表示封锁；0xaa表示解封*/ 
	UINT8  bRegionBlockPosition;              /*表示区域封锁上下行信息*/ 
	UINT16 wNum_ManRegion;
	ManRegion_STRU stManRegion[MAX_ManRegion];     /*管理区域结构体*/
	UINT8 bWSLoseInputFlag;                   /*轨旁未接收数据的周期数*/
	UINT8 bTMCLoseInputFlag;                   /*TMC接收数据的周期数*/
	UINT8 bRRBfITS;								/*与TMC断开封锁后，收到来自ITS的封锁解封信息标志位；此时解封正常*/
	UINT8 TMCInputFlag;							/*收到过TMC的信息标志位；0x55表示上电收到过TMC数据；0xaa表示上电未收到过TMC数据*/						
	UINT16 wDetectAreaNum;              /*检测区数量*/
	DAInfo_STRU stDAInfo[MAX_DA];               /*检测区信息结构体*/
	UINT16 wCommandNum_HoldTrain;													 /*扣车命令个数*/
	HoldTrain_CommandData_STRU     stOCRevHoldTrainCmdDataStru[CMD_MAX];
	UINT16 wCommandNum_JumpTrain;													 /*跳停命令个数*/
	JumpTrain_CommandData_Send_STRU     stOCRevJumpTrainCmdDataStru[CMD_MAX];			 /*跳停命令数组*/
	UINT16 wNum_MultSwitch;                 /*交叉渡线的数量*/
	MULTSWITCH_STRU   stOCMultSWitchDataStru[MAX_SWITCH];			 /*交叉渡线状态数组*/
	UINT16 wNum_StationIn;
	STATIONBUTTON_STRU stStationInButton[MAX_EMP];
	UINT16 wNum_StationOut;
	STATIONBUTTON_STRU stStationOutButton[MAX_EMP];
	UINT16 wNum_Signal;								/*信号机的数量*/
	SIGNAL_STRU stSignal[MAX_SIGNAL];
	UINT16 ElecPowerState;	/*上电解锁状态0x01:未收到上电解锁命令
								0x02:收到上电解锁一次命令
								0x03：收到上电解锁二次命令，即已上电解锁解封完成*/
	UINT16 wNum_TSR;
	TSR_STRU stTSRDataStru[MAX_LINK_NUM];
	UINT16 TSR_ValidFlag;		/*0xff：临时限速信息无效，即上电未与TMC通信；
								0x55：有临时限速信息；
								0xaa：没有临时限速信息*/

	
}OC_STRU;


/*  ***************************************命令结构体*****************************************/
/*道岔控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/  
	UINT16 wSwitchID;                 /*道岔控制的道岔ID（不包含设备类型）*/
	UINT8 bCC_PositionNormal_Reverse;               /*命令内容（扳到定位还是反位）*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}Switch_CommandData_STRU;

/*道岔控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/  
	UINT16 wSwitchID;                 /*道岔控制的道岔ID（不包含设备类型）*/
	UINT8 bCC_LockAdd_Delete;              /*增加锁还是删除锁;0x55表示增加锁，0XAA表示删除锁*/
	UINT8 bCommandState;               /*命令内容（扳到1位还是2位）*/
	UINT8 All_Self;                   /*定义是清除自身资源锁还是清除所有交叉渡线资源锁,0x55是清除所有资源锁，该权限只有ATS有。0XAA是自身*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}MultSwitch_CommandData_STRU;

/*ATS锁控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wSwitchID;                /*锁控制的道岔ID*/
	UINT8 bCC_LockAdd_Delete;              /*增加锁还是删除锁;0x55表示增加锁，0XAA表示删除锁*/
	UINT8 bCC_ELcok_SLock;              /*独占锁还是共享锁；0x55表示独占锁，0XAA表示共享锁*/
	UINT8 bCC_LockPositionNormal_Reverse;              /*定位锁还是反位锁；0x55表示定位锁，0xAA表示反位锁*/
	UINT16 wDestID;                  /*若是删除锁命令，则删除对象的锁归属ID*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}ATSLock_CommandData_STRU;

/*VOBC锁控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wSwitchID;                /*锁控制的道岔ID*/
	UINT8 bCC_LockAdd_Delete;              /*增加锁还是删除锁*/
	UINT8 bCC_ELcok_SLock;              /*独占锁还是共享锁*/
	UINT8 bCC_LockPositionNormal_Reverse;              /*定位锁还是反位锁*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}VOBCLock_CommandData_STRU;

/*屏蔽门控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wPSDID;                    /*命令控制的屏蔽门ID*/
	UINT8 bCC_Open_Close;               /*命令内容（打开还是关闭）*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}PSD_CommandData_STRU;

/*计轴器控制命令结构体*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wACID;                     /*命令控制的计轴区段ID*/
	UINT8 bCC_ResetAct;               /*计轴区段复位激活还是未激活*/
}AC_CommandData_STRU;

/*列车分组管理命令结构体*/
typedef	struct
{
	UINT16 wOCRegionID;               /*OC管理区域ID*/
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT8 bCC_TrainContact;               /*列车连接信息*/
	UINT32 dwIP;                       /*列车dwIP信息*/
}CTMan_CommandData_STRU;

/*ATS清0*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wSwitchID;                /*锁控制的道岔ID*/
	UINT16 wZeroLable;               /*清零标志位，该标志位为55表示清零道岔*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}ATSLOCKDELETE_CommandData_STRU;

/*道岔复位*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wSwitchID;                /*锁控制的道岔ID*/
	UINT16 wResetLable;               /*道岔复位标志位，该标志位为55表示复位道岔*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}SWITCH_RESET_CommandData_STRU;
/*区域封锁*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 RegionID;                /*区域封锁的ID*/
	UINT16 wBlockLable;               /*区域封锁标志位，该标志位为55表示封锁；该标志为AA表示解封*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}REGION_BLOCK_CommandData_STRU;
/*信号机控制*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 SignalID;                  /*信号机的ID*/
	UINT16 SignalType;                /*信号机点亮类型0x01:信号机亮红灯
									  0x02:信号机亮绿灯
									  0x03:信号机亮黄灯
									  0x04：信号机打叉灭灯状态
										*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;

}SIGNAL_CTRL_CommandData_STRU;

/*临时限速设置*/
typedef	struct
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT8  TSRSpeed;                  /*临时限速速度*/
	UINT16 TSRType;					/*0x55：设置临时限速；0xaa：取消临时限速*/
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

/*ATS清0倒计时*/
typedef	struct
{
	UINT16 g_dwSwitchID_ATSQingLing;				/*ATS清零命令关联道岔*/
	UINT32 g_dwTime_ATSQingLing;					/*ATS清零命令开始持续时间(以周期为单位)*/

}ATSLOCKDELETE_COUNTDOWN_STRU;
typedef	struct
{   
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wDRBID;                    /*无人折返按钮ID*/
	UINT16 wDRBStatus;                /*无人折返闪灯状态*/
}
DRB_CommandData_STRU;

/*删除/增加非通信列车*/
typedef struct  
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT8 DelOrAdd;						/*增加还是删除非通信车，0X55表示增加，0Xaa表示删除*/
	UINT16 wIVOCID;                   /*需要删除/增加的失联列车ID*/
	UINT16 wDeleteTrainOneorAll;      /*删除单个特定列车或者所有失联列车*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}DeleteAddTrain_CommandData_STRU;
/*上电解锁*/
typedef struct  
{
	UINT16 wSourceID;                 /*源对象的类型和ID信息*/
	UINT16 wOCID;                     /*需要上电解锁的OCID*/
	UINT8  bPowerLock;				  /*上电解锁命令参数，0x55为有效*/
	UINT8  bOnceOrTwice;				/*上电解锁一次命令或者二次命令*/
	UINT32 CommandID;
	UINT32 TimeHigh;
	UINT32 TimeLow;
}PowerLock_CommandData_STRU;
typedef	struct
{   
	UINT16 wCommandNum_Switch;														 /*道岔命令个数*/
	Switch_CommandData_STRU  stOCRevSwitchCmdDataStru[CMD_MAX];						 /*OC接收的道岔控制命令数组*/

	UINT16 wCommandNum_MultSwitch;														 /*交叉渡线申请锁命令个数*/
	MultSwitch_CommandData_STRU  stOCRevMultSwitchCmdDataStru[CMD_MAX];						 /*OC接收的交叉渡线互斥锁申请命令数组*/

	UINT16 wCommandNum_ATSLock;														 /*ATS资源锁命令个数*/
	ATSLock_CommandData_STRU stOCRevATSLockCmdDataStru[CMD_MAX];					 /*ATS资源锁结构体*/
	UINT16 wCommandNum_VOBCLock;													 /*VOBC资源锁命令个数*/
	VOBCLock_CommandData_STRU    stOCRevVOBCLockCmdDataStru[CMD_MAX];				 /*OC接收的VOBC资源锁控制命令数组*/
	UINT16 wCommandNum_HCOCLock;													 /*HCOC资源锁命令个数*/
	VOBCLock_CommandData_STRU    stOCRevTMCLockCmdDataStru[CMD_MAX];				 /*OC接收的VOBC资源锁控制命令数组*/
	UINT16 wCommandNum_TMCLock;													 /*HCOC资源锁命令个数*/
	ATSLock_CommandData_STRU    stOCRevHCOCLockCmdDataStru[CMD_MAX];				 /*OC接收的HCOC资源锁控制命令数组*/
	UINT16 wCommandNum_PSD;															 /*屏蔽门命令个数*/
    PSD_CommandData_STRU     stOCRevPSDCmdDataStru[CMD_MAX];						 /*OC接收的屏蔽门控制命令数组*/   
	UINT16 wCommandNum_AC;															 /*计轴区段命令个数*/
    AC_CommandData_STRU     stOCRevACCmdDataStru[CMD_MAX];							 /*OC接收的计轴区段控制命令数组*/  
	UINT16 wCommandNum_CTMan;														 /*列车分组管理命令个数*/
    CTMan_CommandData_STRU     stOCRevCTManCmdDataStru[CMD_MAX];					 /*列车分组管理命令数组*/  
	UINT16 wCommandNum_ATSLockDelete;												 /*ATS清0命令个数*/
    ATSLOCKDELETE_CommandData_STRU     stOCRevATSLockDeleteCmdDataStru[CMD_MAX];	 /*ATS清0命令个数*/ //???注释为何为列车清零？by hdh/*列车清0命令数组*/  
	UINT16 wCommandNum_DRB;															 /*无人折返按钮命令个数*/
    DRB_CommandData_STRU     stOCRevDRBCmdDataStru[CMD_MAX];						 /*列车清0命令数组*/  
	UINT16 wCommandNum_HoldTrain;													 /*扣车命令个数*/
    HoldTrain_CommandData_STRU     stOCRevHoldTrainCmdDataStru[CMD_MAX];			 /*扣车命令数组*/  
	UINT16 wCommandNum_JumpTrain;													 /*跳停命令个数*/
    JumpTrain_CommandData_STRU     stOCRevJumpTrainCmdDataStru[CMD_MAX];			 /*跳停命令数组*/  
	UINT16 wCommandNum_DeleteAddTrain;                                                  /*删除失联列车命令个数*/
	DeleteAddTrain_CommandData_STRU  stOCRevDeleteAddTrainCmdDataStru[CMD_MAX];            /*删除失联列车命令数组*/
	PowerLock_CommandData_STRU    stOCRevPowerLockCmdDataStru;                       /*上电解锁命令参数结构体*/
	UINT16 wCommandNum_SwitchReset;													 /*道岔复位命令个数*/
	SWITCH_RESET_CommandData_STRU    stOCRevATSSwitchResetCmdDataStru[CMD_MAX];	     /*道岔复位命令个数*/ 
	
	UINT16 wCommandNum_RegionBlock;  
	REGION_BLOCK_CommandData_STRU    stOCRevRegionBlockCmdDataStru[CMD_MAX];                       /*封锁命令参数结构体*/
	
	UINT16 wCommandNum_SignalCtrl;  
	SIGNAL_CTRL_CommandData_STRU    stOCRevSignalCtrlCmdDataStru[CMD_MAX];                       /*封锁命令参数结构体*/

	UINT16 wCommandNum_TSRCtrl;
	TSR_CTRL_CommandData_STRU	stOCRevTSRCtrlCmdDataStru[CMD_MAX];

}CommandData_STRU;

typedef struct
{
	UINT8 Result;		/*命令执行结果*/
	UINT8 Reason;		/*命令失败原因*/
	UINT32 TimeHigh;		/*命令发送时间,高4字节*/
	UINT32 TimeLow;		/*命令发送时间,低4字节*/
	UINT32 CommandID;	/*命令编号*/
	UINT16 SouceID;		/*命令发送源*/
}CommandResult_STRU;

typedef	struct
{   
	UINT16 wCommandNum_Switch;														 /*道岔命令个数*/
	CommandResult_STRU  stOCRevSwitchCmdResultStru[CMD_MAX];						 /*OC接收的道岔控制命令数组*/

	UINT16 wCommandNum_MultSwitch;														 /*交叉渡线申请锁命令个数*/
	CommandResult_STRU  stOCRevMultSwitchCmdResultStru[CMD_MAX];						 /*OC接收的交叉渡线互斥锁申请命令数组*/

	UINT16 wCommandNum_Lock;														 /*资源锁命令个数*/
	CommandResult_STRU stOCRevLockCmdResultStru[CMD_MAX];					 /*资源锁结构体*/

	UINT16 wCommandNum_PSD;															 /*屏蔽门命令个数*/
	CommandResult_STRU     stOCRevPSDCmdResultStru[CMD_MAX];						 /*OC接收的屏蔽门控制命令数组*/ 

	UINT16 wCommandNum_AC;															 /*计轴区段命令个数*/
	CommandResult_STRU     stOCRevACCmdResultStru[CMD_MAX];							 /*OC接收的计轴区段控制命令数组*/


	UINT16 wCommandNum_ATSLockDelete;												 /*ATS清0命令个数*/
	CommandResult_STRU     stOCRevATSLockDeleteCmdResultStru[CMD_MAX];	 /*ATS清0命令个数*/ 


	UINT16 wCommandNum_HoldTrain;													 /*扣车命令个数*/
	CommandResult_STRU     stOCRevHoldTrainCmdResultStru[CMD_MAX];			 /*扣车命令数组*/

	UINT16 wCommandNum_JumpTrain;													 /*跳停命令个数*/
	CommandResult_STRU     stOCRevJumpTrainCmdResultStru[CMD_MAX];			 /*跳停命令数组*/ 

	UINT16 wCommandNum_DeleteAddTrain;                                                  /*删除失联列车命令个数*/
	CommandResult_STRU  stOCRevDeleteTrainCmdResultStru[CMD_MAX];            /*删除失联列车命令数组*/

	CommandResult_STRU    stOCRevPowerLockCmdResultStru;                       /*上电解锁命令参数结构体*/
	
	UINT16 wCommandNum_RegionBlock;  
	CommandResult_STRU    stOCRevRegionBlockCmdResultStru[CMD_MAX];                       /*封锁命令参数结构体*/

	UINT16 wCommandNum_SwitchReset;													 /*道岔复位命令个数*/
	CommandResult_STRU    stOCRevATSSwitchResetCmdResultStru[CMD_MAX];	     /*道岔复位命令个数*/ 

	UINT16 wCommandNum_SignalCtrl;													 /*信号机控制命令个数*/
	CommandResult_STRU    stOCRevSignalCtrlCmdResultStru[CMD_MAX];	     /*信号机控制命令个数*/ 

	UINT16 wCommandNum_TSRCtrl;													 /*临时限速控制命令个数*/
	CommandResult_STRU    stOCRevTSRCtrlCmdResultStru[CMD_MAX];	     /*临时限速控制命令个数*/ 

}ALLCommandResult_STRU;


/* 协议统一结构体 */
//typedef struct  
//{
//	UINT8 bDevID;            		/* 设备ID*/
//	UINT8 bDevType;            	/* 设备type*/
//	UINT32* pdwCycleNum;               /* 周期号 */
//	UINT16 wInputSize; 				/* 接受数据中最大一帧的应用数据长度*/
//	UINT16 wOutputSize; 				/* 发送数据中最大一帧的应用数据长度*/
//	QueueStruct stOutnetQueueA;		/* 接收队列A */
//	QueueStruct stOutnetQueueB;		/* 接收队列B */
//	UINT32 dwOutnetQueueSize;			/* 接收队列长度，由协议初始化时填写 */
//	QueueStruct stDataToApp;			/* 协议输入给应用的数据队列 */
//	UINT32 dwDataToAppSize;			/* 协议输入给应用的数据队列长度 */
//	QueueStruct stOutputDataQueue;	/* 应用给协议输出的数据队列 */
//	UINT32 dwOutputDataQueueSize;		/* 应用给协议输出的数据队列长度 */
//	UINT16 wVarArraySize;		    /* 中间变量数组长度 */
//	UINT8 *pbVarArray;			    /* 中间变量数组 */
//	UINT8 *pbAppArray;				/* 输出给应用的数据 */
//	UINT32 dwAppArraySize;			/* 输出给应用的数组长度 */
//	UINT8 *pbRecordArray;				/* 协议记录数组 */
//	UINT16 wRecordArraySize;			/* 协议记录数组大小 */
//}ProtclConfigInfoStru;

typedef struct
{
	UINT32 DataVersion_Map;     /*电子地图数据版本号*/
	UINT32 DataVersion_Confi;	/*配置数据数据版本号*/
	UINT32 DataVersion_IP;		/*IP数据版本号*/
	UINT32 DataVersion_Dyn;		/*动态数据版本号*/
	UINT32 DataVersion_Prot;	/*协议数据版本号*/
}DataVersionStru;


#ifdef __cplusplus
}
#endif

#endif