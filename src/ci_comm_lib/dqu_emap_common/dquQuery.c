/************************************************************************
* 文件名    ：  dsuQuery.c
* 版权说明  ：  北京交控科技有限公司
* 版本号  	：  1.0
* 创建时间	：	2009.09.26
* 作者  		软件部	
* 功能描述	：	dsu查询函数文件  
* 使用注意	： 
* 修改记录	：	
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dquQuery.h"
#include "dquQueryExp.h"
#include "dquCbtcType.h"

static DSU_EMAP_STRU *qds_dsuEmapStru=NULL;
static DSU_STC_LIMIT_LINKIDX_STRU *qds_dsuLimitStcLinkIdx=NULL;

/*设置当前数据源*/
UINT8 dquSetCurrentQueryDataSource(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx)
{
	UINT8 chReturnValue=0;             /*用于函数返回值*/
	UINT8 CbtcVersionType=0;

	CbtcVersionType = GetCbtcSysType();

	if (DQU_CBTC_FAO == CbtcVersionType)
	{
		if (NULL!=pDsuEmapStru)
		{
			qds_dsuEmapStru=pDsuEmapStru;
			chReturnValue=1;
		}
		else
		{
			chReturnValue=0;
		}
	} 
	else
	{
		if ((NULL!=pDsuEmapStru) && (NULL!=pDsuLimitStcLinkIdx))
		{
			qds_dsuEmapStru=pDsuEmapStru;
			qds_dsuLimitStcLinkIdx=pDsuLimitStcLinkIdx;
			chReturnValue=1;
		}
		else
		{
			chReturnValue=0;
		}
	}
	return chReturnValue;
}
/*
* 功能描述： 利用运行方向信息以及道岔状态信息，查询得到当前link沿运行方向的相邻link。
* 输入参数： UINT16 LinkId, 当前Link编号
*            UCHAR Direction, 当前方向
*            UCHAR PointNum,  道岔信息，道岔个数
*            const POINT_STRU *pPointStru, 道岔信息
* 输出参数： UINT16 *pLinkId，当前Link的相邻Link编号
* 返回值：   1,查询成功
*            0,查询失败,输入数据无效，道岔失表
*		      	 0xFF,线路终点，未找到道岔
*             
*/
UINT8 dsuGetAdjacentLinkID(UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru,UINT16 *pLinkId)
{
	return dsuGetAdjacentLinkIDExp(qds_dsuEmapStru,LinkId,Direction,PointNum,pPointStru,pLinkId);
}


/*
* 功能描述： 功能函数，根据当前位置，和位移以及输入的道岔信息，得到新位置.
如果一个点即当前Lnk起点，又是下一个Link终点，按照离当前点pLODStru最近的Link Id返回
* 输入参数： const LOD_STRU * pLODStru, 当前位置
*            INT32 Displacement,        位移，有符号，如果符号为负表示方向取反
*            UCHAR PointNum,            道岔数量
*            const POINT_STRU * pPointStru,  道岔信息
* 输出参数： LOD_STRU *pNewLODStru       新位置
* 返回值：   1,成功
*            0,失败
*/
UCHAR dsuLODCalcu(const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru) 
{
	return dsuLODCalcuExp(qds_dsuEmapStru,pLODStru,Displacement, PointNum,pPointStru,pNewLODStru);
}

/*
*访问接口
* 功能描述： 由A，B两者位置得到两者位移。
* 输入参数： const LOD_STRU * pLODAStru, A点位置
*            const LOD_STRU * pLODBStru, B点位置
*            UINT8 PointNum,             道岔数量
*            const POINT_STRU * pPointStru, 道岔信息结构体
* 输出参数： INT32 *Displacement，       AB位移，是有符号数。符号表示方向，            
* 返回值：   1,成功
*            0,失败
* 使用注意：合库和燕房项目:按照B的方向查找，AB位移以LinkB方向为参照；
            互联互通项目因增加逻辑方向变化点，算法改为按照A的方向查找，AB位移以LinkA方向为参照。
* author:qxt 20170811
*/
UINT8 dsuDispCalcuByLOC(const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
	UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	return dsuDispCalcuByLOCExp(qds_dsuEmapStru, pLODAStru, pLODBStru,PointNum,pPointStru,Displacement);
}


