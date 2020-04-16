/************************************************************************
*
* 文件名     ：     dquCIPZStruFrmChar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从数据到CI配置表转换处理过程
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIPZStruFrmChar.h"
#include "dquEmapInit.h"
#include "dquCIReadCommonChar.h"

DquCiTransmitStruct *pDquCiTranmitInfoStr = NULL;         /*联锁间接口数据表映射结构体*/
UINT16 dquCiTranmitInfoLen = 0;


DquOutStationSigStruct  *pDquOutStationSigStr = NULL;    /*出站信号机关联逻辑区段数据表映射结构体*/
UINT16 dquOutStationSigStrLen = 0;

DquCiFuncSwitchStruct *pDquCiFuncSwitchStru = NULL;    /*功能开关配置数据表映射结构体*/
UINT16 dquCiFuncSwitchStruLen = 0;

DquDiffLineDevStruct *pTempDquDiffLineDevData = NULL;	/*相邻线路设备数据表映射结构体*/
UINT16 tempDquDiffLineDevDataLen = 0;

DquVirtualDeviceStruct *pDquVirtualDeviceStr = NULL;    /*虚拟设备数据表映射结构体*/
UINT16 dquVirtualDeviceStrLen = 0;

/*********************************************
*函数功能：从数据到CI配置表转换
*参数说明：pDataAddr数据地址，pPZSysParStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZ_SystemParameterStruFromChar(UINT8 * pDataAddr,SystemParameterStruct* pPZSysParStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;


    if ((NULL == pDataAddr) || (NULL == pPZSysParStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pPZSysParStr->CiSum =  (UINT8)dquCiShortFromChar(pData); /*实际联锁集中站数量*/
	pData+=2;

	pPZSysParStr->AppCycleTime  = (UINT32) dquCiShortFromChar(pData); /*联锁应用周期(单位:ms) (配置字段)*/
	pData+=2;		
    /*上电解锁超限时间(单位:s) (配置字段)*/
	pPZSysParStr->StartUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;
    /*进路选排超限时间(单位:s) (配置字段)*/
	pPZSysParStr->RouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2;
	    /*进路锁闭超限时间(单位:s) (配置字段)*/
	pPZSysParStr->RouteLockTime =  dquCiShortFromChar(pData); 
	pData+=2;
	   /*保护区段锁闭超限时间(单位:s) (配置字段)*/
	pPZSysParStr->OverlapLockTime =  dquCiShortFromChar(pData); 
	pData+=2;
               /*进路开放超限时间(单位:s) (配置字段)*/
	pPZSysParStr->RouteOpenSigTime =  dquCiShortFromChar(pData); 
	pData+=2;  
               /*点式进路延时解锁时间(单位:s) (配置字段)*/
     pPZSysParStr->BlocRouteDelayTime =  dquCiShortFromChar(pData); 
	pData+=2;           
	
    /*CBTC进路延时解锁时间(单位:s) (配置字段)*/
	pPZSysParStr->CbtcRouteDelayTime =  dquCiShortFromChar(pData); 
	pData+=2;
	/*引导进路内方区段故障信号开放持续时间(单位:s) (配置字段)*/
	pPZSysParStr->LeadRouteFaultOpenTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	/*区段延时解锁时间(单位:s) (配置字段)*/
	pPZSysParStr->SecDelayUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*信号开放超限时间(单位:s) (配置字段)*/
	pPZSysParStr->SignalOpenTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*信号关闭超限时间(单位:s) (配置字段)*/
	pPZSysParStr->SignalCloseTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	/*道岔动作超限时间(单位:s) (配置字段)*/
	pPZSysParStr->SwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2;           
	/*道岔驱动超限时间(单位:s) (配置字段)*/
	 pPZSysParStr->SwitchDriveTime =  dquCiShortFromChar(pData); 
	pData+=2;
	/*防护区段解锁停稳时间(单位:s) (配置字段)*/
	pPZSysParStr->ProtectSecUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;   
	/*占用变空闲计时时间(单位:s) (配置字段)*/
	pPZSysParStr->OccupyToIdleTime =  dquCiShortFromChar(pData); 
	pData+=2;              
	/*白光带区段故障解锁时间(单位:s) (配置字段)*/
	pPZSysParStr->FaultUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;             
	/*计轴复位超时时间(单位:s) (配置字段)*/
	pPZSysParStr->SectionResetTime =  dquCiShortFromChar(pData); 
	pData+=2;      
	/*非进路选排总时间*/
	pPZSysParStr->NonRouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2;       
	/*非进路延时解锁总时间*/
    pPZSysParStr->NonRouteDelayUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;     
    /*进路选排道岔错峰时间*/
	pPZSysParStr->SwitchDriveCuoFengTime =  dquCiShortFromChar(pData); 
	pData+=2;  
	/*组合进路选排时间*/
    pPZSysParStr->ZuheRouteXuanPaiTime =  dquCiShortFromChar(pData); 
	pData+=2; 
	
    /*等待ZC反馈时间*/
    pPZSysParStr->WaitZCResponseTime =  dquCiShortFromChar(pData); 
    pData+=2; 

	/*区段跳跃锁闭延时解锁时间(单位:s) (配置字段)*/
	pPZSysParStr->SecJumpLockDelayUnlockTime = dquCiShortFromChar(pData);
	pData+=2;

	/*车库门动作超限时间(单位:s) (配置字段)*/
	pPZSysParStr->GaragedoorOperateTime = dquCiShortFromChar(pData);
	pData+=2;
	
	/*CT占用进路内最后一个物理区段下的防护区段解锁停稳时间(单位:s) (配置字段)*/
	pPZSysParStr->CtProtectSecUnlockTime =  dquCiShortFromChar(pData); 
	pData+=2;

	/*单轨单开道岔驱动超限时间(单位:s) (配置字段)*/
	pPZSysParStr->SingleSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*单轨三开道岔驱动超限时间(单位:s) (配置字段)*/
	pPZSysParStr->ThreeSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*单轨五开道岔驱动超限时间(单位:s) (配置字段)*/
	pPZSysParStr->FiveSwitchMoveTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	/*间隙探测时间(单位:s) (配置字段)*/
	pPZSysParStr->gapDetectTime =  dquCiShortFromChar(pData); 
	pData+=2; 

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到CI配置表转换
*参数说明：pDataAddr数据地址，pPZSysParStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquCiTransmitInfoFromChar(UINT8 * pDataAddr,DquCiTransmitStruct* pCiTransmitStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL != pDataAddr) && (NULL != pCiTransmitStr))
	{
		
		pCiTransmitStr->seq = dquCiShortFromChar(pData);       /* 序号 */ 
		pData += 2;

		pCiTransmitStr->devType = *pData;  /* 设备类型 */
		pData += 1;
		
		pCiTransmitStr->devId = dquCiShortFromChar(pData);    /*	设备编号	*/ 
		pData += 2;
		
		pCiTransmitStr->srcCiId = dquCiShortFromChar(pData);  /* 发送CI编号	*/ 
		pData += 2;
		
		pCiTransmitStr->dstCiId= dquCiShortFromChar(pData);   /*接收CI编号 */		
		pData += 2;

		pCiTransmitStr->phyLockDirOppOrNot = *pData;	/*物理区段锁闭方向是否取反标志*/
		pData += 1;

		rtnvalue = 1;
	}
	return rtnvalue;

}

