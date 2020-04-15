/************************************************************************
*
* 文件名   ：  dquCILSStruFrmChar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从数据到进路表数据转换处理过程
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCILSStruFrmChar.h"
#include "dquCIReadCommonChar.h"
/*********************************************
*函数功能：从数据到长进路表转换
*参数说明：pDataAddr数据地址，  pLongRouteStr长进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_LongRouteStruFromChar(UINT8 * pDataAddr,LS_LongRouteStru* pLongRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pLongRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pLongRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<LR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pLongRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到非进路调车表数据结构体转换
*参数说明：pDataAddr数据地址，  pNoRouteDcStr非进路调车数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_NoRouteDcStruFromChar(UINT8 * pDataAddr,LS_NoRouteDcStru* pNoRouteDcStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pNoRouteDcStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	/*ID*/
	pNoRouteDcStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	/*非进路调车包含信号机ID*/
	pNoRouteDcStr->wIncludeSignal =  dquCiShortFromChar(pData);
	pData+=2;

	/*物理区段和检查属性ID*/
	pNoRouteDcStr->wWuliJianchaType =  dquCiShortFromChar(pData);
	pData+=2;

	/*包含道岔编号*/
	for(i=0;i<NR_MAX_INCLUDE_POINT_NUM;i++)
	{
		pNoRouteDcStr->wIncludePoint[i] = dquCiShortFromChar(pData);
		pData+=2;
		pNoRouteDcStr->wIncludePointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	 /*防护道岔Id*/
	pNoRouteDcStr->wFHPointId =  dquCiShortFromChar(pData);
	pData+=2;

	/*非进路调车包含敌对信号结构*/
	for(i=0;i<NR_MAX_INCLUDE_DDXH_NUM;i++)
	{
		pNoRouteDcStr->wDiDuiXinhao[i] = dquCiShortFromChar(pData);
		pData+=2;
		
	}
	
	/*侵限区段ID*/
	for(i=0; i<ROUTE_MAX_QXQD_NUM; i++)
	{
	    pNoRouteDcStr->wIntrudePhySecId[i] = dquCiShortFromChar(pData);
	    pData+=2;
    }
	rtnvalue = 1;
	return rtnvalue;
}


