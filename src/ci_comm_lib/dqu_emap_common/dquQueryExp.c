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

#include "dquQueryExp.h"
#include "dsuVar.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "dquCbtcType.h"

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
UINT8 dsuGetAdjacentLinkIDExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru, UINT16 *pLinkId)
{
	UINT8 chReturnValue=0;             /*用于函数返回值*/
	UINT16 CurrentLinkIndex;           /*当前Link Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;   /*当前Link,获取本地数据使用*/
	UINT16 AdjancentLinkIndex;         /*当前Link Index*/
	DSU_LINK_STRU *pAdjancentLinkStru=NULL; /*当前Link,获取本地数据使用*/
	UINT8 FindFlag = 0;                /*查找标记，用于记录查找是否成功*/
	UINT8 i;                           /*用于循环*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断输入LinkID的有效性*/	
	if((LinkId>LINKINDEXNUM)||(dsuLinkIndex[LinkId] == 0xFFFF))
	{
		/*输入LinkId无效，查询失败，返回*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断输入方向有效性*/
	if((Direction!=EMAP_SAME_DIR)&&(Direction!=EMAP_CONVER_DIR))
	{
		/*输入方向无效，查询失败，返回*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*到此表示输入数据有效*/

	/*根据LinkID获取当前link结构体*/
	CurrentLinkIndex=dsuLinkIndex[LinkId];
	pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

	/*按照输入方向来分情况处理*/
	if(Direction==EMAP_CONVER_DIR)
	{
		/*Link反向查找，按照当前Link始端端点情况分情况处理*/
		if(pCurrentLinkStru->wOrgnJointMainLkId==DSU_NULL_16)
		{
			/*线路终点，当前link无相邻link，返回*/
			chReturnValue = 0xff;
			return chReturnValue;

		}else if(pCurrentLinkStru->wOrgnPointType==EMAP_AXLE_DOT)
		{
			/*计轴点*/
			/*相邻正线Link即为所求*/
			*pLinkId = pCurrentLinkStru->wOrgnJointMainLkId;

			/*查询成功，返回查询结果*/
			chReturnValue=1;
			return chReturnValue;

		}else if(pCurrentLinkStru->wOrgnPointType==EMAP_POINT_DOT)
		{
			/*当前始端端点类型是道岔点*/
			/*判断当前link是否为道岔的汇合Link*/
			if(pCurrentLinkStru->wOrgnJointSideLkId==DSU_NULL_16)
			{
				/*当前link非汇合Link，非混合link相邻Link只有一个，道岔状态也必须正确，以下验证道岔状态是否正确*/

				/*遍历道岔信息，查找当前道岔的状态信息*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wOrgnPointId)
					{
						/*说明已在输入道岔信息中查找到当前Link终端处的道岔*/

						/*置查找位为TRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*没有找到当前道岔信息，当前Link相邻Link可能处于MA范围之外，返回失败*/
					chReturnValue=0xff;
					return chReturnValue;
				}

				/*获取相邻正线Link信息,也就是获取相邻汇合Link信息*/
				AdjancentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wOrgnJointMainLkId];
				pAdjancentLinkStru=dsuStaticHeadStru->pLinkStru+AdjancentLinkIndex;

				if(((pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)&&(LinkId==pAdjancentLinkStru->wTmnlJointMainLkId))
					||((pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)&&(LinkId==pAdjancentLinkStru->wTmnlJointSideLkId)))
				{
					/*道岔状态正确有两种情况：1.相邻道岔为定位，当前Link是相邻Link的正线相邻Iink。
					2.相邻道岔为反位，当前Link是相邻Link的侧线相邻Iink。
					*/
					*pLinkId=pCurrentLinkStru->wOrgnJointMainLkId;
				
					/*查询成功，返回查询结果*/
					chReturnValue=1;
					return chReturnValue;
				}
				else
				{
					/*已找到相邻道岔，但是相邻道岔的道岔状态不对或者道岔状态失表，发生严重错误，函数返回失败*/
					chReturnValue=0;
					return chReturnValue;
				}
				
			}
			else
			{
				/*当前link为道岔的汇合Link*/
				/*遍历道岔信息，查找当前道岔的状态信息*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wOrgnPointId)
					{
						/*说明已在输入道岔信息中查找到当前Link终端处的道岔*/

						/*置查找位为TRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*没有找到当前道岔信息，当前Link相邻Link可能处于MA范围之外，返回失败*/
					chReturnValue=0xff;
					return chReturnValue;
				}
				else
				{
					/*根据道岔的实时状态信息分类讨论*/
					if (pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)
					{
						/*道岔状态位于定位*/
						/*起点相邻正线Link即为所求*/
						*pLinkId=pCurrentLinkStru->wOrgnJointMainLkId;

						/*查询成功，返回查询结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else if(pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)
					{
						/*道岔状态位于反位*/
						/*起点相邻侧线Link即为所求*/
						*pLinkId=pCurrentLinkStru->wOrgnJointSideLkId;
						
						/*查询成功，返回查询结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*道岔失表， POINTLOSESTATUS 失表返回值的处理*/
						chReturnValue=0;
						return chReturnValue;
					}
				}

			}

		}
		else
		{
			/*Link端点类型不合法*/
			chReturnValue=0;
			return chReturnValue;
		}

	}
	else /*if(Direction==SAMEDIR)*/
	{
		/*正向于Link方向查找*/

		/*Link正向查找，按照当前Link终端端点情况分情况处理*/
		if(pCurrentLinkStru->wTmnlJointMainLkId==DSU_NULL_16)
		{
			/*端点类型为线路终点，返回查询结果*/
			chReturnValue = 0xff;
			return chReturnValue;

		}else if(pCurrentLinkStru->wTmnlPointType==EMAP_AXLE_DOT)
		{
			/*当前Link终端端点计轴点*/
			/*相邻正线Link即为所求*/

			*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;
			chReturnValue=1;
			return chReturnValue;

		}else if (pCurrentLinkStru->wTmnlPointType == EMAP_POINT_DOT)
		{
			/*当前Link终端端点道岔点*/

			/*判断当前link是否为道岔的汇合Link*/
			if(pCurrentLinkStru->wTmnlJointSideLkId==DSU_NULL_16)
			{
				/*当前link非汇合Link，非混合link相邻Link只有一个，道岔状态也必须正确，以下验证道岔状态是否正确*/

				/*遍历道岔信息，查找当前道岔的状态信息*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wTmnlPointId)
					{
						/*说明已在输入道岔信息中查找到当前Link终端处的道岔*/

						/*置查找位为TRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*没有找到当前道岔信息，当前Link相邻Link可能处于MA范围之外，返回失败*/
					chReturnValue=0xff;
					return chReturnValue;
				}

				/*获取相邻正线Link信息,也就是获取相邻汇合Link信息*/
				AdjancentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wTmnlJointMainLkId];
				pAdjancentLinkStru=dsuStaticHeadStru->pLinkStru+AdjancentLinkIndex;

				if(((pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)&&(LinkId==pAdjancentLinkStru->wOrgnJointMainLkId))
					||((pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)&&(LinkId==pAdjancentLinkStru->wOrgnJointSideLkId)))
				{
					/*道岔状态正确有两种情况：1.相邻道岔为定位，当前Link是相邻Link的正线相邻Iink。
					2.相邻道岔为反位，当前Link是相邻Link的侧线相邻Iink。
					*/
					*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;

					/*查询成功，返回查询结果*/
					chReturnValue=1;
					return chReturnValue;
				}
				else
				{
					/*已找到相邻道岔，但是相邻道岔的道岔状态不对或者道岔状态失表，发生严重错误，函数返回失败*/
					chReturnValue=0;
					return chReturnValue;
				}
			}
			else
			{
				/*当前汇合Link，按照道岔实时状态分类处理*/

				/*查找当前道岔信息*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wTmnlPointId)
					{
						/*说明已在输入道岔信息中查找到当前Link终端处的道岔*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*没有找到当前道岔信息，当前Link相邻Link可能处于MA范围之外，返回失败*/
					chReturnValue=0xff;
					return chReturnValue;
				}
				else
				{
					/*根据道岔实时状态，分情况处理*/
					if(pPointStru[i].PointStatus==EMAP_POINT_STATUS_MAIN)
					{
						/*当前Link是汇合Link，道岔定位，终点相邻正线Link即为所求*/

						*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;

						/*查询成功，返回查询结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else if (pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)
					{
						/*当前Link是汇合Link，道岔反位，终点相邻侧线Link即为所求*/

						*pLinkId=pCurrentLinkStru->wTmnlJointSideLkId;
						
						/*查询成功，返回查询结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*当前Link是汇合Link，失表  POINTLOSESTATUS，返回失败*/
						chReturnValue=0;
						return chReturnValue;
					}
				}

			}

		}
		else
		{
			/*Link端点类型不合法，函数返回失败*/
			chReturnValue=0;
			return chReturnValue;
		}
	}

	/*此处不可达*/
	
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
*注意：合库和燕房使用
*/
static UCHAR dsuLODCalcuExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 chReturnValue=0;       /*用于函数返回值*/
	UINT16 CurrentLinkId = 0;         /*用于查找过程中临时保存当前LinkID*/
	UINT16 CurrentLinkIndex = 0; /*当前Link Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*用于循环查找过程中临时保存当前Link结构体*/
	UINT8 FindDir = 0;                  /*用于记录实际操作过程中的查找方向*/
	INT32 FindDisplacement = 0;         /*临时保存偏移量*/
	INT32 FindDisplacementSum = 0;       /*用于累加位移偏移量*/
	UINT16 k;                        /*循环用变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断输入LinkID的有效性*/
	if((pLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStru->Lnk] == 0xFFFF))
	{
		/*输入LinkId无效，查询失败，返回*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*判断入口数据偏移量的有效性*/
	if (pLODStru->Off > LinkLengthExp(pDsuEmapStru,pLODStru->Lnk))
	{
		/*数据无效，查询失败，返回*/
		chReturnValue=0;
		return chReturnValue;
	}
	/*判断输入方向有效性*/
	if((pLODStru->Dir!=EMAP_SAME_DIR)&&(pLODStru->Dir!=EMAP_CONVER_DIR))
	{
		/*输入方向无效，查询失败，返回*/
		chReturnValue=0;
		return chReturnValue;
	}	

	/*特殊情况处理，偏移量Displacement为0*/
	if (Displacement == 0)
	{
		/*displacement如果为0，返回当前位置为所求*/
		pNewLODStru->Lnk=pLODStru->Lnk;
		pNewLODStru->Off=pLODStru->Off;
		pNewLODStru->Dir=pLODStru->Dir;

		/*查询成功，返回查询结果*/
		chReturnValue=1;
		return chReturnValue;
	}

	/*如果displacement小于0，则按照displacement绝对值来查找新位置，方向取反*/
	if((pLODStru->Dir==EMAP_SAME_DIR)&&(Displacement>0))
	{
		/*正向查找*/
		FindDir=EMAP_SAME_DIR;
		FindDisplacement=Displacement;
	}
	else if((pLODStru->Dir==EMAP_CONVER_DIR)&&(Displacement<0))
	{
		/*正向查找*/
		FindDir=EMAP_SAME_DIR;
		FindDisplacement=-Displacement;
	}
	else if((pLODStru->Dir==EMAP_SAME_DIR)&&(Displacement<0))
	{
		/*反向查找*/
		FindDir=EMAP_CONVER_DIR;
		FindDisplacement=-Displacement;
	}
	else if((pLODStru->Dir==EMAP_CONVER_DIR)&&(Displacement>0))
	{
		/*反向查找*/
		FindDir=EMAP_CONVER_DIR;
		FindDisplacement=Displacement;
	}
	else
	{
		/*不可达分支*/
	}

	/*根据LinkID从数据库中查找当前link信息*/
	CurrentLinkIndex=dsuLinkIndex[pLODStru->Lnk];
	pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;
	if(FindDir==EMAP_SAME_DIR)
	{
		/*正向查找*/

		/*根据当前位置正向累加偏移量*/
		FindDisplacementSum=pCurrentLinkStru->dwLength-pLODStru->Off;

		/*根据位移判断新位置是否在同一个Link上*/
		if(FindDisplacementSum>=FindDisplacement)
		{
			/*说明在同一个link上，累加位移得到新位置*/
			pNewLODStru->Lnk=pCurrentLinkStru->wId;
			pNewLODStru->Off=pLODStru->Off+FindDisplacement;
			pNewLODStru->Dir=pLODStru->Dir;
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*此处循环并不是为了要遍历每一个Link，而是对循环计数，不能超过LINKINDEXNUM*/
				if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, FindDir, PointNum, pPointStru,&CurrentLinkId) )
				{
					/*pCurrentLinkStru指向相邻Link,相邻Link变为当前Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					FindDisplacementSum+=pCurrentLinkStru->dwLength;
					if(FindDisplacementSum>=FindDisplacement)
					{
						/*累加的位移值加上当前Link长度大于输入偏移量，说明目的位置当前link上*/
						pNewLODStru->Lnk=pCurrentLinkStru->wId;
						pNewLODStru->Off=FindDisplacement-(FindDisplacementSum-pCurrentLinkStru->dwLength);
						pNewLODStru->Dir=pLODStru->Dir;
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*累加的位移值加上当前Link长度小于输入偏移量，说明目的位置不在当前link上，循环继续向前查找下一个Link*/
					}
				}
				else
				{
					/*获取下一个link失败*/
					chReturnValue=0;
					return chReturnValue;
				}
			}
				
			/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
			chReturnValue=0;
			return chReturnValue;
		}
	}
	else
	{
		/*反向查找*/

		/*累加偏移量置0*/
		FindDisplacementSum=0;

		/*根据当前位置反向累加偏移量*/
		FindDisplacementSum=pLODStru->Off;

		if(FindDisplacementSum>=FindDisplacement)
		{
			/*说明在同一个link上*/
			pNewLODStru->Lnk=pCurrentLinkStru->wId;
			pNewLODStru->Off=pLODStru->Off-FindDisplacement;
			pNewLODStru->Dir=pLODStru->Dir;

			/*查询成功，返回查询结果*/
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*此处循环并不是为了要遍历每一个Link，而是对循环计数，不能超过LINKINDEXNUM*/
				if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, FindDir, PointNum, pPointStru,&CurrentLinkId) )
				{
					/*pCurrentLinkStru指向相邻Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					FindDisplacementSum+=pCurrentLinkStru->dwLength;
					if(FindDisplacementSum>=FindDisplacement)
					{
						/*累加的位移值加上当前Link长度大于输入偏移量，说明目的位置当前link上*/
						pNewLODStru->Lnk=pCurrentLinkStru->wId;
						pNewLODStru->Off=(FindDisplacementSum-FindDisplacement);
						pNewLODStru->Dir=pLODStru->Dir;
                        /*查询成功，输出查询结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*累加的位移值加上当前Link长度小于输入偏移量，说明目的位置不在当前link上，循环继续向前查找下一个Link*/
					}
				}
				else
				{
					/*获取下一个link失败*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
			chReturnValue=0;
			return chReturnValue;
		}
	}
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
*author:qxt 20170811 互联互通增加link逻辑方向变化点
*/
static UCHAR dsuLODCalcuExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 chReturnValue=0;       /*用于函数返回值*/
	UINT8 chFlag=1;              /*函数执行失败标记*/
	UINT16 wCurrentLinkId = 0;         /*用于查找过程中临时保存当前LinkID*/
	UINT16 wCurrentLinkIndex = 0; /*当前Link Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*用于循环查找过程中临时保存当前Link结构体*/
	UINT8 wFindDir = 0;                  /*用于记录实际操作过程中的查找方向*/
	INT32 dwFindDisplacement = 0;         /*临时保存偏移量*/
	INT32 dwFindDisplacementSum = 0;       /*用于累加位移偏移量*/
	UINT16 k=0;             /*循环用变量*/
	UINT8 tmpLODDir = 0xff;    /*临时变量,保存入口方向信息*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if ((NULL != pDsuEmapStru)&&(NULL != pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*判断输入LinkID的有效性*/
		if((pLODStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODStru->Lnk]))
		{
			/*输入LinkId无效，查询失败，返回*/
			chFlag=0;
		}

		/*判断入口数据偏移量的有效性*/
		if (pLODStru->Off > LinkLengthExp(pDsuEmapStru,pLODStru->Lnk))
		{
			/*数据无效，查询失败，返回*/
			chFlag=0;
		}
		/*判断输入方向有效性*/
		if((EMAP_SAME_DIR!=pLODStru->Dir)&&(EMAP_CONVER_DIR!=pLODStru->Dir))
		{
			/*输入方向无效，查询失败，返回*/
			chFlag=0;
		}	
	}
	else
	{
		/*电子地图数据指针为空*/
		chFlag = 0;

	}
	
	if (0!=chFlag)
	{
		/*特殊情况处理，偏移量Displacement为0*/
		if (0==Displacement)
		{
			/*displacement如果为0，返回当前位置为所求*/
			pNewLODStru->Lnk=pLODStru->Lnk;
			pNewLODStru->Off=pLODStru->Off;
			pNewLODStru->Dir=pLODStru->Dir;

			/*查询成功，返回查询结果*/
			chReturnValue=1;
		}
		else/*0!=Displacement*/
		{
			tmpLODDir = pLODStru->Dir;
			
			/*如果displacement小于0，则按照displacement绝对值来查找新位置，方向取反*/
			if((EMAP_SAME_DIR==pLODStru->Dir)&&(Displacement>0))
			{
				/*正向查找*/
				wFindDir=EMAP_SAME_DIR;
				dwFindDisplacement=Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(Displacement<0))
			{
				/*正向查找*/
				wFindDir=EMAP_SAME_DIR;
				dwFindDisplacement=-Displacement;
			}
			else if((EMAP_SAME_DIR==pLODStru->Dir)&&(Displacement<0))
			{
				/*反向查找*/
				wFindDir=EMAP_CONVER_DIR;
				dwFindDisplacement=-Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(Displacement>0))
			{
				/*反向查找*/
				wFindDir=EMAP_CONVER_DIR;
				dwFindDisplacement=Displacement;
			}
			else
			{
				/*不可达分支*/
			}

			/*根据LinkID从数据库中查找当前link信息*/
			wCurrentLinkIndex=dsuLinkIndex[pLODStru->Lnk];
			pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

			/*查询的新位置在查询起点所在的Link的情况*/
			if(EMAP_SAME_DIR==wFindDir)
			{
				/*正向查找*/

				/*根据当前位置正向累加偏移量*/
				dwFindDisplacementSum=pCurrentLinkStru->dwLength-pLODStru->Off;

				/*根据位移判断新位置是否在同一个Link上*/
				if(dwFindDisplacementSum>=dwFindDisplacement)
				{
					/*说明在同一个link上，累加位移得到新位置*/
					pNewLODStru->Lnk=pCurrentLinkStru->wId;
					pNewLODStru->Off=pLODStru->Off+dwFindDisplacement;
					pNewLODStru->Dir=pLODStru->Dir;
					chReturnValue=1;
				}
				else
				{
                    /*不作处理*/
				}
			}
			else/*FindDir=EMAP_CONVER_DIR*/
			{
				/*反向查找*/

				/*累加偏移量置0*/
				dwFindDisplacementSum=0;

				/*根据当前位置反向累加偏移量*/
				dwFindDisplacementSum=pLODStru->Off;

				if(dwFindDisplacementSum>=dwFindDisplacement)
				{
					/*说明在同一个link上*/
					pNewLODStru->Lnk=pCurrentLinkStru->wId;
					pNewLODStru->Off=pLODStru->Off-dwFindDisplacement;
					pNewLODStru->Dir=pLODStru->Dir;

					/*查询成功，返回查询结果*/
					chReturnValue=1;
				}
				else
				{
					/*不作处理*/
				}
			}

			/*查询的新位置与查询起点不在同一个Link*/
			if (1!=chReturnValue)
			{
				/*此处循环并不是为了要遍历每一个Link，而是对循环计数，不能超过LINKINDEXNUM*/
				for (k=0;k<LINKINDEXNUM;k++)
				{	
					if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, wFindDir, PointNum, pPointStru,&wCurrentLinkId) )
					{	

						/*判断Link是否为逻辑方向变化点(查询方向为正且该link的终点为逻辑方向变化点 或 查询反方向且该link的起点为逻辑方向变化)*/
						if(((EMAP_SAME_DIR==wFindDir)&&(0x55==pCurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wFindDir)&&(0xaa==pCurrentLinkStru->wLogicDirChanged)))
						{	
							wFindDir =~ wFindDir;/*有变化点，改变查找方向*/
							tmpLODDir =~ tmpLODDir;/*有变化点,改变输出位置逻辑方向*/
						}

						/*pCurrentLinkStru指向相邻Link,相邻Link变为当前Link*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						dwFindDisplacementSum+=pCurrentLinkStru->dwLength;
						if(dwFindDisplacementSum>=dwFindDisplacement)/*累加的位移值加上当前Link长度大于输入偏移量，说明目的位置当前link上*/
						{

							/*最终点所在位置为查找方向反向*/
							if (EMAP_CONVER_DIR==wFindDir)
							{
								pNewLODStru->Lnk=pCurrentLinkStru->wId;
								pNewLODStru->Off=dwFindDisplacementSum-dwFindDisplacement;
								pNewLODStru->Dir=tmpLODDir;
							}
							/*最终点所在位置为查找方向正向 */
							else
							{

								pNewLODStru->Lnk=pCurrentLinkStru->wId;
								pNewLODStru->Off=dwFindDisplacement-(dwFindDisplacementSum-pCurrentLinkStru->dwLength);
								pNewLODStru->Dir=tmpLODDir;
							}					
							chReturnValue=1;
							break;
						}
						else
						{												
							/*累加的位移值加上当前Link长度小于输入偏移量，说明目的位置不在当前link上，循环继续向前查找下一个Link*/
						}
					}
					else
					{
						/*获取下一个link失败*/
						chReturnValue=0;
						break;
					}
				}/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
			}			

		}/*(0!=Displacement) end*/

	}/*if (0!=chFlag) end */
	
	return chReturnValue;

}

/*访问接口*/
UINT8 dsuLODCalcuExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
		rtnValue =dsuLODCalcuExp_HLT(pDsuEmapStru,pLODStru,Displacement, PointNum,pPointStru,pNewLODStru);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue =dsuLODCalcuExp_DFT(pDsuEmapStru,pLODStru,Displacement, PointNum,pPointStru,pNewLODStru);
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}

