/************************************************************************
*
* 文件名        ：  dquCIJDQStruFrmChar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  从数据到继电器结构体处理过程 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIJDQStruFrmChar.h"
#include "dquCIReadCommonChar.h"

/*********************************************
*函数功能：初始化继电器接口数据结构体
*参数说明：pDataAddr数据地址，  pRelayCfgDtStr继电器配置数据结构体指针，Name继电器名称
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayConfigDataStruFromChar(UINT8 * pDataAddr,RelayConfigDataStruct* pRelayCfgDtStr,CHAR* Name)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT16 i=0;

    if ((NULL == pDataAddr) || (NULL == pRelayCfgDtStr) || (NULL == Name))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	/*继电器ID (配置字段)*/
	pRelayCfgDtStr->RelayId = dquCiShortFromChar(pData);
	pData += 2;

	for(i=0;i<JDQ_NAME_LEN;i++)
	{
		Name[i] = *pData;
		pData++;
	}
    /*所属联锁ID (配置字段)*/
	pRelayCfgDtStr->BelongOcId = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*采集模式(前节点/后节点/双节点) (配置字段)*/
	pRelayCfgDtStr->CollMode = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系前节点采集地址 (配置字段)*/
	pRelayCfgDtStr->ForeNodeCollAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系前节点采集位置 (配置字段)*/
	pRelayCfgDtStr->ForeNodeCollPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系后节点采集地址 (配置字段)*/
	pRelayCfgDtStr->BackNodeCollAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系后节点采集位置 (配置字段)*/
	pRelayCfgDtStr->BackNodeCollPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*B系前节点采集地址 (配置字段)*/
	pRelayCfgDtStr->ForeNodeCollAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*B系前节点采集位置 (配置字段)*/
	pRelayCfgDtStr->ForeNodeCollPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
	/*B系后节点采集地址 (配置字段)*/
	pRelayCfgDtStr->BackNodeCollAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*B系后节点采集位置 (配置字段)*/
	pRelayCfgDtStr->BackNodeCollPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系驱动地址 (配置字段)*/
	pRelayCfgDtStr->DrvAddrA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*A系驱动位置 (配置字段)*/
	pRelayCfgDtStr->DrvPosA = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*B系驱动地址 (配置字段)*/
	pRelayCfgDtStr->DrvAddrB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;
    /*B系驱动位置 (配置字段)*/
	pRelayCfgDtStr->DrvPosB = (UINT8)(dquCiShortFromChar(pData));
	pData += 2;

	rtnvalue = 1;
	return rtnvalue;
}



/*********************************************
*函数功能：初始化紧急停车按钮继电器结构体
*参数说明：pDataAddr数据地址，  pEmgcyStpStr紧急停车按钮继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_EmegencyStopStructFromChar(UINT8 * pDataAddr,Relay_EmgcyStpStruct* pEmgcyStpStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pEmgcyStpStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pEmgcyStpStr->wEmgcyStopBtnId = dquCiShortFromChar(pData);          /*紧急停车按钮ID */
    pData += 2;   
	pEmgcyStpStr->wBelongOcId = dquCiShortFromChar(pData);              /*所属OC ID*/
	pData += 2;
    pEmgcyStpStr->wEmgcyStopBtnRelayId = dquCiShortFromChar(pData);     /*紧急停车按钮继电器ID */
    pData += 2; 

    rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到屏蔽门继电器结构体