/*********************************************
*函数功能：从数据到自动进路表转换
*参数说明：pDataAddr数据地址，  pAutoRouteStr自动进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AutoRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRouteStru* pAutoRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAutoRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pAutoRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<AR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pAutoRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
    pData+=20;
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到自动折返进路表转换
*参数说明：pDataAddr数据地址，  pAutoRtnRouteStr自动折返进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRtnRouteStru* pAutoRtnRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAutoRtnRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pAutoRtnRouteStr->wId =  dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<ARR_MAX_INCLUDE_ROUTE_NUM;i++)
	{
		pAutoRtnRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
    pData+=20;
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到反向进路表转换
*参数说明：pDataAddr数据地址，  pReverseRouteStr反向进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_ReverseRouteStruFromChar(UINT8 * pDataAddr,LS_ReverseRouteStru* pReverseRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pReverseRouteStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pReverseRouteStr->wId =  dquCiShortFromChar(pData);
		pData+=2;

		for(i=0;i<RR_MAX_INCLUDE_ROUTE_NUM;i++)
		{
			pReverseRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
			pData+=2;
		}
		rtnvalue = 1;	
	}
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到自动进路物理区段及检查类型转换
*参数说明：pDataAddr数据地址， pRoutePhyAndCheckStateStr 自动进路物理区段及检查类型数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RoutePhyAndCheckStateStruFromChar(UINT8 * pDataAddr,LS_RoutePhyAndCheckStateStruct* pRoutePhyAndCheckStateStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRoutePhyAndCheckStateStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pRoutePhyAndCheckStateStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<MAX_INCLUDE_PHYSC_SGMT_NUM;i++)
	{
		pRoutePhyAndCheckStateStr->wPhysicalSgmtId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pRoutePhyAndCheckStateStr->wCheckState[i] = *pData;
		pData++;
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到防护道岔转换
*参数说明：pDataAddr数据地址，  pFHDaoChaStr防护道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_FHDaoChaStruFromChar(UINT8 * pDataAddr,LS_FHDaoChaStruct* pFHDaoChaStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pFHDaoChaStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pFHDaoChaStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<FHDC_MAX_INCLUDE_POINT_NUM;i++)
	{
		pFHDaoChaStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pFHDaoChaStr->wPointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到带动道岔组数据转换
*参数说明：pDataAddr数据地址，  pDrivedSwitchGroupStru防护道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DrvSwiGrpStruFromChar(UINT8 * pDataAddr, LS_RouteDrivedSwitchGroupStruct * pDrivedSwitchGroupStru)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDrivedSwitchGroupStru))
    {
        rtnvalue = 0;
    }
	else
	{
		pDrivedSwitchGroupStru->wId = dquCiShortFromChar(pData);

		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		for(i = 0; i < DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM; i++)
		{
			pDrivedSwitchGroupStru->wPointId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

			pDrivedSwitchGroupStru->wPointSta[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		rtnvalue = 1;
	}

	return rtnvalue;
}


/*********************************************
*函数功能：从数据到敌对信号转换
*参数说明：pDataAddr数据地址， pDDXHStr 敌对信号数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DDXHStruFromChar(UINT8 * pDataAddr,LS_DDXHStruct* pDDXHStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDDXHStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pDDXHStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	pDDXHStr->wDiDuiXinHaoId = dquCiShortFromChar(pData);
	pData+=2;

	for(i=0;i<DDXH_MAX_INCLUDE_POINT_NUM;i++)
	{
		pDDXHStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pDDXHStr->wPointSta[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到敌对信号组转换
*参数说明：pDataAddr数据地址，  pDDXHZStr敌对信号数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DDXHZStruFromChar(UINT8 * pDataAddr,LS_DDXHZStruct* pDDXHZStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pDDXHZStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pDDXHZStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	for(i=0;i<DDXHZ_MAX_DDXH_NUM;i++)
	{
		pDDXHZStr->wDiDuiXinHaoId[i] = dquCiShortFromChar(pData);
	    pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到信号机组转换
*参数说明：pDataAddr数据地址， pSignalStr 信号机数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_SignalStruFromChar(UINT8 * pDataAddr,LS_SignalStruct* pSignalStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pSignalStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pSignalStr->wId = dquCiShortFromChar(pData);
	pData+=2;
	

	for(i=0;i<MAX_INCLUDE_SIGNAL_NUM;i++)
	{
		pSignalStr->wSignalId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到侵限区段及条件道岔表数据结构体转换
*参数说明：pDataAddr数据地址，  pIntruPhySecStr侵限区段及条件道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_IntrudePhySectionStruFromChar(UINT8 * pDataAddr,LS_IntrudePhySectionStruct* pIntruPhySecStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pIntruPhySecStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pIntruPhySecStr->wId = dquCiShortFromChar(pData);
	pData+=2;

	pIntruPhySecStr->wIntrudePhySecId = dquCiShortFromChar(pData);
	pData+=2;
	

	for(i=0;i<QXQD_MAX_INCLUDE_POINT_NUM;i++)
	{
		pIntruPhySecStr->wPointId[i] = dquCiShortFromChar(pData);
		pData+=2;
		pIntruPhySecStr->wPointSta[i] = dquCiShortFromChar(pData);
	    pData+=2;
	}
	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到其他联锁表数据结构体转换
*参数说明：pDataAddr数据地址，  pOtherCIStr其他联锁数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_OtherCIStruFromChar(UINT8 * pDataAddr,LS_OtherCIStruct* pOtherCIStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pOtherCIStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pOtherCIStr->wId = dquCiShortFromChar(pData);
	pData+=2;

#if 0
	for(i=0;i<OTHER_MAX_PSD_NUM;i++)
	{
		pOtherCIStr->wPsdId[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
	for(i=0;i<OTHER_MAX_EMERG_CLSBTN_NUM;i++)
	{
		pOtherCIStr->wEmgClsBtn[i] = dquCiShortFromChar(pData);
		pData+=2;
	}
#else
	/* 一个其他联锁条件支持总共10个设备信息 */
	for(i=0;i<OTHER_CI_MAX_DEV_NUM;i++)
	{
		pOtherCIStr->otherCiDevInfoBuf[i].wDevType = *(pData);
		pData+=1;
		pOtherCIStr->otherCiDevInfoBuf[i].wId = dquCiShortFromChar(pData);
		pData+=2;
		pOtherCIStr->otherCiDevInfoBuf[i].wDevStatus = *(pData);
		pData+=1;
	}