/*
* 功能描述： 由A，B两者位置和B点方向得到两者位移。
* 输入参数： const LOD_STRU * pLODAStru, A点位置
*            const LOD_STRU * pLODBStru, B点位置
*            UINT8 PointNum,             道岔数量
*            const POINT_STRU * pPointStru, 道岔信息结构体
* 输出参数： INT32 *Displacement，       AB位移，是有符号数。符号表示方向，
             AB位移以LinkB方向为参照。AB方向与LinkB方向相同，位移值取正，否则位移值取负。
* 返回值：   1,成功
*            0,失败
* 使用注意：注意此处只按照B的方向进行查找，未关注A点的方向。（合库和燕房项目使用）
*/
static UINT8 dsuDispCalcuByLOCExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
	UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 chReturnValue = 0;     /*用于函数返回值*/
	UINT8 chFunRtn=0;     /*调用函数返回值*/
	UINT16 CurrentLinkId = 0;     /*用于查找过程中临时保存当前LinkID*/
	UINT16 CurrentLinkIndex; /*查找过程中记录Link Index临时变量*/

	UINT8 AFindFlag=2;/*0，查找失败。1，查找成功。2,继续查找。*/
	UINT8 BFindFlag=2;/*0，查找失败。1，查找成功。2,继续查找。*/
	INT32 AFindDisplacementSum=0;        /*A查找B过程中的位移累加值*/
	INT32 BFindDisplacementSum=0;        /*B查找A过程中位移累加值*/
	DSU_LINK_STRU *pACurrentLinkStru=NULL;    /*A当前Link*/
	DSU_LINK_STRU *pBCurrentLinkStru=NULL;    /*B当前Link*/
	UINT16 k;                        /*循环用变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*预置函数返回值为0*/
	*Displacement=0;

	/*判断输入A LinkID的有效性*/
	if((pLODAStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODAStru->Lnk] == 0xFFFF))
	{
		/*输入LinkId无效,查询失败，返回*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*判断输入B LinkID的有效性*/
	if((pLODBStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODBStru->Lnk] == 0xFFFF))
	{
		/*输入LinkId无效,查询失败，返回*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*判断入口数据linkA偏移量的有效性*/
	if (pLODAStru->Off > LinkLengthExp(pDsuEmapStru,pLODAStru->Lnk))
	{
		/*入口数据异常，查询失败*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*判断入口数据linkB偏移量的有效性*/
	if (pLODBStru->Off > LinkLengthExp(pDsuEmapStru,pLODBStru->Lnk))
	{
		/*入口数据异常，查询失败*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*判断linkB运行方向的正确性*/
	if ((EMAP_SAME_DIR != pLODBStru->Dir) && (EMAP_CONVER_DIR != pLODBStru->Dir))
	{
		/*入口数据异常，查询失败*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*特殊情况处理，LinkA与LinkB处于同一Link上*/
	if(pLODAStru->Lnk == pLODBStru->Lnk)
	{
		/*A,B位于同一link，计算两点间距离信息*/
		*Displacement = pLODBStru->Off-pLODAStru->Off;
		
		if (EMAP_SAME_DIR == pLODBStru->Dir)
		{
			/**/
		}
		else
		{
			/*方向相反，转换位移符号*/
			*Displacement=-(*Displacement);
		}

		/*函数查询成功，返回*/		
		chReturnValue=1;
		return 1;
	}


	/*根据LinkID获取当前link*/
	CurrentLinkIndex=dsuLinkIndex[pLODAStru->Lnk];
	pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;
	CurrentLinkIndex=dsuLinkIndex[pLODBStru->Lnk];
	pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

	/*按照B的方向不同进行分类*/
	/*查找基本方法为A和B各自按照B的方向向前查找，如果找到对方，记录查找到的位移，返回结果*/
	if (pLODBStru->Dir==EMAP_SAME_DIR)
	{
		/*沿正向查找*/
		/*累加位移值*/
		AFindDisplacementSum=pACurrentLinkStru->dwLength-pLODAStru->Off;
		BFindDisplacementSum=pBCurrentLinkStru->dwLength-pLODBStru->Off;
		for (k=0;k<LINKINDEXNUM;k++)
		{
			/*此处循环是为了计数，循环次数不能超过LINKINDEXNUM*/
			if(AFindFlag==2)
			{
				/*A向前查找一个link*/
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, EMAP_SAME_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1==chFunRtn )
				{
					/*pACurrentLinkStru指向相邻Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pACurrentLinkStru->wId==pLODBStru->Lnk)
					{
						/*A找到了B*/

						/*查找成功*/
						AFindFlag=1;
						/*累加偏移量，计算结果值*/
						AFindDisplacementSum+=(pLODBStru->Off);
						/*此时A沿正向找到了B，偏移量为正*/
						*Displacement=AFindDisplacementSum;

						/*查找成功，返回结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*A没有找到B，累加偏移量，继续查找*/
						AFindDisplacementSum+=pACurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*线路终点或者道岔未找到，A点停止查找*/
					AFindFlag=0;
				}
				else
				{
					/*A获取下一个link失败,Link失表或者输入错误*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*B向前查找一个link*/
			if(BFindFlag==2){
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId,EMAP_SAME_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1== chFunRtn)
				{
					/*pBCurrentLinkStru指向相邻Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
					{
						/*B找到了A*/
						BFindFlag=1;

						/*累加偏移量，计算结果值*/
						BFindDisplacementSum+=(pLODAStru->Off);

						/*此时B沿正向找到了A，偏移量为负*/
						*Displacement=-BFindDisplacementSum;

						/*查找成功，返回结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*如果B没有找到A，累加偏移量，继续查找*/
						BFindDisplacementSum+=pBCurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*线路终点或者道岔未找到，停止查找*/
					BFindFlag=0;			
				}
				else
				{
					/*B获取下一个link失败,Link失表或者输入错误*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			if((AFindFlag==0)&&(BFindFlag==0))
			{
				/*A，B均结束查找并且没有找到对方，函数返回失败*/
				chReturnValue=0;
				return chReturnValue;
			}
			else
			{
				/*此时AFindFlag，BFindFlag至少有一个为2，继续查找*/
			}

		}
		
		/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
		chReturnValue=0;
		return chReturnValue;

	}
	else /*if (pLODBStru->Dir==CONVERDIR)*/
	{
		/*A,B均沿反向向查找*/
		AFindDisplacementSum=pLODAStru->Off;
		BFindDisplacementSum=pLODBStru->Off;
		for (k=0;k<LINKINDEXNUM;k++)
		{
			/*此处循环是为了计数，循环次数不能超过LINKINDEXNUM*/
			if(AFindFlag==2)
			{
				/*A向前查找一个link*/
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, EMAP_CONVER_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1==chFunRtn )
				{
					/*pACurrentLinkStru指向相邻Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pACurrentLinkStru->wId==pLODBStru->Lnk)
					{
						/*A找到了B*/

						/*查找成功*/
						AFindFlag=1;
						AFindDisplacementSum+=(pACurrentLinkStru->dwLength-pLODBStru->Off);

						/*此时A沿反向找到了B，偏移量为正*/
						*Displacement=AFindDisplacementSum;

						/*查找成功，返回结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*如果A没有找到B，累加偏移量，继续查找*/
						AFindDisplacementSum+=pACurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*线路终点或者道岔未找到，停止查找*/
					AFindFlag=0;
				}
				else/*( 0==chReturnValue )*/
				{
					/*A获取下一个link失败,Link失表或者输入错误*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*B向前查找一个link*/
			if(BFindFlag==2){
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId, EMAP_CONVER_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1== chFunRtn)
				{
					/*pACurrentLinkStru指向相邻Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
					{
						/*B找到了A*/

						/*查找成功*/
						BFindFlag=1;
						BFindDisplacementSum+=(pBCurrentLinkStru->dwLength-pLODAStru->Off);

						/*此时B沿反向找到了A，偏移量为负*/
						*Displacement=-BFindDisplacementSum;

						/*查找成功，返回结果*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*如果B没有找到A，累加偏移量，继续查找*/
						BFindDisplacementSum+=pBCurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*线路终点或者道岔未找到，停止查找*/
					BFindFlag=0;			
				}
				else /*( 0==chReturnValue )*/
				{
					/*B获取下一个link失败,Link失表或者输入错误*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			if((AFindFlag==0)&&(BFindFlag==0))
			{
				/*A，B均结束查找并且没有找到对方，函数返回失败*/
				chReturnValue=0;
				return chReturnValue;
			}
			else
			{
				/*此时AFindFlag，BFindFlag至少有一个为2，继续查找*/
			}

		}

		/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
		chReturnValue=0;
		return chReturnValue;

	}
}

/*
* 功能描述： 由A，B两者位置和A点方向得到两者位移。
* 输入参数： const LOD_STRU * pLODAStru, A点位置
*            const LOD_STRU * pLODBStru, B点位置
*            UINT8 PointNum,             道岔数量
*            const POINT_STRU * pPointStru, 道岔信息结构体
* 输出参数： INT32 *Displacement，       AB位移，是有符号数。符号表示方向，
             AB位移以LinkA方向为参照。AB方向与LinkA方向相同，位移值取正，否则位移值取负。
* 返回值：   1,成功
*            0,失败
* 使用注意：注意此处只按照A的方向进行查找，未关注B点的方向。（互联互通项目使用）
* author:qxt 20170811
*/
static UINT8 dsuDispCalcuByLOCExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
									  UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 chReturnValue =0;     /*用于函数返回值*/
	UINT8 chFunRtn=0;     /*调用函数返回值*/
	UINT8 chFlag=1;           /*函数执行失败标记*/
	UINT16 wCurrentLinkId = 0;     /*用于查找过程中临时保存当前LinkID*/
	UINT16 wCurrentLinkIndex=0; /*查找过程中记录Link Index临时变量*/

	UINT8 wAFindFlag=2;/*0，查找失败。1，查找成功。2,继续查找。*/
	UINT8 wBFindFlag=2;/*0，查找失败。1，查找成功。2,继续查找。*/
	INT32 dwAFindDisplacementSum=0;        /*A查找B过程中的位移累加值*/
	INT32 dwBFindDisplacementSum=0;        /*B查找A过程中位移累加值*/
	DSU_LINK_STRU *pACurrentLinkStru=NULL;    /*A当前Link*/
	DSU_LINK_STRU *pBCurrentLinkStru=NULL;    /*B当前Link*/
	UINT16 k=0;                        /*循环用变量*/
	UINT8 wAFindDir = 0;               /*用于记录实际操作A查找B过程中的查找方向,初始值为A的方向 */
    UINT8 wBFindDir = 0;               /*用于记录实际操作B查找A过程中的查找方向,初始值为A的方向 */
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

	/*预置函数返回值为0*/
	*Displacement=0;

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*判断输入A LinkID的有效性*/
		if((pLODAStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODAStru->Lnk]))
		{
			/*输入LinkId无效,查询失败*/
			chFlag=0;
		}

		/*判断输入B LinkID的有效性*/
		if((pLODBStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODBStru->Lnk]))
		{
			/*输入LinkId无效,查询失败*/
			chFlag=0;
		}

		/*判断入口数据linkA偏移量的有效性*/
		if (pLODAStru->Off > LinkLengthExp(pDsuEmapStru,pLODAStru->Lnk))
		{
			/*入口数据异常，查询失败*/
			chFlag=0;
		}

		/*判断入口数据linkB偏移量的有效性*/
		if (pLODBStru->Off > LinkLengthExp(pDsuEmapStru,pLODBStru->Lnk))
		{
			/*入口数据异常，查询失败*/
			chFlag=0;
		}

		/*判断linkA运行方向的正确性*/
		if ((EMAP_SAME_DIR != pLODAStru->Dir) && (EMAP_CONVER_DIR != pLODAStru->Dir))
		{
			/*入口数据异常，查询失败*/
			chFlag=0;
		}
	}
	else
	{
		/*电子地图数据指针为空*/
		chFlag = 0;		
	}	

	/*常规防护已完成 继续执行*/
	if (0!=chFlag)
	{
		/*特殊情况处理，LinkA与LinkB处于同一Link上*/
		if(pLODAStru->Lnk == pLODBStru->Lnk)
		{
			/*A,B位于同一link，计算两点间距离信息*/
			*Displacement = pLODBStru->Off-pLODAStru->Off;

			if (EMAP_SAME_DIR == pLODAStru->Dir)
			{
				/*无需处理*/
			}
			else
			{
				/*方向相反，转换位移符号*/
				*Displacement=-(*Displacement);
			}

			/*函数查询成功*/
			chReturnValue = 1;
			
		}
		else/*A、B不在同一link*/
		{
			/*根据LinkID获取当前link*/
			wCurrentLinkIndex=dsuLinkIndex[pLODAStru->Lnk];
			pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;
			wCurrentLinkIndex=dsuLinkIndex[pLODBStru->Lnk];
			pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

			/*初始查找方向*/
			wAFindDir=pLODAStru->Dir;
			wBFindDir=pLODAStru->Dir;
			/*按照A的方向不同进行分类*/
			if (EMAP_SAME_DIR==pLODAStru->Dir)
			{
				/*沿正向查找*/
				/*累加位移值*/
				dwAFindDisplacementSum=pACurrentLinkStru->dwLength-pLODAStru->Off;
				dwBFindDisplacementSum=pBCurrentLinkStru->dwLength-pLODBStru->Off;
			}
			else /*if (pLODAStru->Dir==CONVERDIR)*/
			{
				/*A,B均沿反向向查找*/
				dwAFindDisplacementSum=pLODAStru->Off;
				dwBFindDisplacementSum=pLODBStru->Off;
			}

			/*查找基本方法为A和B各自按照A的方向向前查找，如果找到对方，记录查找到的位移，返回结果*/
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*此处循环是为了计数，循环次数不能超过LINKINDEXNUM*/
				if(2==wAFindFlag)
				{
					/*A向前查找一个link*/
					chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, wAFindDir, PointNum, pPointStru,&wCurrentLinkId);
					if( 1==chFunRtn )
					{
						/*如果该Link为逻辑方向变化点(查询方向为正且该link的终点为逻辑方向变化点 或 查询反方向且该link的起点为逻辑方向变化)*/
						/* 0x55表示link终点为逻辑方向变化点，0xaa表示link起点为逻辑方向变化点*/
						if(((EMAP_SAME_DIR==wAFindDir)&&(0x55==pACurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wAFindDir)&&(0xaa==pACurrentLinkStru->wLogicDirChanged)))
						{							 
							wAFindDir=~wAFindDir;/*有变化点，改变查找方向*/
						}

						/*pACurrentLinkStru指向相邻Link*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						if(pACurrentLinkStru->wId==pLODBStru->Lnk)
						{
							/*A找到了B*/

							/*查找成功*/
							wAFindFlag=1;
							/*累加偏移量，计算结果值*/
							if (EMAP_SAME_DIR==wAFindDir)/*最终找到B点时的查询方向为正时*/
							{
								dwAFindDisplacementSum+=(pLODBStru->Off);
							}
							else/*最终找到B点时的查询方向为反时*/
							{
								dwAFindDisplacementSum+=(pACurrentLinkStru->dwLength-pLODBStru->Off);
							}
							/*此时A沿A的方向找到了B，偏移量为正*/
							*Displacement=dwAFindDisplacementSum;
                             chReturnValue = 1;
							/*查找成功，退出*/
							break;
							
						}
						else
						{
							/*A没有找到B，累加偏移量，继续查找*/
							dwAFindDisplacementSum+=pACurrentLinkStru->dwLength;
						}
					}
					else
					{
						/*A获取下一个link失败,Link失表或者输入错误*/
						wAFindFlag=0;

						/*本来此处应该break  但是发现还有B点未进行查找 因此此处不进行break*/
					}
				}

				/*B向前查找一个link*/
				if(2==wBFindFlag){
					chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId,wBFindDir, PointNum, pPointStru,&wCurrentLinkId);
					if( 1== chFunRtn)
					{
						/*如果该Link为逻辑方向变化点(查询方向为正且该link的终点为逻辑方向变化点 或 查询反方向且该link的起点为逻辑方向变化)*/
						/* 0x55表示link终点为逻辑方向变化点，0xaa表示link起点为逻辑方向变化点*/
						if(((EMAP_SAME_DIR==wBFindDir)&&(0x55==pBCurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wBFindDir)&&(0xaa==pBCurrentLinkStru->wLogicDirChanged)))
						{							 
							wBFindDir=~wBFindDir;/*有变化点，改变查找方向*/
						}
						/*pBCurrentLinkStru指向相邻Link*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
						{
							/*B找到了A*/
							wBFindFlag=1;

							/*累加偏移量，计算结果值*/
							if (EMAP_SAME_DIR==wBFindDir)/*最终找到B点时的查询方向为正时*/
							{
								dwBFindDisplacementSum+=(pLODAStru->Off);
							}
							else/*最终找到B点时的查询方向为反时*/
							{
								dwBFindDisplacementSum+=(pBCurrentLinkStru->dwLength-pLODAStru->Off);
							}

							/*此时B沿A的方向找到了A，偏移量为负*/
							*Displacement=-dwBFindDisplacementSum;
                             chReturnValue = 1;
							/*查找成功，退出*/
							break;
						}
						else
						{
							/*如果B没有找到A，累加偏移量，继续查找*/
							dwBFindDisplacementSum+=pBCurrentLinkStru->dwLength;
						}
					}
					else
					{
						/*A获取下一个link失败,Link失表或者输入错误*/
						wBFindFlag=0;			

						/*本来此处应该break  但是发现还有B点未进行查找 因此此处不进行break*/
					}
				}

				if((0==wAFindFlag)&&(0==wBFindFlag))
				{
					/*A，B均结束查找并且没有找到对方，函数返回失败*/
					chReturnValue=0;
					break;
				}
				else
				{
					/*此时AFindFlag，BFindFlag至少有一个为2，继续查找*/
				}

			}/*for循环end*/

			/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
			/*如果没找到，此时chReturnValue=0*/
		}
	}/*（0!=chReturnValue）end*/

	return chReturnValue;
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
* 使用注意：合库和燕房项目算法是按照B的方向查找，AB位移以LinkB方向为参照；
            互联互通项目因增加逻辑方向变化点，算法改为按照A的方向查找，AB位移以LinkA方向为参照。
* author:qxt 20170811
*/
UINT8 dsuDispCalcuByLOCExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
						   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
		rtnValue=dsuDispCalcuByLOCExp_HLT(pDsuEmapStru, pLODAStru, pLODBStru,PointNum,pPointStru,Displacement);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue=dsuDispCalcuByLOCExp_DFT(pDsuEmapStru, pLODAStru, pLODBStru,PointNum,pPointStru,Displacement);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
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
*               UINT16 *wObjNumber                             Link个数
* 返回值：   	0,  查询失败
*               1,  查询成功                                                             
* 使用注意：    目前的做法没有包含两端的Link Id。
*               函数要求两个点之间可以形成区域，否则返回0
*               合库和燕房项目使用
*/
static UINT8 dsuGetLinkBtwLocExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
					   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
					   UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wObjNumber)
{
	UINT16 chFindObjNum = 0;   /*找到的link的数目*/
	UINT16 wCurrentLinkID = 0;  /*当前link编号*/
	UINT16 wNextLinkID = 0;     /*相邻link编号*/
	UINT16 wFindLink[DSU_MAX_LINK_BETWEEN];/*找到的link序列数组*/
	UINT16 i = 0;           /*循环用变量*/
	UINT8 chReturnValue = 0;/*函数返回值*/
	UINT16 k;                        /*循环用变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*Link个数返回值预置0*/
	*wObjNumber=0;

	/*判断入口数据Link的有效性*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF)
	    ||(pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断入口数据方向的正确性*/
	if ((pLODEndStru->Dir != EMAP_SAME_DIR) && (pLODEndStru->Dir != EMAP_CONVER_DIR))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		chReturnValue = 0;
		return chReturnValue;
	}

	if(pLODStartStru->Lnk==pLODEndStru->Lnk)
	{
		/*始端与终端在同一个Link上，*/

		/*link数为0*/
		*wObjNumber=0;


		if (((pLODEndStru->Dir==EMAP_SAME_DIR)&&(pLODEndStru->Off>=pLODStartStru->Off))
			||((pLODEndStru->Dir==EMAP_CONVER_DIR)&&(pLODEndStru->Off<=pLODStartStru->Off)))
		{	
			/*查询确保函数从起点能够到达终点*/
			/*返回查询成功，退出*/
			chReturnValue = 1;
			return chReturnValue;	
		}
		else
		{
			/*返回查询成功，退出*/
			chReturnValue = 0;
			return chReturnValue;	
		}

	}	
	
	/*将始端link作为当前link*/
	wCurrentLinkID = pLODStartStru->Lnk;

	/*在找到的当前link不等于终端link时，持续while循环*/
	for (k=0;k<LINKINDEXNUM;k++)
	{
		/*此处循环是为了计数，循环次数不能超过LINKINDEXNUM*/

		/*调用功能函数“查询当前link的相邻link”，查询当前link的相邻link*/
		chReturnValue=dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkID, pLODEndStru->Dir,  PointNum, pPointStru,&wNextLinkID);

		/*通过函数返回值，判断是否正确*/
		if (chReturnValue!=1)
		{
			/*查询失败，返回查询错误*/
			chFindObjNum=0;
			chReturnValue = 0;
			return chReturnValue;
		}
		
		/*将找到的相邻link作为新的当前link*/
		wCurrentLinkID=wNextLinkID;
		
		if(wCurrentLinkID != pLODEndStru->Lnk)
		{
			if (chFindObjNum>=DSU_MAX_LINK_BETWEEN)
			{
				/*缓冲区已满，返回失败，此种情况实际执行时不应该出现*/
				chFindObjNum=0;
				chReturnValue = 0;
				return chReturnValue;
			}
			else
			{
				/*当前Link不等于结束Link，更新局部数组，同时将找到的link数目加一*/
				wFindLink[chFindObjNum] = wCurrentLinkID;
				chFindObjNum++;
			}
	
		}
		else
		{
			/*当前Link是结束Link，当前Link不写入结果中，找到终端link，退出循环*/
			break;
		}


	}

	if(k==LINKINDEXNUM)
	{
		/*循环因为k==LINKINDEXNUM结束，说明查找次数超过LINKINDEXNUM，此现象不可能发生*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*退出循环，找到终端link,输出找到的link序列*/
	for (i=0; i<chFindObjNum; i++)
	{
		wLinkID[i] = wFindLink[i];
	}

	/*找到的Link数量赋值*/
	*wObjNumber=chFindObjNum;

	/*返回查询成功，退出*/
	chReturnValue = 1;
	return chReturnValue;	
}

/*
* 函数功能：	此函数的功能是查询两个位置间包含的link序列。
* ****************************************************************************************************
*        1          2              3                4                    5              6
*       -----×---------*--×--------------×--------------------×-------*-----×------------
*                       S                                                 E
*        图例：  ×，Link端点。*，查询输入的位置。1，2，3...Link示意编号
*        如图所示：Link3，4为所求结果。
*        此处方向按照pLODStartStru方向来查找，
*        S沿着pLODStartStru->Dir方向查找，找到E则返回成功，找不到则返回失败。
* ****************************************************************************************************
* 入口参数：	const LOD_STRU * pLODStartStru,	始端位置
*               const LOD_STRU * pLODEndStru	终端位置
*               const UINT8 PointNum			道岔数目 
*               const POINT_STRU * pPointStru	道岔信息
* 出口参数：	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*               UINT16 *wObjNumber                             Link个数
* 返回值：   	0,  查询失败
*               1,  查询成功                                                             
* 使用注意：    目前的做法没有包含两端的Link Id。
*               函数要求两个点之间可以形成区域，否则返回0
*               内部调用函数，互联互通项目使用
*  author: qxt 20170814
*/
static UINT8 dsuGetLinkBtwLocExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
									 UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
									 UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wObjNumber)
{
	UINT16 chFindObjNum = 0;   /*找到的link的数目*/
	UINT16 wCurrentLinkID = 0;  /*当前link编号*/
	UINT16 wNextLinkID = 0;     /*相邻link编号*/
	UINT16 wFindLink[DSU_MAX_LINK_BETWEEN];/*找到的link序列数组*/
	UINT16 i = 0;           /*循环用变量*/
	UINT8 chReturnValue = 0;/*函数返回值*/
	UINT16 k=0;                        /*循环用变量*/
	UINT16 wCurrentLinkIndex = 0; /*当前Link Index */
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*用于循环查找过程中临时保存当前Link结构体*/
	UINT8 wFindDir = 0;               /*用于记录实际操作过程中的查找方向*/
	UINT8 chFlag=1;      /*函数执行失败标识位*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;/*add by qxt 20161029*/
	/*Link个数返回值预置0*/
	*wObjNumber=0;
	
	if ((NULL != pDsuEmapStru)&&(NULL !=pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;/*add by qxt 20161029*/

		/*判断入口数据Link的有效性*/
		if((pLODStartStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16==dsuLinkIndex[pLODStartStru->Lnk])
			||(pLODEndStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16==dsuLinkIndex[pLODEndStru->Lnk]))
		{
			/*入口数据无效，数据异常，查询失败，返回错误*/
			chFlag = 0;
		}

		/*判断入口数据方向的正确性*/
		if ((EMAP_SAME_DIR!=pLODStartStru->Dir) && (EMAP_CONVER_DIR!=pLODStartStru->Dir))
		{
			/*入口数据无效，数据异常，查询失败，返回错误*/
			chFlag = 0;
		}
	}
	else
	{
		/*电子地图数据指针为空*/
		chFlag = 0;
	}

	

	if (0!=chFlag)
	{
		/*始端与终端在同一个Link上，*/
		if(pLODStartStru->Lnk==pLODEndStru->Lnk)
		{			
     		/*link数为0*/
			*wObjNumber=0;
			if (((EMAP_SAME_DIR==pLODStartStru->Dir)&&(pLODEndStru->Off>=pLODStartStru->Off))
				||((EMAP_CONVER_DIR==pLODStartStru->Dir)&&(pLODEndStru->Off<=pLODStartStru->Off)))
			{	
				/*查询确保函数从起点能够到达终点*/
				/*返回查询成功，退出*/
				chReturnValue = 1;
			}
			else
			{
				/*返回查询成功，退出*/
				chReturnValue = 0;
			}

		}
		else/*始端与终端不在同一个Link上 start*/
		{
			/*将始端link作为当前link*/
			wCurrentLinkID = pLODStartStru->Lnk;

			/*根据LinkID从数据库中查找当前link信息*/
			wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkID];
			pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;
			/*将初始查询方向设置为起点的方向 */
			wFindDir=pLODStartStru->Dir;

			/*在找到的当前link不等于终端link时，持续while循环*/ /*此处循环是为了计数，循环次数不能超过LINKINDEXNUM*/
			for (k=0;k<LINKINDEXNUM;k++)
			{		
				/*调用功能函数“查询当前link的相邻link”，查询当前link的相邻link*/
				chReturnValue=dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkID,wFindDir,  PointNum, pPointStru,&wNextLinkID);

				/*通过函数返回值，判断是否正确*/
				if (chReturnValue!=1)
				{
					/*查询失败，返回查询错误*/
					chFindObjNum=0;
					chReturnValue = 0;
					break;
				}


				/*判断该Link是否有逻辑方向变化点(查询方向为正且该link的终点为逻辑方向变化点 或 查询反方向且该link的起点为逻辑方向变化)*/
				if(((EMAP_SAME_DIR==wFindDir)&&(0x55==pCurrentLinkStru->wLogicDirChanged))||  
					((EMAP_CONVER_DIR==wFindDir)&&(0xaa==pCurrentLinkStru->wLogicDirChanged)))
				{							 
					wFindDir=~wFindDir;/*有变化点，改变查找方向*/
				}

				/*将找到的相邻link作为新的当前link*/
				wCurrentLinkID=wNextLinkID;

				/*pCurrentLinkStru指向相邻Link,相邻Link变为当前Link*/
				wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkID];
				pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

				if(wCurrentLinkID != pLODEndStru->Lnk)
				{
					if (chFindObjNum>=DSU_MAX_LINK_BETWEEN)
					{
						/*缓冲区已满，返回失败，此种情况实际执行时不应该出现*/
						chFindObjNum=0;
						chReturnValue = 0;
						break;
					}
					else
					{
						/*当前Link不等于结束Link，更新局部数组，同时将找到的link数目加一*/
						wFindLink[chFindObjNum] = wCurrentLinkID;
						chFindObjNum++;
					}

				}
				else
				{
					/*当前Link是结束Link，当前Link不写入结果中，找到终端link，退出循环*/
					break;
				}

			}/*for循环结束*/

			/*查找次数超过LINKINDEXNUM*/
			if((k!=LINKINDEXNUM)&&(0!=chReturnValue))
			{
				/*退出循环，找到终端link,输出找到的link序列*/
				for (i=0; i<chFindObjNum; i++)
				{
					wLinkID[i] = wFindLink[i];
				}

				/*找到的Link数量赋值*/
				*wObjNumber=chFindObjNum;
				/*返回查询成功*/
				chReturnValue = 1;

			}		

		}/*始端与终端不在同一个Link上 end*/		

	}/*(0!=chFlag) end*/	
	return chReturnValue;	
}

/*访问接口*/
UINT8 dsuGetLinkBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						  UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						  UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wObjNumber)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
		rtnValue=dsuGetLinkBtwLocExp_HLT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,wLinkID,wObjNumber);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue=dsuGetLinkBtwLocExp_DFT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,wLinkID,wObjNumber);
	}
	else
	{
		rtnValue = 0; 
	}
	return rtnValue;
}

/*
* 函数功能： 此函数的功能是将判断当前位置是否在指定区域范围内。
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       查找算法为，计算位移DisplacementSC，DisplacementCE。如果两个位移符号相同，说明C在SE区域内
*
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
*  合库和燕房使用
*/
static UINT8 dsuCheckLocInAreaExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU * pPointStru, 
						const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*函数返回值*/
	INT32 DisplacementSC; /*start -> current 位移*/
	INT32 DisplacementCE; /*current -> end 位移 */
	INT32 DisplacementSE; /*start -> end 位移 */
	LOD_STRU CurrentLODStru; /*另设一个变量来存储当前LOD，方向取pLODEndStru方向，这样在计算位移时，
							  按照统一的方向来计算位移*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}


	/*默认置不在区域内*/
	*pLocInArea=0;

	/*判断入口数据当前位置link编号输入的有效性*/
	if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据当前位置偏移量输入的有效性*/
	if (pCurrentLODStru->Off > LinkLengthExp(pDsuEmapStru,pCurrentLODStru->Lnk))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据当前位置方向的有效性*/
	if ((EMAP_SAME_DIR != pCurrentLODStru->Dir) && (EMAP_CONVER_DIR != pCurrentLODStru->Dir))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	
	/*判断入口数据起点位置link编号输入的有效性*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据起点位置link偏移量输入的有效性*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据起点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断入口数据输入终点位置link编号的有效性*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据终点位置link偏移量输入的有效性*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据终点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}


	CurrentLODStru.Lnk=pCurrentLODStru->Lnk;
	CurrentLODStru.Off=pCurrentLODStru->Off;
	CurrentLODStru.Dir=pLODEndStru->Dir;

	/*利用位移判断3点关系*/

	/*查询DisplacementSE*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, pLODEndStru,  PointNum,   pPointStru, &DisplacementSE);
	if(chReturnValue!=1)
	{
		/*位移查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/*SE区域存在，函数返回值为1*/
		chReturnValue=1;
	}

	/*查询DisplacementSC*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, &CurrentLODStru,  PointNum,   pPointStru, &DisplacementSC);
	if(chReturnValue!=1)
	{
		/*位移查询失败*/
		*pLocInArea=0;
		chReturnValue=0;
		return chReturnValue;
	}

	/*查询DisplacementCE*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,&CurrentLODStru, pLODEndStru,  PointNum,   pPointStru, &DisplacementCE);
	if(chReturnValue!=1)
	{
		/*位移查询失败*/
		*pLocInArea=0;
		chReturnValue=0;
		return chReturnValue;
	}

	if((DisplacementSC==0)&&(DisplacementCE==0))
	{
		/*三点重合*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	if(DisplacementSC==0)
	{
		/*只与区域的起点重合*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	if(DisplacementCE==0)
	{
		/*当前位置在区域终点*/
		*pLocInArea=0;
		chReturnValue = 1;
		return chReturnValue;
	}

	/*判断DisplacementSC，DisplacementCE符号是否相同*/
	if( ((DisplacementSC>0)&&(DisplacementCE>0))||((DisplacementSC<0)&&(DisplacementCE<0)))
	{
		/*DisplacementSC，DisplacementCE符号相同*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}
	else
	{
		/*DisplacementSC，DisplacementCE符号不相同，当前位置不在区域内*/
		*pLocInArea=0;
		chReturnValue = 1;
		return chReturnValue;
	}

}