/*
* 函数功能：	此函数的功能是查询两个位置间包含的link序列。
* ****************************************************************************************************
*        1          2              3                4                    5              6
*       -----×---------*--×--------------×--------------------×-------*-----×------------
*                       S                                                 E
*        图例：  ×，Link端点。*，查询输入的位置。1，2，3...Link示意编号
*        如图所示：Link3，4为所求结果。
*        此处方向按照pLODEndStru方向来查找，
*        S沿着pLODEndStru->Dir方向查找，找到E则返回成功，找不到则返回失败。
* ****************************************************************************************************
* 入口参数：	const LOD_STRU * pLODStartStru,	始端位置
*               const LOD_STRU * pLODEndStru	终端位置
*               const UINT8 PointNum			道岔数目 
*               const POINT_STRU * pPointStru	道岔信息
* 出口参数：	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*               UINT16 *wLinkNumber                             Link个数
* 返回值：   	0,  查询失败
*               1,  查询成功                                                             
* 使用注意：    目前的做法没有包含两端的Link Id。
*               函数要求两个点之间可以形成区域，否则返回0
*/

UINT8 dsuGetLinkBtwLoc(const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
	UINT8 PointNum,const DQU_POINT_STATUS_STRU *pPointStru, 
	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber)
{
	return dsuGetLinkBtwLocExp(qds_dsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,wLinkID,wLinkNumber);	
}


/*
* 函数功能： 此函数的功能是将判断当前位置是否在指定区域范围内。
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*  合库和燕房项目： 查找算法为，计算位移DisplacementSC，DisplacementCE。如果两个位移符号相同，说明C在SE区域内
*  互联互通项目（新增逻辑方向变化点）：   查找算法为，计算位移DisplacementCS，DisplacementCE。如果两个位移符号不相同，说明C在SE区域内
* ****************************************************************************************************
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
*            const UINT8 PointNum,			道岔数目 
*            const POINT_STRU * pPointStru,	道岔信息
*			 const LOD_STRU * pCurrentLODStru,当前位置
* 出口参数： UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
* 使用注意：1.在当前位置在区域端点位置时，暂时按如下规则判断：
*             1) 当前位置在区域起点，认为该点属于本区域；
*             2) 当前位置在区域终点，认为该点不属于本区域；
*             3) 3点重合返回在区域内。
*           2.如果待查找的起点与终点都在区域内，可以调用两次本函数，互换始端与终端位置，只要两次调用有一次在区域内，说明在点在始端与终端形成的闭区域内
*           3.如果SE区域不存在，则函数返回0失败。如果SC,或者SE不存在，则函数返回1成功，但是点C不在区域内
*           4.燕房和合库项目：始端位置和终端位置传入的方向需保证一致
*             互联互通：保证始端位置的方向正确即可。
*/
UINT8 dsuCheckLocInArea(const LOD_STRU * pLODStartStru, const LOD_STRU *pLODEndStru,
	UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
	const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea)
{
	return dsuCheckLocInAreaExp(qds_dsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,pCurrentLODStru,pLocInArea);
}

/************************************************************************

函数功能：	此函数的功能是利用link的ID信息查询link的长度信息
入口参数：	const UINT16 wLinkID		link的ID	
出口参数：	无
返回值：	UINT32 dwLinkLength	查询失败: 0xffffffff; 查询成功: 返回查找到的link长度                                                                
************************************************************************/
/*功能函数，查询link长度*/
UINT32 LinkLength(UINT16 wLinkID)
{
	return LinkLengthExp(qds_dsuEmapStru,wLinkID);
}

/*
* 函数功能： 此函数的功能是将判断当前位置是否在指定区域范围内。
此函数的功能与CheckLocInArea是有区别的。CheckLocInArea对方向没有要求，假设所有涉及的方向都是正确的。
而CheckLocInAreaQuick要求从StartLOD能够到达EndLOD。
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       查找算法为，首先判断当前位置与输入的S，E是否在同一Link上，
*       如果在同一Link上，根据方向和位移判断是否在区域内，如果不在同一Link，判断CLinkId是否与SE之间的某一LinkId相同
*       由于两点之间的Link可以保存下来，所以对于多次调用本函数查询是否在区域内的函数效率很有帮助
*
* ****************************************************************************************************
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
* 出口参数：	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*               UINT16 *wLinkNumber                             Link个数
*			 const LOD_STRU * pCurrentLODStru,当前位置
* 出口参数： UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
* 使用注意：1.在当前位置在区域端点位置时，暂时按如下规则判断：
*             1) 当前位置在区域起点，认为该点属于本区域；
*             2) 当前位置在区域终点，认为该点不属于本区域；
*             3) 3点重合返回在区域内。
*           2.如果待查找的起点与终点都在区域内，可以调用两次本函数，互换始端与终端位置，只要两次调用有一次在区域内，说明在点在始端与终端形成的闭区域内
*           3.如果SE区域不存在，则函数返回0失败。如果SC,或者SE不存在，则函数返回1成功，但是点C不在区域内
*/
UINT8 dsuCheckLocInAreaQuick(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	return dsuCheckLocInAreaQuickExp(qds_dsuEmapStru,pLODStartStru,pLODEndStru,wLinkNumber,wLinkID,pCurrentLODStru,pLocInArea);
}

