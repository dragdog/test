/************************************************************************
*
* 文件名   ：  dquStructInit.c
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2009.11.21
* 作者     ：  软件部
* 功能描述 ：  数据查询函数结构处理过程  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#include <stdio.h>
#include "dquStructInit.h"
#include "dquCbtcType.h"
 

static void ROUTE_STRU_init_HLT(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru);

/**************************************
函数功能：link结构体初始化函数
参数说明：[in] pData 数据地址（二进制文件）
          [out] pLinkStrc 被初始化的结构体指针
返回值：无
备注：结构内容已经完善
***************************/
static void LINK_STRU_init_DFT(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	 UINT8 * pData = pDataAdd;
	/*link编号*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
				
	/*link长度（cm）*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
 
	/*Link所处线路的上下行信息*/   /*新增---严广学*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
 
   /*link起点端点类型*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link起点端点编号*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link起点相连接的相邻正线link编号*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
		
	/*link起点相连接的相邻侧线link编号*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点端点类型*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
	
	/*link终点端点编号*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻正线link编号*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻侧线link编号*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所属ZC区域编号*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*所属ATS区域编号*/
	pLinkStrc->wManageATS = ShortFromChar(pData);
	pData += 2;

	/*所属逻辑CI区域编号*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*所属物理CI区域编号*/   /*----确定长度为2个字节---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link限速信息属性*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link坡度信息属性*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link所属车站区域ID:2013年8月13日,由车载提出删除;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;		
	

}

static void LINK_STRU_init_HLT(UINT8 *pDataAdd, DSU_LINK_STRU *pLinkStrc)
{
	UINT8 *pData = pDataAdd;
	/*link编号*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
					
	/*link长度（cm）*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
	 
	/*Link所处线路的上下行信息*/   /*新增---严广学*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
	 
   /*link起点端点类型*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link起点端点编号*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link起点相连接的相邻正线link编号*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
			
	/*link起点相连接的相邻侧线link编号*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点端点类型*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
		
	/*link终点端点编号*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻正线link编号*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻侧线link编号*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所属ZC区域编号*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*所属ATS区域编号*/
	pLinkStrc->wManageStation = ShortFromChar(pData);
	pData += 2;

	/*所属逻辑CI区域编号*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*所属物理CI区域编号*/   /*----确定长度为2个字节---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link限速信息属性*/
	pLinkStrc->wLinkResSpeed = ShortFromChar(pData);
	pData += 2;

	/*Link坡度信息属性*/
	pLinkStrc->wGrade = ShortFromChar(pData);
	pData += 2;

	/*Link所属车站区域ID:2013年8月13日,由车载提出删除;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;
		

	pLinkStrc->wManageATS = ShortFromChar(pData);       
	pData += 2;

	pLinkStrc->wLinkLogicDirUpAndDown = *pData;       /*互联互通需求新增，系统定义逻辑方向与上下行方向的关系 add by qxt 20160811*/
	pData++;

	pLinkStrc->wLogicDirChanged = *pData;            /*逻辑方向变化点，解决灯泡线问题， add by qxt 20170816*/
	pData++;

	pLinkStrc->wTunAttribute = *pData;				/*Link隧道属性，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	pData++;
	
}

static void LINK_STRU_init_DG(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	 UINT8 * pData = pDataAdd;
	/*link编号*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
				
	/*link长度（cm）*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
 
	/*Link所处线路的上下行信息*/   /*新增---严广学*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
 
   /*link起点端点类型*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link起点端点编号*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link起点相连接的相邻正线link编号*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
		
	/*link起点相连接的相邻侧线link编号*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点端点类型*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
	
	/*link终点端点编号*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻正线link编号*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻侧线link编号*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所属ZC区域编号*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*所属ATS区域编号*/
	pLinkStrc->wManageStation = ShortFromChar(pData);
	pData += 2;

	/*所属逻辑CI区域编号*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*所属物理CI区域编号*/   /*----确定长度为2个字节---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link限速信息属性*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link坡度信息属性*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link所属车站区域ID:2013年8月13日,由车载提出删除;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;
	

	pLinkStrc->wManageATS= ShortFromChar(pData);       
	pData += 2;

	pLinkStrc->wLinkLogicDirUpAndDown= *pData;       /*互联互通需求新增，系统定义逻辑方向与上下行方向的关系 add by qxt 20160811*/
    pData++;

}


/**************************************
函数功能：link结构体初始化函数-车车 add by sds 2019-6-13
参数说明：[in] pData 数据地址（二进制文件）
          [out] pLinkStrc 被初始化的结构体指针
返回值：无
备注：结构内容已经完善
***************************/
static void LINK_STRU_init_CC(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	UINT8 * pData = pDataAdd;
	/*link编号*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;

	/*link长度（cm）*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;

	/*Link所处线路的上下行信息*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;

	/*link起点端点类型*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link起点端点编号*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link起点相连接的相邻正线link编号*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link起点相连接的相邻侧线link编号*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点端点类型*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;

	/*link终点端点编号*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻正线link编号*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link终点相连接的相邻侧线link编号*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所属OC区域编号*/
	pLinkStrc->wManageOC = ShortFromChar(pData);
	pData += 2;

	/*Link限速信息属性*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link坡度信息属性*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link隧道属性*/
	pLinkStrc->wTunAttribute= *pData;
	pData += 1;

	/*所属ITS设备ID*/
	pLinkStrc->wManageITS = ShortFromChar(pData);
	pData += 2;

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;		

}

/*调用接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
void LINK_STRU_init(UINT8 *pDataAdd,DSU_LINK_STRU *pLinkStrc)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		LINK_STRU_init_DFT(pDataAdd, pLinkStrc);
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		LINK_STRU_init_HLT(pDataAdd, pLinkStrc);
	}
	else if(DQU_CBTC_DG == cbtcVersionType)
	{
		LINK_STRU_init_DG(pDataAdd, pLinkStrc);
	}
	else if(DQU_CBTC_CC == cbtcVersionType)
	{
		LINK_STRU_init_CC(pDataAdd, pLinkStrc);
	}
	else
	{
       /*不可达分支*/
	}
   
}

/**************************************
函数功能：道岔数据结构体初始化函数--车车
参数说明：[in] pData 数据地址（二进制文件）
          [out]pPointStru 被初始化的结构体指针
返回值：无
***************************/
static void POINT_STRU_init_CC(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*道岔编号*/
	pPointStru->wId = ShortFromChar(pData);
	pData += 2;

	/*道岔名称*/
	for (i=0;i<16;i++)
	{
		pPointStru->szCaption[i] = *pData;
		pData++;
	}

	/*所处正线link编号*/
	pPointStru->wMainLkId = ShortFromChar(pData);
	pData += 2;

	/*所处正线link偏移量(cm)*/
	pPointStru->dwMainLkOfst = LongFromChar(pData);
	pData += 4;

	/*所处侧线link编号*/
	pPointStru->wSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所处侧线link偏移量(cm)*/
	pPointStru->dwSideLkOfst = LongFromChar(pData);
	pData += 4;

	/*所处汇合link编号*/
	pPointStru->wConfluentLkId = ShortFromChar(pData);
	pData += 2;

	/*所处汇合link偏移量(cm)*/
	pPointStru->dwConfluentLkOfst = LongFromChar(pData);
	pData += 4;

	/*道岔反位静态限制速度(cm/s)*/
	pPointStru->wSideResSpeed = ShortFromChar(pData);
	pData += 2;

	/*道岔后方防护长度*/
	pPointStru->wRearSwitchProtecDis = ShortFromChar(pData);
	pData += 2;

	/*道岔属性*/
	pPointStru->DProperty = *pData;
	pData ++;
	

}

/**************************************
函数功能：道岔数据结构体初始化函数
参数说明：[in] pData 数据地址（二进制文件）
          [out]pPointStru 被初始化的结构体指针
返回值：无
备注：结构需增加
    UINT16  wManageZC;				   所属ZC区域编号
	UINT16  wManagePhysicalCI;		   所属物理CI区域编号
	UINT16  wManageLogicCI;			   所属逻辑CI区域编号
修改说明：根据合库数据结构V19.0.0需求修改，20180208 lmy
***************************/
static void POINT_STRU_init_DFT(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*道岔编号*/
	pPointStru->wId = ShortFromChar(pData);
	pData += 2;

	/*道岔名称*/
	for (i=0;i<16;i++)
	{
		pPointStru->szCaption[i] = *pData;
		pData++;
	}

	/*联动道岔编号*/
	pPointStru->DPointOtherId = ShortFromChar(pData);
	pData += 2;

	/*道岔点公里标*/
	pPointStru->dwPointKilo = LongFromChar(pData);
	pData += 4;

	/*所处正线link编号*/
	pPointStru->wMainLkId = ShortFromChar(pData);
	pData += 2;

	/*所处正线link偏移量(cm)*/
	pPointStru->dwMainLkOfst = LongFromChar(pData);
	pData += 4;

	/*所处侧线link编号*/
	pPointStru->wSideLkId = ShortFromChar(pData);
	pData += 2;

	/*所处侧线link偏移量(cm)*/
	pPointStru->dwSideLkOfst = LongFromChar(pData);
	pData += 4;

	/*所处汇合link编号*/
	pPointStru->wConfluentLkId = ShortFromChar(pData);
	pData += 2;

	/*所处汇合link偏移量(cm)*/
	pPointStru->dwConfluentLkOfst = LongFromChar(pData);
	pData += 4;

	/*道岔反位静态限制速度(cm/s)*/
	pPointStru->wSideResSpeed = ShortFromChar(pData);
	pData += 2;

}

/**************************************
函数功能：道岔数据结构体初始化函数
参数说明：[in] pData 数据地址（二进制文件）
          [out]pPointStru 被初始化的结构体指针
返回值：无
修改说明：根据车车数据结构V1.0.0需求修改，2019-7-10 by sds
***************************/
void POINT_STRU_init(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType)
		||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		POINT_STRU_init_DFT(pDataAdd, pPointStru);
	} 
	else if(DQU_CBTC_CC == cbtcVersionType)
	{
		POINT_STRU_init_CC(pDataAdd, pPointStru);
	}
	else
	{
		/*不可达分支*/
	}

}

/**************************************
函数功能：信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
	UINT16  wManageZC;					所属ZC区域编号
	UINT16  wManagePhysicalCI;		所属物理CI区域编号
	UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 cbtcVersionType = 0;
	cbtcVersionType = GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType))
	{
		SIGNAL_STRU_init_DFT(pDataAdd, pSignalStru);
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		SIGNAL_STRU_init_CC(pDataAdd, pSignalStru);
	}
	else
	{
		/*分支不可达*/
	}
}