/*
* 函数功能： 此函数的功能是将判断当前位置是否在指定区域范围内。
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       因新增Link逻辑方向变化点，查找算法改为，计算位移DisplacementCS，DisplacementCE。如果两个位移符号相异，说明C在SE区域内
*
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
*author:qxt 20170811
*/
static UINT8 dsuCheckLocInAreaExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*函数返回值*/
	UINT8 chFlag=1;     /*函数执行失败标识位*/
	UINT8 chRtnCS = 0;/*计算位移CS函数返回值*/
	UINT8 chRtnCE = 0;/*计算位移CE函数返回值*/
	INT32 dwDisplacementCS=0; /*current -> start 位移*/
	INT32 dwDisplacementCE=0; /*current -> end 位移 */
	INT32 dwDisplacementSE=0; /*start -> end 位移 */
	LOD_STRU struCurrentLOD; /*另设一个变量来存储当前LOD，方向取pLODStartStru方向，这样在计算位移时，
							  按照统一的方向来计算位移*/
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;

	/*默认置不在区域内*/
	*pLocInArea=0;

	if ((NULL != pDsuEmapStru)&&(NULL !=pDsuEmapStru->dsuEmapIndexStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;

		/*判断入口数据当前位置link编号输入的有效性*/
		if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据当前位置偏移量输入的有效性*/
		if (pCurrentLODStru->Off > LinkLengthExp(pDsuEmapStru,pCurrentLODStru->Lnk))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据当前位置方向的有效性*/
		if ((EMAP_SAME_DIR != pCurrentLODStru->Dir) && (EMAP_CONVER_DIR != pCurrentLODStru->Dir))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}

		/*判断入口数据起点位置link编号输入的有效性*/
		if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据起点位置link偏移量输入的有效性*/
		if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据起点方向的有效性*/
		if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}

		/*判断入口数据输入终点位置link编号的有效性*/
		if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据终点位置link偏移量输入的有效性*/
		if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
		/*判断入口数据终点方向的有效性*/
		if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
		{
			/*入口参数异常，查询失败*/
			chFlag = 0;
		}
	}
	else
	{
		/*电子地图数据指针为空*/
		chFlag = 0;
	}

	
	if (0!=chFlag)
	{
		/*利用位移判断3点关系*/
		struCurrentLOD.Lnk=pCurrentLODStru->Lnk;
		struCurrentLOD.Off=pCurrentLODStru->Off;
		struCurrentLOD.Dir=pLODStartStru->Dir;
		/*查询DisplacementSE*/
		chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, pLODEndStru,  PointNum,   pPointStru, &dwDisplacementSE);
		if(chReturnValue!=1)
		{
			/*位移查询失败*/
			chReturnValue = 0;
		}
		else/*SE区域存在*/
		{
			
			/*查询DisplacementCS*/
			chRtnCS= dsuDispCalcuByLOCExp(pDsuEmapStru,&struCurrentLOD,pLODStartStru,PointNum,   pPointStru, &dwDisplacementCS);

			/*查询DisplacementCE*/
			chRtnCE= dsuDispCalcuByLOCExp(pDsuEmapStru,&struCurrentLOD,pLODEndStru,PointNum, pPointStru, &dwDisplacementCE);

			if((1!=chRtnCS)||(1!=chRtnCE))
			{
				/*位移查询失败*/
				*pLocInArea=0;
				chReturnValue=0;
			}

			else
			{
				/*三点重合*/
				if((0==dwDisplacementCS)&&(0==dwDisplacementCE))
				{
					*pLocInArea=1;
					chReturnValue = 1;
				}
                /*只与区域的起点重合*/
				else if(0==dwDisplacementCS)
				{
					
					*pLocInArea=1;
					chReturnValue = 1;
				}

				/*当前位置在区域终点*/
				else if(0==dwDisplacementCE)
				{
					
					*pLocInArea=0;
					chReturnValue = 1;
				}

				/*判断DisplacementCS，DisplacementCS符号是否不同 changed by qxt*/
				else if( ((dwDisplacementCS>0)&&(dwDisplacementCE<0))||((dwDisplacementCS<0)&&(dwDisplacementCE>0)))
				{
					/*DisplacementCS，DisplacementCE符号不相同*/
					*pLocInArea=1;
					chReturnValue = 1;
				}
				else
				{
					/*DisplacementCS，DisplacementCE符号相同，当前位置不在区域内*/
					*pLocInArea=0;
					chReturnValue = 1;
				}

			}		
		}/*SE区域存在 end*/	
	}

	return chReturnValue;
}

/*访问接口*/
UINT8 dsuCheckLocInAreaExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
						   UINT8 PointNum, const DQU_POINT_STATUS_STRU * pPointStru, 
						   const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue =dsuCheckLocInAreaExp_HLT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,pCurrentLODStru,pLocInArea);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType)|| (DQU_CBTC_DG == cbtcVersionType)|| (DQU_CBTC_CC == cbtcVersionType))
	{
		rtnValue =dsuCheckLocInAreaExp_DFT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,pCurrentLODStru,pLocInArea);
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}