/*
* 函数功能： 此函数的功能是判断当前位置是否在指定区域范围内，此函数是对dsuCheckLocInAreaQuick
的一个补充。用于将当前方向取反，再计算是否在区域内。
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
* 出口参数： UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*            UINT16 *wLinkNumber                             Link个数
*			 const LOD_STRU * pCurrentLODStru,当前位置
*            UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
*/
UINT8 dsuCheckLocInAreaQuick2(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	return dsuCheckLocInAreaQuickExp2( qds_dsuEmapStru,pLODStartStru,pLODEndStru,wLinkNumber,wLinkID,pCurrentLODStru,pLocInArea);
}

/*****************************以下为坡度平缓计算过程中需要用到的函数**************************/
/************************************************************************

函数功能：	此函数的功能是查询当前Garde的相邻Grade
入口参数：	const UINT16 wCurrentGradeID	当前坡度编号
const UCHAR chDirection			期望运行方向
const UCHAR PointNum			MA范围内道岔数目
const DQU_POINT_STATUS_STRU * pPointStru	MA范围内道岔信息		
出口参数：	无
返回值：	UINT16 wFindGradeID	查询失败: 0xffff; 查询成功: 返回相邻的坡度ID                                                              
************************************************************************/
/*功能函数,查询当前Garde的相邻Grade*/
UINT16 FindAdjacentGradeID(const UINT16 wCurrentGradeID, const UCHAR chDirection, const UCHAR PointNum,
	const DQU_POINT_STATUS_STRU * pPointStru)
{
	return FindAdjacentGradeIDExp(qds_dsuEmapStru,wCurrentGradeID,chDirection,PointNum,pPointStru);
}
/*
*函数功能：查询某点所在坡度ID
*输入参数：UINT16 linkId,UINT32 offset待计算点
*输出参数：UINT16 GradeID所在坡度ID
*返回值：  0函数执行失败，1成功
*/
UINT8 dsuGetGradeIdbyLoc(const UINT16 linkId,const UINT32 offset,UINT16* GradeID)
{
	return dsuGetGradeIdbyLocExp(qds_dsuEmapStru,linkId,offset,GradeID);
}
/*************
*函数功能：计算某点坡度平缓后的坡度值
*输入参数：Point 所求坡度点
*输出参数： 
*返回值：  0函数执行失败 1 点在坡度第一段 2 点在坡度第2段 3 点在坡度第3段
****************/
UINT8 dsuCalculateGradeValue(UINT16 LinkId,UINT32 ofst,FLOAT32* GradeVal)
{
	return dsuCalculateGradeValueExp(qds_dsuEmapStru,LinkId,ofst,GradeVal); 
}

/*************
*函数功能：计算某坡度平缓后，分成的几个部分的分界点与起点的距离
*输入参数：gradeId 坡度ID
*输出参数：UINT32* endPointDistance UINT32* startPointDistance
*返回值：  0函数执行失败   1成功
备注：     如果不需要平缓计算的点，设为无效值ffffffff  返回值为1
****************/
UINT8 dsuDividPointDistanceFromOrigin(UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance)
{
	return dsuDividPointDistanceFromOriginExp(qds_dsuEmapStru,GradeId,linkId,endPointDistance,startPointDistance);
}