/**************************************
函数功能：DFT信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
[out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
UINT16  wManageZC;					所属ZC区域编号
UINT16  wManagePhysicalCI;		所属物理CI区域编号
UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init_DFT(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*编号*/
	pSignalStru->wId = ShortFromChar(pData);
    pData += 2;
	
	/*信号机名称*/

	for(i=0;i<12;i++)
	{
		pSignalStru->szCaption[i] = *pData;
       pData += 1;
	}

	/*信号机类型*/
	pSignalStru->wType = ShortFromChar(pData);
    pData += 2;
	
	/*信号机属性*/
	pSignalStru->wAttribute = ShortFromChar(pData);
    pData += 2;
	
	/*信号机所处link编号*/
	pSignalStru->wLkId = ShortFromChar(pData);
    pData += 2;
	
	/*信号机所处link偏移量(cm)*/
	pSignalStru->dwLkOfst = LongFromChar(pData);
    pData += 4;

	/*信号机防护方向(16进制)*/
	pSignalStru->wProtectDir = ShortFromChar(pData);
    pData += 2;
	
	/*信号机防护点所处link编号*/
	pSignalStru->wProtectLinkId = ShortFromChar(pData);
    pData += 2;
	
	/*信号机防护点所处link偏移量(cm)*/
	pSignalStru->dwProtectLinkOfst = LongFromChar(pData);
    pData += 4;
	
	/*开口信号机标志*/	

	pSignalStru->wPassSignal = ShortFromChar(pData);
    pData += 2;

	/*信号机判断闯信号功能标志*/
	pSignalStru->wEnableSignal = ShortFromChar(pData);
    pData += 2;
		
	/*信号机亮灭功能标志*/
	pSignalStru->wShineSignal = ShortFromChar(pData);
    pData += 2;	   

	/*信号机灯列*/
	pSignalStru->dwSignalDengLie = LongFromChar(pData);
	pData += 4;

	/*信号机灯位封闭*/
	pSignalStru->sDengWeiFengBi = *pData;
	pData++;
}

/**************************************
函数功能：CC信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
[out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
UINT16  wManageZC;					所属ZC区域编号
UINT16  wManagePhysicalCI;		所属物理CI区域编号
UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init_CC(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i = 0;
	/*编号*/
	pSignalStru->wId = ShortFromChar(pData);
	pData += 2;

	/*信号机名称*/

	for (i = 0; i<12; i++)
	{
		pSignalStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*信号机类型*/
	pSignalStru->wType = ShortFromChar(pData);
	pData += 2;

	/*信号机属性*/
	pSignalStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*信号机所处link编号*/
	pSignalStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*信号机所处link偏移量(cm)*/
	pSignalStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*信号机防护方向(16进制)*/
	pSignalStru->wProtectDir = ShortFromChar(pData);
	pData += 2;

	/*信号机灯列*/
	pSignalStru->dwSignalDengLie = LongFromChar(pData);
    pData += 4;	 

	/*信号机灯位封闭*/
	pSignalStru->sDengWeiFengBi = *pData;
    pData ++;	
}