/************************************************************************

函数功能：	此函数的功能是利用link的ID信息查询link的长度信息
入口参数：	const UINT16 wLinkID		link的ID	
出口参数：	无
返回值：	UINT32 dwLinkLength	查询失败: 0xffffffff; 查询成功: 返回查找到的link长度                                                                
************************************************************************/
/*功能函数，查询link长度*/
UINT32 LinkLengthExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 wLinkID)
{
	UINT32 dwLinkLength = 0;/*函数返回值*/
	DSU_LINK_STRU *pstLinK;/*指向link信息的结构体指针*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		dwLinkLength = 0xffffffff;
		return dwLinkLength;
	}

	/*判断数据输入的有效性*/
	if((wLinkID>LINKINDEXNUM)||(dsuLinkIndex[wLinkID] == 0xFFFF))
	{
		/*无效数据输入，查询失败，返回默认值*/
		dwLinkLength = 0xffffffff;
		return dwLinkLength;
	}
	/*利用管理函数找到link长度信息，返回*/
	pstLinK = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[wLinkID];
	dwLinkLength = pstLinK->dwLength;
	return dwLinkLength;
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
*               UINT16 *wObjNumber                             Link个数
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
UINT8 dsuCheckLocInAreaQuickExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wObjNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*函数返回值*/
	UINT16 i = 0;          /*循环用变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*默认置不在区域内*/
	*pLocInArea=0;

	/*判断入口数据输入的有效性*/
	if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据输入的有效性*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*判断入口数据输入的有效性*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*三点重合，返回在区域内*/
	if((pCurrentLODStru->Off==pLODStartStru->Off)&&(pCurrentLODStru->Off==pLODEndStru->Off)&&
	   (pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
	{
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	/*判断C点是否在S点或者E点所在Lnk上，并且在SE区域内*/

	if(pLODEndStru->Dir==EMAP_SAME_DIR)
	{	
		/*当前方向为正方向*/

		if ((pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
		{
			/*S,C,ELink相同*/
			if ((pCurrentLODStru->Off>=pLODStartStru->Off)&&(pCurrentLODStru->Off<pLODEndStru->Off))
			{
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}

		else if(pCurrentLODStru->Lnk==pLODStartStru->Lnk)
		{
			if (pCurrentLODStru->Off>=pLODStartStru->Off)
			{
				/*当前C点Lnk与S点相同且当前C点Off大于等于S点Off，在区域内*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}

		}
		else if(pCurrentLODStru->Lnk==pLODEndStru->Lnk)
		{
			if (pCurrentLODStru->Off<pLODEndStru->Off)
			{
				/*当前C点Lnk与E点相同且当前C点Off小于E点of，在区域内*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}

		}
		else
		{
			/*不满足，C点与S点或E点不相同Lnk*/
		}
	}
	else if(pLODEndStru->Dir==EMAP_CONVER_DIR)
	{
		/*当前方向为反方向*/

		if ((pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
		{
			if ((pCurrentLODStru->Off<=pLODStartStru->Off)&&(pCurrentLODStru->Off>pLODEndStru->Off))
			{
				/*当前C点Lnk与S点相同且在区域内*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else if(pCurrentLODStru->Lnk==pLODStartStru->Lnk)
		{
			if (pCurrentLODStru->Off<=pLODStartStru->Off)
			{
				/*当前C点Lnk与S点相同且在区域内*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else if(pCurrentLODStru->Lnk==pLODEndStru->Lnk)
		{
			if (pCurrentLODStru->Off>pLODEndStru->Off)
			{
				/*当前C点Lnk与E点相同且在区域内*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else
		{

			/*不满足，C点与S点或E点不相同*/
		}
	}
	else
	{
		/*入口参数异常，查询失败*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*判断C点不在S点或者E点所在Lnk上，循环查找C点Lnk是否与SE中间某一个Lnk相等*/
	for(i=0;i<wObjNumber;i++)
	{
		if(pCurrentLODStru->Lnk==wLinkID[i])
		{
			/*当前C点Lnk与SE区域内的某一个LnkId相同，返回在区域内*/
			*pLocInArea=1;
			chReturnValue = 1;
			return chReturnValue;
		}
	}

	/*当前C点与SE中任意一个Lnk都不相同，返回不在区域内*/
	*pLocInArea=0;
	chReturnValue = 1;
	return chReturnValue;

}

/*
* 函数功能： 此函数的功能是判断当前位置是否在指定区域范围内，此函数是对dsuCheckLocInAreaQuick
             的一个补充。用于将当前方向取反，再计算是否在区域内。
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
* 出口参数： UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*            UINT16 *wObjNumber                             Link个数
*			 const LOD_STRU * pCurrentLODStru,当前位置
*            UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
*/
UINT8 dsuCheckLocInAreaQuickExp2(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wObjNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	LOD_STRU TempLODStartStru;
	LOD_STRU TempLODEndStru;
	LOD_STRU TempCurrentLODStru;


	memcpy(&TempLODStartStru,pLODStartStru,sizeof(LOD_STRU));
	memcpy(&TempLODEndStru,pLODEndStru,sizeof(LOD_STRU));
	memcpy(&TempCurrentLODStru,pCurrentLODStru,sizeof(LOD_STRU));

	/*将三个LOD的方向取反*/
	if (pLODEndStru->Dir==EMAP_SAME_DIR)
	{
		TempLODStartStru.Dir=EMAP_CONVER_DIR;
		TempLODEndStru.Dir=EMAP_CONVER_DIR;
		TempCurrentLODStru.Dir=EMAP_CONVER_DIR;
	}
	else
	{
		TempLODStartStru.Dir=EMAP_SAME_DIR;
		TempLODEndStru.Dir=EMAP_SAME_DIR;
		TempCurrentLODStru.Dir=EMAP_SAME_DIR;
	}

	return dsuCheckLocInAreaQuickExp(pDsuEmapStru,&TempLODStartStru,&TempLODEndStru,wObjNumber,wLinkID,&TempCurrentLODStru,pLocInArea);
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
UINT16 FindAdjacentGradeIDExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wCurrentGradeID, const UCHAR chDirection,
						   const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru)
{
	UINT16 wFindGradeID = 0;/*函数返回值*/
	DSU_GRADE_STRU *pstGrade=NULL;/*指向坡度信息的结构体指针*/
	const DQU_POINT_STATUS_STRU * pstPointStrut=NULL;/*存储道岔信息的局部变量*/
	UINT16 wPointId = 0;/*存储坡度关联道岔点的ID*/
	UCHAR i = 0;/*循环用变量*/
	
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*数据无效，返回查询失败*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}

	/*判断输入的有效性*/
	if((wCurrentGradeID>GRADEINDEXNUM)||(dsuGradeIndex[wCurrentGradeID] == 0xFFFF))
	{
		/*数据无效，返回查询失败*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}	

	/*判断入口数据方向的有效性*/
	if ((EMAP_SAME_DIR != chDirection) && (EMAP_CONVER_DIR != chDirection))
	{
		/*数据无效，返回查询失败*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}
	
	/*在期望运行方向与link的逻辑方向相同时，向当前坡度的末端相邻坡度进行查询*/
	if (EMAP_SAME_DIR == chDirection)
	{

		/*利用管理函数直接找到当前坡度的信息*/
		pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wCurrentGradeID];

		if ((0xffff == pstGrade->wTmnlJointSideGradeId)&&(0xffff == pstGrade->wTmnlJointMainGradeId))
		{
			/*当前坡度已经没有相邻坡度，函数返回0*/
			wFindGradeID = 0xffff;
		     return wFindGradeID;
		}
		/*在当前坡度的末端相邻侧线坡度为空时，其末端相邻正线坡度即为所求*/
		if (0xffff == pstGrade->wTmnlJointSideGradeId)
		{
			/*查询成功，返回相关信息*/
			wFindGradeID = pstGrade->wTmnlJointMainGradeId;
			return wFindGradeID;
		}
		else
		{
			/*在当前坡度的末端相邻侧线坡度不为空时，需利用实时的道岔状态来判断相邻坡度*/

			/*导入道岔信息*/
			pstPointStrut = pPointStru;

			/*遍历道岔信息*/				
			wPointId = pstGrade->wTmnlRelatePointId;

			for (i=0; i<PointNum; i++)
			{
				if (wPointId == pstPointStrut->PointId)
				{
					/*道岔失去表示,数据异常，查询失败，返回默认值，退出*/
					if (EMAP_POINT_STATUS_LOSE == pstPointStrut->PointStatus)
					{
						wFindGradeID = 0xffff;
						return wFindGradeID;
					}
					else
					{
						/*道岔处于定位,其末端相邻正线坡度即为所求*/
						if (EMAP_POINT_STATUS_MAIN == pstPointStrut->PointStatus)
						{
							wFindGradeID = pstGrade->wTmnlJointMainGradeId;
							return wFindGradeID;								
						}
						else
						{
							/*道岔处于反位,其末端相邻侧线坡度即为所求*/
							wFindGradeID = pstGrade->wTmnlJointSideGradeId;
							return wFindGradeID;		
						}							
					}
				}
				pstPointStrut++;
			}

			/*没有找到相关的道岔信息,查询失败,返回默认值*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}				
	}/*end of  if (SAMEDIR == chDirection)*/
	else
	{
		/*在期望运行方向与link的逻辑方向相反时，向当前坡度的始端端相邻坡度进行查询*/



		/*利用管理函数直接找到当前坡度的信息*/
		pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wCurrentGradeID];

		if ((0xffff == pstGrade->wOrgnJointSideGradeId)&&(0xffff == pstGrade->wOrgnJointMainGradeId))
		{
			/*当前坡度已经没有相邻坡度，函数返回0*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}
		/*在当前坡度的始端相邻侧线坡度为空时，其始端相邻正线坡度即为所求*/
		if (0xffff == pstGrade->wOrgnJointSideGradeId)
		{
			/*查询成功，返回坡度ID*/
			wFindGradeID = pstGrade->wOrgnJointMainGradeId;
			return wFindGradeID;
		}
		else
		{
			/*在当前坡度的始端相邻侧线坡度不为空时，需利用实时的道岔状态来判断相邻坡度*/

			/*导入道岔信息*/
			pstPointStrut = pPointStru;

			/*遍历道岔信息*/
			wPointId = pstGrade->wOrgnRelatePointId;

			for (i=0; i<PointNum; i++)
			{
				if (wPointId == pstPointStrut->PointId)
				{
					/*道岔失去表示,数据异常，查询失败，返回默认值，退出*/
					if (EMAP_POINT_STATUS_LOSE == pstPointStrut->PointStatus)
					{
						wFindGradeID = 0xffff;
						return wFindGradeID;
					}
					else
					{
						/*道岔处于定位,其始端相邻正线坡度即为所求*/
						if(EMAP_POINT_STATUS_MAIN == pstPointStrut->PointStatus)
						{
							wFindGradeID = pstGrade->wOrgnJointMainGradeId;
							return wFindGradeID;								
						}
						else
						{
							/*道岔处于反位,其始端相邻侧线坡度即为所求*/
							wFindGradeID = pstGrade->wOrgnJointSideGradeId;
							return wFindGradeID;		
						}							
					}
				}
				pstPointStrut++;
			}

			/*没有找到相关的道岔信息,查询失败,返回默认值*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}
	}
}
/*
*函数功能：查询某点所在坡度ID
*输入参数：UINT16 linkId,UINT32 offset待计算点
*输出参数：UINT16 GradeID所在坡度ID
*返回值：  0函数执行失败，1成功
*/
static UINT8 dsuGetGradeIdbyLocExp_DFT(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT16 i=0,j=0;							/*循环计数变量*/
	DSU_GRADE_STRU* curGrade=NULL;          		/*当前坡度临时变量*/
	UINT8 bReturnValue;						/*函数返回值变量*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link信息结构体指针*/
	
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != pDsuEmapStru) && (NULL != GradeID))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	if((linkId>LINKINDEXNUM)||((dsuLinkIndex[linkId]) == 0xFFFF))
	{
		/*入口参数异常，查询失败*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*获取当前位置的link对应的位置信息行*/
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	/*当前link的“Link坡度信息属性”为无统一坡度*/
	if(0 == pLinkStru->wGrade)
	{
		for(i=0;i<dsuDataLenStru->wGradeLen;i++)
		{
			/*初始化当前坡度*/
			curGrade = dsuStaticHeadStru->pGradeStru + i;
			/*如果起始link与输入link相同*/
			if(linkId == curGrade->wOrgnLinkId)
			{
				/*输入偏移量>=当前坡度起点偏移量*/
				if((offset>=curGrade->dwOrgnLinkOfst) && ((offset - curGrade->dwOrgnLinkOfst)<= curGrade->dwGradeLength))
				{
					/*输入当前坡度ID*/
					*GradeID = curGrade->wId;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{
					/*继续向下执行*/
				}
			}
			/*如果终端link与输入link相同*/
			else if(linkId == curGrade->wTmnlLinkId)
			{
				/*输入偏移量<=当前坡度终点偏移量*/
				if(offset <= curGrade->dwTmnlLinkOfst)
				{
					/*输入当前坡度ID*/
					*GradeID = curGrade->wId;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{
					/*继续向下执行*/
				}
			}
			/*与中间link相同*/
			else
			{
				j=0;
				while(0xffff !=(curGrade->wIncludeLinkId[j]))
				{
					if(linkId == (curGrade->wIncludeLinkId[j]))
					{
						*GradeID = curGrade->wId;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						j++;
					}
				}
			}
		}
		/*遍历完坡度，没找到该点*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	/*当前link的“Link坡度信息属性”为统一坡度*/
	{
		/*统一坡度不需要使用坡度ID信息,置为无效*/
		*GradeID = 0xffff;
		bReturnValue = 1;
		return bReturnValue;
	}
}

/*
*函数功能：查询某点所在坡度ID
*输入参数：UINT16 linkId,UINT32 offset待计算点
*输出参数：UINT16 GradeID所在坡度ID
*返回值：  0函数执行失败，1成功
*/
static UINT8 dsuGetGradeIdbyLocExp_CPK(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT16 i = 0;							/*循环计数变量*/
	UINT8 bReturnValue = 0;					/*函数返回值变量*/
	DSU_LINK_STRU *pLinkStru = NULL;		/*Link信息结构体指针*/
	DSU_GRADE_STRU *pGradeStru = NULL;		/*坡度信息结构体指针*/

	DSU_GRADE_LINKIDX_STRU *pTempGradeLinkIdxStru = NULL;
	UINT16 tempGradeID = 0;

	UINT16 LINKINDEXNUM = 0;
	UINT16 *dsuLinkIndex = NULL;
	UINT16 *dsuGradeIndex = NULL;			/*坡度信息索引数组*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;

	if ((NULL != pDsuEmapStru) && (NULL != pDsuEmapStru->dsuEmapIndexStru) && (NULL != pDsuEmapStru->dsuStaticHeadStru) && (NULL != GradeID))
	{
		/*全局变量改传参调用涉及的全局变量*/
		LINKINDEXNUM = pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex = pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex = pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru = pDsuEmapStru->dsuStaticHeadStru;

		if ((linkId <= LINKINDEXNUM) && (DSU_NULL_16 != dsuLinkIndex[linkId]))
		{
			/*获取当前位置的link对应的位置信息行*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

			if((offset >= 0) && (offset <= pLinkStru->dwLength))
			{
				pTempGradeLinkIdxStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
				pTempGradeLinkIdxStru = pTempGradeLinkIdxStru + pLinkStru->wId;

				if (0 != pLinkStru->wGrade)/*当前link的“Link坡度信息属性”为统一坡度*/
				{
					if (0 == pTempGradeLinkIdxStru->wGradeCount)
					{
						*GradeID = DSU_NULL_16;/*统一坡度不需要使用坡度ID信息,置为无效*/
						bReturnValue = 1;
					}
					else
					{
						/*错误分支*/
						bReturnValue = 0;
					}			
				}
				else/*当前link的“Link坡度信息属性”为无统一坡度*/
				{
					if (1 == pTempGradeLinkIdxStru->wGradeCount)
					{
						tempGradeID = pTempGradeLinkIdxStru->wGradeId[0];
						*GradeID = tempGradeID;
						bReturnValue = 1;
					} 
					else if (1 < pTempGradeLinkIdxStru->wGradeCount)
					{
						for (i = 0; i < pTempGradeLinkIdxStru->wGradeCount; i++)
						{
							tempGradeID = pTempGradeLinkIdxStru->wGradeId[i];

							/*获取当前坡度对应的信息行*/
							pGradeStru = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[tempGradeID];

							/*当一个link上有多个坡度时，处在该link中间某段的坡度必不含中间link，故只需判断该link上每个坡度的首、尾link与输入的参数linkId是否相同即可。*/
							if ((linkId == pGradeStru->wOrgnLinkId) && (linkId == pGradeStru->wTmnlLinkId))/*一个link对应多个坡度，且坡度位于link的中间某段*/
							{
								if ((offset >= pGradeStru->dwOrgnLinkOfst) && ((offset - pGradeStru->dwOrgnLinkOfst) <= pGradeStru->dwGradeLength))
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 
							} 
							else if ((linkId != pGradeStru->wOrgnLinkId) && (linkId == pGradeStru->wTmnlLinkId))/*一个link对应多个坡度，且坡度位于link的始端*/
							{
								if (offset <= pGradeStru->dwTmnlLinkOfst)
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 								
							}
							else if ((linkId == pGradeStru->wOrgnLinkId) && (linkId != pGradeStru->wTmnlLinkId))/*一个link对应多个坡度，且坡度位于link的终端*/
							{
								if (offset >= pGradeStru->dwOrgnLinkOfst)
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 								
							}
							else
							{
								/*错误分支*/
								bReturnValue = 0;
							}

						}
						/*link上没有找到对应的坡度*/
						if(i == pTempGradeLinkIdxStru->wGradeCount)
						{		   
						   bReturnValue = 0;
						}
					}
					else
					{
						/*错误分支*/
						bReturnValue = 0;
					}
				}
			}
			else
			{
				/*输入偏移量参数不合法，查询失败*/
				bReturnValue = 0;
			}			
		} 
		else
		{
			/*输入link编号异常，查询失败*/
			bReturnValue = 0;
		}				
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
	}	 
	return bReturnValue;
}

/*
*函数功能：查询某点所在坡度ID
*输入参数：UINT16 linkId,UINT32 offset待计算点
*输出参数：UINT16 GradeID所在坡度ID
*返回值：  0函数执行失败，1成功
*修改说明：对CPK/HLT查询某点所在坡度ID算法进行了优化，共同使用dsuGetGradeIdbyLocExp_CPK，
*		  FAO/DG继续使用原有算法，
*         modify by lmy 20171221
*/
UINT8 dsuGetGradeIdbyLocExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType = 0;
	cbtcVersionType = GetCbtcSysType();

	if((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType))
	{
		rtnValue = dsuGetGradeIdbyLocExp_CPK(pDsuEmapStru, linkId, offset, GradeID);
	} 
	else if((DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue = dsuGetGradeIdbyLocExp_DFT(pDsuEmapStru, linkId, offset, GradeID);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
}

/*************
*函数功能：计算某点坡度平缓后的坡度值
*输入参数：Point 所求坡度点
*输出参数： 
*返回值：  0函数执行失败 1 点在坡度第一段 2 点在坡度第2段 3 点在坡度第3段
****************/
UINT8 dsuCalculateGradeValueExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32* GradeVal)
{
	UINT8  funcRtn;
	UINT16 gradeId;             /*点所在坡度ID*/
	UINT8  bReturnValue = 0;    /*函数返回值*/
	UINT32 arcEndOfst  = 0;     /*坡度第一个分界点所处相对位置*/
	UINT32 arcStartOfst = 0;    /*坡度第二个分界点所处相对位置*/
	UINT32 relLocation = 0;     /*当前点所处相对位置*/
	FLOAT64 angleValue = 0;     /*角度变化值*/
	FLOAT64 tempAngleValue = 0;     /*计算出的临时值*/
	DSU_GRADE_STRU* curGrade=NULL;        /*当前坡度临时变量*/
	DSU_GRADE_STRU  curGradeTmp;        /*当前坡度临时变量*/
	DSU_GRADE_STRU* frontGrade=NULL;        /*当前坡度前方相邻坡度临时变量*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*当前坡度后方相邻坡度临时变量*/
	FLOAT32 curGradeVal;
	FLOAT64 tempValue;              /*临时变量，用来存表达式计算值*/
	UINT8 GradeTrend = 0;
	UINT8 RtnGetGradFrnt = 0;
	UINT8 RtnGetGradNxt = 0;

	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*计算出点所在坡度*/
	funcRtn = dsuGetGradeIdbyLocExp(pDsuEmapStru,LinkId,ofst,&gradeId);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*计算出点所在坡度两分界点所处相对位置*/
	funcRtn = dsuDividPointDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,&arcEndOfst,&arcStartOfst);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*计算出点所在坡度两分界点所处相对位置*/
	funcRtn = dsuGetDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,ofst,&relLocation);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}

	/*为了方便计算，对分界点的相对距离进行以下处理*/
	if(0xFFFFFFFF == arcEndOfst)
	{
		arcEndOfst = 0;
	}

	if(0xffff != gradeId)
	{
		/*初始化当前坡度*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[gradeId];
		/*方向为正，坡度值为正*/
		if(0x55 == curGrade->wInclineDir)
		{
			curGradeVal = (FLOAT32)(1.0*curGrade->wGradValue)/10000;
		}
		else
			/*方向反，坡度值为负*/
		{
			curGradeVal = (FLOAT32)(-1.0*curGrade->wGradValue)/10000;
		}

		/*获取前面相邻坡度*/
		if(0xffff != curGrade->wOrgnJointMainGradeId)
		{
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		else
		{
			/*线路终点或关联统一坡度,不再进行坡度平缓*/
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*返回成功*/
			bReturnValue = 2;
			return bReturnValue;
		}

		/*获取后面相邻坡度*/
		if(0xffff != curGrade->wTmnlJointMainGradeId)
		{
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		else
		{
			/*线路终点或关联统一坡度,不再进行坡度平缓*/
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*返回成功*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	else
	{	
		/*获取当前的统一坡度值,因为0不再进行运算和方向判定*/
		curGradeVal = dsuStaticHeadStru->pConGradeStru->wGrade;

		/*当前为统一坡度时,为方便后面计算,在该处赋值*/
		curGradeTmp.wId = 0xFFFF;
		curGradeTmp.dwRadius = 0;
		curGradeTmp.wInclineDir = 0x55;
		curGradeTmp.wGradValue = curGradeVal;
		curGrade = &curGradeTmp;


		/*使用前的初始化*/
		frontGrade = NULL;
		rearGrade = NULL;

		/*获取前面相邻坡度*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,LinkId, &frontGrade);

		/*获取后面相邻坡度*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,LinkId, &rearGrade);

		/*若前面或后面相邻坡度查询失败*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}

		/*20141122添加：若查询前面和后面坡度时,发现当前坡度在线路终点,不再进行平缓*/
		if((2==RtnGetGradFrnt)||(2==RtnGetGradNxt))
		{
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*返回成功*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*
	1 没有分界点的时候，即相邻坡度与当前坡度差小于千分之20
	2 当前点处于分界点所标示的弧度外
	*/
	if((relLocation >= arcEndOfst) && (relLocation <= arcStartOfst))
	{

		* GradeVal = (FLOAT32)(10000*curGradeVal);
		/*返回成功*/
		bReturnValue = 2;
		return bReturnValue;
	}
	/*当前点在第一个分界点弧度内*/
	else if(relLocation < arcEndOfst)
	{
		if(NULL != frontGrade)
		{
			/*计算角度变化值*/
			if(0 == frontGrade->dwRadius)
			{
				/*曲率半径为0，角度变化值为0*/
				angleValue = 0;
			}
			else
			{
				/*角度变化值*/
				angleValue = (FLOAT32)((arcEndOfst + relLocation)/(1.0*frontGrade->dwRadius));
			}
			/*坡度是上坡，取正值*/
			if(0x55 == frontGrade->wInclineDir)
			{
				tempValue = (1.0*frontGrade->wGradValue)/10000;
			}
			/*下坡取负值*/
			else 
			{
				tempValue = (-1.0*frontGrade->wGradValue)/10000;
			}
			/*if用来判断角度变化值是增加值还是减少值*/
			if(dquGradeTrendExp(pDsuEmapStru,frontGrade->wId,curGrade->wId,&GradeTrend))
			{
				/*坡度减少*/
				if(1 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue - angleValue;
				}
				/*坡度增加*/
				else if(2 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue + angleValue;
				}
				/*坡度不变化*/
				else
				{
					tempAngleValue = tempValue ;
				}

				/*转换为FLOAT32类型*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*返回成功*/
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				/*不可达*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*转换为FLOAT32类型*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*返回成功*/
			bReturnValue = 1;
			return bReturnValue;
		}
	}
	/*在第三分界段内*/
	else
	{
		if(NULL != rearGrade)
		{
			/*计算角度变化值*/
			if(0 == curGrade->dwRadius)
			{
				/*曲率半径为0，角度变化值为0*/
				angleValue = 0;
			}
			else
			{
				/*角度变化值*/
				angleValue = (FLOAT32)((relLocation - arcStartOfst)/(1.0*curGrade->dwRadius));
			}

			if(0x55 == curGrade->wInclineDir)
			{
				tempValue = (1.0*curGrade->wGradValue)/10000;
			}
			else
			{
				tempValue = (-1.0*curGrade->wGradValue)/10000;
			}

			/*if用来判断角度变化值是增加值还是减少值*/
			if(dquGradeTrendExp(pDsuEmapStru,curGrade->wId,rearGrade->wId,&GradeTrend))
			{
				/*坡度值减少*/
				if(1 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue - angleValue;				
				}
				/*坡度值增加*/
				else if(2 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue + angleValue;
				}
				/*坡度值不变化*/
				else
				{
					tempAngleValue = tempValue;
				}

				/*转换为FLOAT32类型*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*返回成功*/
				bReturnValue = 3;
				return bReturnValue;
			}
			else
			{
				/*不可达*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*转换为FLOAT32类型*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*返回成功*/
			bReturnValue = 3;
			return bReturnValue;
		}
	}	
}

UINT8 dsuCalculateGradeValueExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32* GradeVal)
{
	UINT8  funcRtn;
	UINT16 gradeId;             /*点所在坡度ID*/
	UINT8  bReturnValue = 0;    /*函数返回值*/
	UINT32 arcEndOfst  = 0;     /*坡度第一个分界点所处相对位置*/
	UINT32 arcStartOfst = 0;    /*坡度第二个分界点所处相对位置*/
	UINT32 relLocation = 0;     /*当前点所处相对位置*/
	FLOAT64 angleValue = 0;     /*角度变化值*/
    FLOAT64 tempAngleValue = 0;     /*计算出的临时值*/
    DSU_GRADE_STRU* curGrade=NULL;        /*当前坡度临时变量*/
    DSU_GRADE_STRU  curGradeTmp;        /*当前坡度临时变量*/
	DSU_GRADE_STRU* frontGrade=NULL;        /*当前坡度前方相邻坡度临时变量*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*当前坡度后方相邻坡度临时变量*/
	FLOAT32 curGradeVal;
	FLOAT64 tempValue;              /*临时变量，用来存表达式计算值*/
	UINT8 GradeTrend = 0;
	UINT8 RtnGetGradFrnt = 0;
	UINT8 RtnGetGradNxt = 0;

	/*全局变量改传参调用涉及的全局变量*/
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*计算出点所在坡度*/
	funcRtn = dsuGetGradeIdbyLocExp(pDsuEmapStru,LinkId,ofst,&gradeId);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*计算出点所在坡度两分界点所处相对位置*/
	funcRtn = dsuDividPointDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,&arcEndOfst,&arcStartOfst);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*计算出点所在坡度两分界点所处相对位置*/
	funcRtn = dsuGetDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,ofst,&relLocation);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}

	/*为了方便计算，对分界点的相对距离进行以下处理*/
	if(0xFFFFFFFF == arcEndOfst)
	{
		arcEndOfst = 0;
	}
	
	if(0xffff != gradeId)
	{
		/*初始化当前坡度*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[gradeId];
		/*方向为正，坡度值为正*/
		if(0x55 == curGrade->wInclineDir)
		{
			curGradeVal = (FLOAT32)(1.0*curGrade->wGradValue)/10000;
		}
		else
		/*方向反，坡度值为负*/
		{
			curGradeVal = (FLOAT32)(-1.0*curGrade->wGradValue)/10000;
		}
		
		/*获取前面相邻坡度*/
		if(0xffff != curGrade->wOrgnJointMainGradeId)
		{
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		/*else
		{
			*GradeVal = (FLOAT32)(10000*curGradeVal);
			bReturnValue = 2;
			return bReturnValue;
		}*/
		
		/*获取后面相邻坡度*/
		if(0xffff != curGrade->wTmnlJointMainGradeId)
		{
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		/*else
		{
			*GradeVal = (FLOAT32)(10000*curGradeVal);
			bReturnValue = 2;
			return bReturnValue;
		}*/
	}
	else
	{	
		/*获取当前的统一坡度值,因为0不再进行运算和方向判定*/
		curGradeVal = dsuStaticHeadStru->pConGradeStru->wGrade;
		
		/*当前为统一坡度时,为方便后面计算,在该处赋值*/
		curGradeTmp.wId = 0xFFFF;
		curGradeTmp.dwRadius = 0;
		curGradeTmp.wInclineDir = 0x55;
		curGradeTmp.wGradValue = (UINT16)curGradeVal;
		curGrade = &curGradeTmp;
		

		/*使用前的初始化*/
		frontGrade = NULL;
		rearGrade = NULL;
			
		/*获取前面相邻坡度*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,LinkId, &frontGrade);
		
		/*获取后面相邻坡度*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,LinkId, &rearGrade);
		
		/*若前面或后面相邻坡度查询失败*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}

		/*20141122添加：若查询前面和后面坡度时,发现当前坡度在线路终点,不再进行平缓*/
		if((2==RtnGetGradFrnt)||(2==RtnGetGradNxt))
		{
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*返回成功*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	
	/*
	 1 没有分界点的时候，即相邻坡度与当前坡度差小于千分之20
	 2 当前点处于分界点所标示的弧度外
	*/
	if((relLocation >= arcEndOfst) && (relLocation <= arcStartOfst))
	{

		* GradeVal = (FLOAT32)(10000*curGradeVal);
		/*返回成功*/
	    bReturnValue = 2;
	    return bReturnValue;
	}
	/*当前点在第一个分界点弧度内*/
	else if(relLocation < arcEndOfst)
	{
		if(NULL != frontGrade)
		{
			/*计算角度变化值*/
			if(0 == frontGrade->dwRadius)
			{
			  /*曲率半径为0，角度变化值为0*/
			  angleValue = 0;
			}
			else
			{
				/*角度变化值*/
			   angleValue = (FLOAT32)((arcEndOfst + relLocation)/(1.0*frontGrade->dwRadius));
			}
			/*坡度是上坡，取正值*/
			if(0x55 == frontGrade->wInclineDir)
			{
				tempValue = (1.0*frontGrade->wGradValue)/10000;
			}
			/*下坡取负值*/
			else 
			{
				tempValue = (-1.0*frontGrade->wGradValue)/10000;
			}
			/*if用来判断角度变化值是增加值还是减少值*/
			if(dquGradeTrendExp(pDsuEmapStru,frontGrade->wId,curGrade->wId,&GradeTrend))
			{
				/*坡度减少*/
				if(1 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue - angleValue;
				}
				/*坡度增加*/
				else if(2 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue + angleValue;
				}
				/*坡度不变化*/
				else
				{
					tempAngleValue = tempValue ;
				}

				/*转换为FLOAT32类型*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*返回成功*/
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				/*不可达*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*转换为FLOAT32类型*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*返回成功*/
			bReturnValue = 1;
			return bReturnValue;
		}
	}
	/*在第三分界段内*/
	else
	{
		if(NULL != rearGrade)
		{
			/*计算角度变化值*/
			if(0 == curGrade->dwRadius)
			{
			  /*曲率半径为0，角度变化值为0*/
			  angleValue = 0;
			}
			else
			{
				/*角度变化值*/
			   angleValue = (FLOAT32)((relLocation - arcStartOfst)/(1.0*curGrade->dwRadius));
			}

			if(0x55 == curGrade->wInclineDir)
			{
				tempValue = (1.0*curGrade->wGradValue)/10000;
			}
			else
			{
				tempValue = (-1.0*curGrade->wGradValue)/10000;
			}
			
			/*if用来判断角度变化值是增加值还是减少值*/
			if(dquGradeTrendExp(pDsuEmapStru,curGrade->wId,rearGrade->wId,&GradeTrend))
			{
				/*坡度值减少*/
				if(1 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue - angleValue;				
				}
				/*坡度值增加*/
				else if(2 == GradeTrend)
				{
					/*计算平缓后的坡度值*/
					tempAngleValue = tempValue + angleValue;
				}
				/*坡度值不变化*/
				else
				{
					tempAngleValue = tempValue;
				}
			
				/*转换为FLOAT32类型*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*返回成功*/
				bReturnValue = 3;
				return bReturnValue;
			}
			else
			{
				/*不可达*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*转换为FLOAT32类型*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*返回成功*/
			bReturnValue = 3;
			return bReturnValue;
		}
	}	
}

/*************
*访问接口
*函数功能：计算某点坡度平缓后的坡度值
*输入参数：Point 所求坡度点
*输出参数： 
*返回值：  0函数执行失败 1 点在坡度第一段 2 点在坡度第2段 3 点在坡度第3段
*备注：增加对单轨项目标识的处理，modify by qxt 20170726
****************/
UINT8 dsuCalculateGradeValueExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32 *GradeVal)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuCalculateGradeValueExp_FAO(pDsuEmapStru,LinkId,ofst, GradeVal);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)  )
	{
		rtnValue=dsuCalculateGradeValueExp_CPK(pDsuEmapStru,LinkId,ofst, GradeVal);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
}

/*************
*函数功能：计算某坡度平缓后，分成的几个部分的分界点与起点的距离
*输入参数：gradeId 坡度ID
*输出参数：UINT32* endPointDistance UINT32* startPointDistance
*返回值：  0函数执行失败   1成功
备注：     如果不需要平缓计算的点，设为无效值ffffffff  返回值为1
****************/
UINT8 dsuDividPointDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance)
{
	DSU_GRADE_STRU* curGrade=NULL;        /*当前坡度临时变量*/
	DSU_GRADE_STRU* frontGrade=NULL;      /*前面坡度*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*后面面坡度*/
	UINT16 gradeValue = 0;           /*坡度差值*/
	UINT32 arcLength = 0;			 /*弧长*/
	UINT8 bReturnValue = 0;			 /*函数返回值*/
	UINT8 RtnGetGradFrnt = 0;		 /*获取前面坡度结果*/
	UINT8 RtnGetGradNxt = 0;		 /*获取后面坡度结果*/

	DSU_GRADE_STRU curGradeTmp;		/*当前坡度临时变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*若输入的坡度编号不为空,则在坡度表中处理*/
	if(0xffff != GradeId)
	{
		if((GradeId>GRADEINDEXNUM)||((dsuGradeIndex[GradeId]) == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*初始化当前坡度*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeId];
		
		/*前面有相邻坡度*/
		if(curGrade->wOrgnJointMainGradeId != 0xFFFF)
		{
			/*初始化前面相邻坡度*/
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		/*前面没有相邻坡度*/
		else
		{
			/*终点设为无效值*/
			* endPointDistance = 0xFFFFFFFF;
			frontGrade = NULL;
		}
		
		/*后面有相邻坡度*/
		if(curGrade->wTmnlJointMainGradeId != 0xFFFF)
		{
			/*初始化后面相邻坡度*/
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		/*后面没有相邻坡度*/
		else
		{
			/*起点设为无效值*/
			* startPointDistance = 0xFFFFFFFF;
			rearGrade = NULL;
		}
	}
	else
	/*当前所在坡度为统一坡度*/
	{
		memset(&curGradeTmp, 0, sizeof(curGradeTmp));

		/*根据信息,填写当前坡度信息*/
		curGradeTmp.wInclineDir = EMAP_SAME_DIR;
		curGradeTmp.wGradValue =  dsuStaticHeadStru->pConGradeStru->wGrade;
		curGradeTmp.dwRadius = 0;

		curGrade = &curGradeTmp;

		/*使用前的初始化*/
		frontGrade = NULL;
		rearGrade = NULL;
		
		/*获取前面相邻坡度*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,linkId, &frontGrade);
		
		/*获取后面相邻坡度*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,linkId, &rearGrade);
		
		/*若前面或后面相邻坡度查询失败*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
		
	/**************计算linkEndId 和  endOfst******************/
	/*前面有相邻坡度*/
	if(NULL != frontGrade)
	{
		/*竖曲线半径无效*/
		if(frontGrade->dwRadius == 0xFFFFFFFF)
		{
			/*终点设为无效值*/
			* endPointDistance = 0xFFFFFFFF;
		}
		else
		{
			/*计算坡度差值，都是上坡或下坡*/
			if((frontGrade->wInclineDir) == (curGrade->wInclineDir))
			{
				gradeValue = abs((INT16)curGrade->wGradValue - (INT16)frontGrade->wGradValue);
			}
			else
			{
				/*一个上坡，一个下坡*/
				gradeValue = curGrade->wGradValue + frontGrade->wGradValue;
			}
			/*计算弧长*/
			arcLength = gradeValue*((frontGrade->dwRadius)/20000);
			* endPointDistance = arcLength;
		}
	}
	else
	{
		/*终点设为无效值*/
		* endPointDistance = 0xFFFFFFFF;
	}
	/**************计算完毕linkEndId 和  endOfst******************/
		

	/**************计算linkStartId 和  startOfst******************/
	/*后面有相邻坡度*/
	if(NULL != rearGrade)
	{
		/*竖曲线半径无效*/
		if(rearGrade->dwRadius == 0xFFFFFFFF)
		{
			/*终点设为无效值*/
			* startPointDistance = 0xFFFFFFFF;
		}
		else
		{
			/*计算坡度差值，都是上坡或下坡*/
			if((rearGrade->wInclineDir) == (curGrade->wInclineDir))
			{
				gradeValue = abs((INT16)curGrade->wGradValue - (INT16)rearGrade->wGradValue);
			}
			else
			{
				/*一个上坡，一个下坡*/
				gradeValue = curGrade->wGradValue + rearGrade->wGradValue;
			}
			/*计算弧长*/
			arcLength = gradeValue*((curGrade->dwRadius)/20000);
			* startPointDistance = curGrade->dwGradeLength - arcLength;
		}
	}
	else
	{
		/*起点设为无效值*/
		* startPointDistance = 0xFFFFFFFF;
	}
	/**************计算完毕linkStartId 和  startOfst******************/

	bReturnValue = 1;
	return bReturnValue;
}

/*************
*函数功能：给定坡度上某一点及坡度id，计算该点距离坡度起点的距离
*输入参数：gradeId 坡度ID
           UINT16 linkId UINT32 Ofst
*输出参数：UINT32 *length  长度
          
*返回值：  0函数执行失败   1成功  
****************/
UINT8 dsuGetDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32 Ofst,UINT32* length)
{
	UINT16 j=0;                 /*循环变量*/
	DSU_GRADE_STRU* curGrade=NULL;   /*坡度结构体临时变量*/
	DSU_LINK_STRU * plink=NULL;      /*link结构体指针临时变量*/
	UINT8 bReturnValue;         /*函数返回值*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 GRADEINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	if(0xffff != GradeId)
	{
		if((GradeId>GRADEINDEXNUM)||((dsuGradeIndex[GradeId]) == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}

		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeId];
		if(0 == curGrade->dwOrgnLinkOfst)
		{
			* length = 0;
		}
		else
		{
			* length = 1;
		}
		/*点在起始link上*/
		if((linkId == curGrade->wOrgnLinkId) && (Ofst >= curGrade->dwOrgnLinkOfst))
		{
			* length += Ofst - curGrade->dwOrgnLinkOfst;
			bReturnValue = 1;
			 return bReturnValue;
		}
		else
		{
			plink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[curGrade->wOrgnLinkId];
			* length += plink->dwLength - curGrade->dwOrgnLinkOfst;
			j=0;
			/*先从中间link上找找*/
			while(0xFFFF != curGrade->wIncludeLinkId[j])
			{
				if(j>=GRADE_INCLUDE_LINK_NUM)
				{
					/*数据错误，退出 不可能有这么多包含link*/
					bReturnValue = 0;
					return bReturnValue;
				}
				plink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[curGrade->wIncludeLinkId[j]];
				/*落在中间某link上*/
				if((linkId == curGrade->wIncludeLinkId[j]) && (Ofst <= plink->dwLength))
				{
					* length += Ofst;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{ 
					/*否则把中间link的长度累加起来*/
					* length += plink->dwLength;
					j++;
				}
			}
			/*中间link没找到，看末端link*/
			if((linkId == curGrade->wTmnlLinkId) && (Ofst <= curGrade->dwTmnlLinkOfst))
			{
				* length += Ofst;
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				bReturnValue = 0;
				return bReturnValue;
			}
		}
	}
	else
	{
		* length = Ofst;
		bReturnValue = 1;
		return bReturnValue;	
	}
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
static UCHAR dsuGradientGetExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient)
{
	UINT16 wCurrentGradeID = 0;/*当前的坡度编号*/
	UINT16 wNextGradeID = 0;/*当前坡度的相邻坡度编号*/
	UCHAR bReturnValue = 0;/*函数返回值*/
	UINT8 bLocGradeReturn = 0;/*函数“获取当前位置的坡度信息”返回值*/
	UCHAR rstGradeNxt = 0;  /*获取相邻link坡度的返回值*/
	UCHAR rstGetIdNxt = 0;	/*获取相邻link的返回值*/
	DSU_GRADE_STRU *pstGrade=NULL;/*指向坡度信息的结构体指针*/
	DSU_GRADE_STRU stGrade; /*指向坡度信息的结构体变量*/
	UINT16 i=0;  
	UINT16 wCurrentLinkId;	/*当前坡度的link的ID*/

	FLOAT32 StartPointGradeVal;
	FLOAT32 EndPointGradeVal;
	UINT16 StartGradeId;
	UINT16 EndGradeId;
	UINT8  StartPointGradeRtn;
	UINT8  EndPointGradeRtn;
	FLOAT32 Coefficient = 1;          /*坡度系数，上坡为1 下坡为-1*/
	FLOAT32 CurGradeVal;
	FLOAT32 minGradeVal;
	FLOAT32 maxGradeVal;

	UINT16  StartLinkId;
	UINT32  StartOfst;
	UINT16  EndLinkId;
	UINT32  EndOfst;

	/*全局变量改传参调用涉及的全局变量*/
	UINT16	 LINKINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据起点link偏移量的有效性*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据起点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据Link的有效性*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据终点link偏移量的有效性*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据终点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}


	/*方向为正，系数取1*/
	if(0x55 == pLODStartStru->Dir)
	{
		Coefficient = 1;
		StartLinkId = pLODStartStru->Lnk;
		StartOfst = pLODStartStru->Off;
		EndLinkId = pLODEndStru->Lnk;
		EndOfst = pLODEndStru->Off;
	}
	/*方向为反，系数取-1 起点终点调换位置*/
	else 
	{
		Coefficient = -1;
		StartLinkId = pLODEndStru->Lnk;
		StartOfst = pLODEndStru->Off;
		EndLinkId = pLODStartStru->Lnk;
		EndOfst = pLODStartStru->Off;
	}

	/*****************以下计算起点和终点坡度值*********************/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,StartLinkId,StartOfst,&StartGradeId);
	/*函数调用失败，返回错误*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	StartPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,StartLinkId,StartOfst,&StartPointGradeVal);
	/*函数调用失败，返回错误*/
	if (0 == StartPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*终点坡度值计算*/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,EndLinkId,EndOfst,&EndGradeId);
	/*函数调用失败，返回错误*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	EndPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,EndLinkId,EndOfst,&EndPointGradeVal);
	/*函数调用失败，返回错误*/
	if (0 == EndPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*****************起点和终点坡度值计算完毕*********************/


	/****如果两点在同一坡度内****/
	if((StartGradeId == EndGradeId)&&
		(0xffff != StartGradeId)&&(0xffff != EndGradeId))
	{
		/*在同一个分界段内*/
		if(StartPointGradeRtn == EndPointGradeRtn)
		{
			/*返回最小的那个*/
			if(EndPointGradeVal < StartPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = EndPointGradeVal;
			}
		}
		/*不在同一分界段内，需考虑中间分界段坡度值*/
		else
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*当前坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}

			/*取得最小值*/
			if((EndPointGradeVal<=CurGradeVal)&& (EndPointGradeVal<=StartPointGradeVal))
			{
				minGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal<=CurGradeVal)&& (StartPointGradeVal<=EndPointGradeVal))
			{
				minGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = CurGradeVal;
			}
			/*取最大值*/
			if((EndPointGradeVal>=CurGradeVal)&& (EndPointGradeVal>=StartPointGradeVal))
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal>=CurGradeVal)&& (StartPointGradeVal>=EndPointGradeVal))
			{
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				maxGradeVal = CurGradeVal;
			}

		}
	}
	/*****不在同一坡度*****/
	else
	{
		if(0xffff != StartGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*当前坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*取得当前坡度段起点前方的最大最小坡度值*/
			if(1 == StartPointGradeRtn)
			{
				if(CurGradeVal>StartPointGradeVal)
				{
					minGradeVal = StartPointGradeVal;
					maxGradeVal = CurGradeVal;
				}
				else
				{
					minGradeVal = CurGradeVal;
					maxGradeVal = StartPointGradeVal;
				}
			}
			else if(2 == StartPointGradeRtn)
			{
				minGradeVal = CurGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
		}
		else
		{
			/*起始坡度为统一坡度,仅对最大最小值进行初始赋值*/
			minGradeVal = StartPointGradeVal;
			maxGradeVal = StartPointGradeVal;
		}

		if(0xffff != EndGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[EndGradeId];
			/*终端所在坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*取得当前坡度段起点前方的最大最小坡度值*/
			if(1 == EndPointGradeRtn)
			{
				/*从末端点中找到最小最大坡度值*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
			else if(2 == EndPointGradeRtn)
			{
				/*从末端点中找到最小最大坡度值*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
			else
			{
				/*算出最大值*/
				if((EndPointGradeVal>= maxGradeVal)&&(EndPointGradeVal>= CurGradeVal))
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal>= maxGradeVal)&&(CurGradeVal>= EndPointGradeVal))
				{
					maxGradeVal = CurGradeVal;
				}
				else
				{
					/*继续执行*/
				}

				/*算出最小值*/
				if((EndPointGradeVal<= minGradeVal)&&(EndPointGradeVal<= CurGradeVal))
				{
					minGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal<= minGradeVal)&&(CurGradeVal<=EndPointGradeVal ))
				{
					minGradeVal = CurGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
		}
		else
		{
			/*终点坡度为统一坡度*/
			/*从末端点中找到最小最大坡度值*/
			if(maxGradeVal<EndPointGradeVal)
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if(minGradeVal>EndPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
			}
			else
			{
				/*继续执行*/
			}
		}


		/*设当前坡度id为起始坡度id*/
		wCurrentGradeID = StartGradeId;
		/*记录当前link的id信息*/
		wCurrentLinkId = StartLinkId;

		/*循环获取起点终点间的坡度信息并比对得到最大,最小值*/
		for(i=0;i<dsuDataLenStru->wGradeLen;i++)
		{
			if(0xffff != wCurrentGradeID)
			{
				/*在坡度表中查询当前坡度的前面坡度*/
				wNextGradeID = FindAdjacentGradeIDExp(pDsuEmapStru,wCurrentGradeID, 0x55, PointNum, pPointStru);

				/*获取前方坡度*/
				if (0xffff != wNextGradeID)
				{
					/*若查询得到的坡度关联ID不为空,利用管理函数找到当前的坡度信息*/
					pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wNextGradeID];	
					/*获取当前所用坡度的终点link信息*/
					wCurrentLinkId = pstGrade->wTmnlLinkId;
				}				
				else
				{
					/*若在坡度表中获取到的终点关联坡度为无效,则可能为线路终点,或者在统一坡度表中的信息*/



					/*在link表中查询前面坡度是否在统一坡度表中,*/
					rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);

					if((1 == rstGradeNxt)&&(NULL == pstGrade))
					{
						/*若前面坡度在统一坡度表中,则获取统一坡度表信息,参与坡度比较*/
						stGrade.wInclineDir = EMAP_SAME_DIR;
						stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
						pstGrade = &stGrade;

						/*相邻坡度为统一坡度,编号置为无效*/
						wNextGradeID = 0xffff;
					}
					else
					{
						/*若前面坡度未在统一坡度表中,则当前查询应为线路终点*/
						/*线路终点(找到线路终点也未能找到终点坡度ID)*/
						bReturnValue = 0;
						return bReturnValue;
					}
					/*更新当前link的ID信息以便统一坡度表关系的查询*/
					rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

					if(1 != rstGetIdNxt)
					{
						/*查询线路link失败*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
			}
			else
			{
				/*查询当前link的始端相邻坡度*/
				rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);

				if(1 == rstGradeNxt)
				{
					if(NULL == pstGrade)
					{
						/*若前面坡度在统一坡度表中,则获取统一坡度表信息,参与坡度比较*/
						stGrade.wInclineDir = EMAP_SAME_DIR;
						stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
						pstGrade = &stGrade;

						/*相邻坡度为统一坡度,编号置为无效*/
						wNextGradeID = 0xffff;
					}
					else
					{
						/*按照实际查询得到pstGrade的坡度表中的坡度值进行处理*/
						/*记录当前link的相邻坡度ID*/
						wNextGradeID = pstGrade->wId;
					}

					/*为避免在坡度表中的查询不完全,若在统一坡度表中的处理不计入循环*/
					if(i>=1)
					{
						i--;
					}
				}
				else
				{
					/*线路终点(找到线路终点也未能找到终点坡度ID)*/
					bReturnValue = 0;
					return bReturnValue;
				}

				if (wCurrentLinkId == EndLinkId)
				{
					/*查到终端坡度*/
					break;
				}

				/*20141122添加：更新当前link的ID信息以便统一坡度表关系的查询*/
				rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

				if(1 != rstGetIdNxt)
				{
					/*查询线路link失败*/
					bReturnValue = 0;
					return bReturnValue;
				}
			}

			/*更新当前坡度ID信息以便坡度表关系的查询*/
			wCurrentGradeID = wNextGradeID;

			if( ((wNextGradeID == EndGradeId) && ((0xffff != wNextGradeID) && (0xffff != EndGradeId))) ||
				((wCurrentLinkId == EndLinkId) && (0xffff == wNextGradeID)) )
			{
				/*查到终端坡度*/
				break;
			}

			if(0x55 == pstGrade->wInclineDir)
			{
				/*如果有更大的坡度或更小的坡度，替换当前值*/
				if(minGradeVal>=pstGrade->wGradValue)
				{
					minGradeVal = pstGrade->wGradValue;
				}
				if(maxGradeVal<=pstGrade->wGradValue)
				{
					maxGradeVal = pstGrade->wGradValue;
				}
			}
			else
			{
				/*如果有更大的坡度或更小的坡度，替换当前值*/
				if(minGradeVal>=-1.0*pstGrade->wGradValue)
				{
					minGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
				}
				if(maxGradeVal<=-1.0*pstGrade->wGradValue)
				{
					maxGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
				}
			}
		}

		if(i>=dsuDataLenStru->wGradeLen)
		{
			/*没找到末端坡度*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}

	/*根据运行方向确定最差坡度值*/
	if(0x55 == pLODStartStru->Dir)
	{
		* Gradient = minGradeVal/10;
		bReturnValue = 1;
		return bReturnValue;
	}
	else
	{
		* Gradient = (FLOAT32)(-1.0 * maxGradeVal/10);
		bReturnValue = 1;
		return bReturnValue;
	}

}

static UCHAR dsuGradientGetExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient)
{
	UINT16 wCurrentGradeID = 0;/*当前的坡度编号*/
	UINT16 wNextGradeID = 0;/*当前坡度的相邻坡度编号*/
	UCHAR bReturnValue = 0;/*函数返回值*/
	UINT8 bLocGradeReturn = 0;/*函数“获取当前位置的坡度信息”返回值*/
	UCHAR rstGradeNxt = 0;  /*获取相邻link坡度的返回值*/
	UCHAR rstGetIdNxt = 0;	/*获取相邻link的返回值*/
    DSU_GRADE_STRU *pstGrade=NULL;/*指向坡度信息的结构体指针*/
	DSU_GRADE_STRU stGrade; /*指向坡度信息的结构体变量*/
	UINT16 i=0;  
	UINT16 wCurrentLinkId;	/*当前坡度的link的ID*/

	FLOAT32 StartPointGradeVal;
	FLOAT32 EndPointGradeVal;
	UINT16 StartGradeId;
	UINT16 EndGradeId;
	UINT8  StartPointGradeRtn;
	UINT8  EndPointGradeRtn;
	FLOAT32 Coefficient = 1;          /*坡度系数，上坡为1 下坡为-1*/
	FLOAT32 CurGradeVal;
	FLOAT32 minGradeVal;
	FLOAT32 maxGradeVal;

	UINT16  StartLinkId;
	UINT32  StartOfst;
	UINT16  EndLinkId;
	UINT32  EndOfst;

	DSU_LINK_STRU *pLinkStru=NULL;				/*Link信息结构体指针*/
    DSU_LINK_STRU *pLinkStruEnd=NULL;		   /*Link信息结构体指针 add by zlj 20161024*/
	/*全局变量改传参调用涉及的全局变量*/
	UINT16	 LINKINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据起点link偏移量的有效性*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据起点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据Link的有效性*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据终点link偏移量的有效性*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*判断入口数据终点方向的有效性*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}


	/*方向为正，系数取1*/
	if(0x55 == pLODStartStru->Dir)
	{
		Coefficient = 1;
		StartLinkId = pLODStartStru->Lnk;
		StartOfst = pLODStartStru->Off;
		EndLinkId = pLODEndStru->Lnk;
		EndOfst = pLODEndStru->Off;
	}
	/*方向为反，系数取-1 起点终点调换位置*/
	else 
	{
		Coefficient = -1;
		StartLinkId = pLODEndStru->Lnk;
		StartOfst = pLODEndStru->Off;
		EndLinkId = pLODStartStru->Lnk;
		EndOfst = pLODStartStru->Off;
	}

	/*****************以下计算起点和终点坡度值*********************/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,StartLinkId,StartOfst,&StartGradeId);
	/*函数调用失败，返回错误*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	StartPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,StartLinkId,StartOfst,&StartPointGradeVal);
	/*函数调用失败，返回错误*/
	if (0 == StartPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*终点坡度值计算*/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,EndLinkId,EndOfst,&EndGradeId);
	/*函数调用失败，返回错误*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	EndPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,EndLinkId,EndOfst,&EndPointGradeVal);
	/*函数调用失败，返回错误*/
	if (0 == EndPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*****************起点和终点坡度值计算完毕*********************/

	
	/****如果两点在同一坡度内****/
	if((StartGradeId == EndGradeId)&&
	   (0xffff != StartGradeId) && (0xffff != EndGradeId))
	{
		/*在同一个分界段内*/
		if(StartPointGradeRtn == EndPointGradeRtn)
		{
			/*返回最小的那个*/
			if(EndPointGradeVal < StartPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = EndPointGradeVal;
			}
		}
		/*不在同一分界段内，需考虑中间分界段坡度值*/
		else
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*当前坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}

			/*取得最小值*/
			if((EndPointGradeVal<=CurGradeVal)&& (EndPointGradeVal<=StartPointGradeVal))
			{
				minGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal<=CurGradeVal)&& (StartPointGradeVal<=EndPointGradeVal))
			{
				minGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = CurGradeVal;
			}
			/*取最大值*/
			if((EndPointGradeVal>=CurGradeVal)&& (EndPointGradeVal>=StartPointGradeVal))
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal>=CurGradeVal)&& (StartPointGradeVal>=EndPointGradeVal))
			{
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				maxGradeVal = CurGradeVal;
			}

		}
	}
    /*****不在同一坡度*****/
	else
	{
		if(0xffff != StartGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*当前坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*取得当前坡度段起点前方的最大最小坡度值*/
			if(1 == StartPointGradeRtn)
			{
				if(CurGradeVal>StartPointGradeVal)
				{
					minGradeVal = StartPointGradeVal;
					maxGradeVal = CurGradeVal;
				}
				else
				{
					minGradeVal = CurGradeVal;
					maxGradeVal = StartPointGradeVal;
				}
			}
			else if(2 == StartPointGradeRtn)
			{
				minGradeVal = CurGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
		}
		else
		{
			/*起始坡度为统一坡度,仅对最大最小值进行初始赋值*/
			minGradeVal = StartPointGradeVal;
			maxGradeVal = StartPointGradeVal;
		}
		
		if(0xffff != EndGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[EndGradeId];
			/*终端所在坡度值，上坡为正，下坡为负*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*取得当前坡度段起点前方的最大最小坡度值*/
			if(1 == EndPointGradeRtn)
			{
				/*从末端点中找到最小最大坡度值*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
			else if(2 == EndPointGradeRtn)
			{
				/*从末端点中找到最小最大坡度值*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
			else
			{
				/*算出最大值*/
				if((EndPointGradeVal>= maxGradeVal)&&(EndPointGradeVal>= CurGradeVal))
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal>= maxGradeVal)&&(CurGradeVal>= EndPointGradeVal))
				{
					maxGradeVal = CurGradeVal;
				}
				else
				{
					/*继续执行*/
				}

				/*算出最小值*/
				if((EndPointGradeVal<= minGradeVal)&&(EndPointGradeVal<= CurGradeVal))
				{
					minGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal<= minGradeVal)&&(CurGradeVal<=EndPointGradeVal ))
				{
					minGradeVal = CurGradeVal;
				}
				else
				{
					/*继续执行*/
				}
			}
		}
		else
		{
			/*终点坡度为统一坡度*/
			/*从末端点中找到最小最大坡度值*/
			if(maxGradeVal<EndPointGradeVal)
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if(minGradeVal>EndPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
			}
			else
			{
				/*继续执行*/
			}
		}
		

		/*设当前坡度id为起始坡度id*/
		wCurrentGradeID = StartGradeId;
		/*记录当前link的id信息*/
		wCurrentLinkId = StartLinkId;

		pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[wCurrentLinkId];

		pLinkStruEnd = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[EndLinkId];/*add by zlj 20161024*/
		/*当前link的“Link坡度信息属性”为无统一坡度*/
		/*if(0 == pLinkStru->wGrade)*/ /*changed by zlj 20161024*/
        if((0 == pLinkStru->wGrade)||(0 == pLinkStruEnd->wGrade))
		{

			/*循环获取起点终点间的坡度信息并比对得到最大,最小值*/
			for(i=0;i<dsuDataLenStru->wGradeLen;i++)
			{
				if(0xffff != wCurrentGradeID)
				{
					/*在坡度表中查询当前坡度的前面坡度*/
					wNextGradeID = FindAdjacentGradeIDExp(pDsuEmapStru,wCurrentGradeID, 0x55, PointNum, pPointStru);
					
					/*获取前方坡度*/
					if (0xffff != wNextGradeID)
					{
						/*若查询得到的坡度关联ID不为空,利用管理函数找到当前的坡度信息*/
						pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wNextGradeID];	
						/*获取当前所用坡度的终点link信息*/
						wCurrentLinkId = pstGrade->wTmnlLinkId;
					}				
					else
					{
						/*若在坡度表中获取到的终点关联坡度为无效,则可能为线路终点,或者在统一坡度表中的信息*/
						
						/*在link表中查询前面坡度是否在统一坡度表中,*/
						rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);
						
						if((1 == rstGradeNxt)&&(NULL == pstGrade))
						{
							/*若前面坡度在统一坡度表中,则获取统一坡度表信息,参与坡度比较*/
							stGrade.wInclineDir = EMAP_SAME_DIR;
							stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
							pstGrade = &stGrade;
							
							/*相邻坡度为统一坡度,编号置为无效*/
							wNextGradeID = 0xffff;
						}
						else
						{
							/*若前面坡度未在统一坡度表中,则当前查询应为线路终点*/
							/*线路终点(找到线路终点也未能找到终点坡度ID)*/
							bReturnValue = 0;
							return bReturnValue;
						}
						/*更新当前link的ID信息以便统一坡度表关系的查询*/
						rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

						if(1 != rstGetIdNxt)
						{
							/*查询线路link失败*/
							bReturnValue = 0;
							return bReturnValue;
						}
					}
				}
				else
				{
					/*查询当前link的始端相邻坡度*/
					rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);
					
					if(1 == rstGradeNxt)
					{
						if(NULL == pstGrade)
						{
							/*若前面坡度在统一坡度表中,则获取统一坡度表信息,参与坡度比较*/
							stGrade.wInclineDir = EMAP_SAME_DIR;
							stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
							pstGrade = &stGrade;

							/*相邻坡度为统一坡度,编号置为无效*/
							wNextGradeID = 0xffff;
						}
						else
						{
							/*按照实际查询得到pstGrade的坡度表中的坡度值进行处理*/
							/*记录当前link的相邻坡度ID*/
							wNextGradeID = pstGrade->wId;
						}
						
						/*为避免在坡度表中的查询不完全,若在统一坡度表中的处理不计入循环*/
						if(i>=1)
						{
							i--;
						}
					}
					else
					{
						/*线路终点(找到线路终点也未能找到终点坡度ID)*/
						bReturnValue = 0;
						return bReturnValue;
					}

					if (wCurrentLinkId == EndLinkId)
					{
						/*查到终端坡度*/
						break;
					}
					/*20141122添加：更新当前link的ID信息以便统一坡度表关系的查询*/
					rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

					if(1 != rstGetIdNxt)
					{
						/*查询线路link失败*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				
				/*更新当前坡度ID信息以便坡度表关系的查询*/
				wCurrentGradeID = wNextGradeID;
				
				if( ((wNextGradeID == EndGradeId)&&((0xffff != wNextGradeID)&&(0xffff != EndGradeId)))||
					((wCurrentLinkId == EndLinkId)&&(0xffff == wNextGradeID)) )
				{
					/*查到终端坡度*/
					break;
				}
			
				if(0x55 == pstGrade->wInclineDir)
				{
					/*如果有更大的坡度或更小的坡度，替换当前值*/
					if(minGradeVal>=pstGrade->wGradValue)
					{
						minGradeVal = pstGrade->wGradValue;
					}
					if(maxGradeVal<=pstGrade->wGradValue)
					{
						maxGradeVal = pstGrade->wGradValue;
					}
				}
				else
				{
					/*如果有更大的坡度或更小的坡度，替换当前值*/
					if(minGradeVal>=-1.0*pstGrade->wGradValue)
					{
						minGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
					}
					if(maxGradeVal<=-1.0*pstGrade->wGradValue)
					{
						maxGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
					}
				}
			}
        }
		else
		{
			/*找到统一坡度*/
			if(fabs(minGradeVal-maxGradeVal)<0.001f)
			{
				*Gradient = minGradeVal/10;
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
			    /*不可达分支*/
				bReturnValue = 0;
				return bReturnValue;
			}

		}

		if(i>=dsuDataLenStru->wGradeLen)
		{
			/*没找到末端坡度*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	
	/*根据运行方向确定最差坡度值*/
	if(0x55 == pLODStartStru->Dir)
	{
		*Gradient = minGradeVal/10;
		bReturnValue = 1;
	    return bReturnValue;
	}
	else
	{
		*Gradient = (FLOAT32)(-1.0 * maxGradeVal/10);
		bReturnValue = 1;
	    return bReturnValue;
	}

}


/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UCHAR dsuGradientGetExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
	const UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, FLOAT32 *Gradient)
{
	UINT8 cbtcVersionType=0;
	UCHAR bReturnValue = 0;/*函数返回值*/
	cbtcVersionType=GetCbtcSysType();
	
	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		bReturnValue=dsuGradientGetExp_FAO(pDsuEmapStru,pLODStartStru, pLODEndStru, PointNum,pPointStru,Gradient);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType)||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		bReturnValue=dsuGradientGetExp_CPK(pDsuEmapStru,pLODStartStru, pLODEndStru, PointNum,pPointStru,Gradient);
	}

	else
	{
		bReturnValue = 0;		
	}
	return bReturnValue;
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
UINT8 dquGradeTrendExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend)
{
	UINT8 bReturnValue = 0;	
	INT32 signedFirstGrade=0;/*带有符号的坡度值--前一个坡度*/
	INT32 signedSecondGrade=0;/*带有符号的坡度值--后一个坡度*/
	DSU_GRADE_STRU *fistGrade=NULL;/*指向坡度信息的结构体指针*/
	DSU_GRADE_STRU *secondGrade=NULL;/*指向坡度信息的结构体指针*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}


	if(0xffff != fistGradeID)
	{
		if((fistGradeID>GRADEINDEXNUM)||((dsuGradeIndex[fistGradeID]) == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*初始化坡度*/
		fistGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[fistGradeID];
			
		/*0x55=EMAP_SAME_INC_LINE表示上坡,坡度值为正数
		  0xaa=EMAP_CONVER_INC_LINE表示下坡，坡度值为负数*/
	
		/*前一个坡度*/
		if (fistGrade->wInclineDir == EMAP_SAME_INC_LINE) 
		{
			signedFirstGrade=(INT32)fistGrade->wGradValue;
		}
		else if (fistGrade->wInclineDir == EMAP_CONVER_INC_LINE)
		{
			signedFirstGrade=(-1)*((INT32)(fistGrade->wGradValue));
		}
		else
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	else
	{
		signedFirstGrade = dsuStaticHeadStru->pConGradeStru->wGrade;
	}	

	if(0xffff != secondGradeID)
	{
		if((secondGradeID>GRADEINDEXNUM)||((dsuGradeIndex[secondGradeID]) == 0xFFFF))
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*初始化坡度*/
		secondGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[secondGradeID];
		
		/*后一个坡度*/
		if (secondGrade->wInclineDir == EMAP_SAME_INC_LINE) 
		{
			signedSecondGrade=(INT32)secondGrade->wGradValue;
		}
		else if (secondGrade->wInclineDir == EMAP_CONVER_INC_LINE)
		{
			signedSecondGrade=(-1)*((INT32)(secondGrade->wGradValue));
		}
		else
		{
			/*入口参数异常，查询失败*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	else
	{
		signedSecondGrade = dsuStaticHeadStru->pConGradeStru->wGrade;
	}	
	
	if((0xffff != fistGradeID)&&(0xffff != secondGradeID)&&
		(secondGradeID != fistGrade->wTmnlJointMainGradeId)&&(secondGradeID != fistGrade->wTmnlJointSideGradeId))
	{
	    /*入口参数异常，查询失败*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*
	用后一个坡度的坡度值减去前一个坡度的坡度值
	结果为正：坡度值增加
	结果为负：坡度值减少
	结果为零：坡度值不变化
	*/

	if (signedSecondGrade-signedFirstGrade > 0)
	{
		*GradeTrend = 2;
		bReturnValue = 1;
		return bReturnValue;
	}
	else if (signedSecondGrade-signedFirstGrade < 0)
	{
		*GradeTrend = 1;
		bReturnValue = 1;
		return bReturnValue;
	}
	else
	{
		*GradeTrend = 0;
		bReturnValue = 1;
		return bReturnValue;
	}

}

/*
函数功能：获取前面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeFrnt 坡度 
			Null：  线路终点或前面坡度为统一坡度  
			非Null：前面坡度在坡度表中可查
  返回值：0失败 1成功 2线路终点
*/
UINT8 dsuGetGradeFrntExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt)
{	
	UINT16 GradeIDOrg = 0;					/*记录前面坡度的ID*/
	UINT16 GradeID = 0;						/*记录当前坡度的ID*/
	UINT8  bReturnValue = 0;				/*函数返回值*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link信息结构体指针*/
	DSU_LINK_STRU *pLinkStruOrg=NULL;       		/*link的临时变量*/
	DSU_GRADE_STRU* gradeFrntTmp=NULL;			/*临时坡度变量*/
	UINT16 i = 0;							/*循环计数变量*/
	UINT8 RstGetOrg = 0;					/*查询前面坡度Id返回值*/
	UINT8 RstGetCur = 0;					/*查询当前坡度Id返回值*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*若当前link位置相邻的link在坡度表中可以查询到,则获取对应数据行;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wOrgnJointMainLkId)
		{
			pLinkStruOrg = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wOrgnJointMainLkId];

			/*若相邻link的坡度不为统一坡度*/
			if(0 == pLinkStruOrg->wGrade)
			{
				/*查询起点关联link对应的坡度ID*/
				RstGetOrg = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruOrg->wId, pLinkStruOrg->dwLength, &GradeIDOrg);

				/*查询当前link对应的坡度ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, 0, &GradeID);

				if((0 == RstGetOrg)||(0 == RstGetCur))
				{
					/*查询失败*/
					bReturnValue = 0;
					return bReturnValue;
				}

				/*相邻link与当前link的坡度ID不同*/
				if(GradeIDOrg != GradeID)
				{	
					if(0xffff != GradeIDOrg)
					{				
						/*初始化前面相邻坡度*/
						gradeFrntTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDOrg];
						*gradeFrnt = gradeFrntTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*数据出错(link表中不为统一坡度,查询得到为统一坡度)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruOrg;
				}
			}
			else
			{
				/*若相邻link的坡度仍为统一坡度*/
				*gradeFrnt = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*若当前link无相邻link则为线路终点,起点设为无效值*/
			*gradeFrnt = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*若循环完成未能找到对应坡度ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeFrnt = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
}

UINT8 dsuGetGradeFrntExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt)
{	
	UINT16 GradeIDOrg = 0;					/*记录前面坡度的ID*/
	UINT16 GradeID = 0;						/*记录当前坡度的ID*/
	UINT8  bReturnValue = 0;				/*函数返回值*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link信息结构体指针*/
	DSU_LINK_STRU *pLinkStruOrg=NULL;       		/*link的临时变量*/
	DSU_GRADE_STRU* gradeFrntTmp=NULL;			/*临时坡度变量*/
	UINT16 i = 0;							/*循环计数变量*/
	UINT8 RstGetOrg = 0;					/*查询前面坡度Id返回值*/
	UINT8 RstGetCur = 0;					/*查询当前坡度Id返回值*/
	
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	
	/*若当前link位置相邻的link在坡度表中可以查询到,则获取对应数据行;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];
	
	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wOrgnJointMainLkId)
		{
			pLinkStruOrg = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wOrgnJointMainLkId];
			
			/*若相邻link的坡度不为统一坡度*/
			if(0 == pLinkStruOrg->wGrade)
			{
				/*查询起点关联link对应的坡度ID*/
				RstGetOrg = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruOrg->wId, pLinkStruOrg->dwLength, &GradeIDOrg);
				
				/*查询当前link对应的坡度ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, 0, &GradeID);
				
				if((0 == RstGetOrg)||(0 == RstGetCur))
				{
					/*查询失败*/
					bReturnValue = 0;
					return bReturnValue;
				}
				
				/*相邻link与当前link的坡度ID不同*/
				if(GradeIDOrg != GradeID)
				{	
					if(0xffff != GradeIDOrg)
					{				
						/*初始化前面相邻坡度*/
						gradeFrntTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDOrg];
						*gradeFrnt = gradeFrntTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*数据出错(link表中不为统一坡度,查询得到为统一坡度)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruOrg;
				}
			}
			else
			{
				/*若相邻link的坡度仍为统一坡度*/
				*gradeFrnt = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*若当前link无相邻link则为线路终点,起点设为无效值*/
			*gradeFrnt = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	
	/*若循环完成未能找到对应坡度ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeFrnt = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*不可达分支*/
		bReturnValue=0;
		return bReturnValue;
	}
}
/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UINT8 dsuGetGradeFrntExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU **gradeFrnt)
{
	UINT8 rtnValue = 0;/*函数返回值*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuGetGradeFrntExp_FAO(pDsuEmapStru,linkId, gradeFrnt);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || ( DQU_CBTC_HLT == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue=dsuGetGradeFrntExp_CPK(pDsuEmapStru,linkId, gradeFrnt);
	}
	else
	{
		rtnValue = 0;
	}
	return rtnValue; 
}

/*
函数功能：获取后面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeRear 坡度 
			Null：  线路终点或后面坡度为统一坡度  
			非Null：后面坡度在坡度表中可查
  返回值：0失败 1成功 2线路终点
*/
UINT8 dsuGetGradeRearExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear)
{
	UINT16 GradeIDNxt = 0;					/*记录后面坡度的ID*/
	UINT16 GradeID = 0;						/*记录当前坡度的ID*/
	UINT8  bReturnValue = 0;				/*函数返回值*/
	DSU_LINK_STRU* pLinkStru=NULL;				/*Link信息结构体指针*/
	DSU_LINK_STRU* pLinkStruNxt=NULL;			/*link的临时变量*/
	DSU_GRADE_STRU* gradeRearTmp=NULL;			/*坡度临时变量*/
	UINT16 i = 0;							/*循环计数变量*/
	UINT8 RstGetNxt = 0;					/*查询前面坡度Id返回值*/
	UINT8 RstGetCur = 0;					/*查询当前坡度Id返回值*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*若当前link位置相邻的link在坡度表中可以查询到,则获取对应数据行;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wTmnlJointMainLkId)
		{
			pLinkStruNxt = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wTmnlJointMainLkId];

			/*若相邻link的坡度不为统一坡度*/
			if(0 == pLinkStruNxt->wGrade)
			{
				/*查询起点关联link对应的坡度ID*/
				RstGetNxt = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruNxt->wId, 0, &GradeIDNxt);

				/*查询当前link对应的坡度ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, pLinkStru->dwLength, &GradeID);

				if((0 == RstGetNxt)||(0 == RstGetCur))
				{
					/*查询失败*/
					bReturnValue = 0;
					return bReturnValue;
				}

				/*相邻link与当前link的坡度ID不同*/
				if(GradeIDNxt != GradeID)
				{
					if(0xffff != GradeIDNxt)
					{
						/*初始化后面相邻坡度*/
						gradeRearTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDNxt];
						*gradeRear = gradeRearTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*则数据出错(link表中不为统一坡度,查询得到为统一坡度)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruNxt;
				}
			}
			else
			{
				/*若相邻link的坡度仍为统一坡度*/
				*gradeRear = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*若当前link无相邻link则为线路终点*/
			*gradeRear = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*若循环完成未能找到对应坡度ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeRear = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}

}