*参数说明：pDataAddr数据地址，  pScrnStr屏蔽门继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_ScreenStructFromChar(UINT8 * pDataAddr,Relay_ScreenStruct* pScrnStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pScrnStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pScrnStr->wPsdId = dquCiShortFromChar(pData);                          /*屏蔽门ID (配置字段)*/
    pData += 2;   
	pScrnStr->wBelongOcId = dquCiShortFromChar(pData);                        /*所属OC ID*/
	pData += 2;
    pScrnStr->wRelayRelatedId = dquCiShortFromChar(pData);                    /*互锁解除继电器ID */
    pData += 2; 
	pScrnStr->wRelayDoorClosedId = dquCiShortFromChar(pData);                 /*门关继电器ID */
    pData += 2; 
	pScrnStr->wRelayCloseDoorId = dquCiShortFromChar(pData);                  /*关门继电器ID */
    pData += 2;   
    pScrnStr->wRelayFourEditGroupOpenDoorId= dquCiShortFromChar(pData);       /*4编组开门继电器ID */
    pData += 2; 
    pScrnStr->wRelaySixEditGroupOpenDoorId= dquCiShortFromChar(pData);        /*6编组开门继电器ID */
    pData += 2;
	pScrnStr->wRelayEightEditGroupOpenDoorId= dquCiShortFromChar(pData);      /*8编组开门继电器ID */
	pData += 2;
	pScrnStr->wRelayDoorButtonId = dquCiShortFromChar(pData);                 /*PCB按钮继电器*/
	pData += 2;

    rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到物理区段继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  物理区段继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_PhysicalSegmentStructFromChar(UINT8 * pDataAddr,Relay_PhysicalSegmentStruct* pPhysclSgmtStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pPhysclSgmtStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pPhysclSgmtStr->wPhysicalId = dquCiShortFromChar(pData);               /*物理区段ID*/
    pData += 2;   
    pPhysclSgmtStr->wBelongOcId = dquCiShortFromChar(pData);               /*所属OC ID*/
    pData += 2; 
	pPhysclSgmtStr->wSwitchModuleIdA = dquCiShortFromChar(pData);          /*A系物理区段模块ID*/
    pData += 2;
	pPhysclSgmtStr->wSwitchPosA = dquCiShortFromChar(pData);               /*A系物理区段位置*/
	pData += 2;
	pPhysclSgmtStr->wSwitchModuleIdB = dquCiShortFromChar(pData);          /*B系物理区段模块ID*/
	pData += 2;
	pPhysclSgmtStr->wSwitchPosB = dquCiShortFromChar(pData);               /*B系物理区段位置*/
	pData += 2;  
    
    rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到道岔继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  道岔继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_PointStructFromChar(UINT8 * pDataAddr,Relay_PointStruct* pSwitchStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
    if ((NULL == pDataAddr) || (NULL == pSwitchStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }
	pSwitchStr->wSwitchId = dquCiShortFromChar(pData);                /*道岔ID*/
	pData += 2;   
	pSwitchStr->wBelongOcId = dquCiShortFromChar(pData);              /*所属OC ID */
	pData += 2; 
	pSwitchStr->wSwitchModuleIdA = dquCiShortFromChar(pData);         /*A系道岔模块ID */
	pData += 2;
	pSwitchStr->wSwitchPosA = dquCiShortFromChar(pData);              /*A系转辙机位置*/
	pData += 2;   
	pSwitchStr->wSwitchModuleIdB = dquCiShortFromChar(pData);         /*B系道岔模块ID */
	pData += 2; 
	pSwitchStr->wSwitchPosB = dquCiShortFromChar(pData);              /*B系转辙机位置 */
	pData += 2;
	
    rtnvalue = 1;
	return rtnvalue;
}


/*********************************************
*函数功能：从数据到道岔继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  道岔继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_MultSwtichStructFromChar(UINT8 * pDataAddr,Relay_MultSwitchStruct* pMultSwitchStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	if ((NULL == pDataAddr) || (NULL == pMultSwitchStr))
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	pMultSwitchStr->wMultSwitchId = dquCiShortFromChar(pData);                /*道岔ID*/
	pData += 2;   
	pMultSwitchStr->wBelongOcId = dquCiShortFromChar(pData);              /*所属OC ID */
	pData += 2;  
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到信号机继电器结构体
*参数说明：pDataAddr数据地址，  pSgnlStr信号机继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_SignalStructFromChar(UINT8 * pDataAddr,Relay_SignalStruct* pSgnlStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pSgnlStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

    pSgnlStr->wSignalId = dquCiShortFromChar(pData);               /*信号机ID*/
	pData += 2;
    pSgnlStr->wBelongOcId = dquCiShortFromChar(pData);             /*所属OC ID*/
	pData += 2;
    pSgnlStr->wSignalModuleIdA = dquCiShortFromChar(pData);        /*A系信号机模块ID*/
	pData += 2;
    pSgnlStr->wSignalPosA = dquCiShortFromChar(pData);             /*A系信号机位置*/
	pData += 2;
    pSgnlStr->wSignalModuleIdB = dquCiShortFromChar(pData);        /*B系信号机模块ID*/
	pData += 2;
    pSgnlStr->wSignalPosB = dquCiShortFromChar(pData);             /*B系信号机位置*/
	pData += 2;

	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*函数功能：从数据到洗车机继电器结构体
*参数说明：pDataAddr数据地址，  pWashMachineStr 洗车机继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_WashMachineStructFromChar(UINT8 * pDataAddr,Relay_WashMachineStruct * pWashMachineStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 1;

    if ((NULL == pDataAddr) || (NULL == pWashMachineStr))
    {
        rtnvalue = 0;
        
    }
    if(1 == rtnvalue)
    {
	    pWashMachineStr->wId = dquCiShortFromChar(pData);                              /*洗车机ID*/
		pData += 2;
	    pWashMachineStr->WashMachineReadyRelayId = dquCiShortFromChar(pData);          /*洗车机就绪继电器ID*/
		pData += 2;
		pWashMachineStr->WashMachineRequestVerifyRelayId = dquCiShortFromChar(pData);  /*洗车请求恢复确认继电器ID*/
		pData += 2;
	    pWashMachineStr->TrainEmergencyStopRelayId = dquCiShortFromChar(pData);        /*列车急停继电器ID*/
		pData += 2;
	    pWashMachineStr->HeadWashAllowRelayId = dquCiShortFromChar(pData);             /*出发1继电器ID*/
		pData += 2;
	    pWashMachineStr->TailWashAllowRelayId = dquCiShortFromChar(pData);             /*出发2继电器ID*/
		pData += 2;
	    pWashMachineStr->WashTrainRequestRelayId= dquCiShortFromChar(pData);           /*洗车请求继电器ID*/
		pData += 2;
		
	    pWashMachineStr->TrainHeadStopSteadyRelayId = dquCiShortFromChar(pData);       /*列车停稳1继电器ID*/
		pData += 2; 
	    pWashMachineStr->TrainTailStopSteadyRelayId = dquCiShortFromChar(pData);       /*列车停稳2继电器ID*/
		pData += 2;		
	    pWashMachineStr->TrainOutWashTrainPortAllowRelayId = dquCiShortFromChar(pData);/*出库继电器ID*/
		pData += 2;
		pWashMachineStr->PauseTrainWashRequestRelayId = dquCiShortFromChar(pData);		/*暂停洗车继电器ID*/
		pData += 2;
		
		rtnvalue = 1;
    }
	 
	return rtnvalue;

}
/*********************************************
*函数功能：从数据到非进路数据继电器结构体
*参数说明：pDataAddr数据地址，  pNoRotStr非进路数据继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_NoRouteDcStruFromChar(UINT8 * pDataAddr,Relay_NoRouteDcStruct* pNoRotStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pNoRotStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pNoRotStr->wId = dquCiShortFromChar(pData);               /*非进路调车ID*/
	pData += 2;
	pNoRotStr->wRelayShenQingScId = dquCiShortFromChar(pData);         /* 车辆段采集的申请试车继电器ID */
	pData += 2;
	pNoRotStr->wRelayTongYiScId = dquCiShortFromChar(pData);         /* 车辆段驱动的同意试车继电器ID */
	pData += 2;
   
	pNoRotStr->wRelayDrvShiCheShengQingId = dquCiShortFromChar(pData);         /* 试车线驱动的申请试车继电器ID */
	pData += 2;

	pNoRotStr->wRelayColTongYiScId = dquCiShortFromChar(pData);	/* 试车线采集同意试车继电器ID */
	pData += 2;

	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*函数功能：从数据到报警灯继电器结构体
*参数说明：pDataAddr数据地址， pBjdStr 报警灯继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_BaoJingDengStruFromChar(UINT8 * pDataAddr,Relay_BaoJingDengStruct* pBjdStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT16 i=0;

    if ((NULL == pDataAddr) || (NULL == pBjdStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pBjdStr->wId = dquCiShortFromChar(pData);               /*ID*/
	pData += 2;
	pBjdStr->wBelongCiId = dquCiShortFromChar(pData);               /*所属ID*/
	pData += 2;
	pBjdStr->wBjdID = dquCiShortFromChar(pData);               /*报警灯ID*/
	pData += 2;
	pBjdStr->wBjdYouXiao = *pData;         /*是否有效*/
	pData += 1;
	for(i=0;i<BJD_MAX_NUM;i++)
	{
		pBjdStr->wBjdJdqId[i] = dquCiShortFromChar(pData);         /*对端继电器ID*/
		pData += 2;
	}
	
   
	 rtnvalue = 1;
	return rtnvalue;

}

/*********************************************
*函数功能：从数据到无人折返继电器结构体
*参数说明：pDataAddr数据地址，  pWuRenRtnStr无人折返继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_NoReturnStruFromChar(UINT8 * pDataAddr,Relay_NoReturnStruct* pWuRenRtnStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pWuRenRtnStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pWuRenRtnStr->wId = dquCiShortFromChar(pData);               /*ID*/
	pData += 2;
	pWuRenRtnStr->wWuRenReturnRelayId = dquCiShortFromChar(pData);     /*继电器组ID*/
	pData += 2;

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化车库门继电器结构体
*参数说明：pDataAddr数据地址，  pGaragedoorStr车库门继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_GaragedoorStructFromChar(UINT8 * pDataAddr,Relay_GaragedoorStruct* pGaragedoorStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
 
    if ((NULL == pDataAddr) || (NULL == pGaragedoorStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pGaragedoorStr->wGaragedoorId = dquCiShortFromChar(pData);          /* 车库门ID */
		pData += 2;   
		pGaragedoorStr->wGardoorCloseRelayId = dquCiShortFromChar(pData);          /* 车库门关门继电器ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorOpenRelayId = dquCiShortFromChar(pData);          /* 车库门开门继电器ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorCloseOrderRelayId = dquCiShortFromChar(pData);          /* 车库门关门命令继电器ID*/
		pData += 2; 
		pGaragedoorStr->wGardoorOpenOrderRelayId = dquCiShortFromChar(pData);          /* 车库门开门命令继电器ID*/
		pData += 2; 
        pGaragedoorStr->wGardoorModeStateRelayId = dquCiShortFromChar(pData);          /* 车库门模式状态继电器ID*/
        pData += 2; 
        pGaragedoorStr->wGardoorFaultStateRelayId = dquCiShortFromChar(pData);          /* 车库门故障继电器ID*/
        pData += 2; 
		rtnvalue = 1;
	}
	return rtnvalue;
}


/*********************************************
*函数功能：从数据到SPKS继电器结构体
*参数说明：pDataAddr数据地址，  pSpksStr SPKS数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_SpksDataFromChar(UINT8 * pDataAddr,RelaySpksStruct* pSpksStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL == pDataAddr) || (NULL == pSpksStr))
	{
		rtnvalue = 0;		
	}
	else
	{
		pSpksStr->id = dquCiShortFromChar(pData);               /* ID */
		pData += 2;
		pSpksStr->colRelayId = dquCiShortFromChar(pData);     /* 采集继电器组ID */
		pData += 2;
		pSpksStr->disRelayId = dquCiShortFromChar(pData);     /* 表示继电器组ID */
		/*pData += 2;*/
		pSpksStr->cutRelayId = dquCiShortFromChar(pData);     /* 旁路继电器组ID */
		pData += 2;
		rtnvalue = 1;
	}
	
	return rtnvalue;
}


/*********************************************
*函数功能：从数据到防淹门继电器结构体
*参数说明：pDataAddr数据地址，  pFloodGateStr 防淹门数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_FloodGateStructFromChar(UINT8 * pDataAddr,Relay_FloodGateStruct* pFloodGateStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL == pDataAddr) || (NULL == pFloodGateStr))
	{
		rtnvalue = 0;		
	}
	else
	{
		pFloodGateStr->wFloodGateId = dquCiShortFromChar(pData);               /* 防淹门ID */
		pData += 2;
		pFloodGateStr->wCloseApplyRelayId = dquCiShortFromChar(pData);     /* 关门请求继电器ID */
		pData += 2;
		pFloodGateStr->wOpenCloseRelayId = dquCiShortFromChar(pData);     /* 开门且锁闭继电器ID */
		pData += 2;
		pFloodGateStr->wAllowRequestRelayId = dquCiShortFromChar(pData);     /* 允许请求继电器ID */
		pData += 2;

		rtnvalue = 1;
	}
	
	return rtnvalue;
}