/**************************************
函数功能：应答器数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pBaliseStru 被初始化的结构体指针
返回值：无
备注：该结构体无需改动
***************************/
/*默认版本相邻应答器个数为10个*/
static void BALISE_STRU_init_DFT(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pBaliseStru->wId = ShortFromChar(pData);
    pData += 2;

	/*应答器ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
    pData += 4;

	/*应答器名称*/
	for(i=0;i<8;i++)
	{
	   pBaliseStru->szCaption[i] = *pData;
       pData += 1;
	}

	/*应答器所处link编号*/
	pBaliseStru->wLkId = ShortFromChar(pData);
    pData += 2;

	/*应答器所处link偏移量(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
    pData += 4;

	/*与应答器关联的信号机编号*/
	pBaliseStru->wRelateSignalID = ShortFromChar(pData);
    pData += 2;

	/*应答器作用方向()*/
	pBaliseStru->wBaliseDir = ShortFromChar(pData);
    pData += 2;

	/*应答器类型*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
    pData += 2;

	/*在所处link逻辑方向上起点的相邻的应答器数量*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
    pData += 2;

	
	for(i=0;i<10;i++)
	{
		/*在所处link逻辑方向上起点相邻的应答器的编号*/
	   pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
       pData += 4;

	   /*沿线路的起点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
        pData += 2;

	  /*在所处link逻辑方向上起点相邻的应答器的距离*/	
	   pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
       pData += 4;

	    /*起点相邻应答器关联道岔编号*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
        pData += 2;

	   /*起点相邻应答器关联道岔状态*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*在所处link逻辑方向上终点的相邻的应答器数量*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
    pData += 2;

	/*在所处link逻辑方向上终点相邻的应答器的编号*/
	for(i=0;i<10;i++)
	{
	   pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
       pData += 4;	

	   /*沿线路的终点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
        pData += 2;

	   /*在所处link逻辑方向上终点相邻的应答器的距离*/		
	   pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
       pData += 4;

	    /*终点相邻应答器关联道岔编号*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
        pData += 2;
	
	    /*终点相邻应答器关联道岔状态*/	
	
		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*LEU编号*/
	pBaliseStru->wLeuId = ShortFromChar(pData);
    pData += 2;
}
/*互联互通版本相邻应答器个数增加到是16个*/
static void BALISE_STRU_init_HLT(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pBaliseStru->wId = ShortFromChar(pData);
	pData += 2;

	/*应答器ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
	pData += 4;

	/*应答器名称*/
	for(i=0;i<8;i++)
	{
		pBaliseStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*应答器所处link编号*/
	pBaliseStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*应答器所处link偏移量(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*与应答器关联的信号机编号*/
	pBaliseStru->wRelateSignalID = ShortFromChar(pData);
	pData += 2;

	/*应答器作用方向()*/
	pBaliseStru->wBaliseDir = ShortFromChar(pData);
	pData += 2;

	/*应答器类型*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
	pData += 2;

	/*在所处link逻辑方向上起点的相邻的应答器数量*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<16;i++)
	{
		/*在所处link逻辑方向上起点相邻的应答器的编号*/
		pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
		pData += 4;

		/*沿线路的起点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*在所处link逻辑方向上起点相邻的应答器的距离*/	
		pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
		pData += 4;

		/*起点相邻应答器关联道岔编号*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*起点相邻应答器关联道岔状态*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*在所处link逻辑方向上终点的相邻的应答器数量*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
	pData += 2;

	/*在所处link逻辑方向上终点相邻的应答器的编号*/
	for(i=0;i<16;i++)
	{
		pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
		pData += 4;	

		/*沿线路的终点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*在所处link逻辑方向上终点相邻的应答器的距离*/		
		pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
		pData += 4;

		/*终点相邻应答器关联道岔编号*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*终点相邻应答器关联道岔状态*/	

		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*LEU编号*/
	pBaliseStru->wLeuId = ShortFromChar(pData);
	pData += 2;

	/*电子地图版本信息---根据互联互通需求增加 by hgj 20150507*/
	pBaliseStru->wEmapVersion = ShortFromChar(pData);
	pData += 2;

}


/**************************************
函数功能：应答器数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pAxleStru 被初始化的结构体指针
返回值：无
修改说明：根据车车数据结构V1.0.0需求增加，2019-7-10 sds
***************************/
static void BALISE_STRU_init_CC(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pBaliseStru->wId = ShortFromChar(pData);
	pData += 2;

	/*应答器ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
	pData += 4;

	/*应答器名称*/
	for(i=0;i<8;i++)
	{
		pBaliseStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*应答器所处link编号*/
	pBaliseStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*应答器所处link偏移量(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*应答器类型*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
	pData += 2;

	/*在所处link逻辑方向上起点的相邻的应答器数量*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		/*在所处link逻辑方向上起点相邻的应答器的编号*/
		pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
		pData += 4;

		/*沿线路的起点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*在所处link逻辑方向上起点相邻的应答器的距离*/	
		pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
		pData += 4;

		/*起点相邻应答器关联道岔编号*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*起点相邻应答器关联道岔状态*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*在所处link逻辑方向上终点的相邻的应答器数量*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
	pData += 2;

	/*在所处link逻辑方向上终点相邻的应答器的编号*/
	for(i=0;i<10;i++)
	{
		pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
		pData += 4;	

		/*沿线路的终点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*在所处link逻辑方向上终点相邻的应答器的距离*/		
		pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
		pData += 4;

		/*终点相邻应答器关联道岔编号*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*终点相邻应答器关联道岔状态*/	

		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*电子地图版本信息---根据互联互通需求增加 by hgj 20150507*/
	pBaliseStru->wEmapVersion = ShortFromChar(pData);
	pData += 2;

}

/*调用接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
void BALISE_STRU_init(UINT8 *pDataAdd,DSU_BALISE_STRU *pBaliseStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO ==  cbtcVersionType)||(DQU_CBTC_CPK == cbtcVersionType) )
	{
		BALISE_STRU_init_DFT(pDataAdd, pBaliseStru);/*FAO和合库共用的处理*/
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType))
	{
		BALISE_STRU_init_HLT(pDataAdd, pBaliseStru);/*互联互通、单轨的处理*/
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		BALISE_STRU_init_CC(pDataAdd, pBaliseStru);/*车车的处理*/
	}
	else
	{
       /*不可达分支*/
	}
}

/**************************************
函数功能：计轴器数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pAxleStru 被初始化的结构体指针
返回值：无
备注：该结构体无需改动
修改说明：根据合库数据结构V19.0.0需求修改，20180208 lmy
***************************/
void AXLE_STRU_init(UINT8 *pDataAdd,DSU_AXLE_STRU *pAxleStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pAxleStru->wId = ShortFromChar(pData);
	pData += 2;

	/*计轴器名称*/		
	for(i=0;i<16;i++)
	{
		pAxleStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*计轴类型信息*/
	pAxleStru->wType = ShortFromChar(pData);
	pData += 2;

	/*计轴器公里标*/
	pAxleStru->dwAxleKilo = LongFromChar(pData);
	pData += 4;

	/*计轴器所处link编号*/	
	pAxleStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*计轴器所处link偏移量(cm)*/
	pAxleStru->dwLkOfst = LongFromChar(pData);
	pData += 4;
}

/**************************************
函数功能：安全屏蔽门数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pScreenStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
void SCREEN_STRU_init(UINT8 * pDataAdd,DSU_SCREEN_STRU * pScreenStru)
{
	UINT8 * pData = pDataAdd;
	pScreenStru->wId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wLocalPlatformId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wRelatLkId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wwRelatLkOrient = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wVobcCiID = ShortFromChar(pData);
    pData += 2;
	
}

/**************************************
函数功能：紧急停车按钮数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pEmergStopStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
void EMERG_STOP_STRU_init(UINT8 * pDataAdd,DSU_EMERG_STOP_STRU * pEmergStopStru)
{
	UINT8 * pData = pDataAdd;
	pEmergStopStru->wId = ShortFromChar(pData);
    pData += 2;

	pEmergStopStru->wLocalPlatformId = ShortFromChar(pData);
    pData += 2;

	pEmergStopStru->wRelatLkId = ShortFromChar(pData);
    pData += 2;

}

/**************************************
函数功能：停车区域信息数据结构体初始化函数(原站台表)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStationStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
   UINT32 dwStatTmnlLinkOfst;				站台物理范围终止点link偏移量
   UINT16  wManagePhysicalCI            所属物理CI区域编号
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
static void STATION_STRU_init_DFT(UINT8 * pDataAdd,DSU_STATION_STRU * pStationStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pStationStru->wId = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
        pData += 1;
	}
    /*目的地编码*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
        pData += 1;
	}

	pStationStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStationStru->wAttribute = ShortFromChar(pData);
    pData += 2;

	pStationStru->wMinParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wMaxParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wScreenNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wEmergStopNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wStopPointNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<3;i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wDoorOpenMode = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorOpenTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorCloseMode = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorCloseTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wParkSteadyTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wStationID = ShortFromChar(pData); /*2013年8月13日,由车载提出增加*/
    pData += 2;
}

static void STATION_STRU_init_HLT(UINT8 * pDataAdd,DSU_STATION_STRU * pStationStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}
	/*目的地编码*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}

	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;

	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;
	/*包含的停车点,互联互通为四个*/
	for(i=0;i<4;i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wStationID = ShortFromChar(pData); /*2013年8月13日,由车载提出增加*/
	pData += 2;

	pStationStru->wLuggageOpenMode = ShortFromChar(pData);	/*行李开门方式，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	pData += 2;

	pStationStru->wLuggageCloseMode = ShortFromChar(pData);	/*行李关门方式，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	pData += 2;

	pStationStru->wCrashResSpeed = ShortFromChar(pData);	/*允许碰撞限速值，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	pData += 2;

	pStationStru->wATSStationId = ShortFromChar(pData);	    /*ATS停车区域ID*/
	pData += 2;

}

/***********************************************************************************
函数功能：停车区域信息数据结构体初始化函数(原站台表)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStationStru 被初始化的结构体指针
返回值：无
备注：依据互联互通数据结构V12.0.0修改后，
	  DG和HLT不再使用相同的初始化函数，故增加此函数，add by lmy 20180312
***********************************************************************************/
static void STATION_STRU_init_DG(UINT8 *pDataAdd, DSU_STATION_STRU *pStationStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*索引编号*/
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;
	/*所属车站名称*/
	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}
	/*目的地编码*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}
	/*停车区域Link编号*/
	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;
	/*停车区域属性*/
	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;
	/*站台最小停站时间*/
	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;
	/*站台最大停站时间*/
	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;
	/*站台默认停站时间*/
	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;
	/*屏蔽门数量*/
	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;
	/*屏蔽门编号*/
	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*紧急停车按钮数量*/
	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;
	/*紧急停车按钮编号*/
	for(i = 0; i < 2; i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*包含停车点数目*/
	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;
	/*包含的停车点*/
	for(i = 0; i < 4; i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*站台开门方式*/
	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;
	/*站台开门时间间隔*/
	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;
	/*站台关门方式*/
	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;
	/*站台关门时间间隔*/
	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;
	/*默认停稳时间*/
	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;
	/*站台所属车站编号*/
	pStationStru->wStationID = ShortFromChar(pData); 
	pData += 2;
}

/***********************************************************************************
函数功能：停车区域信息数据结构体初始化函数-车车系统使用 add by sds 2019-6-13
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStationStru 被初始化的结构体指针
返回值：无
备注：
***********************************************************************************/
static void STATION_STRU_init_CC(UINT8 *pDataAdd, DSU_STATION_STRU *pStationStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*索引编号*/
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;

	/*所属车站名称*/
	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*目的地编码*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}

	/*停车区域Link编号*/
	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;

	/*停车区域属性*/
	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*站台最小停站时间*/
	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;

	/*站台最大停站时间*/
	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;

	/*站台默认停站时间*/
	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;

	/*屏蔽门数量*/
	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;

	/*屏蔽门编号*/
	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*紧急停车按钮数量*/
	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;

	/*紧急停车按钮编号*/
	for(i = 0; i < 2; i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*包含停车点数目*/
	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;

	/*包含的停车点*/
	for(i = 0; i < 4; i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*站台开门方式*/
	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;

	/*站台开门时间间隔*/
	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;

	/*站台关门方式*/
	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;

	/*站台关门时间间隔*/
	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;

	/*默认停稳时间*/
	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;

	/*站台所属车站编号*/
	pStationStru->wStationID = ShortFromChar(pData); 
	pData += 2;

	/*ITS停车区域编号*/
	pStationStru->wITSStationId = ShortFromChar(pData); 
	pData += 2;

}

/*调用接口*/
/*备注：依据互联互通数据结构V12.0.0修改，modify by lmy 20180312*/
void STATION_STRU_init(UINT8 *pDataAdd,DSU_STATION_STRU *pStationStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		STATION_STRU_init_DFT(pDataAdd, pStationStru);
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		STATION_STRU_init_HLT(pDataAdd, pStationStru);
	}
	else if (DQU_CBTC_DG == cbtcVersionType)
	{
		STATION_STRU_init_DG(pDataAdd, pStationStru);
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		STATION_STRU_init_CC(pDataAdd, pStationStru);
	}
	else
	{
       /*不可达分支*/
	}
}

/**************************************
函数功能：停车点数据结构体初始化函数--车车系统专用
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStoppointStru 被初始化的结构体指针
返回值：无
备注：

***************************/
static void STOPPOINT_STRU_init_CC(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 * pData = pDataAdd;
	pStoppointStru->wId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wAttribute = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wDir = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwLinkOfst = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATOParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATPParkArea = LongFromChar(pData);
    pData += 4;
	pStoppointStru->dwATPParkArea = 150;
	pStoppointStru->wStationID = ShortFromChar(pData);
    pData += 2;
	
}

/**************************************
函数功能：停车点数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStoppointStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
	UINT16  wMinStopTime;					最小停站时间
	UINT16  wMaxStopTime;					最大停站时间
	UINT16  wStopTime;						默认停站时间

***************************/
static void STOPPOINT_STRU_init_DFT(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 * pData = pDataAdd;
	pStoppointStru->wId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wAttribute = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wDir = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwLinkOfst = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wOverLapId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwATOParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATPParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wStationID = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wStationDoorID = *pData;
	pData += 1;
	
}

/*备注：依据互联互通数据结构V12.0.0修改，modify by lmy 20180312*/
/**************************************
函数功能：停车点数据结构体初始化函数-因增加车车系统 修改函数功能
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStoppointStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
	UINT16  wMinStopTime;					最小停站时间
	UINT16  wMaxStopTime;					最大停站时间
	UINT16  wStopTime;						默认停站时间

***************************/
void STOPPOINT_STRU_init(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType)
		|| (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) )
	{
		STOPPOINT_STRU_init_DFT(pDataAdd, pStoppointStru);
	} 
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		STOPPOINT_STRU_init_CC(pDataAdd, pStoppointStru);
	}
	else
	{
		/*不可达分支*/
	}
}

/**************************************
函数功能：计轴区段数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pAxleSgmtStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
	UINT16  wMiddleLinkId;							计轴区段中间包含link编号
	UINT32 dwOrgnLinkOfst;							起点所处link偏移量
	UINT32 dwTmnlLinkOfst;							终点所处link偏移量
	UINT16  wOrgnRelatPointId;					计轴区段起点关联道岔编号
	UINT16  wOrgnMainAxleSgmtId;					计轴区段起点连接定位计轴区段编号
	UINT16  wOrgnSideAxleSgmtId;					计轴区段起点连接反位计轴区段编号
	UINT16  wTmnlRelatPointId;					计轴区段终点关联道岔编号
	UINT16  wTmnlMainAxleSgmtId;					计轴区段终点连接定位计轴区段编号
	UINT16  wTmnlSideAxleSgmtId;					计轴区段终点连接反位计轴区段编号
	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[3];					关联计轴区段编号
	UINT16  wManageZC;							所属ZC区域编号
	UINT16  wManagePhysicalCI;					所属物理CI区域编号

***************************/
void AXLE_SGMT_STRU_init(UINT8 * pDataAdd,DSU_AXLE_SGMT_STRU * pAxleSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*编号*/
	pAxleSgmtStru->wId = ShortFromChar(pData);
    pData += 2;

	/*计轴区段名称*/
	for(i=0;i<16;i++)
	{
		pAxleSgmtStru->szCaption[i] = *pData;
        pData += 1;
	}

	/*起点计轴器编号*/
	pAxleSgmtStru->wOrgnAxleId = ShortFromChar(pData);
    pData += 2;

	/*终点计轴器编号*/
	pAxleSgmtStru->wTmnlAxleId = ShortFromChar(pData);
    pData += 2;

	/*起点所处link编号*/
	pAxleSgmtStru->wOrgnLkId = ShortFromChar(pData);
    pData += 2;

	/*终点所处link编号*/
	pAxleSgmtStru->wTmnlLkId = ShortFromChar(pData);
    pData += 2;

	/*计轴区段包含的逻辑区段的个数*/
	pAxleSgmtStru->wIncludeLogicSgmtNum = ShortFromChar(pData);
    pData += 2;

	/*计轴区段包含的逻辑区段的编号*/
	for(i=0;i<20;i++)
	{
		pAxleSgmtStru->wLogicSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*关联道岔数目*/
	pAxleSgmtStru->wRelatPointNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<4;i++)
	{
		/*包含道岔编号*/
		pAxleSgmtStru->wRelatPointId[i] = ShortFromChar(pData);
        pData += 2;
	    /*包含道岔状态*/
		pAxleSgmtStru->wRelatPointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*关联道岔数目*/
	pAxleSgmtStru->wRelatePhysicalSgmtId = ShortFromChar(pData);
    pData += 2;

}


/**************************************
函数功能：进路表数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pRouteStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
			UINT16  wCBTCAccessLink1RelatePointNum;		CBTC接近区段1关联道岔数目
	UINT16  wCBTCAccessLink1RelatePointId[];	CBTC接近区段1关联道岔编号	
	UINT16  wCBTCAccessLink1RelatePointSta[];   CBTC接近区段1关联道岔状态
	UINT16  wCBTCAccessLink2RelatePointNum;		CBTC接近区段2关联道岔数目
	UINT16  wCBTCAccessLink2RelatePointId[];	CBTC接近区段2关联道岔编号	
	UINT16  wCBTCAccessLink2RelatePointSta[];   CBTC接近区段2关联道岔状态
	UINT16  wCBTCAccessLink3RelatePointNum;		CBTC接近区段3关联道岔数目	
	UINT16  wCBTCAccessLink3RelatePointId[];	CBTC接近区段3关联道岔编号	
	UINT16  wCBTCAccessLink3RelatePointSta[];   CBTC接近区段3关联道岔状态
	UINT16  wCBTCAccessLink4RelatePointNum;		CBTC接近区段4关联道岔数目
	UINT16  wCBTCAccessLink4RelatePointId[];	CBTC接近区段4关联道岔编号
	UINT16  wCBTCAccessLink4RelatePointSta[];   CBTC接近区段4关联道岔状态
	UINT16  wIncludeAxleSgmtNum;				进路包含的计轴区段数目
	UINT16  wIncludeAxleSgmtId[8];				包含的计轴区段编号
	UINT16  wAxleSgmtLockStatus[8];				包含的计轴区段锁闭状态
	UINT16  wIncludeLinkNum;					包含的link数目
	UINT16  wLinkId[14];						link编号
	UINT16  wObsNum;							进路包含障碍物数
	UINT16  wObsType[8];						障碍物类型
	UINT16  wObsID[8];							障碍物ID
	UINT16	wObsNeedLockStatus[8];			    障碍物需要联锁状态
***************************/
static void ROUTE_STRU_init_FAO(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pRouteStru->wId = ShortFromChar(pData);
	pData += 2;

	/*进路名称*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*进路性质*/
	pRouteStru->wRouteType = ShortFromChar(pData);
	pData += 2;

	/*起始信号机编号*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
	pData += 2;

	/*终止信号机编号*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
	pData += 2;

	/*起点所处link编号*/		/*新增---严广学*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;

	/*终点所处link编号*/		/*新增---严广学*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;

	/*包含道岔数目*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<16;i++)
	{
		/*道岔编号*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
		pData += 2;

		/*道岔状态*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*保护区段数目*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<4;i++)
	{
		/*保护区段编号*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
		pData += 2;

		/*保护区段关联道岔编号*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
		pData += 2;

		/*保护区段关联道岔状态*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*点式接近区段数目*/					
	pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
	pData += 2;

	/*点式接近区段编号,针对合库数据结构V16.0.0（FAO和HLT也跟进），接近区段编号增加“5”，by my 20170220*/			
	for(i=0;i<5;i++)
	{
		pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
		pData += 2;
	} 

	/*CBTC接近区段数目*/					
	pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData);
	pData += 2;

	/*CBTC接近区段编号,针对合库数据结构V16.0.0（FAO和HLT也跟进），接近区段编号增加“5”，by my 20170220*/				
	for(i=0;i<5;i++)
	{
		pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
		pData += 2;
	} 

	/*所属CI区域编号*/						
	pRouteStru->wManageCI = ShortFromChar(pData);
	pData += 2;
	/*进路属性*/						
	pRouteStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*点式触发区段数量	*/					
	pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
	pData += 2;

	/*点式触发区段ID*/
	for(i=0;i<13;i++)
	{
		pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*cbtc触发区段数量	*/					
	pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
	pData += 2;

	/*cbtc触发区段ID*/
	for(i=0;i<13;i++)
	{
		pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/**************************************
函数功能：进路表数据结构体初始化函数-互联互通
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pRouteStru 被初始化的结构体指针
返回值：无
修改说明：
①适配数据结构变化，进路包含的点式接近区段、CBTC接近区段、点式触发区段、CBTC触发区段数量均可配置为0-30的动态范围 by sds 2019-5-15
***************************/
static void ROUTE_STRU_init_HLT(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	static UINT8 runCount=0;/*执行该函数的次数 用于判定是否为第一次进入*/
	static UINT8 maxBlocAccessLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*进路包含的实际点式接近区段最大数量*/
	static UINT8 maxCBTCAccessLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*进路包含的实际CBTC接近区段最大数量*/
	static UINT8 maxBlocTRGAreLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*进路包含的实际点式触发区段最大数量*/

	/*进路ID*/
	pRouteStru->wId = ShortFromChar(pData);
	pData += 2;

	/*进路名称*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*进路性质*/
	pRouteStru->wRouteType = ShortFromChar(pData);
	pData += 2;

	/*起始信号机编号*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
	pData += 2;

	/*终止信号机编号*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
	pData += 2;

	/*起点所处link编号*/		/*新增---严广学*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;

	/*终点所处link编号*/		/*新增---严广学*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;

	/*包含道岔数目*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<16;i++)
	{
		/*道岔编号*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
		pData += 2;

		/*道岔状态*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*保护区段数目*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<4;i++)
	{
		/*保护区段编号*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
		pData += 2;

		/*保护区段关联道岔编号*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
		pData += 2;

		/*保护区段关联道岔状态*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}


	/* 程序中"点式接近区段数目"、"CBTC接近区段数目"、"点式触发区段数量"、"cbtc触发区段数量" 为固定值,
		导致一旦数据表中数据增加就要修改代码,为优化此问题修改结构体添充逻辑, by sds  2019-5-16 */
 	
	/*判断是否第一次执行  第一次执行时计算包含的区段数量   后续进入时复用第一次计算的成果*/
	if(0==runCount)
	{
		/* 获取点式接近区段数目 */					
		pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
		maxBlocAccessLinkNum= ShortFromChar(pData);
		pData += 2;

		/* 根据获取到的点式接近区段数目进行数据填充 */	
		for(i=0; i<pRouteStru->wBlogAccessLinkNum; i++)
		{
			pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 
		 
		/*获取CBTC接近区段数目*/	
		for(i=0; i<MAX_BLOC_ACCESS_LINK_NUM; i++)
		{
			pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData); 
			pData += 2;

			/* 不断读取数据，丢弃无效值，直到读取到正确的值时，退出 */
			if((pRouteStru-> wCBTCAccessLinkNum != DSU_NULL_16) 
				&& (pRouteStru-> wCBTCAccessLinkNum <= MAX_CBTC_ACCESS_LINK_NUM))
			{
				/*进路的CBTC接近区段有效数量赋值*/
				maxCBTCAccessLinkNum=pRouteStru-> wCBTCAccessLinkNum;
				break;
			}
			else
			{	
				/*进路的点式接近区段总数量+1*/
				maxBlocAccessLinkNum++;
			}
		}

		/* 根据获取到的CBTC接近区段数目进行数据填充 */	
		for(i=0;i<pRouteStru-> wCBTCAccessLinkNum;i++)
		{
			pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*所属CI区域编号*/	
		for(i=0; i<MAX_CBTC_ACCESS_LINK_NUM; i++)
		{					
			pRouteStru->wManageCI = ShortFromChar(pData);
			pData += 2;

			/* 不断读取数据，丢弃无效值，直到读取到正确的值时，退出 */
			if (pRouteStru->wManageCI != DSU_NULL_16)
			{
				break;
			}
			else
			{	
				/*进路的CBTC接近区段总数量+1*/
				maxCBTCAccessLinkNum++;
			}
		}

		/*进路属性*/						
		pRouteStru->wAttribute = ShortFromChar(pData);
		pData += 2;
	 

		/*点式触发区段数量	*/					
		pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
		maxBlocTRGAreLinkNum= ShortFromChar(pData);
		pData += 2;

		/*点式触发区段ID*/
		for(i=0;i<pRouteStru->wBlocTrgAreaNum;i++)
		{
			pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*cbtc触发区段数量	*/	
		for(i=0; i<MAX_BLOC_TRG_AREA_NUM; i++)
		{
			pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
			pData += 2;

			/* 不断读取数据，丢弃无效值，直到读取到正确的值时，退出 */
			if((pRouteStru->wCbtcTrgAreaNum != DSU_NULL_16)
				&& (pRouteStru-> wCbtcTrgAreaNum <= MAX_CBTC_TRG_AREA_NUM))
			{
				break;
			}
			else
			{	
				/*进路的点式触发区段总数量+1*/
				maxBlocTRGAreLinkNum++;
			}
		}

		/*cbtc触发区段ID*/
		for(i=0;i<pRouteStru->wCbtcTrgAreaNum;i++)
		{
			pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*执行次数+1*/
		runCount++;
	}
	else
	{

		/* 获取点式接近区段数目 */					
		pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
		pData += 2;

		/* 根据获取到的点式接近区段数目进行数据填充 */	
		for(i=0; i<maxBlocAccessLinkNum; i++)
		{
			pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*获取CBTC接近区段数目*/	
		pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData); 
		pData += 2;

		/* 根据获取到的CBTC接近区段数目进行数据填充 */	
		for(i=0;i<maxCBTCAccessLinkNum;i++)
		{
			pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*所属CI区域编号*/	
		pRouteStru->wManageCI = ShortFromChar(pData);
		pData += 2;

		/*进路属性*/						
		pRouteStru->wAttribute = ShortFromChar(pData);
		pData += 2;


		/*点式触发区段数量	*/					
		pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
		pData += 2;

		/*点式触发区段ID*/
		for(i=0;i<maxBlocTRGAreLinkNum;i++)
		{
			pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*cbtc触发区段数量	*/	
		pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
		pData += 2;

		/*cbtc触发区段ID-依据实际数量进行数据获取*/
		for(i=0;i<pRouteStru->wCbtcTrgAreaNum;i++)
		{
			pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

	}

}

static void ROUTE_STRU_init_CPK(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pRouteStru->wId = ShortFromChar(pData);
    pData += 2;

   /*进路名称*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
       pData += 1;
	}

    /*进路性质*/
	pRouteStru->wRouteType = ShortFromChar(pData);
    pData += 2;

	/*起始信号机编号*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
    pData += 2;

    /*终止信号机编号*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
    pData += 2;

    /*起点所处link编号*/		/*新增---严广学*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
    pData += 2;

    /*终点所处link编号*/		/*新增---严广学*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
    pData += 2;

    /*包含道岔数目*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
    pData += 2;

    
	for(i=0;i<16;i++)
	{
		/*道岔编号*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
        pData += 2;

        /*道岔状态*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

    /*保护区段数目*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
    pData += 2;

															
   for(i=0;i<4;i++)
	{
		/*保护区段编号*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
        pData += 2;
	
       /*保护区段关联道岔编号*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
        pData += 2;

        /*保护区段关联道岔状态*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

    /*点式接近区段数目*/					
	pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
    pData += 2;

   /*点式接近区段编号,针对合库数据结构V16.0.0（FAO和HLT也跟进），接近区段编号增加“5”，by my 20170220*/			
	 for(i=0;i<5;i++)
	{
		pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
        pData += 2;
	} 

  /*CBTC接近区段数目*/					
	pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData);
    pData += 2;

    /*CBTC接近区段编号,针对合库数据结构V16.0.0（FAO和HLT也跟进），接近区段编号增加“5”，by my 20170220*/				
     for(i=0;i<5;i++)
	{
		pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
        pData += 2;
	} 

	/*所属CI区域编号*/						
	pRouteStru->wManageCI = ShortFromChar(pData);
    pData += 2;
	/*进路属性*/						
	pRouteStru->wAttribute = ShortFromChar(pData);
    pData += 2;

	/*点式触发区段数量	*/					
	pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
    pData += 2;

	/*点式触发区段ID*/
	for(i=0;i<4;i++)
	{
		pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*cbtc触发区段数量	*/					
	pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
    pData += 2;

	/*cbtc触发区段ID*/
	for(i=0;i<4;i++)
	{
		pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/*调用接口
备注：
①增加对单轨项目标识的处理，modify by qxt 20170726
②修改互联互通项目数据结构进路表处理逻辑，兼容对进路内包含的点式/CBTC接近区段，点式/CBTC触发区段动态列数的处理，m by sds 2019-5-16*/
void ROUTE_STRU_init(UINT8 *pDataAdd,DSU_ROUTE_STRU *pRouteStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) )
	{
		ROUTE_STRU_init_FAO(pDataAdd, pRouteStru);
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType)
	{
		ROUTE_STRU_init_CPK(pDataAdd, pRouteStru);
	}
	else if(DQU_CBTC_HLT == cbtcVersionType)
	{
		ROUTE_STRU_init_HLT(pDataAdd, pRouteStru);
	}
	else
	{
      /*不可达分支*/
	}
}

/**************************************
函数功能：逻辑区段数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pProtectLinkStru被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	  UINT16  wRouteId;							进路编号
	  UINT16  wIncludeAxleSgmtId;					包含的计轴区段编号
	  UINT16  wObsNum;							包含障碍物数目
	  UINT16  wObsType;							障碍物类型	
***************************/

void LOGIC_SGMT_STRU_init(UINT8 * pDataAdd,DSU_LOGIC_SGMT_STRU * pLogicSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT32 i = 0;
	/*编号*/
	pLogicSgmtStru->wId = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<16;i++)
	{
		/*逻辑区段名称*/
		pLogicSgmtStru->szCaption[i] = *pData;
		pData++;
	}
	
	/*起点所处link编号*/
	pLogicSgmtStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;
		
  /*起点所处link偏移量(cm)*/
	pLogicSgmtStru->dwOrgnLkOfst = LongFromChar(pData);
	pData += 4;

		/*终点所处link编号*/
	pLogicSgmtStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;
		
  /*终点所处link偏移量(cm)*/
	pLogicSgmtStru->dwTmnlLkOfst = LongFromChar(pData);
	pData += 4;

	/*所属计轴区段编号*/
	pLogicSgmtStru->wAxleSgmtId = ShortFromChar(pData);
	pData += 2;

	/*ZC-CI逻辑区段占用状态码位
	pLogicSgmtStru->wZcCiLogicStatusId = ShortFromChar(pData);
	pData += 2;*/
	
	/*ZC-CI逻辑区段CT列车占用状态码位（预留）
	pLogicSgmtStru->wZcCiCTLogicStatusId = ShortFromChar(pData);
	pData += 2;*/

	/*ZC-CI逻辑区段UT列车占用状态码位（预留）
	pLogicSgmtStru->wZcCiUTLogicStatusId = ShortFromChar(pData);
	pData += 2;*/
}

/**************************************
函数功能：点式接近区段信息数据结构体初始化函数(原BLOC模式接近区段表)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pBlocAccessLinkStru被初始化的结构体指针
返回值：无
备注：结构体无需更改

***************************/
void BLOC_ACCESS_LINK_STRU_init(UINT8 * pDataAdd,DSU_BLOC_ACCESS_LINK_STRU * pBlocAccessLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pBlocAccessLinkStru->wId = ShortFromChar(pData);
    pData += 2;

	/*包含的计轴区段数目*/
	pBlocAccessLinkStru->wIncludeAxleSgmtNum = ShortFromChar(pData);
    pData += 2;
	
    /*包含的区段信息*/
	for(i=0;i<(pBlocAccessLinkStru->wIncludeAxleSgmtNum);i++)
	{
		pBlocAccessLinkStru->wIncludeAxleSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	}
}

/**************************************
函数功能：CBTC接近区段信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pCBTCAccessLinkStru被初始化的结构体指针
返回值：无
备注：结构体无需更改

***************************/
void CBTC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*编号*/
	pCBTCAccessLinkStru->wId = ShortFromChar(pData);
	pData += 2;

	/*包含的计轴区段数目*/
	pCBTCAccessLinkStru->wIncludeLogicSgmtNum = ShortFromChar(pData);
	pData += 2;
	
	/*包含的计轴区段编号*/
	for(i=0;i<(pCBTCAccessLinkStru->wIncludeLogicSgmtNum);i++)
	{
		pCBTCAccessLinkStru->wIncludeLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/**************************************
函数功能：坡度数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pGradeStru被初始化的结构体指针
返回值：无
备注：结构体需增加：
UINT32  dwGradeLength;							坡度长度
UINT16  wIncludeLinkId[3];							坡度线路中间包含link编号


***************************/
void GRADE_STRU_init(UINT8 * pDataAdd,DSU_GRADE_STRU * pGradeStru)
{
	UINT8 * pData = pDataAdd;
	/*编号*/
	pGradeStru->wId = ShortFromChar(pData);
    pData += 2;
	
	/*坡度起点所处link编号*/
	pGradeStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;	

	/*坡度起点所处link偏移量*/
    pGradeStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;		
	
	/*坡度终点所处link编号*/
	pGradeStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;							
									
	/*坡度终点所处link偏移量*/
	pGradeStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;	

	/*起点关联道岔编号*/
	pGradeStru->wOrgnRelatePointId = ShortFromChar(pData);
    pData += 2;	

	/*起点正线坡度编号*/
	pGradeStru->wOrgnJointMainGradeId = ShortFromChar(pData);
    pData += 2;

	/*起点侧线坡度编号*/
	pGradeStru->wOrgnJointSideGradeId = ShortFromChar(pData);
    pData += 2;

	/*终点关联道岔编号*/
    pGradeStru->wTmnlRelatePointId  = ShortFromChar(pData);
    pData += 2;
					
	/*终点正线坡度编号*/
	pGradeStru->wTmnlJointMainGradeId  = ShortFromChar(pData);
    pData += 2;
						
	/*终点侧线坡度编号*/
	pGradeStru->wTmnlJointSideGradeId = ShortFromChar(pData);
    pData += 2;
				
	/*坡度值*/
	pGradeStru->wGradValue = ShortFromChar(pData);
    pData += 2;
							
	/*坡段相对于线路逻辑方向的倾斜方向*/
    pGradeStru->wInclineDir = ShortFromChar(pData);
    pData += 2;

   /*竖曲线半径*/		/*新增---严广学*/
	pGradeStru->dwRadius = LongFromChar(pData);
    pData += 4;								
}
/**************************************
函数功能：线路静态限速数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pStaticResSpeedStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void STATIC_RES_SPEED_STRU_init(UINT8 * pDataAdd,DSU_STATIC_RES_SPEED_STRU * pStaticResSpeedStru)
{
	UINT8 * pData = pDataAdd;
	/*编号*/
	pStaticResSpeedStru->wId = ShortFromChar(pData);
    pData += 2;

    /*该限速区域所处link编号*/
	pStaticResSpeedStru->wLinkId = ShortFromChar(pData);
    pData += 2;							
					
	/*起点所处link偏移量(cm)*/
	pStaticResSpeedStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;
					
	/*终点所处link偏移量(cm)*/
    pStaticResSpeedStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
				
	/*关联道岔编号*/
    pStaticResSpeedStru->wRelatePointId = ShortFromChar(pData);
    pData += 2;
					
	/*静态限速值*/
	pStaticResSpeedStru->wResSpeed = ShortFromChar(pData);
    pData += 2;					
}
/**************************************
函数功能：默认行车序列数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pDefaultrouteStru被初始化的结构体指针
返回值：无
备注：结构体增加项：
	UINT16  wCurrentPlatformId;				当前站台编号
	UINT16  wCurrentPlatformFlag;			当前站台标志位

***************************/
void DEFAULTROUTE_STRU_init(UINT8 * pDataAdd,DSU_DEFAULTROUTE_STRU * pDefaultrouteStru)
{
	UINT8 * pData = pDataAdd;
	/*编号*/
	pDefaultrouteStru->wId = ShortFromChar(pData);
    pData += 2;					

	/*link编号*/
	pDefaultrouteStru->wLinkId = ShortFromChar(pData);
    pData += 2;	

	/*运行方向*/
    pDefaultrouteStru->wLinkDir = ShortFromChar(pData);
    pData += 2;	
						
	/*前方站台编号*/
    pDefaultrouteStru->wNextPlatformId = ShortFromChar(pData);
    pData += 2;

	/*目标站台编号*/
	pDefaultrouteStru->wDestinationId = ShortFromChar(pData);
    pData += 2;		
}
/**************************************
函数功能：站间运营级别数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pRunLevelStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：


***************************/
static void RUN_LEVEL_STRU_init_CPK(UINT8 * pDataAdd,DSU_RUN_LEVEL_STRU * pRunLevelStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*索引编号*/
	pRunLevelStru->wId = ShortFromChar(pData);
    pData += 2;		
												
	/*起点目的地编号*/
    pRunLevelStru->wOrgnDesId = ShortFromChar(pData);
    pData += 2;	
					
	/*终点目的地编号*/
    pRunLevelStru->wTmnlDesId = ShortFromChar(pData);
    pData += 2;
									
	/*运行级别分级数目*/		/*新增---严广学*/
    pRunLevelStru->wRunLevelNum = ShortFromChar(pData);
    pData += 2;
				
	
	for(i=0;i<10;i++)
	{
		/*运营级别编号*/
		pRunLevelStru->wRunLevelID[i] = ShortFromChar(pData);
        pData += 2;
						
	   /*该运营级别时间*/
		pRunLevelStru->wRunTime[i] = ShortFromChar(pData);
        pData += 2;
							
        /*该运营级别速度*/
		pRunLevelStru->wRunSpeed[i] = ShortFromChar(pData);
        pData += 2;
	}			 			
}

/**************************************
函数功能：站间运营级别数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pRunLevelStru被初始化的结构体指针
返回值：无
备注：FAO和HLT共用
***************************/
static void RUN_LEVEL_STRU_init_DFT(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i=0;
	
	/*索引编号*/
	pRunLevelStru->wId = ShortFromChar(pData);
	pData += 2;		

	/*起点目的地编号*/
	pRunLevelStru->wOrgnDesId = ShortFromChar(pData);
	pData += 2;	

	/*终点目的地编号*/
	pRunLevelStru->wTmnlDesId = ShortFromChar(pData);
	pData += 2;

	/*运行级别分级数目*/		/*新增---严广学*/
	pRunLevelStru->wRunLevelNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<5;i++)
	{
		/*运营级别编号*/
		pRunLevelStru->wRunLevelID[i] = ShortFromChar(pData);
		pData += 2;

		/*该运营级别时间*/
		pRunLevelStru->wRunTime[i] = ShortFromChar(pData);
		pData += 2;

		/*该运营级别速度*/
		pRunLevelStru->wRunSpeed[i] = ShortFromChar(pData);
		pData += 2;
	}	
}

/**************************************
函数功能：站间运营级别数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pRunLevelStru被初始化的结构体指针
返回值：无
备注：调用接口
增加对单轨项目标识的处理，modify by qxt 20170726
***************************/
void RUN_LEVEL_STRU_init(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
		RUN_LEVEL_STRU_init_DFT(pDataAdd, pRunLevelStru);
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		RUN_LEVEL_STRU_init_CPK(pDataAdd, pRunLevelStru);
	}
	else
	{
		/*不可达分支*/
	}
}

/**************************************
函数功能：退出CBTC区域数据结构体初始化函数(原退出线路区域)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pExitRouteStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：


***************************/
void EXIT_ROUTE_STRU_init(UINT8 * pDataAdd,DSU_EXIT_ROUTE_STRU * pExitRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*索引编号*/
	pExitRouteStru->wId = ShortFromChar(pData);
    pData += 2;
												
	/*起点所处link编号*/
    pExitRouteStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;
					
	/*起点link偏移量(cm)*/
    pExitRouteStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*终点所处link编号*/
    pExitRouteStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;
						
	/*终点link偏移量(cm)*/
    pExitRouteStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
								
	/*退出CBTC区域时列车运行方向*/
	pExitRouteStru->wDIR = ShortFromChar(pData);
    pData += 2;	

	/*退出CBTC区域时列车运行方向*/
	pExitRouteStru->wType = ShortFromChar(pData);
    pData += 2;

	/*包含道岔数量*/
	pExitRouteStru->wIncludePointNum=ShortFromChar(pData);
	pData+=2;

	for (i=0;i<10;i++)
	{
		/*包含道岔id*/
		pExitRouteStru->wPointId[i]=ShortFromChar(pData);
		pData+=2;

		/*包含道岔状态*/
		pExitRouteStru->wPointStatus[i]=ShortFromChar(pData);
		pData+=2;
	}
}
/**************************************
函数功能：非CBTC区域数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pTrainLineStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：

***************************/
void TRAIN_LINE_STRU_init(UINT8 * pDataAdd,DSU_TRAIN_LINE_STRU * pTrainLineStru)
{
	UINT8 * pData = pDataAdd;
	/*编号*/
	pTrainLineStru->wId= ShortFromChar(pData);
    pData += 2;			 
										
	/*起点所处link编号*/
    pTrainLineStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;		

	/*起点link偏移量(cm)*/
    pTrainLineStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*终点所处link编号*/
    pTrainLineStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;	
							
	/*终点link偏移量(cm)*/
    pTrainLineStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;

	/*非CBTC区域列车运行方向*/
    pTrainLineStru->wDir = ShortFromChar(pData);
    pData += 2;			 					
}
/**************************************
函数功能：无人折返表数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pARStru被初始化的结构体指针
返回值：无
备注：结构体由原来的无人折返进入和无人折返退出表合并而成

***************************/
void AR_STRU_init(UINT8 * pDataAdd,DSU_AR_STRU * pARStru)
{
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pARStru->wId = ShortFromChar(pData);
    pData += 2;			 
											
	/*无人折返出发站台编号*/
	pARStru->wStartPlartformId = ShortFromChar(pData);
    pData += 2;	
				
	/*无人折返进入点编号*/
	pARStru->wARInStopPointId = ShortFromChar(pData);
    pData += 2;	
								
							
	/*无人折返换端点编号*/
    pARStru->wRevertPointId = ShortFromChar(pData);
    pData += 2;
		
	/*无人折返退出站台编号*/
    pARStru->wEndPlartformId = ShortFromChar(pData);
    pData += 2;

	/*无人折返退出点编号*/
    pARStru->wAROutStopPointId = ShortFromChar(pData);
    pData += 2;

			
	/*无人折返灯ID*/
    pARStru->wARLampID = ShortFromChar(pData);
    pData += 2;
		
	/*无人折返按钮D*/
    pARStru->wARButtonId = ShortFromChar(pData);
    pData += 2;

	
		
}
/**************************************
函数功能：ZC共管区域数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pZCComZoneStru被初始化的结构体指针
返回值：无
备注：ZC共管区域表    这个表的前身是zc分界点表

***************************/
void ZC_COM_ZONE_STRU_init(UINT8 * pDataAdd,DSU_ZC_COM_ZONE_STRU * pZCComZoneStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*索引编号*/ 
	pZCComZoneStru->wId = ShortFromChar(pData);
    pData += 2;				
	                                    
	/*共管区域起点所处link编号*/
    pZCComZoneStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	  /*共管区域起点所处link偏移量*/  
	pZCComZoneStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;
	               
	/*共管区域终点所处link编号*/ 
    pZCComZoneStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;
	                      
	/*共管区域终点所处link偏移量*/
   pZCComZoneStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
	
	/*共管区域切换点相关进路编号*/
	pZCComZoneStru->wRelateRouteID = ShortFromChar(pData);
    pData += 2;

	                
	/*共管区域内包含的道岔数目*/
    pZCComZoneStru->wIncludePointNum = ShortFromChar(pData);
    pData += 2;
	                           
	
	for(i=0;i<8;i++)
	{
		/*共管区域内包含的道岔编号*/ 
		pZCComZoneStru->wPointId[i]= ShortFromChar(pData);
        pData += 2;

	   /*共管区域内包含的道岔状态*/ 
		pZCComZoneStru->wPointStatus[i]= ShortFromChar(pData);
        pData += 2;
	}           
}
/**************************************
函数功能：ZC分界点信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pZCDividPointStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void ZC_DIVID_POINT_STRU_init(UINT8 * pDataAdd,DSU_ZC_DIVID_POINT_STRU * pZCDividPointStru)
{
	UINT16 j = 0;
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pZCDividPointStru->wId = ShortFromChar(pData);
    pData += 2;	

	/*相关的反向分界点编号*/
	pZCDividPointStru->wIdconnection = ShortFromChar(pData);
    pData += 2;	

	/*本分界点属性*/
    pZCDividPointStru->wType = ShortFromChar(pData);
    pData += 2;	
	                                  
	/*移交ZC设备编号*/
    pZCDividPointStru->wOutputZcIndex = ShortFromChar(pData);
    pData += 2;	
	                    
	/*移交ZC设备ID*/ 
    pZCDividPointStru->wOutputZcId = ShortFromChar(pData);
    pData += 2;	
	                        
	/*接管ZC设备编号*/ 
    pZCDividPointStru->wInputZcIndex = ShortFromChar(pData);
    pData += 2;	
	                      
	/*接管ZC设备ID*/
    pZCDividPointStru->wInputZcId = ShortFromChar(pData);
    pData += 2;	
	                              
	/*ZC分界点所处link编号*/
    pZCDividPointStru-> wDividPointLinkId = ShortFromChar(pData);
    pData += 2;	
	              
	/*ZC分界点所处link偏移量*/
    pZCDividPointStru->dwDividPointLinkOfst  = LongFromChar(pData);
    pData += 4;
	                       
	/*列车经过切换点时运行方向*/
    pZCDividPointStru->wDir = ShortFromChar(pData);
    pData += 2;		

	/*分界点相关的共管区域数目*/
	pZCDividPointStru->wRelateZCAreaNum = ShortFromChar(pData);
    pData += 2;	

	for(j=0;j<RELATE_ZC_AREA_NUM;j++)
	{
		/*分界点相关的共管区域编号*/
		pZCDividPointStru->wRelateZCAreaId[j] = ShortFromChar(pData);
		pData += 2;	
	}
	
}
/**************************************
函数功能：CI信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pCIStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void CI_STRU_init_DFT(UINT8 * pDataAdd,DSU_CI_STRU * pCIStru)
{
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pCIStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	/*CI设备ID*/
	pCIStru->wID = ShortFromChar(pData);
    pData += 2;		
	
	/*联锁属性*/
	pCIStru->wType = ShortFromChar(pData);
    pData += 2;	
}

void CI_STRU_init_HLT(UINT8 * pDataAdd,DSU_CI_STRU * pCIStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i=0;

	/*索引编号*/
	pCIStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*CI设备ID*/
	pCIStru->wID = ShortFromChar(pData);
	pData += 2;		

	/*联锁属性*/
	pCIStru->wType = ShortFromChar(pData);
	pData += 2;		

	/*CI-VOBC共用数据校验信息*/
	pCIStru->dwVersionOfCIToVOBC = LongFromChar(pData);
	pData += 4;	

	/*CI-ZC共用数据校验信息*/
	pCIStru->dwVersionOfCIToZC = LongFromChar(pData);
	pData += 4;	

	/*CI-ATS共用数据校验信息*/
	pCIStru->dwVersionOfCIToATS = LongFromChar(pData);
	pData += 4;	

	/*相邻CI数量*/
	pCIStru->dwAdjoinCICount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*相邻CI[1-6]的ID*/
		pCIStru->dwAdjoinCIId[i] =ShortFromChar(pData);
		pData += 2;

		/*与相邻CI[1-6]的共用数据校验信息*/
		pCIStru->dwAdjoinCIShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}

}


/*调用接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
void CI_STRU_init(UINT8 *pDataAdd,DSU_CI_STRU *pCIStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		CI_STRU_init_DFT(pDataAdd, pCIStru);
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		CI_STRU_init_HLT(pDataAdd, pCIStru);
	}
	else
	{
       /*不可达分支*/
	}
}


/**************************************
函数功能：ATS信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pATSStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void ATS_STRU_init_DFT(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pATSStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS设备ID*/
	 pATSStru->wID = ShortFromChar(pData);
     pData += 2;	
}

void ATS_STRU_init_HLT(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;

	/*索引编号*/
	pATSStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*ATS设备ID*/
	pATSStru->wID = ShortFromChar(pData);
	pData += 2;	

	/*ATS-VOBC共用数据校验信息*/
	pATSStru->dwVersionOfATSToVOBC = LongFromChar(pData);
	pData += 4;	

	/*ATS-DSU共用数据校验信息*/
	pATSStru->dwVersionOfATSToDSU = LongFromChar(pData);
	pData += 4;	

	/*相邻ATS数量*/
	pATSStru->dwAdjoinATSCount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*相邻ATS[1-6]的ID*/
		pATSStru->dwAdjoinATSId[i] =ShortFromChar(pData);
		pData += 2;

		/*与相邻ATS[1-6]的共用数据校验信息*/
		pATSStru->dwAdjoinATSShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}

}


/*调用接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
void ATS_STRU_init(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		ATS_STRU_init_DFT(pDataAdd, pATSStru);
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		ATS_STRU_init_HLT(pDataAdd, pATSStru);
	}
	else
	{
       /*不可达分支*/
	}
}