UINT8 dsuGetGradeRearExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear)
{
	UINT16 GradeIDNxt = 0;					/*记录后面坡度的ID*/
	UINT16 GradeID = 0;						/*记录当前坡度的ID*/
	UINT8  bReturnValue = 0;				/*函数返回值*/
	DSU_LINK_STRU* pLinkStru=NULL;				/*Link信息结构体指针*/
	DSU_LINK_STRU* pLinkStruNxt=NULL;			/*link的临时变量*/
	DSU_GRADE_STRU* gradeRearTmp=NULL;			/*坡度临时变量*/
	UINT16 i = 0;							/*循环计数变量*/
	UINT8 RstGetNxt = 0;					/*查询前面坡度Id返回值*/
	UINT8 RstGetCur = 0;					/*查询当前坡度Id返回值*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*判断入口数据起点Link的有效性*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*入口数据无效，数据异常，查询失败，返回错误*/
		bReturnValue = 0;
		return bReturnValue;
	}
	
	/*若当前link位置相邻的link在坡度表中可以查询到,则获取对应数据行;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];
	
	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wTmnlJointMainLkId)
		{
			pLinkStruNxt = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wTmnlJointMainLkId];
			
			/*若相邻link的坡度不为统一坡度*/
			if(0 == pLinkStruNxt->wGrade)
			{
				/*查询起点关联link对应的坡度ID*/
				RstGetNxt = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruNxt->wId, 0, &GradeIDNxt);

				/*查询当前link对应的坡度ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, pLinkStru->dwLength, &GradeID);
				
				if((0 == RstGetNxt)||(0 == RstGetCur))
				{
					/*查询失败*/
					bReturnValue = 0;
					return bReturnValue;
				}
				
				/*相邻link与当前link的坡度ID不同*/
				if(GradeIDNxt != GradeID)
				{
					if(0xffff != GradeIDNxt)
					{
						/*初始化后面相邻坡度*/
						gradeRearTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDNxt];
						*gradeRear = gradeRearTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*则数据出错(link表中不为统一坡度,查询得到为统一坡度)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruNxt;
				}
			}
			else
			{
				/*若相邻link的坡度仍为统一坡度*/
				*gradeRear = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*若当前link无相邻link则为线路终点*/
			*gradeRear = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*若循环完成未能找到对应坡度ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeRear = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*不可达分支*/
		bReturnValue=0;
		return bReturnValue;
	}
}
/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UINT8 dsuGetGradeRearExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU **gradeRear)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuGetGradeRearExp_FAO(pDsuEmapStru,linkId, gradeRear);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType)||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue=dsuGetGradeRearExp_CPK(pDsuEmapStru,linkId, gradeRear);
	}
	else
	{
		rtnValue = 0;
	}
	return rtnValue; 
}