/*
功能描述: 将原始数据转换为其他线路设备数据
输入参数:	UINT8 * pDataAddr 数据地址
输出参数:	DquDiffLineDevStruct * pDiffLineDevData 其他线路配置数据结构体指针
返回值:	0 失败
		1 成功
*/
UINT8 PZDquDiffLineDevDataFromChar(UINT8 * pDataAddr, DquDiffLineDevStruct * pDiffLineDevData)
{
	UINT8 retVal = 0;

	if ((NULL != pDataAddr) && (NULL != pDiffLineDevData))
	{
		
		pDiffLineDevData->devSn = dquCiShortFromChar(pDataAddr);	/*序号*/ 
		pDataAddr += 2;

		pDiffLineDevData->devType = *pDataAddr;  					/*设备类型*/
		pDataAddr += 1;
		
		pDiffLineDevData->devId = dquCiShortFromChar(pDataAddr);	/*设备编号*/ 
		pDataAddr += 2;

		retVal = 1;
	}

	return retVal;
}


/*********************************************
*函数功能：从点式临时限速配置数据表到点式临时限速配置结构的转换
*参数说明：pDataAddr数据地址，
*          pDquOutStationSigStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquOutStationSigRelatedLogSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii;
	UINT16 logSecId = 0;


	if ((NULL != pDataAddr) && (NULL != pOutStationSigStr))
	{
		pOutStationSigStr->wRalatedLogSecSum = 0;                       /*关联逻辑区段数量初始化为0*/

		pOutStationSigStr->wSignalId = dquCiShortFromChar(pData);        /* 信号机ID*/ 
		pData += 2;

		for(ii = 0;ii < OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX;ii++)
		{
			logSecId = dquCiShortFromChar(pData);                           /*逻辑区段ID*/
			pData += 2;

			if((0 != logSecId) && (65535 != logSecId))
			{
				pOutStationSigStr->wRalatedLogSecIdBuf[pOutStationSigStr->wRalatedLogSecSum] = logSecId;
				pOutStationSigStr->wRalatedLogSecSum++;
			}
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}