/*************
*函数功能：给定坡度上某一点及坡度id，计算该点距离坡度起点的距离
*输入参数：gradeId 坡度ID
UINT16 linkId UINT32 Ofst
*输出参数：UINT32 *length  长度

*返回值：  0函数执行失败   1成功  
****************/
UINT8 dsuGetDistanceFromOrigin(UINT16 GradeId,UINT16 linkId,UINT32 Ofst,UINT32* length)
{
	return dsuGetDistanceFromOriginExp(qds_dsuEmapStru,GradeId,linkId,Ofst,length);	
}

/************************************************************************

函数功能：	此函数的功能是查询给定的两位置间的最差坡度。函数查询起点至
终点范围内所有的线路坡度，并挑出最差坡度（最大下坡）。规定
上坡为正，下坡为负，函数的功能即为挑出范围内坡度的最小值。
函数返回最差坡度的千分数。

入口参数：	const LOD_STRU * pLODStartStru		查询起点位置和方向（实际为列车车尾位置和方向）
const LOD_STRU * pLODEndStru		查询终点位置和方向
const UCHAR PointNum				MA中的道岔数量
const POINT_STRU * pPointStru		道岔信息结构体
出口参数：	FLOAT64 * Gradient					列车车尾至MA终点范围内最差坡度的千分数（上坡为正，下坡为负）
返回值：	UCHAR bReturnValue					 	
0：查询失败；
1：查询正确；
注：在计算的过程中，用的万分值，返回值前，有/10的操作，因此返回
的坡度值为千分值                                                     
************************************************************************/
/*14 两位置间最差坡度查询*/
UCHAR dsuGradientGet(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient)
{
	return dsuGradientGetExp(qds_dsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,Gradient);
}

/*
函数功能：判断后一坡度的坡度值相比前一个坡度的坡度值是增加，不变化还是减少 
输入参数：UINT16 fistGradeID,UINT16 secondGradeID
输出参数：GradeTrend 坡度 
1： 减少  
2： 增加
0： 不变化
返回值：0失败 1成功
*/
UINT8 dquGradeTrend(UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend)
{
	return dquGradeTrendExp(qds_dsuEmapStru,fistGradeID,secondGradeID,GradeTrend);
}

/*
函数功能：获取前面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeFrnt 坡度 
Null：  线路终点或前面坡度为统一坡度  
非Null：前面坡度在坡度表中可查
返回值：0失败 1成功 2线路终点
*/
UINT8 dsuGetGradeFrnt(UINT16 linkId, DSU_GRADE_STRU** gradeFrnt)
{
	return dsuGetGradeFrntExp(qds_dsuEmapStru,linkId,gradeFrnt); 
}

/*
函数功能：获取后面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeRear 坡度 
Null：  线路终点或后面坡度为统一坡度  
非Null：后面坡度在坡度表中可查
返回值：0失败 1成功 2线路终点
*/
UINT8 dsuGetGradeRear(UINT16 linkId, DSU_GRADE_STRU** gradeRear)
{
	return dsuGetGradeRearExp(qds_dsuEmapStru,linkId,gradeRear); 
}

/**
函数功能：获取当前LINK的静态限速个数
输入参数：
@linkId：当前link
输出参数：无
返回值：静态限速个数
*/
UINT8 GetStcLmtSpdCntByLnkId(const UINT16 linkId)
{
	return GetStcLmtSpdCntByLnkIdExp(qds_dsuEmapStru,qds_dsuLimitStcLinkIdx,linkId);
}

/**
函数功能：获取制定LINK的的指定下标的静态临时限速的对象指针
输入参数：
@linkId：当前Link编号
@idx：对应Link的第几个限速，下标取值范围0-9
返回值：
@NULL：获取失败 
@非空：对应限速结构体指针
*/
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkId(const UINT16 linkId,const UINT8 idx)
{
	return GetStcLmtSpdInfoByLnkIdExp(qds_dsuEmapStru,qds_dsuLimitStcLinkIdx,linkId,idx);
}