/**
函数功能：获取当前LINK的静态限速个数
输入参数：
@linkId：当前link
输出参数：无
返回值：静态限速个数
*/
UINT8 GetStcLmtSpdCntByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId)
{
	UINT8 ret = 0;

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	if ((NULL != pDsuEmapStru) && (NULL !=pDsuLimitStcLinkIdx))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
	}
	else
	{
		/*电子地图数据指针为空*/
		ret = 0;
	}

	if(linkId <= LINKINDEXNUM)
	{
		if (pDsuLimitStcLinkIdx[linkId].linkId == linkId)
		{
		    /*返回linkId对应静态限速表的限速个数*/
			ret = pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount;
		}
		else
		{
			ret = 0;
		}
		
	}
	else
	{
		ret = 0;
	}

	return ret;
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
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId,const UINT8 idx)
{
	DSU_STATIC_RES_SPEED_STRU *pStcLmtSpdInfo = NULL;

	UINT16 LINKINDEXNUM=0;
	UINT16* dsuStaticResSpeedIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if ( (NULL != pDsuEmapStru) && (NULL != pDsuLimitStcLinkIdx) )
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuStaticResSpeedIndex=pDsuEmapStru->dsuEmapIndexStru->dsuStaticResSpeedIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		pStcLmtSpdInfo = NULL;
	}

	if ((idx >= LINK_MAX_LIMIT_SPD) || (idx < 0))
	{
		return pStcLmtSpdInfo;
	}
	if(linkId <= LINKINDEXNUM)
	{
		if(idx < pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount)
		{
			/*将linkId对应静态限速表的第idx静态限速取出来*/
			pStcLmtSpdInfo = dsuStaticHeadStru->pStaticResSpeedStru + dsuStaticResSpeedIndex[pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoIdBuf[idx]];
		}
	}
	else
	{
		pStcLmtSpdInfo = NULL;
	}

	return pStcLmtSpdInfo;
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
UINT8 ConvertToVirtualSwitchesExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM])
{
	UINT8 chReturnValue = 0;/*函数返回值*/
	UINT16 i = 0,j=0,k=0;          /*循环用变量*/
	DSU_MULTI_SWITCH_STRU* pMultiPointStru=NULL; /*多开道岔临时变量结结构体*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 MULTIPOINTINDEXNUM=0;
	UINT16 *dsuMultiPointIndex=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
    DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	
	/*输出参数初始化为无效值65535*/
	for(i=0;i<MAX_VIRTUAL_SWITCH_NUM;i++)
	{
		wVirtualPointIds[i]=DSU_NULL_16;
		wVirtualPointStates[i]=DSU_NULL_8;
	}

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru))
	{
		MULTIPOINTINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->MULTIPOINTINDEXNUM;
		dsuMultiPointIndex=pDsuEmapStru->dsuEmapIndexStru->dsuMultiPointIndex;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*判断入口数据输入的有效性*/
		if ((multiPointId<=MULTIPOINTINDEXNUM)&&(dsuMultiPointIndex[multiPointId]!=DSU_NULL_16)&&(multiPointState<=MAX_VIRTUAL_SWITCH_NUM)&&(multiPointState>=1))
		{
			/*遍历多开道岔表*/
			for (j=0;j<dsuDataLenStru->wMultiPointLen;j++)
			{
				pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+j;
				if ((pMultiPointStru->wId==multiPointId)&&(multiPointState<=pMultiPointStru->wType))
				{
					/*当多开道岔状态是1时，需单独处理，因为N位道岔和R1位道岔属于同一道岔,为防止输出两个相同道岔，遍历虚拟道岔时，数组索引应从1开始*/
					if (1==multiPointState)
					{
						for(k=1;k<pMultiPointStru->wType;k++)
						{
							/*虚拟道岔编号赋值*/
							wVirtualPointIds[k-1]=pMultiPointStru->wSwitchIds[k];
							/*虚拟道岔状态赋值，虚拟道岔状态只与N位相同，其他位道岔状态均取反(2=>1;1=>2*/
							wVirtualPointStates[k-1]=0x03&~pMultiPointStru->wSwitchStates[k];
						}
					}
					else
					{

						for(k=1;k<pMultiPointStru->wType;k++)
						{
							/*虚拟道岔编号赋值*/
							wVirtualPointIds[k-1]=pMultiPointStru->wSwitchIds[k];
							/*虚拟道岔状态赋值*/
							if (k==(multiPointState-1))
							{
								wVirtualPointStates[k-1]=pMultiPointStru->wSwitchStates[k];
							}
							/*索引在multiPointState-1之前的虚拟道岔均为无效的*/
							else if (k<(multiPointState-1))
							{
								wVirtualPointStates[k-1]=EMAP_POINT_STATUS_LOSE;
							}
							/*索引在multiPointState-1之后的虚拟道岔状态置反*/
							else
							{  
								/*道岔状态取反(2=>1;1=>2)*/
								wVirtualPointStates[k-1]=0x03&~pMultiPointStru->wSwitchStates[k];

							}

						}

					}
													
					chReturnValue=pMultiPointStru->wType-1;
				}
			}
		}
		else
		{
			/*入口参数异常，查询失败*/
			chReturnValue = 0;

		}		    
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;	
	}
	return chReturnValue;

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
UINT8 ConvertToMultiSwitchExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState)
{
	UINT8 chReturnValue = 0;      /*函数返回值*/
	UINT16 i= 0,j=0,k=0;          /*循环用变量*/
	UINT8 IdCount=0;              /* 用于计数，输入参数虚拟道岔编号与表中道岔编号相等的个数。*/
	UINT8 StateCount=0;            /*虚拟道岔状态相等的个数，有且只能有一个*/
	UINT8 tempPointState=0;        /*多开道岔状态*/ 
	DSU_MULTI_SWITCH_STRU *pMultiPointStru=NULL; /*多开道岔临时变量结结构体*/
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 *dsuMultiPointIndex=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

	/*输出参数初始化为无效值*/
	*multiPointId=DSU_NULL_16;
	*multiPointState=DSU_NULL_8;

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru))
	{
		dsuMultiPointIndex=pDsuEmapStru->dsuEmapIndexStru->dsuMultiPointIndex;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*判断入口数据输入的有效性*/
		if (wVirtualPointNum<=MAX_VIRTUAL_SWITCH_NUM)
		{
			/*遍历多开道岔表*/
			for (i=0;i<dsuDataLenStru->wMultiPointLen;i++)
			{
				IdCount=0;
				StateCount=0;
				pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+i;
				
				for(k=0;k<wVirtualPointNum;k++)
				{
					for(j=0;j<pMultiPointStru->wType;j++)
					{
						if (wVirtualPointIds[k]==pMultiPointStru->wSwitchIds[j])
						{
							IdCount++;
							if (wVirtualPointStates[k]==pMultiPointStru->wSwitchStates[j])
							{
								tempPointState=j; /*多开道岔的状态=该虚拟道岔索引值+1*/
		
								StateCount++;  /*虚拟道岔状态相等的个数只能有一个，*/
							}
						}
					}
                  
				}
				/*找到所对应的多开道岔满足的条件：
				①当输入的道岔有N位（也是R1位）的道岔时，找到的相等的虚拟道岔数量IdCount等于输入参数虚拟道岔数量wVirtualPointNum+1。（因为N位和R位是相同道岔编号，遍历时会多计一次）；
                  当输入的道岔没有N位（也是R1位）的道岔时，找到的相等的虚拟道岔数量IdCount等于输入参数虚拟道岔数量wVirtualPointNum。
				②输入参数道岔状态和表中虚拟道岔状态相等的个数只能有一个，即StateCount=1
				*/
				if (((wVirtualPointNum==IdCount)||(wVirtualPointNum+1==IdCount))&&(1==StateCount))
				{
					*multiPointId=pMultiPointStru->wId;  /*多开道岔编号*/
					*multiPointState=tempPointState+1;    /*多开道岔状态(索引值+1)*/
					chReturnValue=1;
					break;
				}
			}

		}
		else
		{	
			/*入口参数异常，查询失败*/
			chReturnValue = 0;			
		}	
	}
	else
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;	
	}
	return chReturnValue;


}


/************************************************
 *函数功能:根据link获取对应的OC的ID(车车新增接口)
 *输入参数:linkId
 *输出参数:ocId
 *返回值:0失败,1成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcByLinkExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkId,UINT16 *ocId)
{
	UINT8 rstValue = 1;
	UINT16 ocIndex=0;
	*ocId = DSU_NULL_16;

	/*非法防护*/
	if(NULL == pDsuEmapStru)
	{
		rstValue = 0;
	}
	else
	{
		/*linkID值非法或非预期*/
		if((linkId > (pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM))||(0 == linkId))
		{
			rstValue = 0;
		}
		else
		{
			/*查询OC索引信息*/
			ocIndex = (pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkId]))->wManageOC;
			
			/*OC索引值非法或非预期*/
			if(ocIndex > (pDsuEmapStru->dsuEmapIndexStru->OCINDEXNUM))
			{
				rstValue = 0;
			}
			else
			{
				/*获取OC设备ID*/
				*ocId = (pDsuEmapStru->dsuStaticHeadStru->pOCStru+(pDsuEmapStru->dsuEmapIndexStru->dsuOCIndex[ocIndex]))->wId;
			}

		}
	}


	return rstValue;
}


/************************************************
 *函数功能:获取信号机位置(车车新增接口)
 *输入参数:wId:信号机索引编号
 *输出参数:position:信号机所处link以及偏移量
 *返回值:0:失败 1:成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetSignalPositionExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, LOD_STRU *position)
{
	/*函数返回值*/
	UINT8 rstValue = 0;

	/*指向信号机信息的结构体指针*/
	DSU_SIGNAL_STRU *pStru=NULL;

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 tmpSignalIndexNum=0;
	UINT16* signalIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;


	/*非法防护*/
	if (NULL != pDsuEmapStru)
	{
		/*获取信号机索引数组空间大小*/
		tmpSignalIndexNum=pDsuEmapStru->dsuEmapIndexStru->SIGNALINDEXNUM;
		signalIndex=pDsuEmapStru->dsuEmapIndexStru->dsuSignalIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*判断数据输入的有效性*/
		if((wId > tmpSignalIndexNum) || (DSU_NULL_16==signalIndex[wId]))	
		{		
			rstValue = 0;
		}
		else
		{
			pStru = dsuStaticHeadStru->pSignalStru+signalIndex[wId];
			position->Lnk = pStru->wLkId;
			position->Off = pStru->dwLkOfst;

			rstValue = 1;
		}
	}
	
	return rstValue;
}


/************************************************
 *函数功能:获取当前link是否有折返停车点(不含折返后停车点)(车车新增接口)
 *输入参数:wId:Link索引编号
 *输出参数:result:0x55有;0xAA:无
 *返回值:0:失败 1:成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuCheckArStopOnLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, UINT8 *result)
{
	UINT8 rstValue = 0;
	UINT16 len=0;/*停车点表数据量*/
	UINT16 i=0;
	UINT32 stopAttribute=0;
	const UINT32 bitAndValue = 20;/*逻辑与因子*/
	const UINT32 expectValue = 4;/*逻辑与的期望结果*/	
	DSU_STOPPOINT_STRU *pStru;/*指向停车点信息的结构体指针*/

	/*全局变量改传参调用涉及的全局变量*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	
	*result = DSU_FALSE;/*默认无*/

	/*非法防护*/
	if (NULL != pDsuEmapStru)
	{
		/*获取停车点结构体的数量*/
		len = pDsuEmapStru->dsuDataLenStru->wStopPointLen;		
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*逐个遍历数据*/
		for(i=0;i<len;i++)
		{
			/*获取实时数据*/
			pStru = dsuStaticHeadStru->pStopPointStru + i;

			/*满足link相等条件*/
			if(wId == pStru->wLinkId)
			{

				/*获取停车点属性*/
				stopAttribute = pStru->wAttribute;

				/*判定是否有符合预期的停车点信息*/
				if((stopAttribute & bitAndValue) == expectValue)
				{
					rstValue = 1;
					*result = DSU_TRUE;

					break;
				}
			}
		}
	}
	
	return rstValue;
}


/************************************************
 *函数功能:查询link序列内的道岔信息(车车新增接口)
 *输入参数:pLinkStru:Link序列信息
 *输出参数:pBaliseStru:link序列内包含的道岔信息
 *返回值:0:失败 1:成功
 *备注：add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetPointsBetweenLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pPointStru)
{
	UINT8 rstValue = 1;
	UINT16 i=0;
	DSU_LINK_STRU *pLinkCur=NULL;
	DSU_LINK_STRU *pLinkNxt=NULL;

	/*非法防护*/
	if((NULL == pDsuEmapStru)||(NULL == pLinkStru)||(NULL == pPointStru))
	{
		rstValue = 0;
	}
	else
	{

		/*道岔数量默认重置为0*/
		pPointStru->PointNum = 0;

		/*当只有一个Link的时候*/
		if(1 == pLinkStru->ObjNum)
		{
			/*获取当前link信息*/
			pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[0]]);
			
			/*起点是道岔 将起点相关信息值取出并使用*/
			if(EMAP_POINT_DOT == pLinkCur->wOrgnPointType)
			{
				pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wOrgnPointId;
				pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
				pPointStru->PointNum ++;
			}

			/*终点是道岔 将终点相关信息值取出并使用*/
			if(EMAP_POINT_DOT == pLinkCur->wTmnlPointType)
			{
				pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wTmnlPointId;
				pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
				pPointStru->PointNum ++;
			}
		}
		else if(1 < pLinkStru->ObjNum)
		{
			/*计算link序列方向*/
			pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[0]]);
			if(((pLinkStru->ObjIds[1]) == pLinkCur->wTmnlJointMainLkId) || ((pLinkStru->ObjIds[1]) == pLinkCur->wTmnlJointSideLkId))/*顺着link方向*/
			{
				/*step1:处理第一个link的最外层端点*/
				if(EMAP_POINT_DOT == pLinkCur->wOrgnPointType)
				{
					pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wOrgnPointId;
					pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
					pPointStru->PointNum ++;
				}

				/*step2:处理中间link*/
				for(i=0;i<((pLinkStru->ObjNum)-1);i++)
				{
					pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i]]);
					
					if(EMAP_POINT_DOT == pLinkCur->wTmnlPointType)
					{
						pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wTmnlPointId;

						if(DSU_NULL_16 != (pLinkCur->wTmnlJointSideLkId)) /*有侧线连接，说明当前是汇合link*/
						{
							if(pLinkCur->wTmnlJointSideLkId == pLinkStru->ObjIds[i+1])
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_SIDE; 
							}
							else
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_MAIN; 
							}
						}
						else/*没有侧线连接，说明当前不是定位link就是反位link*/
						{
							pLinkNxt = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i+1]]);

							if(pLinkNxt->wOrgnJointSideLkId == pLinkStru->ObjIds[i])
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_SIDE; 
							}
							else
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_MAIN; 
							}
						}
						
						pPointStru->PointNum ++;
					}
				}

				/*step3:处理最后一个link的最外层端点*/
				pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i]]);
				if(EMAP_POINT_DOT == pLinkCur->wTmnlPointType)
				{
					pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wTmnlPointId;
					pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
					pPointStru->PointNum ++;
				}
			}
			else
			{
				/*step1:处理第一个link的最外层端点*/
				if(EMAP_POINT_DOT == pLinkCur->wTmnlPointType)
				{
					pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wTmnlPointId;
					pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
					pPointStru->PointNum ++;
				}

				/*step2:处理中间link*/
				for(i=0;i<((pLinkStru->ObjNum)-1);i++)
				{
					pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i]]);
					
					if(EMAP_POINT_DOT == pLinkCur->wOrgnPointType)
					{
						pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wOrgnPointId;

						if(DSU_NULL_16 != pLinkCur->wOrgnJointSideLkId)/*有侧线连接，说明当前是汇合link*/
						{
							if(pLinkCur->wOrgnJointSideLkId == pLinkStru->ObjIds[i+1])
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_SIDE; 
							}
							else
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_MAIN; 
							}
						}
						else/*没有侧线连接，说明当前不是定位link就是反位link*/
						{
							pLinkNxt = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i+1]]);

							if(pLinkNxt->wTmnlJointSideLkId == pLinkStru->ObjIds[i])
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_SIDE; 
							}
							else
							{
								pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = EMAP_POINT_STATUS_MAIN; 
							}
						}
						
						pPointStru->PointNum ++;
					}
				}

				/*step3:处理第一个link的最外层端点*/
				pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkStru->ObjIds[i]]);
				if(EMAP_POINT_DOT == pLinkCur->wOrgnPointType)
				{
					pPointStru->PointInfo[(pPointStru->PointNum)].PointId = pLinkCur->wOrgnPointId;
					pPointStru->PointInfo[(pPointStru->PointNum)].PointExpectedStatus = DSU_NULL_8;
					pPointStru->PointNum ++;
				}
			}
		}
		else
		{
			/*nothing to do*/
		}
	}

	
	return rstValue;
}


/************************************************
 *函数功能:传入一个位置和一段长度，查找此长度范围内的道岔信息和link序列(车车新增接口)
 * 输入参数：	const LOD_STRU * pLODStru,	始端位置     const INT32     disPlacement,	查找长度
 * 输出参数：	DQU_LINKS_STRU *pLinkStru		Link序列 DQU_BALISE_STRU *pBaliseStru    道岔序列
 * 返回值：   	0,  查询失败  1,  查询成功 
 *备注：  经过的道岔均按定位处理 add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetLinkAndBaliseBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, const INT32 Displacement, 
								COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pBaliseStru, LOD_STRU *pEndLodStru)
{
	UINT8 chReturnValue=0;       /*用于函数返回值*/
	UINT16 CurrentLinkId = 0;         /*用于查找过程中临时保存当前LinkID*/
	UINT16 CurrentLinkIndex = 0; /*当前Link Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*用于循环查找过程中临时保存当前Link结构体*/
	UINT8 FindDir = 0;                  /*用于记录实际操作过程中的查找方向*/
	INT32 FindDisplacement = 0;         /*临时保存偏移量*/
	INT32 FindDisplacementSum = 0;       /*用于累加位移偏移量*/
	UINT16 k;                        /*循环用变量*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

	/*初始化link序列和道岔序列的有效数量为0*/
	pLinkStru->ObjNum =0;
	pBaliseStru->PointNum=0;

	/*数据防护*/
	if (NULL == pDsuEmapStru)
	{
		/*电子地图数据指针为空*/
		chReturnValue = 0;
	}
	else
	{
		/*数据赋值*/
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*特殊情况处理，偏移量Displacement为0*/
		if (Displacement == 0)
		{
			/*displacement如果为0，返回当前位置为所求*/
			pLinkStru->ObjIds[pLinkStru->ObjNum]=pCurrentLinkStru->wId;
			(pLinkStru->ObjNum)++;

			/*查询成功，返回查询结果*/
			chReturnValue=1;
			memcpy(pEndLodStru, pLODStru,sizeof(LOD_STRU));
		}
		/*判断输入LinkID的有效性*/
		else if((pLODStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16==dsuLinkIndex[pLODStru->Lnk]))
		{
			/*输入LinkId无效，查询失败，返回*/
			chReturnValue=0;
		}
		/*判断入口数据偏移量的有效性*/
		else if (pLODStru->Off > LinkLengthExp(pDsuEmapStru,pLODStru->Lnk))
		{
			/*数据无效，查询失败，返回*/
			chReturnValue=0;
		}
		/*判断输入方向有效性*/
		else if((EMAP_SAME_DIR!=pLODStru->Dir)&&(EMAP_CONVER_DIR!=pLODStru->Dir))
		{
			/*输入方向无效，查询失败，返回*/
			chReturnValue=0;
		}	
		else
		{

			/*如果displacement小于0，则按照displacement绝对值来查找新位置，方向取反*/
			if((EMAP_SAME_DIR==pLODStru->Dir)&&(0<Displacement))
			{
				/*正向查找*/
				FindDir=EMAP_SAME_DIR;
				FindDisplacement=Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(0>Displacement))
			{
				/*正向查找*/
				FindDir=EMAP_SAME_DIR;
				FindDisplacement=-Displacement;
			}
			else if((EMAP_SAME_DIR==pLODStru->Dir)&&(0>Displacement))
			{
				/*反向查找*/
				FindDir=EMAP_CONVER_DIR;
				FindDisplacement=-Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(0<Displacement))
			{
				/*反向查找*/
				FindDir=EMAP_CONVER_DIR;
				FindDisplacement=Displacement;
			}
			else
			{
				/*不可达分支*/
			}

			/*根据LinkID从数据库中查找当前link信息*/
			CurrentLinkIndex=dsuLinkIndex[pLODStru->Lnk];
			pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

			/*累加偏移量置0*/
			FindDisplacementSum=0;

			if (EMAP_SAME_DIR == FindDir)
			{
				/*根据当前位置正向累加偏移量*/
				FindDisplacementSum = pCurrentLinkStru->dwLength - pLODStru->Off;
			}
			else
			{
				/*根据当前位置反向累加偏移量*/
				FindDisplacementSum = pLODStru->Off;
			}

			/*根据位移判断新位置是否在同一个Link上*/
			if(FindDisplacementSum>=FindDisplacement)
			{
				/*在同一个link上*/
				pLinkStru->ObjIds[pLinkStru->ObjNum]=pCurrentLinkStru->wId;
				(pLinkStru->ObjNum)++;
				/*计算路径终点*/
				pEndLodStru->Lnk = pCurrentLinkStru->wId;
				pEndLodStru->Dir = pLODStru->Dir;
				if (EMAP_SAME_DIR == pLODStru->Dir)
				{
					pEndLodStru->Off = pLODStru->Dir + FindDisplacement;
				}
				else
				{
					pEndLodStru->Off = pLODStru->Dir - FindDisplacement;
				}
				chReturnValue=1;
			}
			else
			{
				for (k=0;k<OBJECT_NUM_MAX;k++)
				{		
					/*正向查找*/
					if(EMAP_SAME_DIR==FindDir)
					{
						/*连接link有效*/
						if(DSU_NULL_16 != pCurrentLinkStru->wTmnlJointMainLkId)
						{
							/*端点是道岔*/
							if(EMAP_POINT_DOT==pCurrentLinkStru->wTmnlPointType)
							{
								/*直接赋值端点编号和状态*/
								pBaliseStru->PointInfo[pBaliseStru->PointNum].PointId=pCurrentLinkStru->wTmnlPointId;
								pBaliseStru->PointInfo[pBaliseStru->PointNum].PointExpectedStatus=DSU_TRUE;
								(pBaliseStru->PointNum)++;	
							}
							/*端点为非道岔*/
							else
							{
								/*不进行处理*/				
							}


							/*pCurrentLinkStru指向相邻Link,相邻Link变为当前Link*/
							CurrentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wTmnlJointMainLkId];
							pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

							/*累加偏移长度*/
							FindDisplacementSum+=pCurrentLinkStru->dwLength;
							pLinkStru->ObjIds[pLinkStru->ObjNum]=pCurrentLinkStru->wId;
							(pLinkStru->ObjNum)++;
							
							/*累加的位移值加上当前Link长度大于输入偏移量，说明目的位置当前link上*/
							if(FindDisplacementSum>=FindDisplacement)
							{
								/*计算路径终点*/
								pEndLodStru->Lnk = pCurrentLinkStru->wId;
								pEndLodStru->Dir = pLODStru->Dir;
								if (EMAP_SAME_DIR == pLODStru->Dir)
								{
									pEndLodStru->Off = pCurrentLinkStru->dwLength - (FindDisplacementSum - FindDisplacement);
								}
								else
								{
									pEndLodStru->Off = FindDisplacementSum - FindDisplacement;
								}
								chReturnValue=1;
								break;
							}
							else
							{
								/*累加的位移值加上当前Link长度小于输入偏移量，说明目的位置不在当前link上，循环继续向前查找下一个Link*/
							}
						}
						else
						{
							/*获取下一个link失败*/
							/*已查找至线路终点*/
							/*计算路径终点*/
							pEndLodStru->Lnk = pCurrentLinkStru->wId;
							pEndLodStru->Dir = pLODStru->Dir;
							if (EMAP_SAME_DIR == pLODStru->Dir)
							{
								pEndLodStru->Off = pCurrentLinkStru->dwLength;
							}
							else
							{
								pEndLodStru->Off = 0;
							}
							chReturnValue=1;
							break;
						}
					}
					else/*反向查找*/
					{
						/*连接link有效*/
						if(DSU_NULL_16!=pCurrentLinkStru->wOrgnJointMainLkId)
						{
							/*端点是道岔*/
							if(EMAP_POINT_DOT==pCurrentLinkStru->wOrgnPointType)
							{
								/*直接赋值端点编号和状态*/
								pBaliseStru->PointInfo[pBaliseStru->PointNum].PointId=pCurrentLinkStru->wOrgnPointId;
								pBaliseStru->PointInfo[pBaliseStru->PointNum].PointExpectedStatus=DSU_TRUE;
								(pBaliseStru->PointNum)++;	
							}
							/*端点为非道岔*/
							else
							{
								/*不进行处理*/				
							}

							/*pCurrentLinkStru指向相邻Link,相邻Link变为当前Link*/
							CurrentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wOrgnJointMainLkId];
							pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

							/*累加偏移长度*/
							FindDisplacementSum+=pCurrentLinkStru->dwLength;
							pLinkStru->ObjIds[pLinkStru->ObjNum]=pCurrentLinkStru->wId;
							(pLinkStru->ObjNum)++;

							/*累加的位移值加上当前Link长度大于输入偏移量，说明目的位置当前link上*/
							if(FindDisplacementSum>=FindDisplacement)
							{
								/*计算路径终点*/
								pEndLodStru->Lnk = pCurrentLinkStru->wId;
								pEndLodStru->Dir = pLODStru->Dir;
								if (EMAP_SAME_DIR == pLODStru->Dir)
								{
									pEndLodStru->Off = pCurrentLinkStru->dwLength - (FindDisplacementSum - FindDisplacement);
								}
								else
								{
									pEndLodStru->Off = FindDisplacementSum - FindDisplacement;
								}
								chReturnValue=1;
								break;
							}
							else
							{
								/*累加的位移值加上当前Link长度小于输入偏移量，说明目的位置不在当前link上，循环继续向前查找下一个Link*/
							}
						}
						else
						{
							/*获取下一个link失败*/
							/*已查找至线路终点*/
							/*计算路径终点*/
							pEndLodStru->Lnk = pCurrentLinkStru->wId;
							pEndLodStru->Dir = pLODStru->Dir;
							if (EMAP_SAME_DIR == pLODStru->Dir)
							{
								pEndLodStru->Off = pCurrentLinkStru->dwLength;
							}
							else
							{
								pEndLodStru->Off = 0;
							}
							chReturnValue = 1;
							break;
						}

					}
				}

			}

		}

	}

	return chReturnValue;
}