/*********************************************
*函数功能：从功能开关配置数据表到功能开关配置结构的转换
*参数说明：pDataAddr 数据地址，
*          pDquCiFuncSwitchStru CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquCiFunctionSwitchInfoFromChar(UINT8 * pDataAddr, DquCiFuncSwitchStruct * pCiFuncSwitchStru)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii = 0;

	if ((NULL != pDataAddr) && (NULL != pDquCiFuncSwitchStru))
	{
		for(ii = 0; ii < CI_FUNCTION_SWITCH_SUM_MAX;ii++)
		{

			pCiFuncSwitchStru->wCiFuncSwitchBuf[ii] = * pData;    /*功能开关配置*/
			pData += 1;
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}

/*********************************************
*函数功能：从虚拟设备配置数据表到虚拟设备配置结构的转换
*参数说明：pDataAddr数据地址，
*          pVirtualDeviceStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquVirtualDeviceInfoFromChar(UINT8 * pDataAddr, DquVirtualDeviceStruct *pVirtualDeviceStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;

	if ((NULL != pData) && (NULL != pVirtualDeviceStr))
	{

		pVirtualDeviceStr->vDeviceId = dquCiShortFromChar(pData);    /*虚拟设备ID*/ 
		pData += 2;

		pVirtualDeviceStr->vDeviceType = *pData;					 /* 虚拟设备类型*/
		pData += 1;

		rtnvalue = 1;
	}

	return rtnvalue;
}

/*********************************************
*函数功能：从点式临时限速配置数据（计轴区段）表到点式临时限速配置结构的转换
*参数说明：pDataAddr数据地址，
*          pDquOutStationSigStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquOutStationSigRelatedAxisSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr)
{
	UINT8 * pData = pDataAddr;
	UINT8 rtnvalue = 0;
	UINT8 ii;
	UINT16 axisSecId = 0;


	if ((NULL != pDataAddr) && (NULL != pOutStationSigStr))
	{
		pOutStationSigStr->wRelatedAxisSecSum = 0;                       /*关联计轴区段数量初始化为0*/

		pOutStationSigStr->wSignalId = dquCiShortFromChar(pData);        /* 信号机ID*/ 
		pData += 2;

		for(ii = 0;ii < OUTSTATION_SIGNAL_RELATED_AXISSEC_SUM_MAX;ii++)
		{
			axisSecId = dquCiShortFromChar(pData);                           /*计轴区段ID*/
			pData += 2;

			if((0 != axisSecId) && (UINT16_MAX != axisSecId))
			{
				pOutStationSigStr->wRelatedAxisSecIdBuf[pOutStationSigStr->wRelatedAxisSecSum] = axisSecId;
				pOutStationSigStr->wRelatedAxisSecSum++;
			}
		}
		rtnvalue = 1;
	}
	return rtnvalue;

}