/*
函数功能：将多开道岔转为虚拟道岔
输入参数：multiPointId 多开道岔编号
          multiPointState 多开道岔状态

输出参数：wVirtualPointIds 拆分后虚拟道岔编号 
          wVirtualPointStates 拆分后虚拟道岔状态
返回值：返回值：拆分后虚拟道岔个数, 0失败  

函数算法：多开道岔包含的虚拟道岔： N位，R1位，L1位，R2位，L2位
            所对应在数组中的索引： 0，  1，   2，   3，   4
		    所对应的多开道岔状态： 1，  2，   3，   4，   5
		   因此当 索引=多开道岔状态multiPointState-1 时，所对应的虚拟道岔保持原值，其余虚拟道岔状态取反（定位<=>反位）

注意事项：输出参数虚拟道岔wVirtualPointIds和状态wVirtualPointStates，对于无效道岔其道岔状态设置为3。如
         
		               / L                                      / L
		              /                                        /
		             /                                        /       101                
		-|------------------------         ===》   ---|------/----------------- N      
		   1         \           N                          102       \
		              \                                                \
					   \                                                \
		              R                                                 R
		           图1.实体三开道岔                            图2.虚拟单开道岔

		   当图1实体三开道岔1的状态为3时，推出图2虚拟单开道岔为L位。此时101为无效道岔，输出其状态为3，返回值道岔个数为2，如下
		   wVirtualPointIds[0]=102;
		   wVirtualPointStates[0]=2;
		   wVirtualPointIds[1]=101;
		   wVirtualPointStates[1]=3;
author:单轨项目 add by qxt 20170622
*/
UINT8 ConvertToVirtualSwitches(const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM])
{
     return ConvertToVirtualSwitchesExp(qds_dsuEmapStru,multiPointId,multiPointState,wVirtualPointIds, wVirtualPointStates);
}

/*
函数功能：将虚拟道岔转为多开道岔
输入参数：wVirtualPointIds[] 虚拟道岔编号 
          wVirtualPointStates[] 虚拟道岔状态
		  wVirtualPointNum    虚拟道岔数量
输出参数：multiPointId 对应多开道岔编号
          multiPointState 对应多开道岔状态
返回值：0失败 1成功

注意事项：输入参数虚拟道岔wVirtualPointIds和状态wVirtualPointStates，对于无效的虚拟道岔，不需要传入。如
           
		 		      /L                                                 L
		             /                                                /
		            /                                                /
		           /                                                /
		---|------/-----------\------ N      =====>        ------------------ N
		          1          2 \                                    \
							    \                                    \
		                          R                                      R
		         图1.虚拟单开道岔                             图2.实体三开道岔
		 
		 当图1虚拟道岔编号1的状态为2时，推出图2多开道岔的状态为L位。此时图一虚拟道岔2为无效的，其输入参数可设置为如下：
         wVirtualPointIds[0]=1;
		 wVirtualPointStates[0]=2;
         wVirtualPointNum=1;
author:单轨项目 add by qxt 20170622
*/
UINT8 ConvertToMultiSwitch(const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState)
{
      return ConvertToMultiSwitchExp(qds_dsuEmapStru,wVirtualPointIds, wVirtualPointStates,  wVirtualPointNum,multiPointId,multiPointState);
}