/************************************************
 *函数功能:沿着应答器终点相邻方向查找两个应答器之间的距离(车车新增接口)
 *输入参数：wB1Index 应答器1索引编号  wB2Index 应答器2索引编号  wMaxLen	 两个应答器之间的最大距离
 *输出参数：*len 两个应答器之间的实际距离 
 *isFound 是否找到指定应答器
 *返回值：0:失败或未找到 1:成功找到 
 *备注：  经过的道岔均按定位处理 add by sds 2019-6-17
 ************************************************/
static UINT8 GetLenBetweenBaliseExpTmnl(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wB1Index, const UINT16 wB2Index, const UINT32 wMaxLen, UINT32 *len, UINT8 *isFound)
{
	UINT8 rstValue = 0;		/*函数返回值 0:未找到或者找到但有错误 1:正确找到*/
	DSU_BALISE_STRU *pPreBalise = NULL;/*上一层应答器*/

	UINT16 tmpArray[101]={0};		/*临时变量，保存当前遍历到的应答器索引编号，假设最多wMaxLen范围内可能存在100层应答器*/
	UINT8 tmpArrayIndex[101]={0};	/*临时变量，保存当前遍历到的应答器所在层中的索引，假设最多wMaxLen范围内可能存在100层应答器*/
	UINT8 tmpLevel = 1;				/*临时变量，保存当前处理的应答器所在层*/
	UINT32 tmpLen=0;				/*临时长度*/
	*len = DSU_NULL_32;
	*isFound = 0;
	
	/*把应答器1作为起始层开始查找*/
	tmpArray[0] = wB1Index;

	while((0<tmpLevel)  && (100>tmpLevel))
	{
		/*上一层应答器*/
		pPreBalise = pDsuEmapStru->dsuStaticHeadStru->pBaliseStru + (pDsuEmapStru->dsuEmapIndexStru->dsuBaliseIndex[(tmpArray[tmpLevel-1])]);

		/*判断上一层应答器的相邻应答器是否都遍历完成*/
		if(tmpArrayIndex[tmpLevel] < (pPreBalise->wTmnlBaliseNum))
		{
			/*设置当前层应答器*/
			tmpArray[tmpLevel] = (pPreBalise->dwTmnlBaliseId[tmpArrayIndex[tmpLevel]])&&DSU_NULL_16;
			tmpLen += pPreBalise->dwDistanceToTmnlBalise[tmpArrayIndex[tmpLevel]];

			/*找到应答器*/
			if(wB2Index == tmpArray[tmpLevel])
			{
				*isFound = 1;
				if(tmpLen <= wMaxLen)
				{
					*len = tmpLen;
					rstValue = 1;
				}

				break;
			}
			else
			{
				/*长度没超，继续找下一层*/
				if(tmpLen < wMaxLen)
				{
					tmpLevel += 1;
					tmpArrayIndex[tmpLevel] = 0;
				}
				else/*长度已超，长度回退，从本层下一个应答器继续*/
				{
					tmpLen -= pPreBalise->dwDistanceToTmnlBalise[tmpArrayIndex[tmpLevel]];
					tmpArrayIndex[tmpLevel] += 1;
				}
			}
		}
		else/*本层查找完，回到上一层的下一个应答器开始*/
		{			
			if((pPreBalise->wTmnlBaliseNum) > 0)
			{
				/*距离回退*/
				tmpLen -= pPreBalise->dwDistanceToTmnlBalise[tmpArrayIndex[tmpLevel] - 1];
			}

			tmpLevel -= 1;
			tmpArrayIndex[tmpLevel] += 1;
		}

	}

	return rstValue;
}


/************************************************
 *函数功能:沿着应答器起点相邻方向查找两个应答器之间的距离(车车新增接口)
 *输入参数：wB1Index 应答器1索引编号  wB2Index 应答器2索引编号  wMaxLen	 两个应答器之间的最大距离
 *输出参数：*len 两个应答器之间的实际距离 
 *isFound 是否找到指定应答器
 *返回值：0:失败或未找到 1:成功找到
 *备注：  经过的道岔均按定位处理 add by sds 2019-6-17
 ************************************************/
static UINT8 GetLenBetweenBaliseExpOrgn(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wB1Index, const UINT16 wB2Index, const UINT32 wMaxLen, UINT32 *len, UINT8 *isFound)
{
	UINT8 rstValue = 0;		/*函数返回值 0:未找到或者找到但有错误 1:正确找到*/
	DSU_BALISE_STRU *pPreBalise = NULL;/*上一层应答器*/

	UINT16 tmpArray[101]={0};		/*临时变量，保存当前遍历到的应答器索引编号，假设最多wMaxLen范围内可能存在100层应答器*/
	UINT8 tmpArrayIndex[101]={0};	/*临时变量，保存当前遍历到的应答器所在层中的索引，假设最多wMaxLen范围内可能存在100层应答器*/
	UINT8 tmpLevel = 1;				/*临时变量，保存当前处理的应答器所在层*/
	UINT32 tmpLen=0;				/*临时长度*/
	*len = DSU_NULL_32;
	*isFound = 0;
	
	/*把应答器1作为起始层开始查找*/
	tmpArray[0] = wB1Index;

	while(tmpLevel > 0 && tmpLevel < 100)
	{
		/*上一层应答器*/
		pPreBalise = pDsuEmapStru->dsuStaticHeadStru->pBaliseStru + (pDsuEmapStru->dsuEmapIndexStru->dsuBaliseIndex[(tmpArray[tmpLevel-1])]);

		/*判断上一层应答器的相邻应答器是否都遍历完成*/
		if(tmpArrayIndex[tmpLevel] < (pPreBalise->wOrgnBaliseNum))
		{
			/*设置当前层应答器*/
			tmpArray[tmpLevel] = (pPreBalise->dwOrgnBaliseId[tmpArrayIndex[tmpLevel]])&&DSU_NULL_16;
			tmpLen += pPreBalise->dwDistanceToOrgnBalise[tmpArrayIndex[tmpLevel]];

			if(wB2Index == tmpArray[tmpLevel])/*找到应答器*/
			{
				*isFound = 1;
				if(tmpLen <= wMaxLen)
				{
					*len = tmpLen;
					rstValue = 1;
				}

				break;
			}
			else
			{
				if(tmpLen < wMaxLen)/*长度没超，继续找下一层*/
				{
					tmpLevel += 1;
					tmpArrayIndex[tmpLevel] = 0;
				}
				else/*长度已超，长度回退，从本层下一个应答器继续*/
				{
					tmpLen -= pPreBalise->dwDistanceToOrgnBalise[tmpArrayIndex[tmpLevel]];
					tmpArrayIndex[tmpLevel] += 1;
				}
			}
		}
		else/*本层查找完，回到上一层的下一个应答器开始*/
		{			
			if((pPreBalise->wOrgnBaliseNum) > 0)
			{
				tmpLen -= pPreBalise->dwDistanceToOrgnBalise[tmpArrayIndex[tmpLevel] - 1];/*距离回退*/
			}

			tmpLevel -= 1;
			tmpArrayIndex[tmpLevel] += 1;
		}

	}

	return rstValue;
}

/************************************************
 *函数功能:查找两个应答器之间的距离(车车新增接口)
 *输入参数：wBaliseId1 应答器1ID wBaliseId2 应答器2ID  wMaxLen	 两个应答器之间的最大距离
 *输出参数：*len 两个应答器之间的实际距离 
 *返回值：0：失败  1：成功
 *备注：  经过的道岔均按定位处理 add by sds 2019-6-17
 ************************************************/
UINT8 GetLenBetweenBaliseExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT32 wBaliseId1, const UINT32 wBaliseId2, const UINT32 wMaxLen, UINT32 *len)
{	
	UINT16 wB1Index,wB2Index;/*索引编号*/
	UINT8 rstValue = 0;
	UINT8 isFound = 0;
	/*根据应答器ID获取索引编号*/
	wB1Index = wBaliseId1&DSU_NULL_16;
	wB2Index = wBaliseId2&DSU_NULL_16;

	/*检查应答器索引编号有效性*/
	if (NULL != pDsuEmapStru && NULL != (pDsuEmapStru->dsuEmapIndexStru))
	{		
		if((wB1Index > (pDsuEmapStru->dsuEmapIndexStru->BALISEINDEXNUM)) || (DSU_NULL_16==(pDsuEmapStru->dsuEmapIndexStru->dsuBaliseIndex[wB1Index])))
		{
			rstValue = 0;
		}
		else
		{
			if((wB2Index > (pDsuEmapStru->dsuEmapIndexStru->BALISEINDEXNUM)) || ((DSU_NULL_16==pDsuEmapStru->dsuEmapIndexStru->dsuBaliseIndex[wB2Index])))
			{
				rstValue = 0;
			}
			else
			{
				rstValue = 1;
			}
		}

	}

	/*先沿着终点方向查询*/
	if(1 == rstValue)
	{
		rstValue = GetLenBetweenBaliseExpTmnl(pDsuEmapStru, wB1Index, wB2Index, wMaxLen, len, &isFound);
	}

	/*再沿着起点方向查询*/
	if(0 == rstValue && 0 == isFound)
	{
		rstValue = GetLenBetweenBaliseExpOrgn(pDsuEmapStru, wB1Index, wB2Index, wMaxLen, len, &isFound);
	}

	return rstValue;
}


/************************************************
 *函数功能:沿指定方向查找两个位置间的道岔序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)  pEnd   查找终点位置(方向无效) pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pPointExp 查找到的序列 (0x55:定位 0xAA:反位)
 *返回值：0:失败或未找到 1:成功找到
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 GetPointsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_POINT_STRU *pPointExp)
{
	UINT8 rstValue = 1;
	UINT16 i=0;
	UINT16 j=0;
	UINT16 linkStart=0;
	UINT16 linkEnd=0;
	UINT32 linkOftStart=0;
	UINT32 linkOftEnd=0;
	DSU_LINK_STRU *pLink=NULL;
	DSU_LINK_STRU *qLink=NULL;
	UINT8 pointIncludeTag = 0x55;/*道岔是否要包含标志位*/

	/*基本条件判断*/
	if ((NULL == pDsuEmapStru) || (NULL == pStart) || (NULL == pEnd) || (NULL == pPointAll) || (NULL == pPointExp))	/*判断指针为空*/
	{
		rstValue = 0;
	}
	else
	{
		if(pPointAll->PointNum > POINT_NUM_MAX)
		{
			rstValue = 0;
		}
		else
		{
			pPointExp->PointNum = 0;/*初始化数量为0*/
		}
	}

	/*处理逻辑*/
	if(1 == rstValue)
	{		
		/*根据方向调整起终点link，保证查找是是顺着link方向的*/
		if(EMAP_SAME_DIR == pStart->Dir)
		{
			linkStart = pStart->Lnk;
			linkOftStart= pStart->Off;
			linkEnd = pEnd->Lnk;
			linkOftEnd= pEnd->Off;
		}
		else
		{
			linkStart = pEnd->Lnk;
			linkOftStart= pEnd->Off;
			linkEnd = pStart->Lnk;
			linkOftEnd= pStart->Off;
		}

		pLink = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkStart]);/*当前link*/
		
		/*对起始查找link的起点是否是道岔类型进行检查(端点=道岔并且查找的点在Link的起终点偏移量位置)*/
		//if (EMAP_POINT_DOT == pLink->wOrgnPointType)
		if ((EMAP_POINT_DOT == pLink->wOrgnPointType) && ((0 == linkOftStart)||(pLink->dwLength == linkOftStart)))
		{
			pPointExp->PointNum = (i + 1);
			for (j = 0; j < pPointAll->PointNum; j++)
			{
				if (pPointAll->PointInfo[j].PointId == pLink->wOrgnPointId)
				{
					/*设置找到的道岔信息*/
					pPointExp->PointInfo[i].PointId = pPointAll->PointInfo[j].PointId;
					pPointExp->PointInfo[i].PointStatus = pPointAll->PointInfo[j].PointStatus;
					i++;
				}
			}

		}

		while(1)
		{
			/*找到无效link就退出*/
			if(DSU_NULL_16==linkStart)
			{
				rstValue = 0;
				break;
			}

			pLink = pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkStart]);/*当前link*/
			
			if(EMAP_POINT_DOT == pLink->wTmnlPointType)
			{
				pointIncludeTag = 0x55;
				if (linkEnd == pLink->wId)
				{
					if ((0 == linkOftEnd) || (pLink->dwLength == linkOftEnd))
					{
						pointIncludeTag = 0x55;
					}
					else
					{
						pointIncludeTag = 0xAA;
					}					
				}
				else
				{
					/*do nothing*/
				}

				if (0x55 == pointIncludeTag)
				{
					pPointExp->PointNum = (i + 1);

					/*查找道岔*/
					for (j = 0; j<pPointAll->PointNum; j++)
					{
						if (pPointAll->PointInfo[j].PointId == pLink->wTmnlPointId)
						{
							/*设置找到的道岔信息*/
							pPointExp->PointInfo[i].PointId = pPointAll->PointInfo[j].PointId;
							pPointExp->PointInfo[i].PointStatus = pPointAll->PointInfo[j].PointStatus;

							/*道岔反位&&当前link的终点相邻侧线link有效*/
							if ((EMAP_POINT_STATUS_MAIN != pPointAll->PointInfo[j].PointStatus) && (DSU_NULL_16 != pLink->wTmnlJointSideLkId))
							{
								linkStart = pLink->wTmnlJointSideLkId;
							}
							else
							{
								linkStart = pLink->wTmnlJointMainLkId;
							}

							break;
						}
					}

					if (j >= pPointAll->PointNum)/*表示没找到对应的道岔*/
					{
						rstValue = 0;
						break;
					}
					else
					{
						i += 1;
					}
				}
				else
				{
					/*不需增加终点道岔，且已经查找到终点link，退出*/
					break;
				}				
			}
			else
			{
				linkStart = pLink->wTmnlJointMainLkId;
			}

			qLink = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkEnd]);/*当前link*/
			if ((linkStart == qLink->wTmnlJointMainLkId) || (linkStart == qLink->wTmnlJointSideLkId))
			{
				/*已经查找到终点link，退出*/
				break;
			}
		}
	}

	if((1 == rstValue) && (EMAP_SAME_DIR != pStart->Dir) && (pPointExp->PointNum > 0))
	{
		/*查到的道岔顺序翻转,i,j,linkStart只是当做现成的变量*/
		DQU_POINT_STATUS_STRU tmp;
		i = 0;
		j = pPointExp->PointNum-1;
		while(i<j)
		{
			tmp = pPointExp->PointInfo[i];
			pPointExp->PointInfo[i] = pPointExp->PointInfo[j];
			pPointExp->PointInfo[j] = tmp;
			i++;
			j--;
		}
	}

	return rstValue;
}


/************************************************
 *函数功能:沿指定方向查找两个位置间的PSD序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)  pEnd   查找终点位置(方向无效)  pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pObjectExp 查找到的序列
 *返回值：0:失败或未找到 1:成功找到
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 GetPsdsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	UINT8 rstValue = 1;
	UINT16 i=0;
	UINT16 j=0;
	UINT16 linkStart=0;
	UINT16 linkEnd=0;
	DSU_LINK_STRU *pLink=NULL;

	/*基本条件判断*/
	if ((NULL == pDsuEmapStru) || (NULL == pStart) || (NULL == pEnd) || (NULL == pPointAll) || (NULL == pObjectExp))	/*判断指针为空*/
	{
		rstValue = 0;
	}
	else
	{
		if(pPointAll->PointNum > POINT_NUM_MAX)
		{
			rstValue = 0;
		}
		else
		{
			pObjectExp->ObjNum = 0;/*初始化数量为0*/
		}
	}

	/*处理逻辑*/
	if(1 == rstValue)
	{		
		/*根据方向调整起终点link，保证查找是是顺着link方向的*/
		if(EMAP_SAME_DIR == pStart->Dir)
		{
			linkStart = pStart->Lnk;
			linkEnd = pEnd->Lnk;
		}
		else
		{
			linkStart = pEnd->Lnk;
			linkEnd = pStart->Lnk;
		}

		/*先对终点link进行判断*/
		for (j = 0; j<pDsuEmapStru->dsuDataLenStru->wScreenLen; j++)
		{
			if (linkEnd == (pDsuEmapStru->dsuStaticHeadStru->pScreenStru + j)->wRelatLkId)
			{
				pObjectExp->ObjNum = (i + 1);
				pObjectExp->ObjIds[i] = (pDsuEmapStru->dsuStaticHeadStru->pScreenStru + j)->wId;

				i += 1;
			}
		}

		while(linkStart != linkEnd)
		{
			/*找到无效link就退出*/
			if(DSU_NULL_16==linkStart)
			{
				rstValue = 0;
				break;
			}
			
			/*遍历PSD表，查找对应link信息*/
			for(j=0;j<pDsuEmapStru->dsuDataLenStru->wScreenLen;j++)
			{
				if(linkStart == (pDsuEmapStru->dsuStaticHeadStru->pScreenStru+j)->wRelatLkId)
				{
					pObjectExp->ObjNum = (i + 1);
					pObjectExp->ObjIds[i] = (pDsuEmapStru->dsuStaticHeadStru->pScreenStru+j)->wId;

					i+=1;
				}
			}

			pLink = pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkStart]);/*当前link*/

			if(EMAP_POINT_DOT == pLink->wTmnlPointType)
			{
				/*查找道岔*/
				for(j=0;j<pPointAll->PointNum;j++)
				{
					if(pPointAll->PointInfo[j].PointId == pLink->wTmnlPointId)
					{
						/*道岔反位&&当前link的终点相邻侧线link有效*/
						if((EMAP_POINT_STATUS_MAIN != pPointAll->PointInfo[j].PointStatus) && (DSU_NULL_16!=pLink->wTmnlJointSideLkId))
						{
							linkStart = pLink->wTmnlJointSideLkId;
						}
						else
						{
							linkStart = pLink->wTmnlJointMainLkId;
						}

						break;
					}
				}

				if(j >= pPointAll->PointNum)/*表示没找到对应的道岔*/
				{
					rstValue = 0;
					break;
				}
			}
			else
			{
				linkStart = pLink->wTmnlJointMainLkId;
			}
		}
	}

	return rstValue;
}

/************************************************
 *函数功能:沿指定方向查找两个位置间的ESB序列(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向) pEnd   查找终点位置(方向无效)  pPointAll 全部道岔信息(比查找的结果范围大且无序)
 *输出参数：pObjectExp 查找到的序列
 *返回值：0:失败或未找到 1:成功找到
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 GetEsbsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	UINT8 rstValue = 1;
	UINT16 i=0;
	UINT16 j=0;
	UINT16 linkStart=0;
	UINT16 linkEnd=0;
	DSU_LINK_STRU *pLink=NULL;

	/*基本条件判断*/
	if ((NULL == pDsuEmapStru) || (NULL == pStart) || (NULL == pEnd) || (NULL == pPointAll) || (NULL == pObjectExp))	/*判断指针为空*/
	{
		rstValue = 0;
	}
	else
	{
		if(pPointAll->PointNum > POINT_NUM_MAX)
		{
			rstValue = 0;
		}
		else
		{
			pObjectExp->ObjNum = 0;/*初始化数量为0*/
		}
	}

	/*处理逻辑*/
	if(1 == rstValue)
	{		
		/*根据方向调整起终点link，保证查找是是顺着link方向的*/
		if(EMAP_SAME_DIR == pStart->Dir)
		{
			linkStart = pStart->Lnk;
			linkEnd = pEnd->Lnk;
		}
		else
		{
			linkStart = pEnd->Lnk;
			linkEnd = pStart->Lnk;
		}

		while(linkStart != linkEnd)
		{
			/*找到无效link就退出*/
			if(DSU_NULL_16==linkStart)
			{
				rstValue = 0;
				break;
			}
			
			/*遍历ESB表，查找对应link信息*/
			for(j=0;j<pDsuEmapStru->dsuDataLenStru->wEmergStopLen;j++)
			{
				if(linkStart == (pDsuEmapStru->dsuStaticHeadStru->pEmergStopStru+j)->wRelatLkId)
				{
					pObjectExp->ObjNum = (i + 1);
					pObjectExp->ObjIds[i] = (pDsuEmapStru->dsuStaticHeadStru->pEmergStopStru+j)->wId;

					i+=1;
				}
			}

			pLink = pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkStart]);/*当前link*/

			if(EMAP_POINT_DOT == pLink->wTmnlPointType)
			{
				/*查找道岔*/
				for(j=0;j<pPointAll->PointNum;j++)
				{
					if(pPointAll->PointInfo[j].PointId == pLink->wTmnlPointId)
					{
						/*道岔反位&&当前link的终点相邻侧线link有效*/
						if((EMAP_POINT_STATUS_MAIN != pPointAll->PointInfo[j].PointStatus) && (DSU_NULL_16 != pLink->wTmnlJointSideLkId))
						{
							linkStart = pLink->wTmnlJointSideLkId;
						}
						else
						{
							linkStart = pLink->wTmnlJointMainLkId;
						}

						break;
					}
				}

				if(j >= pPointAll->PointNum)/*表示没找到对应的道岔*/
				{
					rstValue = 0;
					break;
				}
			}
			else
			{
				linkStart = pLink->wTmnlJointMainLkId;
			}
		}
	}
	
	return rstValue;
}

/************************************************
 *函数功能:通过输入的link序列、车尾位置和配置距离，查询车尾位置至配置距离范围内的link数量和ID(车车新增接口)
 *输入参数:pLinkIn:输入的link序列
 		   pTailLoc:车尾位置
		   dis:配置距离		  
 *输出参数:pLinkOut:查询到的link数量和ID
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 dsuLinksWithCfgDisInInputLinksExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis,COM_DQU_OBJECT_STRU *pLinkOut)
{
	UINT8 rstValue = 1;
	DSU_LINK_STRU *pLinkTmp=NULL;
	UINT32 len=0;
	UINT8 i=0;
	pLinkOut->ObjNum = 0;

	/*异常防护*/
	if((NULL == pDsuEmapStru) || (NULL == pLinkIn) || (NULL == pTailLoc))
	{
		rstValue = 0;
	}
	else if((pTailLoc->Dir != DSU_TRUE) && (pTailLoc->Dir != DSU_FALSE))
	{
		rstValue = 0;
	}
	else
	{
		pLinkTmp = pDsuEmapStru->dsuStaticHeadStru->pLinkStru;		

		/*查找车尾所在link在link序列中的位置*/
		for(i=0;i<pLinkIn->ObjNum;i++)
		{
			if((pTailLoc->Lnk) == (pLinkIn->ObjIds[i]))
			{
				break;
			}
		}

		if(i == (pLinkIn->ObjNum))
		{				
			rstValue = 0;/*车尾所在link不在link序列*/
		}
		else
		{
			i++;
			pLinkOut->ObjNum = 1;
			pLinkOut->ObjIds[0] = (pTailLoc->Lnk);

			/*根据方向计算当前车尾位置的长度*/
			if (DSU_FALSE == pTailLoc->Dir)
			{
				len = pTailLoc->Off;
			}			
			else
			{
				len = (pLinkTmp + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pTailLoc->Lnk]))->dwLength - pTailLoc->Off;
			}

			for(;i<pLinkIn->ObjNum;i++)
			{
				if(len >= dis)
				{
					break;
				}
				else
				{
					pLinkOut->ObjIds[(pLinkOut->ObjNum)] = (pLinkIn->ObjIds[i]);
					(pLinkOut->ObjNum) = (pLinkOut->ObjNum) + 1;
					len += (pLinkTmp + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkIn->ObjIds[i]]))->dwLength;
				}
			}
		}
	}

	return rstValue;
}