/**************************************
函数功能：保护区段信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pProtectLinkStru被初始化的结构体指针
返回值：无
备注：结构体需增加：
	  UINT16  wRouteId;							进路编号
	  UINT16  wIncludeAxleSgmtId;			包含的计轴区段编号（更改为数组）
	  UINT16  wObsNum;							包含障碍物数目
	  UINT16  wObsType;							障碍物类型

***************************/
void PROTECT_LINK_STRU_init(UINT8 * pDataAdd,DSU_PROTECT_LINK_STRU * pProtectLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT32 i=0;
	/*编号*/
	 pProtectLinkStru->wId = ShortFromChar(pData);
     pData += 2;

	 /*包含的计轴区段数目*/		/*新增---严广学*/
	 pProtectLinkStru->wIncludeAxleSgmtNum = ShortFromChar(pData);
     pData += 2;

	 for(i=0;i<4;i++)
	 {
		 /*包含的计轴区段编号*/		/*新增---严广学*/  /*这个字段原来不是数组*/
	    pProtectLinkStru->wIncludeAxleSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	 }

}

/*线路统一限速信息表*/
void CONRESSPEED_STRU_init(UINT8 * pDataAdd, DSU_CONRESSPEED_STRU * pConRESSpeedStru)
{
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pConRESSpeedStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS设备ID*/
	 pConRESSpeedStru->wSpeed = ShortFromChar(pData);
     pData += 2;	
}  