/************************************************
 函数功能:查询link序列内的道岔信息(车车新增接口)
 输入参数:pLinkStru:Link序列信息
 输出参数:pPointStru:link序列内包含的道岔信息
 返回值:0:失败 1:成功
 备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetPointsBetweenLink(const COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pPointStru)
{
	return dsuGetPointsBetweenLinkExp(qds_dsuEmapStru,pLinkStru,pPointStru);
}


/************************************************
 函数功能:获取当前link是否有折返停车点(不含折返后停车点)(车车新增接口)
 输入参数:wId:Link索引编号
 输出参数:result:0x55有;0xAA:无
 返回值:0:失败 1:成功
 备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuCheckArStopOnLink(const UINT16 wId, UINT8 *result)
{
	return dsuCheckArStopOnLink(qds_dsuEmapStru,wId,result);
}


/************************************************
 函数功能:传入一个位置和一段长度，查找此长度范围内的道岔信息和link序列(车车新增接口)
 * 输入参数：	const LOD_STRU * pLODStru,	始端位置
 const INT32     disPlacement,	查找长度
 * 输出参数：	DQU_LINKS_STRU *pLinkStru		Link序列
 *               DQU_BALISE_STRU *pBaliseStru    道岔序列
 * 返回值：   	0,  查询失败
 *               1,  查询成功                                                             
 * 使用注意：    经过的道岔均按定位处理
 备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetLinkAndBaliseBtwLoc(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, const INT32 Displacement, 
								COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pBaliseStru, LOD_STRU *pEndLodStru)
{
	return dsuGetLinkAndBaliseBtwLocExp(pDsuEmapStru,pLODStru,Displacement,pLinkStru,pBaliseStru, pEndLodStru);
}


/************************************************
 函数功能:通过输入的link序列、车尾位置和配置距离，查询车尾位置至配置距离范围内的link数量和ID(车车新增接口)
 输入参数:pLinkIn:输入的link序列
 pTailLoc:车尾位置
 dis:配置距离		  
 输出参数:pLinkOut:查询到的link数量和ID
 返回值:0:失败 1:成功                                                             
 备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuLinksWithCfgDisInInputLinks(const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis,COM_DQU_OBJECT_STRU *pLinkOut)
{
	return dsuLinksWithCfgDisInInputLinksExp(qds_dsuEmapStru,pLinkIn,pTailLoc,dis,pLinkOut);
}


/************************************************
 函数功能:通过输入的link序列、车尾位置和配置距离，查询车尾位置至配置距离范围内的link所属的OC数量和ID(车车新增接口)
 输入参数:pLinkIn:输入的link序列
 pTailLoc:车尾位置
 dis:配置距离		  
 输出参数:pLinkOut:查询到的OC数量和ID
 返回值:0:失败 1:成功                                                             
 备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcsWithCfgDisInInputLinks(const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis, COM_BASE_COMM_OC_STRU *pOcOut)
{
	return dsuOcsWithCfgDisInInputLinksExp(qds_dsuEmapStru,pLinkIn,pTailLoc,dis,pOcOut);
}


/************************************************
 *函数功能:查找指定距离内两个应答器之间的距离(车车新增接口)
 *输入参数:wBaliseId1:起始应答器ID
 *wBaliseId2:终止应答器ID
 *wMaxLen:指定最大查找距离
 *输出参数:len:应答器之间的距离
 *返回值:0:失败或未找到 1:成功找到                                                             
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetLenBetweenBalise(const UINT32 wBaliseId1, const UINT32 wBaliseId2, const UINT32 wMaxLen, UINT32 *len)
{
	return GetLenBetweenBaliseExp(qds_dsuEmapStru,wBaliseId1,wBaliseId2,wMaxLen,len);
}

/************************************************
 *函数功能:查询设备所属OC(车车新增接口)
 *输入参数:devId:设备ID
 *输出参数:devType:设备类型(1:道岔,2:信号机,3:紧急停车按钮,4:交叉渡线)
 *返回值:0:失败 1:成功                                                             
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcWithInputDevId(const UINT16 devId,const UINT8 devType, UINT16 *ocId)
{
	return dsuOcWithInputDevIdExp(qds_dsuEmapStru,devId,devType,ocId);
}

/************************************************
 *函数功能:查询设备所属OC(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
 *pEnd   查找终点位置(方向无效)
 *pPointAll 从起点到终点的道岔信息(有序排列)
 *输出参数：pSignalOut 查找到的序列
 *返回值:0:失败 1:成功                                                             
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuSignalBetweenPisitions(const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll,COM_DQU_OBJECT_STRU *pSignalOut)
{
	return dsuSignalBetweenPisitionsExp(qds_dsuEmapStru,pStart, pEnd, pPointAll,pSignalOut);
}


/************************************************
 *函数功能:根据link获取对应的OC的ID(车车新增接口)
 *输入参数:linkId
 *输出参数:ocId
 *返回值:0失败,1成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcByLink(const UINT16 wId, UINT16 *ocId)
{
	return dsuOcByLinkExp(qds_dsuEmapStru,wId,ocId);
}


/************************************************
 *函数功能:获取信号机位置(车车新增接口)
 *输入参数:wId:信号机索引编号
 *输出参数:position:信号机所处link以及偏移量
 *返回值:0:失败 1:成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetSignalPosition(const UINT16 wId, LOD_STRU *position)
{
	return dsuGetSignalPositionExp(qds_dsuEmapStru,wId,position);
}

/************************************************
 *函数功能:沿指定方向查找两个位置间的道岔序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
 *pEnd   查找终点位置(方向无效)
 *pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pPointExp 查找到的序列 (0x55:定位 0xAA:反位)
 *返回值：0:失败或未找到 1:成功找到
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetPointsInfoInArea(const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_POINT_STRU *pPointExp)
{
	return GetPointsInfoInAreaExp(qds_dsuEmapStru,pStart,pEnd,pPointAll,pPointExp);
}


/************************************************
 *函数功能:沿指定方向查找两个位置间的PSD序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
 *pEnd   查找终点位置(方向无效)
 *pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pObjectExp 查找到的序列
 *返回值：0:失败或未找到 1:成功找到
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetPsdsInfoInArea(const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	return GetPsdsInfoInAreaExp(qds_dsuEmapStru,pStart,pEnd,pPointAll,pObjectExp);
}

/************************************************
 *函数功能:沿指定方向查找两个位置间的ESB序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
 *pEnd   查找终点位置(方向无效)
 *pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pObjectExp 查找到的序列
 *返回值：0:失败或未找到 1:成功找到
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetEsbsInfoInArea(const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	return GetEsbsInfoInAreaExp(qds_dsuEmapStru,pStart,pEnd,pPointAll,pObjectExp);
}

/************************************************
 *函数功能:查询2个link的相对方向（若LINK B是LINK A的终点相邻LINK  则A到B为正向）(车车新增接口)
 *输入参数:
 *linkAID:LinkA的索引ID
 *linkBID:LinkB的索引ID
 *输出参数:dir:2个link的相对方向  0x55=正向 0xAA=反向  0xFF=无效
 *返回值:0:失败 1:成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetDirOfLinkToLink(const UINT16 linkAID,const UINT16 linkBID,UINT8 *dir)
{
	return dsuGetDirOfLinkToLinkExp(qds_dsuEmapStru,linkAID,linkBID,dir);
}

/************************************************
 *函数功能:根据列车位置获取该位置所属OC(车车新增接口)
 *输入参数：当前位置
 *输出参数：所属OC的ID
 *返回值：0:失败或未找到 1:成功找到
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetLocInOdId(const LOD_STRU *pCurrLoc, UINT16 *pOcID)
{
	return GetLocInOdIdExp(qds_dsuEmapStru,pCurrLoc,pOcID);
}


/************************************************
 *函数功能:根据道岔ID获取道岔的位置(车车新增接口)
 *输入参数：当前位置
 *输出参数：道岔的位置
 *返回值：0:失败或未找到 1:成功找到
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 GetPointLocFromId(const UINT16 PointId, DQU_POINT_LOC_STRU *pPointLoc)
{
	return GetPointLocFromIdExp(qds_dsuEmapStru, PointId, pPointLoc);
}

/************************************************
 *函数功能:根据物理区段序列，查询包含的link序列(车车新增接口)
 *输入参数:		PhysicalSgmt	物理区段信息
 *输出参数:		pLinks link序列信息
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-21
 ************************************************/