/************************************************
 *函数功能:通过输入的link序列、车尾位置和配置距离，查询车尾位置至配置距离范围内的link所属的OC数量和ID(车车新增接口)
 *输入参数:pLinkIn:输入的link序列
		   pTailLoc:车尾位置
		   dis:配置距离		  
 *输出参数:pLinkOut:查询到的OC数量和ID
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcsWithCfgDisInInputLinksExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis, COM_BASE_COMM_OC_STRU *pOcOut)
{
	UINT8 rstValue = 1;
	DSU_LINK_STRU *pLinkTmp;
	DSU_OC_STRU *pOcTmp;
	UINT32 len=0;
	UINT16 tmpOc = 0;
	UINT8 i=0;

	/*初始化数据*/
	pOcOut->CommOcNum = 0;

	/*异常防护*/
	if((NULL == pDsuEmapStru) || (NULL == pLinkIn) || (NULL == pTailLoc))
	{
		rstValue = 0;
	}
	else if((pTailLoc->Dir != DSU_TRUE) && (pTailLoc->Dir != DSU_FALSE))
	{
		rstValue = 0;
	}
	else
	{
		pLinkTmp = pDsuEmapStru->dsuStaticHeadStru->pLinkStru;		
		pOcTmp = pDsuEmapStru->dsuStaticHeadStru->pOCStru;

		/*查找车尾所在link在link序列中的位置*/
		for(i=0;i<pLinkIn->ObjNum;i++)
		{
			if((pTailLoc->Lnk) == (pLinkIn->ObjIds[i]))
			{
				break;
			}
		}

		if(i == (pLinkIn->ObjNum))
		{				
			rstValue = 0;/*车尾所在link不在link序列*/
		}
		else
		{
			i++;
			rstValue = dsuOcByLinkExp(pDsuEmapStru,pTailLoc->Lnk,&tmpOc);			
		}
	}

	if(1 == rstValue)
	{
		pOcOut->CommOcNum = 1;
		pOcOut->CommOcIDs[0] = tmpOc;
			
		/*根据方向计算当前车尾位置的长度*/
		len = pTailLoc->Off;
		if(DSU_FALSE == pTailLoc->Dir )
		{
			len = (pLinkTmp + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pTailLoc->Lnk]))->dwLength;
		}

		for(;i<pLinkIn->ObjNum;i++)
		{
			if(len >= dis)
			{
				break;
			}
			else
			{
				rstValue = dsuOcByLinkExp(pDsuEmapStru,pLinkIn->ObjIds[i],&tmpOc);
				if(1 == rstValue)
				{
					/*如果当前OC与上一个OC一样，则不重复放入*/
					if(tmpOc != pOcOut->CommOcIDs[(pOcOut->CommOcNum)-1])
					{
						pOcOut->CommOcIDs[(pOcOut->CommOcNum)] = tmpOc;
						(pOcOut->CommOcNum) = (pOcOut->CommOcNum) + 1;
					}

					len += (pLinkTmp + (pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[pLinkIn->ObjIds[i]]))->dwLength;
				}
				else
				{
					break;
				}
			}
		}
	}

	if(0 == rstValue)
	{		
		pOcOut->CommOcNum = 0;
	}

	return rstValue;
}

/************************************************
 *函数功能:查询设备所属OC(车车新增接口)
 *输入参数:devId:设备ID		  
 *输出参数:devType:设备类型(1:道岔,2:信号机,3:紧急停车按钮,4:交叉渡线)(对于道岔，暂时取汇合link为准)
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcWithInputDevIdExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 devId,const UINT8 devType, UINT16 *ocId)
{
	UINT8 rstValue = 1;
	UINT16 linkId = 0;
	UINT16 PointId = 0;
	*ocId = DSU_NULL_16;


	/*异常防护*/
	if((NULL == pDsuEmapStru) || (devType < 1) || (devType > 4))
	{
		rstValue = 0;
	}
	else
	{	
		if (1 == devType)/*1:道岔,*/
		{
			if (devId > pDsuEmapStru->dsuEmapIndexStru->POINTINDEXNUM)
			{
				rstValue = 0;
			}
			else
			{
				linkId = (pDsuEmapStru->dsuStaticHeadStru->pPointStru + (pDsuEmapStru->dsuEmapIndexStru->dsuPointIndex[devId]))->wConfluentLkId;
			}
		}
		else if (2 == devType)/*2:信号机,*/
		{
			if (devId > pDsuEmapStru->dsuEmapIndexStru->SIGNALINDEXNUM)
			{
				rstValue = 0;
			}
			else
			{
				linkId = (pDsuEmapStru->dsuStaticHeadStru->pSignalStru + (pDsuEmapStru->dsuEmapIndexStru->dsuSignalIndex[devId]))->wLkId;
			}
		}
		else if (3 == devType)/*3:紧急停车按钮*/
		{
			if (devId > pDsuEmapStru->dsuEmapIndexStru->EMERGSTOPINDEXNUM)
			{
				rstValue = 0;
			}
			else
			{
				linkId = (pDsuEmapStru->dsuStaticHeadStru->pEmergStopStru + (pDsuEmapStru->dsuEmapIndexStru->dsuEmergStopIndex[devId]))->wRelatLkId;
			}
		}
		else if (4 == devType)
		{
			if (devId > pDsuEmapStru->dsuEmapIndexStru->TRANSITZONEINDEXNUM)
			{
				rstValue = 0;
			}
			else
			{
				PointId = (pDsuEmapStru->dsuStaticHeadStru->pTransitZoneStru + (pDsuEmapStru->dsuEmapIndexStru->dsuTransitZoneIndex[devId]))->wLinkBalise[0];

				linkId = (pDsuEmapStru->dsuStaticHeadStru->pPointStru + (pDsuEmapStru->dsuEmapIndexStru->dsuPointIndex[devId]))->wConfluentLkId;
			}

		}
		else
		{
			/*nothing*/
		}
		if (1 == rstValue)
		{
			rstValue = dsuOcByLinkExp(pDsuEmapStru, linkId, ocId);
		}
	}
	return rstValue;
}

/************************************************
 *函数功能:查询link上的信号机(车车新增接口)
 *输入参数:linkId:link
		   dir:信号机作用方向	  
 *输出参数:pObject 查找到的序列
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
static UINT8 GetSignalsOnLink(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId,const UINT16 dir, COM_DQU_OBJECT_STRU *pObject)
{
	UINT8 rstValue = 1;
	UINT16 i=0;
	DSU_SIGNAL_STRU *pSignal=NULL;

	/*异常防护*/
	if((NULL == pDsuEmapStru)||(NULL==pObject))
	{
		rstValue = 0;
	}
	else
	{
		/*获取信号机结构体*/
		pSignal = pDsuEmapStru->dsuStaticHeadStru->pSignalStru;

		/*重置数量*/
		pObject->ObjNum=0;

		for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
		{
			if((linkId == pSignal->wLkId) && (dir == pSignal->wProtectDir))
			{
				pObject->ObjIds[pObject->ObjNum]=pSignal->wId;
				pObject->ObjNum++;

			}
		}

	}

	return rstValue;
}

/************************************************
 *函数功能:查询一定范围内与列车方向(0x55)相同的信号机(车车新增接口)
 *输入参数    pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向) 
		      pEnd   查找终点位置(方向无效) 
			  pPointAll 从起点到终点的道岔信息(有序排列)
 *输出参数：pObjectExp 查找到的序列
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
static UINT8 dsuSignalBetweenPisitionsExpDirTrue(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	UINT8 rstValue = 1;
	DSU_SIGNAL_STRU *pSignal=NULL;
	UINT16 i=0;
	UINT16 j=0;
	UINT16 linkCur=0;
	DSU_LINK_STRU *pLinkCur=NULL;
	
	pSignal = pDsuEmapStru->dsuStaticHeadStru->pSignalStru;

	/*首尾端是同一link的情况*/
	if(pStart->Lnk == pEnd->Lnk)
	{
		for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
		{
			if((pStart->Lnk == pSignal->wLkId) && (DSU_TRUE == pSignal->wProtectDir) && (pSignal->dwLkOfst >= pStart->Off) && (pSignal->dwLkOfst <= pEnd->Off))
			{
				pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
				pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
			}

			pSignal++;
		}
	}
	else
	{
		linkCur = pStart->Lnk;

		while(linkCur != pEnd->Lnk)
		{			
			/*找到无效link就退出*/
			if( DSU_NULL_16== linkCur)
			{
				rstValue = 0;
				break;
			}

			pSignal = pDsuEmapStru->dsuStaticHeadStru->pSignalStru;/*指针重置*/
			
			/*step1:处理当前link*/
			if(linkCur == pStart->Lnk)
			{				
				for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
				{
					if((linkCur == pSignal->wLkId) && (DSU_TRUE == pSignal->wProtectDir) && (pSignal->dwLkOfst >= pStart->Off))
					{
						pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
						pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
					}

					pSignal++;
				}
			}
			else
			{
				for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
				{
					if((linkCur == pSignal->wLkId) && (DSU_TRUE == pSignal->wProtectDir))
					{
						pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
						pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
					}

					pSignal++;
				}
			}

			/*step2:计算下一个link*/
			pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkCur]);/*当前link*/

			if(EMAP_POINT_DOT == pLinkCur->wTmnlPointType)
			{
				if(j >= pPointAll->PointNum)/*输入道岔序列数量不够*/
				{
					rstValue = 0;
					break;
				}

				if(pLinkCur->wTmnlPointId == pPointAll->PointInfo[j].PointId)
				{
					/*道岔反位&&当前link的终点相邻侧线link有效*/
					if((DSU_TRUE != pPointAll->PointInfo[j].PointExpectedStatus) && ( DSU_NULL_16!= pLinkCur->wTmnlJointSideLkId))
					{
						linkCur = pLinkCur->wTmnlJointSideLkId;
					}
					else
					{
						linkCur = pLinkCur->wTmnlJointMainLkId;
					}
					j++;
				}
				else
				{
					rstValue = 0;
					break;
				}
			}
			else
			{
				linkCur = pLinkCur->wTmnlJointMainLkId;
			}
		}
	}

	if(0 == rstValue)
	{
		pObjectExp->ObjNum = 0;
	}

	return rstValue;
}


/************************************************
 *函数功能:查询一定范围内与列车方向(0xAA)相同的信号机(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
		    pEnd   查找终点位置(方向无效)
		    pPointAll 从起点到终点的道岔信息(有序排列)
 *输出参数：pObjectExp 查找到的序列
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
static UINT8 dsuSignalBetweenPisitionsExpDirFalse(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	UINT8 rstValue = 1;
	DSU_SIGNAL_STRU *pSignal=NULL;
	UINT16 i,j,linkCur;
	DSU_LINK_STRU *pLinkCur;
	
	pSignal = pDsuEmapStru->dsuStaticHeadStru->pSignalStru;

	/*首尾端是同一link的情况*/
	if(pStart->Lnk == pEnd->Lnk)
	{
		for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
		{
			if((pStart->Lnk == pSignal->wLkId) && (DSU_FALSE == pSignal->wProtectDir) && (pSignal->dwLkOfst >= pEnd->Off) && (pSignal->dwLkOfst <= pStart->Off))
			{
				pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
				pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
			}

			pSignal++;
		}
	}
	else
	{
		linkCur = pStart->Lnk;
		j=0;/*计算道岔序列索引*/

		while(linkCur != pEnd->Lnk)
		{			
			/*找到无效link就退出*/
			if(linkCur == 0xFFFF)
			{
				rstValue = 0;
				break;
			}

			pSignal = pDsuEmapStru->dsuStaticHeadStru->pSignalStru;/*指针重置*/
			
			/*step1:处理当前link*/
			if(linkCur == pStart->Lnk)
			{				
				for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
				{
					if((linkCur == pSignal->wLkId) && (DSU_FALSE == pSignal->wProtectDir) && (pSignal->dwLkOfst <= pStart->Off))
					{
						pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
						pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
					}

					pSignal++;
				}
			}
			else
			{
				for(i=0;i<pDsuEmapStru->dsuDataLenStru->wSignalLen;i++)
				{
					if((linkCur == pSignal->wLkId) && (DSU_FALSE == pSignal->wProtectDir))
					{
						pObjectExp->ObjNum = pObjectExp->ObjNum + 1;
						pObjectExp->ObjIds[pObjectExp->ObjNum-1] = pSignal->wId;
					}

					pSignal++;
				}
			}

			/*step2:计算下一个link*/
			pLinkCur = pDsuEmapStru->dsuStaticHeadStru->pLinkStru+(pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[linkCur]);/*当前link*/

			if(EMAP_POINT_DOT == pLinkCur->wOrgnPointType)
			{
				if(j >= pPointAll->PointNum)/*输入道岔序列数量不够*/
				{
					rstValue = 0;
					break;
				}

				if(pLinkCur->wOrgnPointId == pPointAll->PointInfo[j].PointId)
				{
					/*道岔反位&&当前link的起点相邻侧线link有效*/
					if((DSU_TRUE != pPointAll->PointInfo[j].PointExpectedStatus) && (DSU_NULL_16!= pLinkCur->wOrgnJointSideLkId))
					{
						linkCur = pLinkCur->wOrgnJointSideLkId;
					}
					else
					{
						linkCur = pLinkCur->wOrgnJointMainLkId;
					}
					j++;
				}
				else
				{
					rstValue = 0;
					break;
				}
			}
			else
			{
				linkCur = pLinkCur->wOrgnJointMainLkId;
			}
		}
	}

	if(0 == rstValue)
	{
		pObjectExp->ObjNum = 0;
	}

	return rstValue;
}

/************************************************
 *函数功能:查询一定范围内与列车方向相同的信号机(车车新增接口)
 *输入参数：pStart 查找起点位置以及方向(0x55:顺着link方向;0xAA:逆着link方向)
		    pEnd   查找终点位置(方向无效)
		    pPointAll 从起点到终点的道岔信息(有序排列)
 *输出参数：pObjectExp 查找到的序列
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 dsuSignalBetweenPisitionsExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp)
{
	UINT8 rstValue = 1;

	/*基本条件判断*/
	if ((NULL == pDsuEmapStru) || (NULL == pStart) || (NULL == pEnd) || (NULL == pPointAll) || (NULL == pObjectExp))	/*判断指针为空*/
	{
		rstValue = 0;
	}
	else
	{
		if(DSU_TRUE == pStart->Dir)
		{
			rstValue = dsuSignalBetweenPisitionsExpDirTrue(pDsuEmapStru,pStart,pEnd,pPointAll,pObjectExp);
		}
		else if(DSU_FALSE == pStart->Dir)
		{
			rstValue = dsuSignalBetweenPisitionsExpDirFalse(pDsuEmapStru,pStart,pEnd,pPointAll,pObjectExp);
		}
		else
		{
			rstValue = 0;
		}
	}

	return rstValue;
}


/************************************************
 *函数功能:根据当前位置获取所属OC(车车新增接口)
 *输入参数：当前位置
 *输出参数：道岔的位置
 *返回值：0:失败或未找到 1:成功找到
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 GetLocInOdIdExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pCurrLoc, UINT16 *pOcID)
{
	UINT16 j = 0;
	UINT8 RtnNo = 0;

	/*异常防护*/
	if ((NULL != pDsuEmapStru) && (NULL != pCurrLoc))
	{
		for (j = 0;j<pDsuEmapStru->dsuDataLenStru->wLinkLen;j++)
		{
			if (pCurrLoc->Lnk == (pDsuEmapStru->dsuStaticHeadStru->pLinkStru + j)->wId)
			{
				*pOcID = (pDsuEmapStru->dsuStaticHeadStru->pOCStru+((pDsuEmapStru->dsuStaticHeadStru->pLinkStru + j)->wManageOC-1))->wId;
				RtnNo = 1;
				break;
			}
		}
	}

	return RtnNo;
}

/************************************************
 *函数功能:根据道岔ID获取道岔的位置(车车新增接口)
 *输入参数：当前位置
 *输出参数：道岔的位置
 *返回值：0:失败或未找到 1:成功找到
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 GetPointLocFromIdExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 PointId, DQU_POINT_LOC_STRU *pPointLoc)
{
	UINT16 j = 0;
	UINT8 RtnNo = 0;

	/*异常防护*/
	if ((NULL != pDsuEmapStru)&& (NULL != pPointLoc))
	{
		/*遍历道岔数据*/
		for (j = 0;j<pDsuEmapStru->dsuDataLenStru->wPointLen;j++)
		{
			if (PointId == (pDsuEmapStru->dsuStaticHeadStru->pPointStru+j)->wId)
			{
				pPointLoc->PointId = PointId;
				pPointLoc->ConfluentLkId = ((pDsuEmapStru->dsuStaticHeadStru->pPointStru+j)->wConfluentLkId);
				pPointLoc->MainLkId = ((pDsuEmapStru->dsuStaticHeadStru->pPointStru + j)->wMainLkId);
				pPointLoc->SideLkId = ((pDsuEmapStru->dsuStaticHeadStru->pPointStru + j)->wSideLkId);

				RtnNo = 1;
				break;
			}
		}
	}

	return RtnNo;
}


/************************************************
 *函数功能:查询2个link的相对方向（若LINK B是LINK A的终点相邻LINK  则A到B为正向）(车车新增接口)
 *输入参数:		linkAID:LinkA的索引ID
				linkBID:LinkB的索引ID
 *输出参数:dir:2个link的相对方向  0x55=正向 0xAA=反向  0xFF=无效
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetDirOfLinkToLinkExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkAID,const UINT16 linkBID,UINT8 *dir)
{

	UINT8 reValue = 0;/*函数返回值*/
	UINT16 wCurrentLinkIndex = 0; /*link序列内当前正在处理的link编号*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*用于循环查找过程中临时保存当前Link结构体*/

	/*全局变量改传参调用涉及的全局变量*/
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;/*add by sds 2019-1-24*/


	/*默认置为无效*/
	*dir = DSU_NULL_8;
	/*电子地图数据结构体防护*/
	if ((NULL != pDsuEmapStru)&&(NULL !=pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{

		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;/*add by sds 2019-1-24*/

		/*根据LinkID从数据库中查找当前link信息*/
		pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+dsuLinkIndex[linkAID];

		/*判断连接方向*/
		if(linkBID==pCurrentLinkStru->wOrgnJointMainLkId
			||linkBID==pCurrentLinkStru->wOrgnJointSideLkId)
		{
			/*反向*/
			*dir=DSU_FALSE; 	
			reValue = 1;

		}
		else if(linkBID==pCurrentLinkStru->wTmnlJointMainLkId
			||linkBID==pCurrentLinkStru->wTmnlJointSideLkId)
		{
			/*正向*/
			*dir=DSU_TRUE; 
			reValue = 1;

		}
		else
		{
			/*两个link不相连，意味着查询失败*/
			reValue = 0;
		}	

	}
	else
	{
		/*电子地图数据指针为空*/
		reValue = 0;
	}

	return reValue;
}

/************************************************
 *函数功能:根据物理区段序列，查询包含的link序列(车车新增接口)
 *输入参数:		PhysicalSgmt	物理区段信息
 *输出参数:		pLinks link序列信息
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-21
 ************************************************/
UINT8 dsuRegionBlockTolinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *PhysicalSgmtStru, COM_DQU_OBJECT_STRU *LinksStru)
{

	UINT16 i, j, n = 0;
	UINT8 RtnNo = 0;
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru = NULL;    /*计轴区段临时变量结构体*/
	UINT16 arTempLink[200] = { 0 };              /*临时存放LINK序列*/
	UINT16 iTempIndex = 0;                       /*存放LINK序列数组所对应的索引*/

												 /*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;                /*全局变量，记录读取的DSU数据中的各类型数据的数量*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;          /*存放数据库中每种结构的头指针*/

	if ((NULL != pDsuEmapStru) && (NULL != PhysicalSgmtStru) && (NULL != LinksStru))
	{
		dsuDataLenStru = pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = pDsuEmapStru->dsuStaticHeadStru;

		iTempIndex = 0;
		/*遍历包含的物理区段*/
		for (i = 0; i < PhysicalSgmtStru->ObjNum; i++)
		{
			/*遍历计轴区段表，寻找与当前物理区段所对应的计轴区段，计算当前计轴区段所包含的LINK*/
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				pAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
				if (pAxleSgmtStru->wRelatePhysicalSgmtId == PhysicalSgmtStru->ObjIds[i])
				{
					/*存储起点LINK*/
					arTempLink[iTempIndex++] = pAxleSgmtStru->wOrgnLkId;

					/*存储中间LINK*/
					for (n = 0; n<MIDDLE_LINK_MAX_NUM; n++)
					{
						if (DSU_NULL_16 != pAxleSgmtStru->wMiddleLinkId[n])
						{
							arTempLink[iTempIndex++] = pAxleSgmtStru->wMiddleLinkId[n];

						}
					}

					/*存储终点LINK*/
					arTempLink[iTempIndex++] = pAxleSgmtStru->wTmnlLkId;
				}
			}
		}
		if (0 == iTempIndex)/*数组越界或未找到对应的link*/
		{
			RtnNo = 0;
		}
		else
		{
			/*去掉重复的Link*/
			LinksStru->ObjNum = RemoveRep(arTempLink, iTempIndex, LinksStru->ObjIds);
			RtnNo = 1;/*查询成功*/
		}
	}

	return RtnNo;
}

/************************************************
 *函数功能:根据link编号，查询所属ITS设备ID(车车新增接口)
 *输入参数:		UINT16 LinkWid	link编号
 *输出参数:		UINT16  ItsId 所属ITS设备ID
 *返回值:0:失败 1:成功
 *备注： add by sds 2019-6-21
 ************************************************/
UINT8 dsuItsIdOfLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 LinkWid, UINT16 *ItsId)
{
	UINT8 RtnNo = 0;
	DSU_LINK_STRU	*pLinkStruStru;                           /*LINK信息结构体指针*/


	if ((NULL != pDsuEmapStru) && (NULL != LinkWid))
	{	
		/*输出所属ITS设备ID信息*/
		pLinkStruStru = pDsuEmapStru->dsuStaticHeadStru->pLinkStru + pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex[LinkWid];
		*ItsId = pLinkStruStru->wManageITS;
		RtnNo = 1;/*查询成功*/
	}
	else
	{
		RtnNo = 0;/*查询失败*/
	}
	return RtnNo;
}

/************************************************
*函数功能:获取x限速区段信息(车车新增接口)
*输入参数:wId:信号机索引编号
*输出参数:position:信号机所处link以及偏移量
*返回值:0:失败 1:成功
*备注：add by sds 2019-6-17
************************************************/
UINT8 dsuGetRespeedSgmtExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, DSU_RESPEED_SGMT_STRU *RespeedSgmtStru)
{
	/*函数返回值*/
	UINT8 rstValue = 0;
	UINT8 i = 0;
	/*全局变量改传参调用涉及的全局变量*/
	UINT16 RespeedSgmtIndexNum = 0;
	UINT16* RespeedIndex = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;


	/*非法防护*/
	if (NULL != pDsuEmapStru)
	{
		///*获取信号机索引数组空间大小*/
		//RespeedSgmtIndexNum = pDsuEmapStru->dsuEmapIndexStru->RESPEEDSGMTINDEXNUM;
		//RespeedIndex = pDsuEmapStru->dsuEmapIndexStru->dsuRespeedSgmtIndex;
		//dsuStaticHeadStru = pDsuEmapStru->dsuStaticHeadStru;

		for (i = 0; i < pDsuEmapStru->dsuDataLenStru->wRespeedSgmtLen; i++)
		{
			if (wId == (pDsuEmapStru->dsuStaticHeadStru->pRespeedSgmtStru + i)->wId)
			{
				memcpy(RespeedSgmtStru, pDsuEmapStru->dsuStaticHeadStru->pRespeedSgmtStru + i, sizeof(DSU_RESPEED_SGMT_STRU));
				rstValue = 1;
			}
		}

	}

	return rstValue;
}

/************************************************
*函数功能:根据计轴区段序列，查询包含的link序列(车车新增接口)
*输入参数:		PhysicalSgmt	物理区段信息
*输出参数:		pLinks link序列信息
*返回值:0:失败 1:成功
*备注： add by sds 2019-6-21
************************************************/
UINT8 dsuAxleSgmtTolinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pAxleSgmtStru, COM_DQU_OBJECT_STRU *LinksStru)
{

	UINT16 i, j, n = 0;
	UINT8 RtnNo = 0;
	DSU_AXLE_SGMT_STRU *TempAxleSgmtStru = NULL;    /*计轴区段临时变量结构体*/
	UINT16 arTempLink[200] = { 0 };              /*临时存放LINK序列*/
	UINT16 iTempIndex = 0;                       /*存放LINK序列数组所对应的索引*/

												 /*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;                /*全局变量，记录读取的DSU数据中的各类型数据的数量*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;          /*存放数据库中每种结构的头指针*/

	if ((NULL != pDsuEmapStru) && (NULL != pAxleSgmtStru) && (NULL != LinksStru))
	{
		dsuDataLenStru = pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = pDsuEmapStru->dsuStaticHeadStru;

		iTempIndex = 0;
		/*遍历包含的物理区段*/
		for (i = 0; i < pAxleSgmtStru->ObjNum; i++)
		{
			/*遍历计轴区段表，寻找与当前物理区段所对应的计轴区段，计算当前计轴区段所包含的LINK*/
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				TempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
				if (TempAxleSgmtStru->wId == pAxleSgmtStru->ObjIds[i])
				{
					/*存储起点LINK*/
					arTempLink[iTempIndex++] = TempAxleSgmtStru->wOrgnLkId;

					/*存储中间LINK*/
					for (n = 0; n<MIDDLE_LINK_MAX_NUM; n++)
					{
						if (DSU_NULL_16 != TempAxleSgmtStru->wMiddleLinkId[n])
						{
							arTempLink[iTempIndex++] = TempAxleSgmtStru->wMiddleLinkId[n];

						}
					}

					/*存储终点LINK*/
					arTempLink[iTempIndex++] = TempAxleSgmtStru->wTmnlLkId;
				}
			}
		}
		if (0 == iTempIndex)/*数组越界或未找到对应的link*/
		{
			RtnNo = 0;
		}
		else
		{
			/*去掉重复的Link*/
			LinksStru->ObjNum = RemoveRep(arTempLink, iTempIndex, LinksStru->ObjIds);
			RtnNo = 1;/*查询成功*/
		}
	}

	return RtnNo;
}