/*线路统一坡度信息表*/
void CONGRADE_STRU_init(UINT8 * pDataAdd, DSU_CONGRADE_STRU * pConGradeStru)
{
	UINT8 * pData = pDataAdd;
	/*索引编号*/
	pConGradeStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS设备ID*/
	 pConGradeStru->wGrade = ShortFromChar(pData);
     pData += 2;	
}  



/*物理区段信息表*/
void PHYSICAL_SGMT_STRU_init(UINT8 * pDataAdd, DSU_PHYSICAL_SGMT_STRU * pPhysicalSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPhysicalSgmtStru->wId = ShortFromChar(pData);
    pData += 2;
									
	 /*物理区段名称*/
	for(i=0;i<16;i++)
	{
      pPhysicalSgmtStru->szName[i] = *pData;
	   pData ++;
	}
	
	   pPhysicalSgmtStru->Type = *pData;
	   pData ++;

	   pPhysicalSgmtStru->LjkFlag = *pData;
       pData ++;
} 

/**************************************
函数功能：点式触发区段表-初始化
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [in] pBlocTrgAreaStru 被初始化的结构体指针
返回值：无
***************************/
void DSU_BLOC_TRG_AREA_STRU_init(UINT8 * pDataAdd, DSU_BLOC_TRG_AREA_STRU * pBlocTrgAreaStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*ID*/
	pBlocTrgAreaStru->wId = ShortFromChar(pData);
    pData += 2;

	pBlocTrgAreaStru->wAxleSgmtNum = ShortFromChar(pData);
    pData += 2;
									
	/*包含的区段信息*/
	for(i=0;i<(pBlocTrgAreaStru->wAxleSgmtNum);i++)
	{
		pBlocTrgAreaStru->wAxleSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
} 


/**************************************
函数功能：CBTC触发区段表-初始化
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [in] pCbtcTrgAreaStru 被初始化的结构体指针
返回值：无
***************************/
void DSU_CBTC_TRG_AREA_STRU_init(UINT8 * pDataAdd, DSU_CBTC_TRG_AREA_STRU * pCbtcTrgAreaStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;

	/*ID*/
	pCbtcTrgAreaStru->wId = ShortFromChar(pData);
    pData += 2;

	pCbtcTrgAreaStru->wLogicSgmtNum = ShortFromChar(pData);
    pData += 2;
 									
	/*包含的区段信息*/
	for(i=0;i<(pCbtcTrgAreaStru->wLogicSgmtNum);i++)
	{
		pCbtcTrgAreaStru->wLogicSgmtId[i] = ShortFromChar(pData);
	    pData += 2;
	}
} 

/*站台区段表*/
static void DSU_PLAT_FORM_STRU_init_FAO(UINT8 * pDataAdd, DSU_PLAT_FORM_STRU * pPlatFormStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPlatFormStru->wId = ShortFromChar(pData);
	pData += 2;


	/*中心公里标*/
	for(i=0;i<16;i++)
	{
		pPlatFormStru->szGongLiBiao[i] = *pData;
		pData ++;
	}
	pPlatFormStru->wStationId = ShortFromChar(pData);
	pData += 2;

	pPlatFormStru->wDir = *pData;
	pData ++;

	pPlatFormStru->wLogicSgmtNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		pPlatFormStru->wLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

static void DSU_PLAT_FORM_STRU_init_CPK(UINT8 * pDataAdd, DSU_PLAT_FORM_STRU * pPlatFormStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPlatFormStru->wId = ShortFromChar(pData);
    pData += 2;

								
	 /*物理区段名称*/
	for(i=0;i<16;i++)
	{
		pPlatFormStru->szGongLiBiao[i] = *pData;
	   pData ++;
	}
	pPlatFormStru->wStationId = ShortFromChar(pData);
    pData += 2;
	/*方向*/		
	pPlatFormStru->wDir = *pData;
	pData ++;

	/*逻辑区段个数*/
	pPlatFormStru->wLogicSgmtNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		pPlatFormStru->wLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
	
	/*站台名称*/
	for(i=0;i<48;i++)
	{
		pPlatFormStru->szStationName[i] = *pData;
	   pData ++;
	}
	   
}

/*访问接口*/
/*备注：根据互联互通数据结构V11.0.0需求修改，modify by lmy 20180208*/
void DSU_PLAT_FORM_STRU_init(UINT8 *pDataAdd, DSU_PLAT_FORM_STRU *pPlatFormStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType))
	{
		DSU_PLAT_FORM_STRU_init_FAO(pDataAdd, pPlatFormStru);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)|| (DQU_CBTC_CC == cbtcVersionType))
	{
		DSU_PLAT_FORM_STRU_init_CPK(pDataAdd, pPlatFormStru);
	}
	else
	{
       /*不可达分支*/
	}
}