#endif
	rtnvalue = 1;
	return rtnvalue;
}



/*********************************************
*函数功能：从数据到进路数据结构体转换
*参数说明：pDataAddr数据地址，pRouteStr  数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RouteStruFromChar(UINT8 * pDataAddr,LS_RouteStruct* pRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRouteStr))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	 /*进路ID */
	pRouteStr->wRouteId = dquCiShortFromChar(pData);
	pData+=2;

	 /*进路方向 */
	pRouteStr->wDir = *pData;
	pData++;

	 

	/*引导属性*/
	pRouteStr->wYinDao = *pData;
	pData++;

	/*始端按钮对应信号机ID*/
	pRouteStr->wStarBtnSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*变通按钮对应信号机ID*/
	pRouteStr->wFlexibleSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*终端按钮对应信号机ID*/
	pRouteStr->wEndBtnSignal = dquCiShortFromChar(pData);
	pData+=2;

	/*始端信号机显示颜色*/
	pRouteStr->wStarSignalClr = dquCiShortFromChar(pData);
	pData+=2;

	/*始端信号机引导开放颜色*/
	pRouteStr->wStarSigYinDaoClr = dquCiShortFromChar(pData);
	pData+=2;

    /*物理区段及检查属性ID*/ 
	pRouteStr->wPhySecCheckStaId= dquCiShortFromChar(pData);
	pData+=2;

    /*进路防护道岔组ID*/ 
	pRouteStr->wFHDCId= dquCiShortFromChar(pData);
	pData+=2;

	/*敌对信号及条件道岔ID*/
	
    pRouteStr->wDDXHId = dquCiShortFromChar(pData);
	   pData+=2;
	
	 /*侵限区段ID*/
	for(i=0;i<ROUTE_MAX_QXQD_NUM;i++)
	{
		pRouteStr->wIntrudePhySecId[i] = dquCiShortFromChar(pData);
	   pData+=2;
	}

     /*进路折返信号机组ID*/
	pRouteStr->wReturnSignalId = dquCiShortFromChar(pData);
	pData+=2;

	/*防护区段解锁对应停稳物理区段ID*/
	pRouteStr->wFHQD_TW_WLQDId = dquCiShortFromChar(pData);
	pData+=2;

	/*进路相关可封锁信号机组ID*/
	pRouteStr->wFS_SignalId = dquCiShortFromChar(pData);
	pData+=2;

	/*进路开放需关闭信号机组ID*/
	pRouteStr->wSignalClsId = dquCiShortFromChar(pData);
	pData+=2;

	 /*进路开放需开放信号机组ID*/
	pRouteStr->wSignalOpenId = dquCiShortFromChar(pData);
	pData+=2;

	/*不同时开放信号机组*/
	pRouteStr->wBTSOpenId = dquCiShortFromChar(pData);
	pData+=2;

	/*带动道岔组ID*/
	pRouteStr->wDrvSwiGrpId = dquCiShortFromChar(pData);
	pData+=2;
	
	/*CBTC级别其他联锁ID*/
	pRouteStr->wCBTCOtherCIID = dquCiShortFromChar(pData);
	pData+=2;
	  /*点式级别其他联锁ID*/
    pRouteStr->wBLCKOtherCIID = dquCiShortFromChar(pData);
	pData+=2;

	/* 防淹门 ID */
	for(i=0;i<ROUTE_MAX_FLOOD_GATE_NUM;i++)
	{
		pRouteStr->wFloodGateIdBuf[i] = dquCiShortFromChar(pData);
		pData+=2;
	}

	rtnvalue = 1;
	return rtnvalue;
}
/*********************************************
*函数功能：从数据到全自动折返进路表转换
*参数说明：pDataAddr数据地址，  pAllAutoRtnRouteStr全自动折返进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AllAutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AllAutoRtnRouteStru* pAllAutoRtnRouteStr)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pAllAutoRtnRouteStr))
    {
        rtnvalue = 0;
    }
	else
	{
		pAllAutoRtnRouteStr->wId =  dquCiShortFromChar(pData);
		pData+=2;

		for(i=0;i<AARR_MAX_INCLUDE_ROUTE_NUM;i++)
		{
			pAllAutoRtnRouteStr->szIncludeRouteId[i] = dquCiShortFromChar(pData);
			pData+=2;
		}
        pData+=20;
		rtnvalue = 1;
	}
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到保护区段锁闭条件表转换
*参数说明：
				pDataAddr数据地址，
				pOverlapLockCondStru 保护区段锁闭条件数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_OverlapLockCondStruFromChar(UINT8 * pDataAddr, LS_OveralpLockCondStru * pOverlapLockCondStru)
{
	UINT8 * pData = pDataAddr;
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pOverlapLockCondStru))
    {
        rtnvalue = 0;
    }
	else
	{
		/*获取保护区段ID*/
		pOverlapLockCondStru->wOverlapId = dquCiShortFromChar(pData);
		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		/*获取保护区段包含折返进路ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeReturnRouteId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*获取保护区段包含折返轨物理区段ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeRetTrkPhySecId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*获取保护区段包含侵限区段及条件道岔ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeInvSecAndConSwiId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*获取保护区段包含侵限侵限敌对保护区段ID*/
		for(i = 0; i < OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM; i++)
		{
			pOverlapLockCondStru->szIncludeInvOppOverlapId[i] = dquCiShortFromChar(pData);
			pData += LS_DATA_DUAL_BYTE_ELEM_LEN;
		}

		/*获取保护区段锁闭条件防护道岔组ID*/
		pOverlapLockCondStru->overlapLocProSwitchGroupID = dquCiShortFromChar(pData);
		pData += LS_DATA_DUAL_BYTE_ELEM_LEN;

		rtnvalue = 1;
	}
	return rtnvalue;
}

/*********************************************
*函数功能：从数据到单轨道岔联锁关系表转换
*参数说明：
				pDataAddr数据地址，
				pRouteMonoRailSwitStru 单轨道岔联锁关系数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RouteMonoRailSwitStruFromChar(UINT8 * pDataAddr, LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru)
{
	UINT8 * pData = pDataAddr;

	UINT16 i = 0;

	UINT8 rtnvalue = 0;

    if ((NULL == pDataAddr) || (NULL == pRouteMonoRailSwitStru))
    {
        rtnvalue = 0;
        return rtnvalue;
    }

	pRouteMonoRailSwitStru->routeId = dquCiShortFromChar(pData);

	pData += 2;

	for(i = 0; i < ROUTE_MAX_MONO_RAIL_SWITCH_NUM ; i++)
	{
		pRouteMonoRailSwitStru->monoRailSwitIdt[i] = dquCiShortFromChar(pData);
		pData += 2;

		pRouteMonoRailSwitStru->monoRailSwitSta[i] = dquCiShortFromChar(pData);
		pData += 2;
	}

	rtnvalue = 1;

	return rtnvalue;
}