UINT8 dsuRegionBlockTolink(const COM_DQU_OBJECT_STRU *PhysicalSgmtStru, COM_DQU_OBJECT_STRU *LinksStru)
{
	return dsuRegionBlockTolinkExp(qds_dsuEmapStru, PhysicalSgmtStru, LinksStru);
}

/************************************************
 *函数功能:根据link编号，查询所属ITS设备ID(车车新增接口)
 *输入参数:		UINT16 LinkWid	link编号
 *输出参数:		UINT16  ItsId 所属ITS设备ID
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-21
 ************************************************/
UINT8 dsuItsIdOfLink(const UINT16 LinkWid, UINT16 *ItsId)
{
	return dsuItsIdOfLinkExp(qds_dsuEmapStru, LinkWid, ItsId);
}

/************************************************
*函数功能:根据限速区段ID获取限速区段信息(车车新增接口)
*输入参数:wId:信号机索引编号
*输出参数:position:信号机所处link以及偏移量
*返回值:0:失败 1:成功
*备注：add by sds 2019-6-17
************************************************/
UINT8 dsuGetRespeedSgmt(const UINT16 wId, DSU_RESPEED_SGMT_STRU *RespeedSgmtStru)
{
	return dsuGetRespeedSgmtExp(qds_dsuEmapStru, wId, RespeedSgmtStru);
}

/************************************************
*函数功能:根据计轴区段序列，查询包含的link序列(车车新增接口)
*输入参数:		PhysicalSgmt	物理区段信息
*输出参数:		pLinks link序列信息
*返回值:0:失败 1:成功
*备注： add by sds 2019-6-21
************************************************/
UINT8 dsuAxleSgmtTolink(const COM_DQU_OBJECT_STRU *pAxleSgmtStru, COM_DQU_OBJECT_STRU *LinksStru)
{
	return dsuAxleSgmtTolinkExp(qds_dsuEmapStru, pAxleSgmtStru, LinksStru);

}