/*往复进路表,2013.10.23*/
void DSU_COME_GO_STRU_init(UINT8 * pDataAdd, DSU_COME_GO_STRU * pComeGoStru)
{
    UINT8 * pData = pDataAdd;
    
    /*编号*/
    pComeGoStru->wId = ShortFromChar(pData);
    pData += 2;
    
    /*进路1编号*/
    pComeGoStru->route1Id = ShortFromChar(pData);
    pData += 2;
    
    /*进路2编号*/
    pComeGoStru->route2Id = ShortFromChar(pData);
    pData += 2;
    
    /*进路3编号*/
    pComeGoStru->route3Id = ShortFromChar(pData);
    pData += 2;
    
    /*进路4编号*/
    pComeGoStru->route4Id = ShortFromChar(pData);
    pData += 2;
    
    /*对应停车区域ID*/
    pComeGoStru->stationId = ShortFromChar(pData);
    pData += 2;
}

/*防淹门表,2014.7.18*/
void DSU_FLOOD_GATE_STRU_init(UINT8 * pDataAdd, DSU_FLOOD_GATE_STRU * pFloodGateStru)
{
	UINT8 * pData = pDataAdd;

	/*编号*/
	pFloodGateStru->wId = ShortFromChar(pData);
	pData += 2;

	/*Link号*/
	pFloodGateStru->wFloodGateLinkID = ShortFromChar(pData);
	pData += 2;

	/*偏移量*/
	pFloodGateStru->dwFloodGateLinkoffset = LongFromChar(pData);
	pData += 4;
	/*防护方向*/
	pFloodGateStru->wDir = *pData;
	pData += 1;
}

/*SPKS开关表*/
void DSU_SPKS_BUTTON_STRU_init(UINT8 *pDataAdd, DSU_SPKS_BUTTON_STRU *pSpksButtonStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*编号*/
	pSpksButtonStru->wId = ShortFromChar(pData);
	pData += 2;

	pSpksButtonStru->wPhysicalSgmtNum = ShortFromChar(pData);
	pData += 2;

	/*物理区段ID*/
	for(i=0;i<30;i++)
	{
		pSpksButtonStru->wRelatePhysicalSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*所属CI区域编号*/
	pSpksButtonStru->wManageCI = ShortFromChar(pData); 
	pData += 2;
}

/*车库门表*/
void DEPOT_GATE_STRU_init(UINT8 *pDataAdd, DSU_DEPOT_GATE_STRU *pDepotGateStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*车库门编号*/
	pDepotGateStru->wId = ShortFromChar(pData);
	pData += 2;

	/*车库门所在Link*/
	pDepotGateStru->wDepotGateLinkID = ShortFromChar(pData);
	pData += 2;

	/*Link偏移量*/
	pDepotGateStru->dwDepotGateLinkoffset = LongFromChar(pData);
	pData += 4;

	/*车库门对应A库线物理区段编号*/
	pDepotGateStru->wAPhysicalSgmtId = ShortFromChar(pData);
	pData += 2;

	/*车库门对应B库线物理区段编号*/
	pDepotGateStru->wBPhysicalSgmtId = ShortFromChar(pData);
	pData += 2;

	/*车库门属性*/
	pDepotGateStru->sDepotAttribute = ShortFromChar(pData);
	pData += 2;

	/*入库进路数量*/
	pDepotGateStru->wRouteInsideNum = ShortFromChar(pData);
	pData += 2;

	/*入库进路编号*/
	for (i=0;i<10;i++)
	{
		pDepotGateStru->wRouteInsideId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*出库进路数量*/
	pDepotGateStru->wRouteOutsideNum = ShortFromChar(pData);
	pData += 2;

	/*出库进路编号*/
	for (i=0;i<8;i++)
	{
		pDepotGateStru->wRouteOutsideId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*车库门对应SPKS开关数量*/
	pDepotGateStru->wSpksNum = ShortFromChar(pData);
	pData += 2;

	/*SPKS编号*/
	for (i=0;i<4;i++)
	{
		pDepotGateStru->wSpksId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*对应防护信号机数量*/
	pDepotGateStru->wProectSignalNum = ShortFromChar(pData);
	pData += 2;

	/*对应防护信号机编号*/
	for (i=0;i<6;i++)
	{
		pDepotGateStru->wProtectSignalId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/*隧道表*/
void TUNNEL_STRU_init(UINT8 *pDataAdd, DSU_TUNNEL_STRU *pTunnelStru)
{
	UINT8 *pData = pDataAdd;
	
	/*索引编号*/
	pTunnelStru->wId = ShortFromChar(pData);
	pData += 2;

	/*隧道所处link编号*/
	pTunnelStru->wTunLinkId = ShortFromChar(pData);
	pData += 2;

	/*起点所处link偏移量*/
	pTunnelStru->dwBeginTunLinkOffset = LongFromChar(pData);
	pData += 4;

	/*起点所处link偏移量*/
	pTunnelStru->dwEndTunLinkOffset = LongFromChar(pData);
	pData += 4;
}


/**************************************
函数功能：ZC信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
		  [out]pZCStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void ZC_STRU_init(UINT8 * pDataAdd,DSU_ZC_STRU * pZCStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i=0;

	/*索引编号*/
	pZCStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*ZC设备ID*/
	pZCStru->wZCId = ShortFromChar(pData);
	pData += 2;		

	/*ZC-VOBC共用数据校验信息*/
	pZCStru->dwVersionOfZCToVOBC = LongFromChar(pData);
	pData += 4;	

	/*ZC-DSU共用数据校验信息*/
	pZCStru->dwVersionOfZCToDSU = LongFromChar(pData);
	pData += 4;	

	/*ZC-ATS共用数据校验信息*/
	pZCStru->dwVersionOfZCToATS = LongFromChar(pData);
	pData += 4;	

	/*相邻ZC数量*/
	pZCStru->dwAdjoinZCCount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*相邻ZC[1-6]的ID*/
		pZCStru->dwAdjoinZCId[i] =ShortFromChar(pData);
		pData += 2;

		/*与相邻ZC[1-6]的共用数据校验信息*/
		pZCStru->dwAdjoinZCShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}
}

/**************************************
函数功能：多开道岔表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pMultiPointStru被初始化的结构体指针
返回值：无
备注：单轨项目新增需求， add by qxt 20170622

***************************/
void MULTI_POINT_STRU_init(UINT8 *pDataAdd,DSU_MULTI_SWITCH_STRU *pMultiPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i=0;
   /*道岔编号*/
	pMultiPointStru->wId=ShortFromChar(pData);
	pData+=2;
   /*道岔类型*/
	pMultiPointStru->wType=*pData;
	pData+=1;
	for (i=0;i<MAX_VIRTUAL_SWITCH_NUM;i++)
	{
		/*虚拟道岔编号*/
		pMultiPointStru->wSwitchIds[i]=ShortFromChar(pData);
		pData+=2;
		/*虚拟道岔状态*/
		pMultiPointStru->wSwitchStates[i]=ShortFromChar(pData);
		pData+=2;
	}
}


/**********************************************************
函数功能：分相区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pPowerlessZoneStru被初始化的结构体指针
返回值：无
备注：依据互联互通数据结构V12.0.0添加，add by lmy 20180312
***********************************************************/
void POWERLESS_ZONE_STRU_init(UINT8 *pDataAdd, DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru)
{
	UINT8 *pData = pDataAdd;

	/*编号*/
	pPowerlessZoneStru->wId = ShortFromChar(pData);
	pData += 2;
	/*中性区起点Link编号*/
	pPowerlessZoneStru->wOrgnLinkId = ShortFromChar(pData);
	pData += 2;
	/*中性区起点Link偏移量*/
	pPowerlessZoneStru->dwOrgnLinkOfst = LongFromChar(pData);
	pData += 4;
	/*中性区终点Link编号*/
	pPowerlessZoneStru->wTmnlLinkId = ShortFromChar(pData);
	pData += 2;
	/*中性区终点Link偏移量*/
	pPowerlessZoneStru->dwTmnlLinkOfst = LongFromChar(pData);
	pData += 4;
	/*防护方向*/
	pPowerlessZoneStru->wProtectDir = *pData;
	pData++;
	/*分相区通过入口速度（km/h）*/
	pPowerlessZoneStru->wEntrySpeed = ShortFromChar(pData);
	pData += 2;
}

/**********************************************************
函数功能：车档表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pCarStopStru被初始化的结构体指针
返回值：无
备注：依据互联互通数据结构V15.0.0添加，add by sds 20181031
***********************************************************/
void CARSTOP_STRU_init(UINT8 *pDataAdd, DSU_CARSTOP_STRU *pCarStopStru)
{
	UINT8 *pData = pDataAdd;

	/*编号*/
	pCarStopStru->carsId = ShortFromChar(pData);
	pData += 2;

	/*所属Link编号*/
	pCarStopStru->carsLinkId = ShortFromChar(pData);
	pData += 2;

	/*所属Link偏移量*/
	pCarStopStru->carsLinkOfst = LongFromChar(pData);
	pData += 4;

}

/**********************************************************
函数功能：限速区段表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pResSpeedSgmt被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void RESPEED_SGMT_STRU_init(UINT8 *pDataAdd, DSU_RESPEED_SGMT_STRU *pResSpeedSgmtStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*编号*/
	pResSpeedSgmtStru->wId = ShortFromChar(pData);
    pData += 2;

	/*名称*/
	for(i=0;i<16;i++)
	{
		pResSpeedSgmtStru->szCaption[i] = *pData;
		pData ++;
	}

	/*起点所处link编号*/
	pResSpeedSgmtStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	/*起点所处link偏移量*/
	pResSpeedSgmtStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*终点所处link编号*/
	pResSpeedSgmtStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;

	/*终点所处link偏移量*/
	pResSpeedSgmtStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;

	/*所属计轴区段编号*/
	pResSpeedSgmtStru->wManageAxleSgmtId = ShortFromChar(pData);
    pData += 2;
}

/**********************************************************
函数功能：OC表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pOCStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void OC_STRU_init(UINT8 *pDataAdd, DSU_OC_STRU *pOCStru)
{
	UINT8 *pData = pDataAdd;

	/*索引编号*/
	pOCStru->wIndex = ShortFromChar(pData);
    pData += 2;
		
	/*OC的ID*/
	pOCStru->wId = ShortFromChar(pData);
    pData += 2;

	/*OC属性*/
	pOCStru->wType = *pData;
    pData += 1;

	/*TMC-IVOC共用数据校验信息*/
	pOCStru->wParityMsgOfTMC_IVOC = LongFromChar(pData);
	pData += 4;

	/*TMC-ITS共用数据校验信息*/
	pOCStru->wParityMsgOfTMC_ITS = LongFromChar(pData);
	pData += 4;

}

/**********************************************************
函数功能：ITS表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pITSStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void ITS_STRU_init(UINT8 *pDataAdd, DSU_ITS_STRU *pITSStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 loopIndex = 0;

	/*索引编号*/
	pITSStru->wIndex = ShortFromChar(pData);
    pData += 2;
		
	/*ITS的ID*/
	pITSStru->wId = ShortFromChar(pData);
    pData += 2;

    /*ITS_IVOC共用数据校验信息*/
	pITSStru->wParityMsgOfITS_IVOC=LongFromChar(pData);	
	pData += 4;

	/*ITS_TMC共用数据校验信息*/
	pITSStru->wParityMsgOfITS_TMC=LongFromChar(pData);	
	pData += 4;

	/*相邻ITS数量*/
	pITSStru->wLinkITSCount = *pData;
	pData += 1;

	/*相邻ITS相关信息*/
	for(loopIndex=0;loopIndex<6;loopIndex++)
	{
		/*相邻ITS的ID*/		        
		pITSStru->wLinkITSID[loopIndex]	= ShortFromChar(pData);	
		pData += 2;

		/*与相邻ITS的共用数据校验信息*/
		pITSStru->wLinkITSParityMsg[loopIndex]=LongFromChar(pData);	
		pData += 4;
	}

}

/**********************************************************
函数功能：非通信车检测区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pNoComTrainArea被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void NOCOM_TRAIN_AREA_STRU_init(UINT8 *pDataAdd, DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru)
{
	UINT8 *pData = pDataAdd;

	/*非通信车检测区ID*/
	pNoComTrainAreaStru->wId = ShortFromChar(pData);
    pData += 2;
		
	/*方向*/
	pNoComTrainAreaStru->wDir = *pData;
	pData+=1;

	/*起点所处link编号*/
	pNoComTrainAreaStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	/*起点所处link偏移量*/
	pNoComTrainAreaStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*终点所处link编号*/
	pNoComTrainAreaStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;

	/*终点所处link偏移量*/
	pNoComTrainAreaStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
}

/**********************************************************
函数功能：交渡分区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pTransitZoneStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void TRANSIT_ZONE_STRU_init(UINT8 *pDataAdd, DSU_TRANSIT_ZONE_STRU  *pTransitZoneStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 loopIndex = 0;

	/*交分渡线编号*/
	pTransitZoneStru->wId = ShortFromChar(pData);
    pData += 2;

	/*关联道岔ID*/
	for(loopIndex=0;loopIndex<4;loopIndex++)
	{				        
		pTransitZoneStru->wLinkBalise[loopIndex]= ShortFromChar(pData);	
		pData += 2;
	}

	/*防护距离*/
	//pTransitZoneStru->wProtecDistance= *pData;	
	//pData += 1;

	/*所属OC编号*/
	//pTransitZoneStru->wManagerOCID = ShortFromChar(pData);
	//pData += 2;


}